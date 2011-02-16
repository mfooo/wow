/* Copyright (C) 2010 - 2011 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_ragefire
SD%Complete: 90%
SDComment: by notagain && /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

enum BossSpells
{
    SPELL_ENRAGE                     = 78722, //soft enrage + fire nova
    SPELL_FLAME_BREATH               = 74404,
    SPELL_BEACON                     = 74453, //mark for conflag, in enter to fly phase, 2 in 10, 5 in 25
    SPELL_CONFLAGATION               = 74452, // after fly up
    SPELL_CONFLAGATION_1             = 74455, // Triggered?
    SPELL_CONFLAGATION_2             = 74456, // Aura

    MAX_BEACON_TARGETS               = 5,
};

static Locations SpawnLoc[]=
{
    {3151.3898f, 636.8519f, 78.7396f},    // 0 Saviana start point
    {3149.635f, 668.9644f, 90.507f},    // 1 Saviana fly phase, o=4,69
};

struct MANGOS_DLL_DECL boss_ragefireAI : public BSWScriptedAI
{
    boss_ragefireAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 nextPoint;
    Unit* marked[MAX_BEACON_TARGETS];
    bool MovementStarted;
    bool conflagated;

    void Reset()
    {
        if(!pInstance)
            return;
        m_creature->SetRespawnDelay(7*DAY);
        if (m_creature->isAlive()) pInstance->SetData(TYPE_RAGEFIRE, NOT_STARTED);
        resetTimers();
        setStage(0);
        nextPoint = 0;
        conflagated = false;
        for (uint8 i = 0; i < MAX_BEACON_TARGETS; ++i)
            marked[i] = NULL;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (!pInstance) return;

        if (type != POINT_MOTION_TYPE || !MovementStarted) return;

        if (id == nextPoint) {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                }
    }

    void SetFly(bool command = false)
    {
        if (command)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_FLY_SIT_GROUND_UP);
            m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        }
        else
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
            m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);
        }
    }

    void StartMovement(uint32 id)
    {
        nextPoint = id;
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(id, SpawnLoc[id].x, SpawnLoc[id].y, SpawnLoc[id].z);
        MovementStarted = true;
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666401,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666402,m_creature,pVictim);
               break;
        };
    }

    void JustReachedHome()
    {
        if (pInstance)
            pInstance->SetData(TYPE_RAGEFIRE, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;

        pInstance->SetData(TYPE_RAGEFIRE, IN_PROGRESS);
        m_creature->SetInCombatWithZone();
        DoScriptText(-1666400,m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;

        pInstance->SetData(TYPE_RAGEFIRE, DONE);
        DoScriptText(-1666403,m_creature);
    }

    void doBeacon(bool command = false)
    {
        if (command)
        {
             for(uint8 i = 0; i < getSpellData(SPELL_BEACON); ++i)
             {
                if (Unit* pTarget = doSelectRandomPlayer(SPELL_BEACON, false, 100.0f))
                {
                    if (doCast(SPELL_BEACON, pTarget) == CAST_OK)
                        marked[i] = pTarget;
                    else marked[i] = NULL;
                }
             }
             conflagated = true;
        }
        else
        {
             m_creature->InterruptNonMeleeSpells(true);
             for(uint8 i = 0; i < getSpellData(SPELL_BEACON); ++i)
             {
                if (marked[i])
                    doCast(SPELL_CONFLAGATION_2, marked[i]);
                marked[i] = NULL;
             }
             doCast(SPELL_CONFLAGATION_1);
             conflagated = false;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (getStage())
        {
            case 0: //GROUND
                 timedCast(SPELL_FLAME_BREATH, diff);
                 timedCast(SPELL_ENRAGE, diff);
                 if ( m_creature->GetHealthPercent() <= 80.0f) setStage(1);
                 break;

            case 1: //Air phase start
                 SetCombatMovement(false);
                 m_creature->InterruptNonMeleeSpells(true);
                 SetFly(true);
                 doBeacon(true);
                 StartMovement(1);
                 setStage(2);
                 break;

            case 2: // Wait for movement
                 if (MovementStarted) return;
                 doCast(SPELL_CONFLAGATION);
                 DoScriptText(-1666404,m_creature);
                 setStage(3);
                 break;

            case 3: // Wait for cast finish
                 if (!m_creature->IsNonMeleeSpellCasted(false))
                 {
                     doBeacon(false);
                     setStage(4);
                 };
                 break;

            case 4: // Air phase
                 timedCast(SPELL_FLAME_BREATH, diff);
                 if (timedQuery(SPELL_BEACON, diff))
                     {
                         doBeacon(true);
                         doCast(SPELL_CONFLAGATION);
                     };
                 if (conflagated && timedQuery(SPELL_CONFLAGATION_1, diff))
                     {
                         doBeacon(false);
                     };
                 if ( m_creature->GetHealthPercent() <= 60.0f) setStage(5);
                 break;

            case 5: //Air phase end
                 StartMovement(0);
                 setStage(6);
                 break;

            case 6: // Wait for movement
                 if (MovementStarted) return;
                 SetFly(false);
                 SetCombatMovement(true);
                 m_creature->GetMotionMaster()->Clear();
                 m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                 setStage(7);
                 break;

            case 7: //GROUND
                 timedCast(SPELL_FLAME_BREATH, diff);
                 timedCast(SPELL_ENRAGE, diff);
                 if ( m_creature->GetHealthPercent() <= 40.0f) setStage(8);
                 break;

            case 8: //Air phase start
                 SetCombatMovement(false);
                 m_creature->InterruptNonMeleeSpells(true);
                 SetFly(true);
                 doBeacon(true);
                 StartMovement(1);
                 setStage(9);
                 break;

            case 9: // Wait for movement
                 if (MovementStarted) return;
                 doCast(SPELL_CONFLAGATION);
                 DoScriptText(-1666404,m_creature);
                 setStage(10);
                 break;

            case 10: // Wait for cast finish
                 if (!m_creature->IsNonMeleeSpellCasted(false))
                 {
                     doBeacon(false);
                     setStage(11);
                 };
                 break;

            case 11: // Air phase
                 timedCast(SPELL_FLAME_BREATH, diff);
                 if (timedQuery(SPELL_BEACON, diff))
                     {
                         doBeacon(true);
                         doCast(SPELL_CONFLAGATION);
                     };
                 if (conflagated && timedQuery(SPELL_CONFLAGATION_1, diff))
                     {
                         doBeacon(false);
                     };
                 if ( m_creature->GetHealthPercent() <= 20.0f) setStage(12);
                 break;

            case 12: //Air phase end
                 StartMovement(0);
                 setStage(13);
                 break;

            case 13: // Wait for movement
                 if (MovementStarted) return;
                 SetFly(false);
                 SetCombatMovement(true);
                 m_creature->GetMotionMaster()->Clear();
                 m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                 setStage(14);
                 break;

            case 14: //GROUND
                 timedCast(SPELL_FLAME_BREATH, diff);
                 timedCast(SPELL_ENRAGE, diff*2);
                 break;

            default:
                break;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ragefire(Creature* pCreature)
{
    return new boss_ragefireAI(pCreature);
}

void AddSC_boss_ragefire()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ragefire";
    newscript->GetAI = &GetAI_boss_ragefire;
    newscript->RegisterSelf();
}
