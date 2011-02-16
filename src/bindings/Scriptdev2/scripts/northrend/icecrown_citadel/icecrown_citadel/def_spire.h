/* Copyright (C) 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ICECROWN_SPIRE_H
#define DEF_ICECROWN_SPIRE_H
#include "BSW_ai.h"

enum
{

    TYPE_TELEPORT               = 0,
    TYPE_MARROWGAR              = 1,
    TYPE_DEATHWHISPER           = 2,
    TYPE_FLIGHT_WAR             = 3,
    TYPE_SAURFANG               = 4,
    TYPE_FESTERGUT              = 5,
    TYPE_ROTFACE                = 6,
    TYPE_PUTRICIDE              = 7,
    TYPE_BLOOD_COUNCIL          = 8,
    TYPE_LANATHEL               = 9,
    TYPE_VALITHRIA              = 10,
    TYPE_SINDRAGOSA             = 11,
    TYPE_KINGS_OF_ICC           = 12,
    TYPE_LICH_KING              = 13,
    TYPE_ICECROWN_QUESTS        = 14,
    TYPE_COUNT                  = 15,
    MAX_ENCOUNTERS,

    TYPE_STINKY,
    TYPE_PRECIOUS,

    NPC_LORD_MARROWGAR          = 36612,
    NPC_LADY_DEATHWHISPER       = 36855,
    NPC_DEATHBRINGER_SAURFANG   = 37813,
    NPC_FESTERGUT               = 36626,
    NPC_ROTFACE                 = 36627,
    NPC_PROFESSOR_PUTRICIDE     = 36678,
    NPC_TALDARAM                = 37973,
    NPC_VALANAR                 = 37970,
    NPC_KELESETH                = 37972,
    NPC_LANATHEL                = 37955,
    NPC_VALITHRIA               = 36789,
    NPC_VALITHRIA_QUEST         = 38589,
    NPC_SINDRAGOSA              = 36853,
    NPC_LICH_KING               = 36597,

    NPC_LANATHEL_INTRO          = 38004,
    NPC_BLOOD_ORB_CONTROL       = 38008,

    NPC_MURADIN                 = 36948,

    NPC_TIRION                  = 38995,
    NPC_MENETHIL                = 38579,
    NPC_SPIRIT_WARDEN           = 38579,

    NPC_FROSTMOURNE_TRIGGER     = 38584,
    NPC_FROSTMOURNE_HOLDER      = 27880,

    NPC_STINKY                  = 37025,
    NPC_PRECIOUS                = 37217,

    NPC_RIMEFANG                = 37533,
    NPC_SPINESTALKER            = 37534,

    NPC_COMBAT_TRIGGER          = 38752,

    GO_TELEPORT_GOSSIP_MESSAGE  = 99323,
    TELEPORT_GOSSIP_MESSAGE     = 99322,

    GO_ICEWALL_1                = 201911,
    GO_ICEWALL_2                = 201910,

    GO_ORATORY_DOOR             = 201563,
    GO_DEATHWHISPER_ELEVATOR    = 202220, //5653

    GO_SAURFANG_DOOR            = 201825,

    GO_GAS_RELEASE_VALVE        = 201616, //72479

    GO_ORANGE_PLAGUE            = 201371, //72536
    GO_GREEN_PLAGUE             = 201370, //72537

    GO_SCIENTIST_DOOR_GREEN     = 201614, //72530
    GO_SCIENTIST_DOOR_ORANGE    = 201613, //72531
    GO_SCIENTIST_DOOR_COLLISION = 201612,
    GO_SCIENTIST_DOOR           = 201372, //72541

    GO_BLOODWING_DOOR           = 201920, //72532
    GO_CRIMSON_HALL_DOOR        = 201376, //72532
    GO_COUNCIL_DOOR_1           = 201377, //72533
    GO_COUNCIL_DOOR_2           = 201378, //72534

    GO_FROSTWING_DOOR           = 201919,
    GO_GREEN_DRAGON_DOOR_1      = 201375, //1202
    GO_GREEN_DRAGON_DOOR_2      = 201374, //1200
    GO_VALITHRIA_DOOR_1         = 201380, //1618
    GO_VALITHRIA_DOOR_2         = 201382, //1482
    GO_VALITHRIA_DOOR_3         = 201383, //1335
    GO_VALITHRIA_DOOR_4         = 201381, //1558

    GO_SINDRAGOSA_DOOR_1        = 201369, //1619
    GO_SINDRAGOSA_DOOR_2        = 201379,

    GO_FROZENTRONE_TR           = 202223, //72061

    GO_SAURFANG_CACHE_10        = 202239,
    GO_SAURFANG_CACHE_25        = 202240,
    GO_SAURFANG_CACHE_10_H      = 202238,
    GO_SAURFANG_CACHE_25_H      = 202241,

    GO_GUNSHIP_ARMORY_A_10      = 201872, //
    GO_GUNSHIP_ARMORY_A_25      = 201873, //
    GO_GUNSHIP_ARMORY_A_10H     = 201874, //
    GO_GUNSHIP_ARMORY_A_25H     = 201875, //

    GO_GUNSHIP_ARMORY_H_10      = 202177, //
    GO_GUNSHIP_ARMORY_H_25      = 202178, //
    GO_GUNSHIP_ARMORY_H_10H     = 202179, //
    GO_GUNSHIP_ARMORY_H_25H     = 202180, //

    GO_DREAMWALKER_CACHE_10      = 201959, //
    GO_DREAMWALKER_CACHE_25      = 202339, //
    GO_DREAMWALKER_CACHE_10_H    = 202338, //
    GO_DREAMWALKER_CACHE_25_H    = 202340, //

    GO_PLAGUE_SIGIL             = 202182,
    GO_FROSTWING_SIGIL          = 202181,
    GO_BLOODWING_SIGIL          = 202183,

    GO_ICESHARD_1               = 202142, //8304
    GO_ICESHARD_2               = 202141, //8364
    GO_ICESHARD_3               = 202143, //8310
    GO_ICESHARD_4               = 202144, //9007

    GO_FROSTY_WIND              = 202188, //
    GO_FROSTY_EDGE              = 202189, //
    GO_SNOW_EDGE                = 202190, //
    GO_ARTHAS_PLATFORM          = 202161, //
    GO_ARTHAS_PRECIPICE         = 202078, //

    TYPE_EVENT_TIMER            = 99,
    TYPE_EVENT                  = 100,
    TYPE_EVENT_NPC              = 101,
    MAP_NUM                     = 631,
    DATA_DIRECTION              = 1001,
    DATA_BLOOD_INVOCATION       = 1002,
    DESPAWN_TIME                = 300000,

};

class MANGOS_DLL_DECL instance_icecrown_spire : public ScriptedInstance
{
public:
    instance_icecrown_spire(Map* pMap);
    ~instance_icecrown_spire() {}

    void Initialize();

    void OnObjectCreate(GameObject* pGo);
    void OnCreatureCreate(Creature* pCreature);

    void OpenDoor(uint64 guid);
    void CloseDoor(uint64 guid);
    void OpenAllDoors();
    void OnPlayerEnter(Player* pPlayer);
    bool IsEncounterInProgress();
    uint32 GetCompletedEncounters(bool /*type*/);

    void SetData(uint32 uiType, uint32 uiData);
    uint32 GetData(uint32 uiType);
    uint64 GetData64(uint32 uiType);

    const char* Save() { return strSaveData.c_str(); }
    void Load(const char* chrIn);

private:

    uint8 Difficulty;
    bool needSave;
    std::string strSaveData;

    //Creatures GUID
    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
    uint64 m_uiMarrogwarGUID;
    uint64 m_uiDeathWhisperGUID;
    uint64 m_uiSaurfangGUID;
    uint64 m_uiRotfaceGUID;
    uint64 m_uiFestergutGUID;
    uint64 m_uiPutricideGUID;
    uint64 m_uiTaldaramGUID;
    uint64 m_uiValanarGUID;
    uint64 m_uiKelesethGUID;
    uint64 m_uiLanathelGUID;
    uint64 m_uiLanathelintroGUID;
    uint64 m_uiValithriaGUID;
    uint64 m_uiValithriaQuestGUID;
    uint64 m_uiSindragosaGUID;
    uint64 m_uiLichKingGUID;
    uint64 m_uiGasReleaseValveGUID;
    uint64 m_uiBloodOrbCtrlGUID;

    uint64 m_uiRimefangGUID;
    uint64 m_uiSpinestalkerGUID;

    uint64 m_uiStinkyGUID;
    uint64 m_uiPreciousGUID;

    uint64 m_uidummyTargetGUID;

    uint64 m_uiIcewall1GUID;
    uint64 m_uiIcewall2GUID;
    uint64 m_uiSaurfangDoorGUID;
    uint64 m_uiOratoryDoorGUID;
    uint64 m_uiDeathWhisperElevatorGUID;
    uint64 m_uiOrangePlagueGUID;
    uint64 m_uiGreenPlagueGUID;
    uint64 m_uiSDoorGreenGUID;
    uint64 m_uiSDoorOrangeGUID;
    uint64 m_uiSDoorCollisionGUID;
    uint64 m_uiScientistDoorGUID;
    uint64 m_uiCrimsonDoorGUID;
    uint64 m_uiBloodwingDoorGUID;
    uint64 m_uiCounsilDoor1GUID;
    uint64 m_uiCounsilDoor2GUID;
    uint64 m_uiGreenDragonDoor1GUID;
    uint64 m_uiGreenDragonDoor2GUID;
    uint64 m_uiFrostwingDoorGUID;

    uint64 m_uiValithriaDoor1GUID;
    uint64 m_uiValithriaDoor2GUID;
    uint64 m_uiValithriaDoor3GUID;
    uint64 m_uiValithriaDoor4GUID;

    uint64 m_uiSindragosaDoor1GUID;
    uint64 m_uiSindragosaDoor2GUID;

    uint64 m_uiIceShard1GUID;
    uint64 m_uiIceShard2GUID;
    uint64 m_uiIceShard3GUID;
    uint64 m_uiIceShard4GUID;

    uint64 m_uiFrostyWindGUID;
    uint64 m_uiFrostyEdgeGUID;
    uint64 m_uiSnowEdgeGUID;
    uint64 m_uiArthasPlatformGUID;
    uint64 m_uiArthasPrecipiceGUID;

    uint64 m_uiFrostmourneGUID;
    uint64 m_uiFrostmourneTriggerGUID;
    uint64 m_uiFrostmourneHolderGUID;

    uint64 m_uiSaurfangCacheGUID;
    uint64 m_uiGunshipArmoryAGUID;
    uint64 m_uiGunshipArmoryHGUID;
    uint64 m_uiValitriaCacheGUID;

    uint64 m_uiGunshipArmoryH_ID;
    uint64 m_uiGunshipArmoryA_ID;

    uint32 m_uiCouncilInvocation;

    uint32 m_auiEvent;
    uint32 m_auiEventTimer;
    uint32 m_uiDirection;

    uint32 m_uiStinkystate;
    uint32 m_uiPreciousstate;

};

#endif
