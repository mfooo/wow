/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
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

#ifndef __InstanceSaveMgr_H
#define __InstanceSaveMgr_H

#include "Common.h"
#include "Platform/Define.h"
#include "Policies/Singleton.h"
#include "ace/Thread_Mutex.h"
#include <list>
#include <map>
#include "Database/DatabaseEnv.h"
#include "DBCEnums.h"
#include "ObjectGuid.h"

struct InstanceTemplate;
struct MapEntry;
struct MapDifficulty;
class Player;
class Group;
class InstanceSaveManager;

// Instance Reset Schedule is calculated from this point in time.
// 2005-12-28 10:00:00 - 10:00:00 = 2005-12-28 00:00:00
// We will add X hours to this value, taking X from config (10 default).
#define INSTANCE_RESET_SCHEDULE_START_TIME  1135717200

/*
    Holds the information necessary for creating a new map for an existing instance
    Is referenced in three cases:
    - player-instance binds for solo players (not in group)
    - player-instance binds for permanent heroic/raid saves
    - group-instance binds (both solo and permanent) cache the player binds for the group leader
*/
class InstanceSave
{
    friend class InstanceSaveManager;
    public:
        /* Created either when:
           - any new instance is being generated
           - the first time a player bound to InstanceId logs in
           - when a group bound to the instance is loaded */
        InstanceSave(uint16 MapId, uint32 InstanceId, Difficulty difficulty, time_t resetTime, bool canReset);

        /* Unloaded when m_playerList and m_groupList become empty
           or when the instance is reset */
        ~InstanceSave();

        uint8 GetPlayerCount() const { return m_playerList.size(); }
        uint8 GetGroupCount() const { return m_groupList.size(); }

        /* A map corresponding to the InstanceId/MapId does not always exist.
        InstanceSave objects may be created on player logon but the maps are
        created and loaded only when a player actually enters the instance. */
        uint32 GetInstanceId() const { return m_instanceid; }
        ObjectGuid GetInstanceGuid() const { return ObjectGuid(HIGHGUID_INSTANCE, GetInstanceId()); }
        uint32 GetMapId() const { return m_mapid; }

        /* Saved when the instance is generated for the first time */
        void SaveToDB();
        /* When the instance is being reset (permanently deleted) */
        void DeleteFromDB();

        /* for normal instances this corresponds to max(creature respawn time) + X hours
           for raid/heroic instances this caches the global respawn time for the map */
        time_t GetResetTime() const { return m_resetTime; }
        void SetResetTime(time_t resetTime) { m_resetTime = resetTime; }
        time_t GetResetTimeForDB() const;

        InstanceTemplate const* GetTemplate() const;
        MapEntry const* GetMapEntry() const;

        /* online players bound to the instance (perm/solo)
           does not include the members of the group unless they have permanent saves */
        void AddPlayer(Player *player) { m_playerList.push_back(player); }
        bool RemovePlayer(Player *player) { m_playerList.remove(player); return UnloadIfEmpty(); }
        /* all groups bound to the instance */
        void AddGroup(Group *group) { m_groupList.push_back(group); }
        bool RemoveGroup(Group *group) { m_groupList.remove(group); return UnloadIfEmpty(); }

        /* instances cannot be reset (except at the global reset time)
           if there are players permanently bound to it
           this is cached for the case when those players are offline */
        bool CanReset() const { return m_canReset; }
        void SetCanReset(bool canReset) { m_canReset = canReset; }

        /* currently it is possible to omit this information from this structure
           but that would depend on a lot of things that can easily change in future */
        Difficulty GetDifficulty() const { return m_difficulty; }

        void SetUsedByMapState(bool state)
        {
            m_usedByMap = state;
            if (!state)
                UnloadIfEmpty();
        }

        void DeleteRespawnTimes();
        time_t GetCreatureRespawnTime(uint32 loguid) const
        {
            RespawnTimes::const_iterator itr = m_creatureRespawnTimes.find(loguid);
            return itr != m_creatureRespawnTimes.end() ? itr->second : 0;
        }
        void SaveCreatureRespawnTime(uint32 loguid, time_t t);
        time_t GetGORespawnTime(uint32 loguid) const
        {
            RespawnTimes::const_iterator itr = m_goRespawnTimes.find(loguid);
            return itr != m_goRespawnTimes.end() ? itr->second : 0;
        }
        void SaveGORespawnTime(uint32 loguid, time_t t);

    private:
        void SetCreatureRespawnTime(uint32 loguid, time_t t);
        void SetGORespawnTime(uint32 loguid, time_t t);

    private:
        typedef UNORDERED_MAP<uint32, time_t> RespawnTimes;
        typedef std::list<Player*> PlayerListType;
        typedef std::list<Group*> GroupListType;

        bool UnloadIfEmpty();
        /* the only reason the instSave-object links are kept is because
           the object-instSave links need to be broken at reset time
           TODO: maybe it's enough to just store the number of players/groups */
        PlayerListType m_playerList;                        // lock InstanceSave from unload
        GroupListType m_groupList;                          // lock InstanceSave from unload
        time_t m_resetTime;
        uint32 m_instanceid;
        uint32 m_mapid;
        Difficulty m_difficulty;
        bool m_canReset;
        bool m_usedByMap;                                   // true when instance map loaded, lock InstanceSave from unload

        // persistent data
        RespawnTimes m_creatureRespawnTimes;                // // lock InstanceSave from unload, for example for temporary bound dungeon unload delay
        RespawnTimes m_goRespawnTimes;                      // lock InstanceSave from unload, for example for temporary bound dungeon unload delay
};

enum ResetEventType
{
    RESET_EVENT_DUNGEON      = 0,                           // no fixed reset time
    RESET_EVENT_INFORM_1     = 1,                           // raid/heroic warnings
    RESET_EVENT_INFORM_2     = 2,
    RESET_EVENT_INFORM_3     = 3,
    RESET_EVENT_INFORM_LAST  = 4,
};

#define MAX_RESET_EVENT_TYPE   5

/* resetTime is a global propery of each (raid/heroic) map
    all instances of that map reset at the same time */
struct InstanceResetEvent
{
    ResetEventType type   :8;                               // if RESET_EVENT_DUNGEON then InstanceID == 0 and applied to all instances for pair (map,diff)
    Difficulty difficulty :8;                               // used with mapid used as for select reset for global cooldown instances (instamceid==0 for event)
    uint16 mapid;
    uint32 instanceId;                                      // used for select reset for normal dungeons

    InstanceResetEvent() : type(RESET_EVENT_DUNGEON), difficulty(DUNGEON_DIFFICULTY_NORMAL), mapid(0), instanceId(0) {}
    InstanceResetEvent(ResetEventType t, uint32 _mapid, Difficulty d, uint32 _instanceid)
        : type(t), difficulty(d), mapid(_mapid), instanceId(_instanceid) {}
    bool operator == (const InstanceResetEvent& e) { return e.mapid == mapid && e.difficulty == difficulty && e.instanceId == instanceId; }
};

class InstanceResetScheduler
{
    public:                                                 // constructors
        explicit InstanceResetScheduler(InstanceSaveManager& mgr) : m_InstanceSaves(mgr) {}
        void LoadResetTimes();

    public:                                                 // accessors
        time_t GetResetTimeFor(uint32 mapid, Difficulty d) const
        {
            ResetTimeByMapDifficultyMap::const_iterator itr  = m_resetTimeByMapDifficulty.find(MAKE_PAIR32(mapid,d));
            return itr != m_resetTimeByMapDifficulty.end() ? itr->second : 0;
        }

        static uint32 GetMaxResetTimeFor(MapDifficulty const* mapDiff);
        static time_t CalculateNextResetTime(MapDifficulty const* mapDiff, time_t prevResetTime);
    public:                                                 // modifiers
        void SetResetTimeFor(uint32 mapid, Difficulty d, time_t t)
        {
            m_resetTimeByMapDifficulty[MAKE_PAIR32(mapid,d)] = t;
        }

        void ScheduleReset(bool add, time_t time, InstanceResetEvent event);

        void Update();

    private:                                                // fields
        InstanceSaveManager& m_InstanceSaves;


        // fast lookup for reset times (always use existing functions for access/set)
        typedef UNORDERED_MAP<uint32 /*PAIR32(map,difficulty)*/,time_t /*resetTime*/> ResetTimeByMapDifficultyMap;
        ResetTimeByMapDifficultyMap m_resetTimeByMapDifficulty;

        typedef std::multimap<time_t /*resetTime*/, InstanceResetEvent> ResetTimeQueue;
        ResetTimeQueue m_resetTimeQueue;
};

class MANGOS_DLL_DECL InstanceSaveManager : public MaNGOS::Singleton<InstanceSaveManager, MaNGOS::ClassLevelLockable<InstanceSaveManager, ACE_Thread_Mutex> >
{
    friend class InstanceResetScheduler;
    public:
        InstanceSaveManager();
        ~InstanceSaveManager();

        void CleanupInstances();
        void PackInstances();

        void LoadCreatureRespawnTimes();
        void LoadGameobjectRespawnTimes();

        InstanceResetScheduler& GetScheduler() { return m_Scheduler; }

        InstanceSave* AddInstanceSave(uint32 mapId, uint32 instanceId, Difficulty difficulty, time_t resetTime, bool canReset, bool load = false);
        InstanceSave *GetInstanceSave(uint32 mapId, uint32 InstanceId);
        void RemoveInstanceSave(uint32 mapId, uint32 instanceId);
        static void DeleteInstanceFromDB(uint32 instanceid);

        /* statistics */
        uint32 GetNumInstanceSaves() { return m_instanceSaveByInstanceId.size() + m_instanceSaveByMapId.size(); }
        uint32 GetNumBoundPlayersTotal();
        uint32 GetNumBoundGroupsTotal();

        void Update() { m_Scheduler.Update(); }
    private:
        typedef UNORDERED_MAP<uint32 /*InstanceId or MapId*/, InstanceSave*> InstanceSaveHashMap;

        //  called by scheduler
        void _ResetOrWarnAll(uint32 mapid, Difficulty difficulty, bool warn, uint32 timeleft);
        void _ResetInstance(uint32 mapid, uint32 instanceId);
        void _CleanupExpiredInstancesAtTime(time_t t);

        void _ResetSave(InstanceSaveHashMap& holder, InstanceSaveHashMap::iterator &itr);
        void _DelHelper(DatabaseType &db, const char *fields, const char *table, const char *queryTail,...);

        // used during global instance resets
        bool lock_instLists;
        // fast lookup by instance id for instanceable maps
        InstanceSaveHashMap m_instanceSaveByInstanceId;
        // fast lookup by map id for non-instanceable maps
        InstanceSaveHashMap m_instanceSaveByMapId;

        InstanceResetScheduler m_Scheduler;
};

#define sInstanceSaveMgr MaNGOS::Singleton<InstanceSaveManager>::Instance()
#endif
