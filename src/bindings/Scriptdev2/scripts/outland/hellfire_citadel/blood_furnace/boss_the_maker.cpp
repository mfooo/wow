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
SDName: Boss_The_Maker
SD%Complete: 80
SDComment: Mind control no support
SDCategory: Hellfire Citadel, Blood Furnace
EndScriptData */

#include "precompiled.h"
#include "blood_furnace.h"

enum
{
    SAY_AGGRO_1                 = -1542009,
    SAY_AGGRO_2                 = -1542010,
    SAY_AGGRO_3                 = -1542011,
    SAY_KILL_1                  = -1542012,
    SAY_KILL_2                  = -1542013,
    SAY_DIE                     = -1542014,

    SPELL_ACID_SPRAY            = 38153,                    // heroic 38973 ??? 38153
    SPELL_EXPLODING_BREAKER     = 30925,
    H_SPELL_EXPLODING_BREAKER   = 40059,
    SPELL_KNOCKDOWN             = 20276,
    SPELL_DOMINATION            = 25772                     // ???
};

struct MANGOS_DLL_DECL boss_the_makerAI : public ScriptedAI
{
    boss_the_makerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 AcidSpray_Timer;
    uint32 ExplodingBreaker_Timer;
    uint32 Domination_Timer;
    uint32 Knockdown_Timer;

    void Reset()
    {
        AcidSpray_Timer = 15000;
        ExplodingBreaker_Timer = 6000;
        Domination_Timer = 20000;
        Knockdown_Timer = 10000;
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THE_MAKER_EVENT,IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THE_MAKER_EVENT,FAIL);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DIE, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THE_MAKER_EVENT,DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (AcidSpray_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ACID_SPRAY);
            AcidSpray_Timer = urand(15000, 23000);
        }else AcidSpray_Timer -=diff;

        if (ExplodingBreaker_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? H_SPELL_EXPLODING_BREAKER : SPELL_EXPLODING_BREAKER);
            ExplodingBreaker_Timer = urand(4000, 12000);
        }else ExplodingBreaker_Timer -=diff;

        if (Domination_Timer < diff)
        {
            Unit* target;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);

            DoCastSpellIfCan(target,SPELL_DOMINATION);

            Domination_Timer = 15000+rand()%10000;
        }else Domination_Timer -=diff;

        if (Knockdown_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KNOCKDOWN);
            Knockdown_Timer = urand(4000, 12000);
        }else Knockdown_Timer -=diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_the_makerAI(Creature* pCreature)
{
    return new boss_the_makerAI(pCreature);
}

void AddSC_boss_the_maker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_the_maker";
    newscript->GetAI = &GetAI_boss_the_makerAI;
    newscript->RegisterSelf();
}
