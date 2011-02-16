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
SDName: Boss_Gluth
SD%Complete: 70
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_ZOMBIE                    = -1533119,

    SPELL_MORTALWOUND               = 54378,
    SPELL_DECIMATE                  = 28374,
    SPELL_ENRAGE                    = 28371,
    SPELL_ENRAGE_H                  = 54427,
    SPELL_BERSERK                   = 26662,

    NPC_ZOMBIE_CHOW                 = 16360,
    SPELL_INFECTED_WOUND            = 29306,
};

#define ZOMBIE_X                    3269.06f
#define ZOMBIE_Y                    -3170.83f
#define ZOMBIE_Z                    297.42f
#define ZOMBIE_O                    0.78f


struct MANGOS_DLL_DECL mob_zombie_chowsAI : public ScriptedAI
{
    mob_zombie_chowsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    bool bIsForceMove;

    void Reset()
    {
        bIsForceMove = false;
    }

    void JustDied(Unit* Killer) {}

    void DoMeleeAttackIfReady()
    {
        //If we are within range melee the target
        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we aren't currently casting
            if (m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_INFECTED_WOUND, true);
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || bIsForceMove)
            return;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_gluthAI : public ScriptedAI
{
    boss_gluthAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> m_lZombieGUIDList;

    uint32 MortalWound_Timer;
    uint32 Decimate_Timer;
    uint32 Enrage_Timer;
    uint32 Summon_Timer;
    uint32 m_uiBerserkTimer;
    uint32 RangeCheck_Timer;

    void Reset()
    {
        MortalWound_Timer = 10000;
        Decimate_Timer = (m_bIsRegularMode ? 120000 : 90000);
        Enrage_Timer = 30000;
        Summon_Timer = 10000;
        RangeCheck_Timer = 1000;

        m_uiBerserkTimer = MINUTE*8*IN_MILLISECONDS;
        m_lZombieGUIDList.clear();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->CanFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            if (m_creature->IsWithinDistInMap(pWho, 38.0f) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim())
                {
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                    AttackStart(pWho);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho);
                }
            }
        }
    }

    void Aggro(Unit *who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, IN_PROGRESS);
    }

    void JustDied(Unit* Killer)
    {
        if (!m_lZombieGUIDList.empty())
        {
            for (std::list<uint64>::iterator itr = m_lZombieGUIDList.begin(); itr != m_lZombieGUIDList.end(); ++itr)
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    pTemp->ForcedDespawn();
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //MortalWound_Timer
        if (MortalWound_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MORTALWOUND);
            MortalWound_Timer = 10000;
        }else MortalWound_Timer -= diff;

        //Decimate_Timer
        if (Decimate_Timer < diff)
        {
            m_creature->MonsterTextEmote("Gluth decimates all nearby flesh!", 0, true);
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DECIMATE); // need core support

            // workaround below
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            if (t_list.size())
            {
                std::list<HostileReference *>::iterator itr = t_list.begin();
                for(; itr!= t_list.end(); ++itr)
                {
                    Unit* pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                    if (pTarget && pTarget->isAlive() && pTarget->GetTypeId() == TYPEID_PLAYER &&
                    	(pTarget->GetHealth() > pTarget->GetMaxHealth() * 0.05))
                        pTarget->SetHealth(pTarget->GetMaxHealth() * 0.05);
                }
            }
            // Move Zombies
            if (!m_lZombieGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lZombieGUIDList.begin(); itr != m_lZombieGUIDList.end(); ++itr)
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        if (pTemp->isAlive())
                        {
                            ((mob_zombie_chowsAI*)pTemp->AI())->bIsForceMove = true;
                            if (m_creature->GetHealth() > m_creature->GetMaxHealth() * 0.05) // remove when SPELL_DECIMATE is working
                                pTemp->SetHealth(pTemp->GetMaxHealth() * 0.05);
                            pTemp->AddThreat(m_creature, 1000000000.0f); // force move toward to Gluth
                        }
            }
            Decimate_Timer = (m_bIsRegularMode ? 120000 : 90000);
        }else Decimate_Timer -= diff;

        //Enrage_Timer
        if (Enrage_Timer < diff)
        {
            m_creature->MonsterTextEmote("Gluth becomes enraged!", 0, true);
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H);
            Enrage_Timer = 30000;
        }else Enrage_Timer -= diff;

        if (RangeCheck_Timer < diff)
        {
            if (!m_lZombieGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lZombieGUIDList.begin(); itr != m_lZombieGUIDList.end(); ++itr)
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        {
                            DoScriptText(EMOTE_ZOMBIE, m_creature);
                            m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.05);
                            pTemp->ForcedDespawn();
                        }
            }
            RangeCheck_Timer = 1000;
        }else RangeCheck_Timer -= diff;

        //Summon_Timer
        if (Summon_Timer < diff)
        {
            for(uint8 i = 0; i < (m_bIsRegularMode ? 1 : 2); i++)
            {
                if (Creature* pZombie = m_creature->SummonCreature(NPC_ZOMBIE_CHOW, ZOMBIE_X + urand (0, 4), ZOMBIE_Y + urand (0, 4), ZOMBIE_Z, ZOMBIE_O, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 150000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        pZombie->AI()->AttackStart(pTarget);
                        m_lZombieGUIDList.push_back(pZombie->GetGUID());
                    }
                }
            }
            Summon_Timer = 10000;
        } else Summon_Timer -= diff;

        //m_uiBerserkTimer
        if (m_uiBerserkTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BERSERK, true);
            m_uiBerserkTimer = MINUTE*5*IN_MILLISECONDS;
        }else m_uiBerserkTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gluth(Creature* pCreature)
{
    return new boss_gluthAI(pCreature);
}

CreatureAI* GetAI_mob_zombie_chows(Creature* pCreature)
{
    return new mob_zombie_chowsAI(pCreature);
}

void AddSC_boss_gluth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gluth";
    newscript->GetAI = &GetAI_boss_gluth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zombie_chows";
    newscript->GetAI = &GetAI_mob_zombie_chows;
    newscript->RegisterSelf();
}
