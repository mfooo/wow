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
SDName: boss_anubarak
SD%Complete: ?%
SDComment: by MaxXx2021
SDCategory: Azjol-Nerub instance
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SPELL_CARRION_BEETLES                         = 53520,
    SPELL_SUMMON_CARRION_BEETLES                  = 53521,
    SPELL_LEECHING_SWARM                          = 53467,
    SPELL_IMPALE                                  = 53454,
    SPELL_POUND                                   = 53472,
    SPELL_SUBMERGE                                = 53421,

    NPC_GUARDIAN                             = 29216,
    NPC_VENOMANCER                           = 29217,
    NPC_DATTER                               = 29213,

    SAY_INTRO                                     = -1601010,
    SAY_AGGRO                                     = -1601000,
    SAY_SLAY_1                                    = -1601001,
    SAY_SLAY_2                                    = -1601002,
    SAY_SLAY_3                                    = -1601003,
    SAY_LOCUST_1                                  = -1601005,
    SAY_LOCUST_2                                  = -1601006,
    SAY_LOCUST_3                                  = -1601007,
    SAY_SUBMERGE_1                                = -1601008,
    SAY_SUBMERGE_2                                = -1601009,
    SAY_DEATH                                     = -1601004
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations SpawnPoint[2] =
{
    {550.7f, 282.8f, 224.3f},
    {551.1f, 229.4f, 224.3f}
};

struct MANGOS_DLL_DECL boss_anubarakAI : public ScriptedAI
{
    boss_anubarakAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    bool bChanneling;
    bool bGuardianSummoned;
    bool bVenomancerSummoned;
    bool bDatterSummoned;
    uint8 uiPhase;
    uint32 uiPhaseTimer;
    uint32 uiEmergeTimer;

    uint32 uiCarrionBeetlesTimer;
    uint32 uiLeechingSwarmTimer;
    uint32 uiImpaleTimer;
    uint32 uiPoundTimer;
    uint32 uiSubmergeTimer;
    uint32 uiUndergroundTimer;
    uint32 uiVenomancerTimer;
    uint32 uiDatterTimer;

    //SummonList lSummons;

    void Reset()
    {

        uiCarrionBeetlesTimer = 8000;
        uiLeechingSwarmTimer = 20000;
        uiImpaleTimer = 9000;
        uiPoundTimer = 15000;

        uiPhase = 0;
        uiPhaseTimer = 0;
        bChanneling = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
        m_creature->SetDisplayId(27856);

        DespawnAll();

        if (pInstance)
            pInstance->SetData(TYPE_ANUBARAK, NOT_STARTED);
    }

    void AttackStart(Unit* who)
    {
       if(!who) return;

       if(uiPhase > 0) return;

       ScriptedAI::AttackStart(who);
    }

    void DespawnAll()
    {
        std::list<Creature*> m_pGuard;
        GetCreatureListWithEntryInGrid(m_pGuard, m_creature, NPC_GUARDIAN, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pGuard.empty())
            for(std::list<Creature*>::iterator itr = m_pGuard.begin(); itr != m_pGuard.end(); ++itr)
            {
                (*itr)->ForcedDespawn();
            }

        std::list<Creature*> m_pVen;
        GetCreatureListWithEntryInGrid(m_pVen, m_creature, NPC_VENOMANCER, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pVen.empty())
            for(std::list<Creature*>::iterator iter = m_pVen.begin(); iter != m_pVen.end(); ++iter)
            {
                (*iter)->ForcedDespawn();
            }

        std::list<Creature*> m_pDat;
        GetCreatureListWithEntryInGrid(m_pDat, m_creature, NPC_DATTER, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pDat.empty())
            for(std::list<Creature*>::iterator iter = m_pDat.begin(); iter != m_pDat.end(); ++iter)
            {
                (*iter)->ForcedDespawn();
            }
    }

    void EnterCombat(Unit *pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (pInstance)
            pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
    }

    void NextPhase()
    {
        m_creature->InterruptNonMeleeSpells(false);
        ++uiPhaseTimer;

        bGuardianSummoned = false;
        bVenomancerSummoned = false;
        bDatterSummoned = false;

        uiUndergroundTimer = 40000;
        uiVenomancerTimer = 25000;
        uiDatterTimer = 32000;

        DoCast(m_creature, SPELL_SUBMERGE);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);

        uiPhase = 1;
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (bChanneling == true)
        {
            for (uint8 i = 0; i < 4; ++i)
                DoCast(m_creature->getVictim(), SPELL_SUMMON_CARRION_BEETLES, true);
            bChanneling = false;
        }

        if (uiPhase == 1)
        {
            if (uiImpaleTimer <= diff)
            {
                if(m_creature->HasAura(SPELL_SUBMERGE))
                {
                   m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                   m_creature->SetDisplayId(11686);       
                   m_creature->StopMoving();
                   m_creature->GetMotionMaster()->Clear(false);
                   m_creature->GetMotionMaster()->MoveIdle();
                }
                if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_IMPALE);
                uiImpaleTimer = 9000;
            } else uiImpaleTimer -= diff;

            if (!bGuardianSummoned)
            {
                for (uint8 i = 0; i < 2; ++i)
                {
                    if (Creature *Guardian = m_creature->SummonCreature(NPC_GUARDIAN,SpawnPoint[i].x,SpawnPoint[i].y,SpawnPoint[i].z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
                    {
                        Guardian->AddThreat(m_creature->getVictim(), 0.0f);
                        Guardian->SetInCombatWithZone();
                    }
                }
                bGuardianSummoned = true;
            }

            if (!bVenomancerSummoned)
            {
                if (uiVenomancerTimer <= diff)
                {
                    if (uiPhaseTimer > 1)
                    {
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            if (Creature *Venomancer = m_creature->SummonCreature(NPC_VENOMANCER,SpawnPoint[i].x,SpawnPoint[i].y,SpawnPoint[i].z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
                            {
                                Venomancer->AddThreat(m_creature->getVictim(), 0.0f);
                                Venomancer->SetInCombatWithZone();
                            }
                        }
                        bVenomancerSummoned = true;
                    }
                } else uiVenomancerTimer -= diff;
            }

            if (!bDatterSummoned)
            {
                if (uiDatterTimer <= diff)
                {
                    if (uiPhaseTimer > 2)
                    {
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            if (Creature *Datter = m_creature->SummonCreature(NPC_DATTER,SpawnPoint[i].x,SpawnPoint[i].y,SpawnPoint[i].z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
                            {
                                Datter->AddThreat(m_creature->getVictim(), 0.0f);
                                Datter->SetInCombatWithZone();
                            }
                        }
                        bDatterSummoned = true;
                    }
                } else uiDatterTimer -= diff;
            }

            if (uiUndergroundTimer <= diff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(27856);
                DoCast(m_creature, 50142);
                uiEmergeTimer = 2000;
                uiPhase = 2;
            } else uiUndergroundTimer -= diff;
        }

        if (uiPhase == 2)
        {
            if (uiEmergeTimer <= diff)
            {
                uiPhase = 0;
                m_creature->RemoveAurasDueToSpell(50142);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            } else uiEmergeTimer -= diff;
        }

        if (uiPhase == 0)
        {
            if (uiLeechingSwarmTimer <= diff)
            {
                DoCast(m_creature, SPELL_LEECHING_SWARM);
                uiLeechingSwarmTimer = 19000;
            } else uiLeechingSwarmTimer -= diff;

            if (uiCarrionBeetlesTimer <= diff)
            {
                bChanneling = true;
                DoCast(m_creature->getVictim(), SPELL_CARRION_BEETLES);
                uiCarrionBeetlesTimer = 25000;
            } else uiCarrionBeetlesTimer -= diff;

            if (uiPoundTimer <= diff)
            {
                 DoCast(m_creature->getVictim(), SPELL_POUND);
                 uiPoundTimer = 16500;
            } else uiPoundTimer -= diff;

            DoMeleeAttackIfReady();
        }

        if(uiPhaseTimer == 0 && m_creature->GetHealthPercent() < 75.0f)
            NextPhase();

        if(uiPhaseTimer == 1 && m_creature->GetHealthPercent() < 50.0f)
            NextPhase();

        if(uiPhaseTimer == 2 && m_creature->GetHealthPercent() < 25.0f)
            NextPhase();
    }

    void JustDied(Unit *pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
            pInstance->SetData(TYPE_ANUBARAK, DONE);
    }

    void KilledUnit(Unit *pVictim)
    {
        if (pVictim == m_creature)
            return;

        DoScriptText(urand(SAY_SLAY_1,SAY_SLAY_2), m_creature);
    }

};

CreatureAI* GetAI_boss_anubarak(Creature *pCreature)
{
    return new boss_anubarakAI (pCreature);
}

void AddSC_boss_anubarak()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_anubarak";
    newscript->GetAI = &GetAI_boss_anubarak;
    newscript->RegisterSelf();
}
