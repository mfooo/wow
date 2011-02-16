/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Patchwerk
SD%Complete: 80
SDComment:
TODO: confirm how hateful strike work  //Hateful strike (deals 30k raw dmg too one of the top 3 on his top aggro list  can be parried dodged mitigated)
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1            = -1533017,
    SAY_AGGRO2            = -1533018,
    SAY_SLAY              = -1533019,
    SAY_DEATH             = -1533020,

    EMOTE_GENERIC_BERSERK   = -1000004,
    EMOTE_GENERIC_ENRAGED   = -1000003,

    SPELL_HATEFULSTRIKE   = 28308,
    SPELL_HATEFULSTRIKE_H = 59192,
    SPELL_ENRAGE          = 28131,
    SPELL_BERSERK         = 26662,
    SPELL_SLIMEBOLT       = 32309,

	// achievement
    MAKE_QUICK_WERK_OF_HIM    = 1856,  // (10man)
	MAKE_QUICK_WERK_OF_HIM_H  = 1857
};

const float MELEE_DISTANCE = 5.0;

struct MANGOS_DLL_DECL boss_patchwerkAI : public ScriptedAI
{
    boss_patchwerkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiHatefulStrikeTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiSlimeboltTimer;
    uint32 uiEncounterTimer;
    bool   m_bEnraged;
    bool   m_bBerserk;

    void Reset()
    {
        m_uiHatefulStrikeTimer = 1000;                      //1 second
        m_uiBerserkTimer = MINUTE*6*IN_MILLISECONDS;         //6 minutes
        m_uiSlimeboltTimer = 10000;
        uiEncounterTimer        = 0;
        m_bEnraged = false;
        m_bBerserk = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_PATCHWERK, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 4))
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PATCHWERK, DONE);

        if (uiEncounterTimer < 179999)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? MAKE_QUICK_WERK_OF_HIM : MAKE_QUICK_WERK_OF_HIM_H);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(urand(0, 1)?SAY_AGGRO1:SAY_AGGRO2, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PATCHWERK, IN_PROGRESS);
    }

    void DoHatefulStrike()
    {
        // The ability is used on highest HP target choosen of the top 2 (3 heroic) targets on threat list being in melee range
        Unit* pTarget = NULL;
        uint32 uiHighestHP = 0;
        uint32 uiTargets = m_bIsRegularMode ? 2 : 3;

        ThreatList const& lThreatList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator iter = lThreatList.begin(); iter != lThreatList.end(); ++iter)
        {
            if (!uiTargets)
                break;

            if (Unit* pTempTarget = m_creature->GetMap()->GetUnit((*iter)->getUnitGuid()))
            {
                if (pTempTarget->GetHealth() > uiHighestHP && m_creature->CanReachWithMeleeAttack(pTempTarget))
                {
                    uiHighestHP = pTempTarget->GetHealth();
                    pTarget = pTempTarget;
                }
            }

            --uiTargets;
        }

        if (pTarget)
            DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_HATEFULSTRIKE : SPELL_HATEFULSTRIKE_H);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Achiev timer
        uiEncounterTimer += uiDiff;

        // Hateful Strike
        if (m_uiHatefulStrikeTimer < uiDiff)
        {
            DoHatefulStrike();
            m_uiHatefulStrikeTimer = 1000;
        }
        else
            m_uiHatefulStrikeTimer -= uiDiff;

        // Soft Enrage at 5%
        if (!m_bEnraged)
        {
            if (m_creature->GetHealth()*20 < m_creature->GetMaxHealth())
            {
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
                DoScriptText(EMOTE_GENERIC_ENRAGED, m_creature);
                m_bEnraged = true;
            }
        }

        // Berserk after 6 minutes
        if (!m_bBerserk)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                DoScriptText(EMOTE_GENERIC_BERSERK, m_creature);
                m_bBerserk = true;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }
        else
        {
            // Slimebolt - casted only while Berserking to prevent kiting
            if (m_uiSlimeboltTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SLIMEBOLT);
                m_uiSlimeboltTimer = 5000;
            }
            else
                m_uiSlimeboltTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_patchwerk(Creature* pCreature)
{
    return new boss_patchwerkAI(pCreature);
}

void AddSC_boss_patchwerk()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_patchwerk";
    NewScript->GetAI = &GetAI_boss_patchwerk;
    NewScript->RegisterSelf();
}
