/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_ESCORTAI_H
#define SC_ESCORTAI_H

#include "../system/system.h"

struct Escort_Waypoint
{
    Escort_Waypoint(uint32 uiId, float fX, float fY, float fZ, uint32 uiWaitTime) :
        uiId(uiId),
        fX(fX),
        fY(fY),
        fZ(fZ),
        uiWaitTime(uiWaitTime)
    {}

    uint32 uiId;
    float  fX;
    float  fY;
    float  fZ;
    uint32 uiWaitTime;
};

enum eEscortState
{
    STATE_ESCORT_NONE       = 0x000,                        //nothing in progress
    STATE_ESCORT_ESCORTING  = 0x001,                        //escort are in progress
    STATE_ESCORT_RETURNING  = 0x002,                        //escort is returning after being in combat
    STATE_ESCORT_PAUSED     = 0x004                         //will not proceed with waypoints before state is removed
};

struct MANGOS_DLL_DECL npc_escortAI : public ScriptedAI
{
    public:
        explicit npc_escortAI(Creature* pCreature);
        ~npc_escortAI() {}

        virtual void Aggro(Unit*);

        virtual void Reset() = 0;

        // CreatureAI functions
        bool IsVisible(Unit*) const;

        void AttackStart(Unit*);

        void EnterCombat(Unit*);

        void MoveInLineOfSight(Unit*);

        void JustDied(Unit*);

        void JustRespawned();

        void EnterEvadeMode();

        void UpdateAI(const uint32);                        //the "internal" update, calls UpdateEscortAI()
        virtual void UpdateEscortAI(const uint32);          //used when it's needed to add code in update (abilities, scripted events, etc)

        void MovementInform(uint32, uint32);

        // EscortAI functions
        //void AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs = 0);

        virtual void WaypointReached(uint32 uiPointId) = 0;
        virtual void WaypointStart(uint32 uiPointId) {}

        void Start(bool bRun = false, uint64 uiPlayerGUID = 0, const Quest* pQuest = NULL, bool bInstantRespawn = false, bool bCanLoopPath = false);

        void SetRun(bool bRun = true);
        void SetEscortPaused(bool uPaused);

        bool HasEscortState(uint32 uiEscortState) { return (m_uiEscortState & uiEscortState); }

        // update current point
        void SetCurrentWaypoint(uint32 uiPointId);

    protected:
        Player* GetPlayerForEscort() { return m_creature->GetMap()->GetPlayer(m_uiPlayerGUID); }
        virtual void JustStartedEscort() {}

    private:
        bool AssistPlayerInCombat(Unit* pWho);
        bool IsPlayerOrGroupInRange();
        void FillPointMovementListForCreature();

        void AddEscortState(uint32 uiEscortState) { m_uiEscortState |= uiEscortState; }
        void RemoveEscortState(uint32 uiEscortState) { m_uiEscortState &= ~uiEscortState; }

        uint64 m_uiPlayerGUID;
        uint32 m_uiWPWaitTimer;
        uint32 m_uiPlayerCheckTimer;
        uint32 m_uiEscortState;

        const Quest* m_pQuestForEscort;                     //generally passed in Start() when regular escort script.

        std::list<Escort_Waypoint> WaypointList;
        std::list<Escort_Waypoint>::iterator CurrentWP;

        bool m_bIsRunning;                                  //all creatures are walking by default (has flag SPLINEFLAG_WALKMODE)
        bool m_bCanInstantRespawn;                          //if creature should respawn instantly after escort over (if not, database respawntime are used)
        bool m_bCanReturnToStart;                           //if creature can walk same path (loop) without despawn. Not for regular escort quests.
};
#endif
