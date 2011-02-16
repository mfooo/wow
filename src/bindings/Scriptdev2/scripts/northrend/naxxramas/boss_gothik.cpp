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
SDName: Boss_Gothik
SDAuthor: ckegg && FallenangelX
SD%Complete: 0
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_SPEECH               = -1533040,
    SAY_KILL                 = -1533041,
    SAY_DEATH                = -1533042,
    SAY_TELEPORT             = -1533043,

   //Gothik
   SPELL_HARVESTSOUL         = 28679,
   SPELL_SHADOWBOLT          = 29317,
   H_SPELL_SHADOWBOLT        = 56405,

   MOB_LIVE_TRAINEE    = 16124,
   MOB_LIVE_KNIGHT     = 16125,
   MOB_LIVE_RIDER      = 16126,
   MOB_DEAD_TRAINEE    = 16127,
   MOB_DEAD_KNIGHT     = 16148,
   MOB_DEAD_RIDER      = 16150,
   MOB_DEAD_HORSE      = 16149
};

#define POS_LIVE 3
#define POS_DEAD 5

const struct Waves { uint32 entry, normal_number, heroic_number, time; }
waves[] =
{
    {MOB_LIVE_TRAINEE, 2, 3, 20000}, //1
    {MOB_LIVE_TRAINEE, 2, 3, 20000}, //2
    {MOB_LIVE_TRAINEE, 2, 3, 10000}, //3
    {MOB_LIVE_KNIGHT,  1, 2, 10000}, //4
    {MOB_LIVE_TRAINEE, 2, 3, 15000}, //5
    {MOB_LIVE_KNIGHT,  1, 2, 5000},  //6
    {MOB_LIVE_TRAINEE, 2, 3, 20000}, //7
    {MOB_LIVE_TRAINEE, 2, 3, 0},     //8
    {MOB_LIVE_KNIGHT,  1, 2, 10000}, //8
    {MOB_LIVE_TRAINEE, 0, 3, 0},     //9
    {MOB_LIVE_RIDER,   1, 0, 10000}, //9
    {MOB_LIVE_RIDER,   0, 1, 0},     //10
    {MOB_LIVE_TRAINEE, 2, 0, 5000},  //10
    {MOB_LIVE_TRAINEE, 0, 3, 0},     //11
    {MOB_LIVE_KNIGHT,  1, 0, 15000}, //11
    {MOB_LIVE_RIDER,   1, 1, 0},     //12
    {MOB_LIVE_TRAINEE, 2, 0, 10000}, //12
    {MOB_LIVE_KNIGHT,  2, 2, 10000}, //13
    {MOB_LIVE_RIDER,   0, 1, 0},     //14
    {MOB_LIVE_TRAINEE, 2, 0, 10000}, //14
    {MOB_LIVE_RIDER,   1, 1, 0},     //15
    {MOB_LIVE_TRAINEE, 0, 3, 5000},  //15
    {MOB_LIVE_KNIGHT,  1, 1, 0},     //16
    {MOB_LIVE_TRAINEE, 0, 3, 5000},  //16
    {MOB_LIVE_RIDER,   0, 1, 0},     //17
    {MOB_LIVE_TRAINEE, 2, 3, 20000}, //17
    {MOB_LIVE_RIDER,   1, 1, 0},     //18
    {MOB_LIVE_KNIGHT,  1, 2, 0},     //18
    {MOB_LIVE_TRAINEE, 2, 3, 15000}, //18
    {MOB_LIVE_TRAINEE, 2, 0, 30000}, //19 (only normal)
    {0, 0, 0, 0}
};

const float PosSummonLive[POS_LIVE][3] =
{
    {2669.7f, -3430.9f, 268.56f},
    {2692.0f, -3430.9f, 268.56f},
    {2714.1f, -3430.9f, 268.56f},
};

const float PosSummonDead[POS_DEAD][3] =
{
    {2725.1f, -3310.0f, 268.85f},
    {2699.3f, -3322.8f, 268.60f},
    {2733.1f, -3348.5f, 268.84f},
    {2682.8f, -3304.2f, 268.85f},
    {2664.8f, -3340.7f, 268.23f},
};

const float PosGroundLive[4] = {2692.174f, -3400.963f, 267.680f, 1.7f};
const float PosGroundDeath[4] = {2690.378f, -3328.279f, 267.681f, 1.7f};

struct MANGOS_DLL_DECL boss_gothikAI : public ScriptedAI
{
    boss_gothikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;
    bool SecondPhase;
    bool BlinkPhase;

    std::list<uint64> SummonsList;

    uint32 waveCount;
    uint32 Summon_Timer;
    uint32 SummonDeathCheck_Timer;
    uint32 HarvestSoul_Timer;
    uint32 ShadowBolt_Timer;
    uint32 Blink_Timer;
    bool CentralGateOpened;

    void Reset()
    {
    	SecondPhase = false;
    	BlinkPhase = false;
        CentralGateOpened = false;

    	SummonsList.clear();

        waveCount = 0;
        Summon_Timer = 25000;
        SummonDeathCheck_Timer = 1000;
        HarvestSoul_Timer = 1000;
        ShadowBolt_Timer = 1000;
        Blink_Timer = 30000;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);

        if (m_pInstance)
        {
            if (GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_MILI_GOTH_COMBAT_GATE)))
                pGate->SetGoState(GO_STATE_ACTIVE);

        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, FAIL);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_SPEECH, m_creature);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetInCombatWithZone();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_GOTHIK, IN_PROGRESS);

            if (GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_MILI_GOTH_COMBAT_GATE)))
                pGate->SetGoState(GO_STATE_READY);
        }
    }

    void KilledUnit(Unit* victim)
    {
        if(!(rand()%5))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, DONE);
    }

    void JustSummoned(Creature* pSummon)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummon->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (SecondPhase)
        {
            if (HarvestSoul_Timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_HARVESTSOUL);
                HarvestSoul_Timer = 15000;
            }
            else
                HarvestSoul_Timer -= diff;

            if (ShadowBolt_Timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOWBOLT : H_SPELL_SHADOWBOLT);
                ShadowBolt_Timer = 1000;
            }
            else
                ShadowBolt_Timer -= diff;

            if (Blink_Timer < diff)
            {
                if (BlinkPhase)
                {
                    m_creature->GetMap()->CreatureRelocation(m_creature, PosGroundLive[0], PosGroundLive[1], PosGroundLive[2], 0.0f);
                    m_creature->SendMonsterMove(PosGroundLive[0], PosGroundLive[1], PosGroundLive[2], SPLINETYPE_NORMAL, SPLINEFLAG_DONE, 0);
                    BlinkPhase = false;
                }
                else
                {
                    m_creature->GetMap()->CreatureRelocation(m_creature, PosGroundDeath[0], PosGroundDeath[1], PosGroundDeath[2], 0.0f);
                    m_creature->SendMonsterMove(PosGroundDeath[0], PosGroundDeath[1], PosGroundDeath[2], SPLINETYPE_NORMAL, SPLINEFLAG_DONE, 0);
                    BlinkPhase = true;
                }
                DoResetThreat();
                Blink_Timer = 15000;
            }
            else
                Blink_Timer -= diff;

            if (!CentralGateOpened && (m_creature->GetHealth()*100 < m_creature->GetMaxHealth()*30) && m_pInstance)
                if (GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_MILI_GOTH_COMBAT_GATE)))
                {
                    pGate->SetGoState(GO_STATE_ACTIVE);
                    CentralGateOpened = true;
                }
        }
        else
        {
            if (Summon_Timer < diff)
            {
                if(waves[waveCount].entry)
                {
                    for(uint32 i = 0; i < (m_bIsRegularMode ? waves[waveCount].normal_number : waves[waveCount].heroic_number); ++i)
                    {
                        uint8 SummonLoc = rand()%POS_LIVE;
                        if (Creature* pTemp = m_creature->SummonCreature(waves[waveCount].entry, PosSummonLive[SummonLoc][0], PosSummonLive[SummonLoc][1], PosSummonLive[SummonLoc][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            SummonsList.push_back(pTemp->GetGUID());
                    }
                    Summon_Timer = waves[waveCount].time;
                    ++waveCount;
                }
                else
                {
                    DoScriptText(SAY_TELEPORT, m_creature);
                    m_creature->GetMap()->CreatureRelocation(m_creature, PosGroundLive[0], PosGroundLive[1], PosGroundLive[2], PosGroundLive[3]);
                    m_creature->SendMonsterMove(PosGroundLive[0], PosGroundLive[1], PosGroundLive[2], SPLINETYPE_NORMAL, SPLINEFLAG_DONE, 0);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                    SecondPhase = true;
                }
            }
            else
                Summon_Timer -= diff;
        }

        if (SummonDeathCheck_Timer < diff)
        {
            if (!SummonsList.empty())
                for (std::list<uint64>::iterator itr = SummonsList.begin(); itr != SummonsList.end(); ++itr)
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        if (pTemp->isDead())
                        {
                            uint8 SummonLoc = rand()%POS_DEAD;
                            uint64 UndeadEntry = 0;
                            switch (pTemp->GetEntry())
                            {
                                case MOB_LIVE_TRAINEE: UndeadEntry = MOB_DEAD_TRAINEE; break;
                                case MOB_LIVE_KNIGHT:  UndeadEntry = MOB_DEAD_KNIGHT; break;
                                case MOB_LIVE_RIDER:
                                {
                                    UndeadEntry = MOB_DEAD_RIDER;
                                    m_creature->SummonCreature(MOB_DEAD_HORSE, PosSummonDead[SummonLoc][0], PosSummonDead[SummonLoc][1], PosSummonDead[SummonLoc][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                                    break;
                                }
                            }
                            m_creature->SummonCreature(UndeadEntry, PosSummonDead[SummonLoc][0], PosSummonDead[SummonLoc][1], PosSummonDead[SummonLoc][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                            SummonsList.remove(pTemp->GetGUID());
                            break;
                        }

            SummonDeathCheck_Timer = 1000;
        }
        else
            SummonDeathCheck_Timer -= diff;
    }
};

CreatureAI* GetAI_boss_gothik(Creature* pCreature)
{
    return new boss_gothikAI(pCreature);
}

void AddSC_boss_gothik()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gothik";
    newscript->GetAI = &GetAI_boss_gothik;
    newscript->RegisterSelf();
}
