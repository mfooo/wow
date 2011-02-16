/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Galdarah
SD%Complete: 80%
SDComment: achievements need to be implemented, channeling before engage is missing
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"
#include "Vehicle.h"

enum
{
    SAY_AGGRO                  = -1604019,
    SAY_TRANSFORM_1            = -1604020,
    SAY_TRANSFORM_2            = -1604021,
    SAY_SUMMON_1               = -1604022,
    SAY_SUMMON_2               = -1604023,
    SAY_SUMMON_3               = -1604024,
    SAY_SLAY_1                 = -1604025,
    SAY_SLAY_2                 = -1604026,
    SAY_SLAY_3                 = -1604027,
    SAY_DEATH                  = -1604028,

    EMOTE_IMPALED              = -1604030,

    ACHIEVEMENT_WHAT_THE_ECK   = 1864,
    ACHIEVEMENT_SHARE_THE_LOVE = 2152,

    NPC_RHINO_SPIRIT           = 29791,
    SPELL_STAMPEDE_RHINO       = 55220,
    SPELL_STAMPEDE_RHINO_H     = 59823,

    // troll form spells
    SPELL_STAMPEDE             = 55218,
    SPELL_WHIRLING_SLASH       = 55250,
    SPELL_WHIRLING_SLASH_H     = 59824,
    SPELL_RHINO_TRANSFORM      = 55297,
    SPELL_PUNCTURE             = 55276,
    SPELL_PUNCTURE_H           = 59826,

    // rhino form spells
    SPELL_TROLL_TRANSFORM      = 55299,
    SPELL_ENRAGE               = 55285,
    SPELL_ENRAGE_H             = 59828,
    SPELL_IMPALING_CHARGE      = 54956,
    SPELL_IMPALING_CHARGE_H    = 59827,
    SPELL_STOMP                = 55292,
    SPELL_STOMP_H              = 59826,

    EQUIP_MAIN                 = 30440
};

/*######
## boss_galdarah
######*/

struct MANGOS_DLL_DECL boss_galdarahAI : public ScriptedAI
{
    boss_galdarahAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsTrollPhase;
    bool m_bIsImpaled;

    uint32 m_uiStampedeTimer;
    uint32 m_uiPhaseChangeTimer;
    uint32 m_uiSpecialAbilityTimer;                         // Impaling Charge and Whirling Slash
    uint32 m_uiImpaleEndTimer;
    uint32 m_uiPunctureTimer;
    uint32 m_uiStompTimer;
    uint32 m_uiEnrageTimer;
    uint8 m_uiAbilityCount;

    void Reset()
    {
        m_bIsTrollPhase         = true;

        m_uiStampedeTimer       = 10000;
        m_uiSpecialAbilityTimer = 12000;
        m_uiImpaleEndTimer      = 5000;
        m_bIsImpaled            = false;
        m_uiPunctureTimer       = 25000;
        m_uiPhaseChangeTimer    = 7000;
        m_uiAbilityCount        = 0;
        SetEquipmentSlots(false, EQUIP_MAIN);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH , IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, NOT_STARTED);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_RHINO_SPIRIT)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);

            if (!pTarget)
                pTarget = m_creature->getVictim();

            if (pTarget)
            {
                pSummoned->CastSpell(pTarget, m_bIsRegularMode ? SPELL_STAMPEDE_RHINO : SPELL_STAMPEDE_RHINO_H, false);
                pSummoned->AI()->AttackStart(pTarget);
            }
        }
    }

    void DoPhaseSwitch()
    {
        if (!m_bIsTrollPhase)
            m_creature->RemoveAurasDueToSpell(SPELL_RHINO_TRANSFORM);

        m_bIsTrollPhase = !m_bIsTrollPhase;

        if (m_bIsTrollPhase)
        {
            DoCastSpellIfCan(m_creature, SPELL_TROLL_TRANSFORM);
            SetEquipmentSlots(false, EQUIP_MAIN);
        }
        else
        {
            DoScriptText(urand(0, 1) ? SAY_TRANSFORM_1 : SAY_TRANSFORM_2, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_RHINO_TRANSFORM);

            m_uiEnrageTimer = 4000;
            m_uiStompTimer  = 1000;
        }

        m_uiAbilityCount        = 0;
        m_uiPhaseChangeTimer    = 7000;
        m_uiSpecialAbilityTimer = 12000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsImpaled)
        {
            if (m_uiImpaleEndTimer < uiDiff)
            {
                if (m_creature->GetVehicleKit())
                    m_creature->GetVehicleKit()->RemoveAllPassengers();
                m_bIsImpaled = false;
            }
            else
                m_uiImpaleEndTimer -= uiDiff;
        }
        
        if (m_uiAbilityCount == 2)
        {
            if (m_uiPhaseChangeTimer < uiDiff)
                DoPhaseSwitch();
            else
                m_uiPhaseChangeTimer -= uiDiff;
        }

        if (m_bIsTrollPhase)
        {
            if (m_uiPunctureTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PUNCTURE : SPELL_PUNCTURE_H);
                m_uiPunctureTimer = 25000;
            }
            else
                m_uiPunctureTimer -= uiDiff;

            if (m_uiStampedeTimer < uiDiff)
            {
                switch(urand(0, 2))
                {
                    case 0: DoScriptText(SAY_SUMMON_1, m_creature); break;
                    case 1: DoScriptText(SAY_SUMMON_2, m_creature); break;
                    case 2: DoScriptText(SAY_SUMMON_3, m_creature); break;
                }

                DoCastSpellIfCan(m_creature->getVictim(), SPELL_STAMPEDE);
                m_uiStampedeTimer = 15000;
            }
            else
                m_uiStampedeTimer -= uiDiff;

            if (m_uiSpecialAbilityTimer < uiDiff)
            {
                if (SpellEntry* pTempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(m_bIsRegularMode ? SPELL_WHIRLING_SLASH : SPELL_WHIRLING_SLASH_H))
                {
                    pTempSpell->Effect[2] = 0;
                    m_creature->CastSpell(m_creature->getVictim(), pTempSpell, false);
                }

                m_uiSpecialAbilityTimer = 12000;
                ++m_uiAbilityCount;
            }
            else
                m_uiSpecialAbilityTimer -= uiDiff;

        }
        else
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H);
                m_uiEnrageTimer = 15000;
            }
            else
                m_uiEnrageTimer -= uiDiff;

            if (m_uiStompTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STOMP : SPELL_STOMP_H);
                m_uiStompTimer = 10000;
            }
            else
                m_uiStompTimer -= uiDiff;

            if (m_uiSpecialAbilityTimer < uiDiff)
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);

                if (!pTarget)
                    pTarget = m_creature->getVictim();

                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_IMPALING_CHARGE : SPELL_IMPALING_CHARGE_H) == CAST_OK)
                {
                    DoScriptText(EMOTE_IMPALED, m_creature, pTarget);
                    m_uiSpecialAbilityTimer = 12000;
                    pTarget->EnterVehicle(m_creature->GetVehicleKit(), 0);
                    m_uiImpaleEndTimer = 5000;
                    m_bIsImpaled = true;

                    ++m_uiAbilityCount;
                }
            }
            else
                m_uiSpecialAbilityTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_galdarah(Creature* pCreature)
{
    return new boss_galdarahAI(pCreature);
}

void AddSC_boss_galdarah()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_galdarah";
    pNewScript->GetAI = &GetAI_boss_galdarah;
    pNewScript->RegisterSelf();
}
