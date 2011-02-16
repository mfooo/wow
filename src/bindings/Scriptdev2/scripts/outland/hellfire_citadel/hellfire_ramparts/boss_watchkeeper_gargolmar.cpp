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
SDName: Boss_Watchkeeper_Gargolmar
SD%Complete: 80
SDComment: Missing adds to heal him. Surge should be used on target furthest away, not random.
SDCategory: Hellfire Citadel, Hellfire Ramparts
EndScriptData */

#include "precompiled.h"

#define SAY_TAUNT               -1543000
#define SAY_HEAL                -1543001
#define SAY_SURGE               -1543002
#define SAY_AGGRO_1             -1543003
#define SAY_AGGRO_2             -1543004
#define SAY_AGGRO_3             -1543005
#define SAY_KILL_1              -1543006
#define SAY_KILL_2              -1543007
#define SAY_DIE                 -1543008

#define SPELL_MORTAL_WOUND      30641
#define H_SPELL_MORTAL_WOUND    36814
#define SPELL_SURGE             34645
#define SPELL_RETALIATION       22857

struct MANGOS_DLL_DECL boss_watchkeeper_gargolmarAI : public ScriptedAI
{
    boss_watchkeeper_gargolmarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    uint32 Surge_Timer;
    uint32 MortalWound_Timer;
    uint32 Retaliation_Timer;

    bool HasTaunted;
    bool YelledForHeal;

    void Reset()
    {
        Surge_Timer = 5000;
        MortalWound_Timer = 4000;
        Retaliation_Timer = 0;

        HasTaunted = false;
        YelledForHeal = false;
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (!m_creature->getVictim() && who->isTargetableForAttack() && (m_creature->IsHostileTo(who)) && who->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->CanFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(who);
            }
            else if (!HasTaunted && m_creature->IsWithinDistInMap(who, 60.0f))
            {
                DoScriptText(SAY_TAUNT, m_creature);
                HasTaunted = true;
            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DIE, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (MortalWound_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MORTAL_WOUND : H_SPELL_MORTAL_WOUND);
            MortalWound_Timer = urand(5000, 13000);
        }else MortalWound_Timer -= diff;

        if (Surge_Timer < diff)
        {
            DoScriptText(SAY_SURGE, m_creature);

            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_SURGE);

            Surge_Timer = urand(5000, 12000);
        }else Surge_Timer -= diff;

        if (m_creature->GetHealthPercent() < 20.0f)
        {
            if (Retaliation_Timer < diff)
            {
                DoCastSpellIfCan(m_creature,SPELL_RETALIATION);
                Retaliation_Timer = 30000;
            }else Retaliation_Timer -= diff;
        }

        if (!YelledForHeal)
        {
            if (m_creature->GetHealthPercent() < 40.0f)
            {
                DoScriptText(SAY_HEAL, m_creature);
                YelledForHeal = true;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_watchkeeper_gargolmarAI(Creature* pCreature)
{
    return new boss_watchkeeper_gargolmarAI(pCreature);
}

void AddSC_boss_watchkeeper_gargolmar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_watchkeeper_gargolmar";
    newscript->GetAI = &GetAI_boss_watchkeeper_gargolmarAI;
    newscript->RegisterSelf();
}
