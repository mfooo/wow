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
SDName: Boss_Faerlina
SD%Complete: 50
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                 = -1533009,
    SAY_AGGRO                 = -1533010,
    SAY_ENRAGE1               = -1533011,
    SAY_ENRAGE2               = -1533012,
    SAY_ENRAGE3               = -1533013,
    SAY_SLAY1                 = -1533014,
    SAY_SLAY2                 = -1533015,
    SAY_DEATH                 = -1533016,

    EMOTE_BOSS_GENERIC_FRENZY = -1000005,

    //SOUND_RANDOM_AGGRO        = 8955,                              //soundId containing the 4 aggro sounds, we not using this

    SPELL_POSIONBOLT_VOLLEY   = 28796,
    H_SPELL_POSIONBOLT_VOLLEY = 54098,
    SPELL_FRENZY              = 28798,
    H_SPELL_FRENZY            = 54100,

    SPELL_RAINOFFIRE          = 28794,                      //Not sure if targeted AoEs work if casted directly upon a pPlayer
    H_SPELL_RAINOFFIRE        = 58936,
    SPELL_FIREBALL            = 54095,
    SPELL_FIREBALL_H          = 54096,
    SPELL_WIDOWS_EMBRACE      = 28732,

    NPC_NAXXRAMAS_WORSHIPPER  = 16506,
    NPC_NAXXRAMAS_FOLLOWER    = 16505,
};

struct MANGOS_DLL_DECL boss_faerlinaAI : public ScriptedAI
{
    boss_faerlinaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPoisonBoltVolleyTimer;
    uint32 m_uiRainOfFireTimer;
    uint32 m_uiEnrageTimer;
    bool   m_bHasTaunted;

    void Reset()
    {
        m_uiPoisonBoltVolleyTimer = urand(14000, 15000);
        m_uiRainOfFireTimer = urand(6000, 8000);
        m_uiEnrageTimer = urand(60000, 80000);

        std::list<Creature*> lUnitList;
        GetCreatureListWithEntryInGrid(lUnitList, m_creature, NPC_NAXXRAMAS_WORSHIPPER, 100.0f);
        if (!lUnitList.empty())
            for(std::list<Creature*>::iterator iter = lUnitList.begin(); iter != lUnitList.end(); ++iter)
                if ((*iter)->isDead())
                    (*iter)->Respawn();

        lUnitList.clear();
        GetCreatureListWithEntryInGrid(lUnitList, m_creature, NPC_NAXXRAMAS_FOLLOWER, 100.0f);
        if (!lUnitList.empty())
            for(std::list<Creature*>::iterator iter = lUnitList.begin(); iter != lUnitList.end(); ++iter)
                if ((*iter)->isDead())
                    (*iter)->Respawn();
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();

        std::list<Creature*> lUnitList;
        GetCreatureListWithEntryInGrid(lUnitList, m_creature, NPC_NAXXRAMAS_WORSHIPPER, 100.0f);
        if (!lUnitList.empty())
            for (std::list<Creature*>::iterator iter = lUnitList.begin(); iter != lUnitList.end(); ++iter)
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                    (*iter)->AI()->AttackStart(pTarget);
                else
                    (*iter)->AI()->AttackStart(pWho);

        m_creature->CallForHelp(20);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, IN_PROGRESS);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80.0f))
        {
            DoScriptText(SAY_GREET, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1)?SAY_SLAY1:SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Poison Bolt Volley
        if (m_uiPoisonBoltVolleyTimer < uiDiff)
        {
            if (!m_creature->HasAura(SPELL_WIDOWS_EMBRACE))
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POSIONBOLT_VOLLEY : H_SPELL_POSIONBOLT_VOLLEY);
            m_uiPoisonBoltVolleyTimer = urand(14000, 15000);
        }
        else
            m_uiPoisonBoltVolleyTimer -= uiDiff;

        // Rain Of Fire
        if (m_uiRainOfFireTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_RAINOFFIRE : H_SPELL_RAINOFFIRE);

            m_uiRainOfFireTimer = urand(6000, 8000);
        }
        else
            m_uiRainOfFireTimer -= uiDiff;

        //Enrage_Timer
        if (m_uiEnrageTimer < uiDiff)
        {
            switch (urand(0, 2))
            {
                case 0: DoScriptText(SAY_ENRAGE1, m_creature); break;
                case 1: DoScriptText(SAY_ENRAGE2, m_creature); break;
                case 2: DoScriptText(SAY_ENRAGE3, m_creature); break;
            }
            m_creature->MonsterTextEmote("%s goes into a frenzy!", 0, true);
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : H_SPELL_FRENZY);
            m_uiEnrageTimer = urand(60000, 80000);
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_worshipperAI : public ScriptedAI
{
    mob_worshipperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFireball_Timer;

    void Reset()
    {
        m_uiFireball_Timer = 0;
    }

    void Aggro(Unit *pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pFaerlina = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FAERLINA)))
                if (pFaerlina->isAlive() && !pFaerlina->getVictim())
                    pFaerlina->AI()->AttackStart(pWho);
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            if (Creature* pFaerlina = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FAERLINA)))
                if ((pFaerlina->HasAura(SPELL_FRENZY) || pFaerlina->HasAura(H_SPELL_FRENZY)) && (m_creature->GetDistance2d(pFaerlina) <= 10.0f))
                {
                    pFaerlina->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_FRENZY : H_SPELL_FRENZY);
                    if (SpellEntry* pSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_WIDOWS_EMBRACE))
                    {
                        pSpell->EffectImplicitTargetA[0] = TARGET_SELF;
                        pSpell->EffectImplicitTargetB[0] = 0;
                        pSpell->EffectImplicitTargetA[1] = TARGET_SELF;
                        pSpell->EffectImplicitTargetB[1] = 0;
                        pFaerlina->CastSpell(pFaerlina, pSpell, true);
                        pFaerlina->MonsterTextEmote("%s is affected by Widow's Embrace!", 0, true);
                    }
                }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFireball_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H);
            m_uiFireball_Timer = 5000 + rand()%3000;
        }
        else
            m_uiFireball_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_worshipper(Creature* pCreature)
{
    return new mob_worshipperAI(pCreature);
}

CreatureAI* GetAI_boss_faerlina(Creature* pCreature)
{
    return new boss_faerlinaAI(pCreature);
}

void AddSC_boss_faerlina()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_faerlina";
    NewScript->GetAI = &GetAI_boss_faerlina;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_worshipper";
    NewScript->GetAI = &GetAI_mob_worshipper;
    NewScript->RegisterSelf();
}
