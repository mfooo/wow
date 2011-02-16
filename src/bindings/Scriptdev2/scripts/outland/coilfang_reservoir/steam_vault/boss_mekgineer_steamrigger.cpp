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
SDName: Boss_Mekgineer_Steamrigger
SD%Complete: 60
SDComment: Mechanics' interrrupt heal doesn't work very well, also a proper movement needs to be implemented -> summon further away and move towards target to repair.
SDCategory: Coilfang Resevoir, The Steamvault
EndScriptData */

/* ContentData
boss_mekgineer_steamrigger
mob_steamrigger_mechanic
EndContentData */

#include "precompiled.h"
#include "steam_vault.h"

#define SAY_MECHANICS               -1545007
#define SAY_AGGRO_1                 -1545008
#define SAY_AGGRO_2                 -1545009
#define SAY_AGGRO_3                 -1545010
#define SAY_AGGRO_4                 -1545011
#define SAY_SLAY_1                  -1545012
#define SAY_SLAY_2                  -1545013
#define SAY_SLAY_3                  -1545014
#define SAY_DEATH                   -1545015

#define SPELL_SUPER_SHRINK_RAY      31485
#define SPELL_SAW_BLADE             31486
#define SPELL_ELECTRIFIED_NET       35107
#define H_SPELL_ENRAGE              1                       //corrent enrage spell not known

#define ENTRY_STREAMRIGGER_MECHANIC 17951

struct MANGOS_DLL_DECL boss_mekgineer_steamriggerAI : public ScriptedAI
{
    boss_mekgineer_steamriggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Shrink_Timer;
    uint32 Saw_Blade_Timer;
    uint32 Electrified_Net_Timer;
    bool Summon75;
    bool Summon50;
    bool Summon25;

    void Reset()
    {
        Shrink_Timer = 20000;
        Saw_Blade_Timer = 15000;
        Electrified_Net_Timer = 10000;

        Summon75 = false;
        Summon50 = false;
        Summon25 = false;

        if (m_pInstance && m_creature->isAlive())
            m_pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER,NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER, DONE);
    }

    void KilledUnit(Unit* victim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
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
            m_pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER, IN_PROGRESS);
    }

    //no known summon spells exist
    void SummonMechanichs()
    {
        DoScriptText(SAY_MECHANICS, m_creature);

        DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,5,5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
        DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,-5,5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
        DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,-5,-5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);

        if (urand(0, 1))
            DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,5,-7,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);

        if (urand(0, 1))
            DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,7,-5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Shrink_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SUPER_SHRINK_RAY);
            Shrink_Timer = 20000;
        }else Shrink_Timer -= diff;

        if (Saw_Blade_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                DoCastSpellIfCan(target,SPELL_SAW_BLADE);
            else
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_SAW_BLADE);

            Saw_Blade_Timer = 15000;
        } else Saw_Blade_Timer -= diff;

        if (Electrified_Net_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ELECTRIFIED_NET);
            Electrified_Net_Timer = 10000;
        }
        else Electrified_Net_Timer -= diff;

        if (!Summon75)
        {
            if (m_creature->GetHealthPercent() < 75.0f)
            {
                SummonMechanichs();
                Summon75 = true;
            }
        }

        if (!Summon50)
        {
            if (m_creature->GetHealthPercent() < 50.0f)
            {
                SummonMechanichs();
                Summon50 = true;
            }
        }

        if (!Summon25)
        {
            if (m_creature->GetHealthPercent() < 25.0f)
            {
                SummonMechanichs();
                Summon25 = true;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_mekgineer_steamrigger(Creature* pCreature)
{
    return new boss_mekgineer_steamriggerAI(pCreature);
}

#define SPELL_DISPEL_MAGIC          17201
#define SPELL_REPAIR                31532
#define H_SPELL_REPAIR              37936

#define MAX_REPAIR_RANGE            (13.0f)                 //we should be at least at this range for repair
#define MIN_REPAIR_RANGE            (7.0f)                  //we can stop movement at this range to repair but not required

struct MANGOS_DLL_DECL mob_steamrigger_mechanicAI : public ScriptedAI
{
    mob_steamrigger_mechanicAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Repair_Timer;

    void Reset()
    {
        Repair_Timer = 2000;
    }

    void MoveInLineOfSight(Unit* who)
    {
        //react only if attacked
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (Repair_Timer < diff)
        {
            if (m_pInstance && m_pInstance->GetData64(DATA_MEKGINEERSTEAMRIGGER) && m_pInstance->GetData(TYPE_MEKGINEER_STEAMRIGGER) == IN_PROGRESS)
            {
                if (Creature* pMekgineer = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_MEKGINEERSTEAMRIGGER)))
                {
                    if (m_creature->IsWithinDistInMap(pMekgineer, MAX_REPAIR_RANGE))
                    {
                        //are we already channeling? Doesn't work very well, find better check?
                        if (!m_creature->GetUInt32Value(UNIT_CHANNEL_SPELL))
                        {
                            //m_creature->GetMotionMaster()->MovementExpired();
                            //m_creature->GetMotionMaster()->MoveIdle();

                            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_REPAIR : H_SPELL_REPAIR, CAST_TRIGGERED);
                        }
                        Repair_Timer = 5000;
                    }
                    else
                    {
                        //m_creature->GetMotionMaster()->MovementExpired();
                        //m_creature->GetMotionMaster()->MoveFollow(pMekgineer,0,0);
                    }
                }
            }else Repair_Timer = 5000;
        }else Repair_Timer -= diff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_steamrigger_mechanic(Creature* pCreature)
{
    return new mob_steamrigger_mechanicAI(pCreature);
}

void AddSC_boss_mekgineer_steamrigger()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_mekgineer_steamrigger";
    newscript->GetAI = &GetAI_boss_mekgineer_steamrigger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_steamrigger_mechanic";
    newscript->GetAI = &GetAI_mob_steamrigger_mechanic;
    newscript->RegisterSelf();
}
