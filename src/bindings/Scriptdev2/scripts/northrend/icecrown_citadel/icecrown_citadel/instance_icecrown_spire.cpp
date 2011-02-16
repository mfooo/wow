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
SDName: instance_icecrown_spire
SD%Complete: 90%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"
#include "World.h"

static Locations SpawnLoc[]=
{
    {-446.788971f, 2003.362915f, 191.233948f},  // 0 Horde ship enter
    {-428.140503f, 2421.336914f, 191.233078f},  // 1 Alliance ship enter
};

    instance_icecrown_spire::instance_icecrown_spire(Map* pMap) : ScriptedInstance(pMap) 
    {
        Difficulty = pMap->GetDifficulty();
        Initialize();
    }

    void instance_icecrown_spire::OpenDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
    }

    void instance_icecrown_spire::CloseDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_READY);
    }

    void instance_icecrown_spire::OpenAllDoors()
    {
        if (m_auiEncounter[1] == DONE) {
                                        OpenDoor(m_uiIcewall1GUID);
                                        OpenDoor(m_uiIcewall2GUID);
                                        OpenDoor( m_uiOratoryDoorGUID);
                                        };
        if (m_auiEncounter[2] == DONE) {
                        if (GameObject* pGO = instance->GetGameObject(m_uiDeathWhisperElevatorGUID))
                            {
                              pGO->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                              pGO->SetGoState(GO_STATE_READY);
                            }
                                       };
        if (m_auiEncounter[4] == DONE) {
                                        OpenDoor(m_uiSaurfangDoorGUID);
                                        OpenDoor(m_uiBloodwingDoorGUID);
                                        OpenDoor(m_uiFrostwingDoorGUID);
                                        };
        if (m_auiEncounter[5] == DONE) OpenDoor(m_uiSDoorOrangeGUID);
        if (m_auiEncounter[6] == DONE) OpenDoor(m_uiSDoorGreenGUID);
        if (m_auiEncounter[6] == DONE && m_auiEncounter[5] == DONE) OpenDoor(m_uiSDoorCollisionGUID);
        if (m_auiEncounter[8] == DONE) {
                                        OpenDoor(m_uiCounsilDoor1GUID);
                                        OpenDoor(m_uiCounsilDoor2GUID);
                                        };
        if (m_auiEncounter[10] == DONE) 
        {
            OpenDoor(m_uiValithriaDoor2GUID);
            OpenDoor(m_uiSindragosaDoor2GUID);
            OpenDoor(m_uiSindragosaDoor1GUID);
        };

    }

    void instance_icecrown_spire::Initialize()
    {
        for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_auiEncounter[0] = 0;

        m_uiMarrogwarGUID = 0;
        m_uiDeathWhisperGUID = 0;
        m_uiSaurfangGUID = 0;
        m_uiSaurfangCacheGUID = 0;
        m_uiGunshipArmoryAGUID = 0;
        m_uiGunshipArmoryHGUID = 0;
        m_uiIcewall1GUID = 0;
        m_uiIcewall2GUID = 0;
        m_uiSDoorOrangeGUID = 0;
        m_uiSDoorGreenGUID = 0;
        m_uiBloodwingDoorGUID = 0;
        m_uiSDoorCollisionGUID = 0;
        m_auiEvent = 0;
        m_auiEventTimer = 1000;
        m_uiCouncilInvocation = 0;
        m_uiDirection = 0;
        m_uiStinkystate = NOT_STARTED;
        m_uiPreciousstate = NOT_STARTED;

        switch (Difficulty) {
                             case RAID_DIFFICULTY_10MAN_NORMAL:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_10;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_10;
                                       break;
                             case RAID_DIFFICULTY_10MAN_HEROIC:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_10H;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_10H;
                                       break;
                             case RAID_DIFFICULTY_25MAN_NORMAL:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_25;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_25;
                                       break;
                             case RAID_DIFFICULTY_25MAN_HEROIC:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_25H;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_25H;
                                       break;
                             default:
                                       m_uiGunshipArmoryH_ID = 0;
                                       m_uiGunshipArmoryA_ID = 0;
                                       break;
                             };
    }

    bool instance_icecrown_spire::IsEncounterInProgress()
    {
        for(uint8 i = 1; i < MAX_ENCOUNTERS-2 ; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    uint32 instance_icecrown_spire::GetCompletedEncounters(bool /*type*/)
    {
        uint32 count = 0;
        uint32 mask = 1;
        for(uint8 i = 1; i < MAX_ENCOUNTERS-2 ; ++i)
        {
            if (m_auiEncounter[i] == DONE)
                count += mask;
            mask = mask << 1;
        }

        return count;
    }

    void instance_icecrown_spire::OnPlayerEnter(Player *pPlayer)
    {
        OpenAllDoors();

    enum PhaseControl
    {
        HORDE_CONTROL_PHASE_SHIFT_1    = 55773,
        HORDE_CONTROL_PHASE_SHIFT_2    = 60028,
        ALLIANCE_CONTROL_PHASE_SHIFT_1 = 55774,
        ALLIANCE_CONTROL_PHASE_SHIFT_2 = 60027,
    };
/*

        if (!sWorld.getConfig(CONFIG_BOOL_ALLOW_TWO_SIDE_INTERACTION_GROUP)) return;

        switch (pPlayer->GetTeam())
        {
            case ALLIANCE:
                  if (pPlayer && pPlayer->IsInWorld() && pPlayer->HasAura(HORDE_CONTROL_PHASE_SHIFT_1))
                      pPlayer->RemoveAurasDueToSpell(HORDE_CONTROL_PHASE_SHIFT_1);
                  pPlayer->CastSpell(pPlayer, HORDE_CONTROL_PHASE_SHIFT_2, false);
                  break;
            case HORDE:
                  if (pPlayer && pPlayer->IsInWorld() && pPlayer->HasAura(ALLIANCE_CONTROL_PHASE_SHIFT_1)) 
                      pPlayer->RemoveAurasDueToSpell(ALLIANCE_CONTROL_PHASE_SHIFT_1);
                  pPlayer->CastSpell(pPlayer, ALLIANCE_CONTROL_PHASE_SHIFT_2, false);
                  break;
        };
*/
    };

    void instance_icecrown_spire::OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_LORD_MARROWGAR: 
                         m_uiMarrogwarGUID = pCreature->GetGUID();
                         break;
            case NPC_LADY_DEATHWHISPER: 
                          m_uiDeathWhisperGUID = pCreature->GetGUID();
                          break;
            case NPC_DEATHBRINGER_SAURFANG: 
                          m_uiSaurfangGUID = pCreature->GetGUID();
                          break;
            case NPC_FESTERGUT:
                          m_uiFestergutGUID = pCreature->GetGUID();
                          break;
            case NPC_ROTFACE:
                          m_uiRotfaceGUID = pCreature->GetGUID();
                          break;
            case NPC_PROFESSOR_PUTRICIDE:
                          m_uiPutricideGUID = pCreature->GetGUID();
                          break;
            case NPC_TALDARAM:
                          m_uiTaldaramGUID = pCreature->GetGUID();
                          break;
            case NPC_VALANAR:
                          m_uiValanarGUID = pCreature->GetGUID();
                          break;
            case NPC_KELESETH:
                          m_uiKelesethGUID = pCreature->GetGUID();
                          break;
            case NPC_LANATHEL:
                          m_uiLanathelGUID = pCreature->GetGUID();
                          break;
            case NPC_LANATHEL_INTRO:
                          m_uiLanathelintroGUID = pCreature->GetGUID();
                          break;
            case NPC_VALITHRIA:
                          m_uiValithriaGUID = pCreature->GetGUID();
                          break;
            case NPC_VALITHRIA_QUEST:
                          m_uiValithriaQuestGUID = pCreature->GetGUID();
                          break;
            case NPC_SINDRAGOSA:
                          m_uiSindragosaGUID = pCreature->GetGUID();
                          break;
            case NPC_LICH_KING:
                          m_uiLichKingGUID = pCreature->GetGUID();
                          break;
            case NPC_RIMEFANG:
                          m_uiRimefangGUID = pCreature->GetGUID();
                          break;
            case NPC_SPINESTALKER:
                          m_uiSpinestalkerGUID = pCreature->GetGUID();
                          break;
            case NPC_STINKY:
                          m_uiStinkyGUID = pCreature->GetGUID();
                          break;
            case NPC_PRECIOUS:
                          m_uiPreciousGUID = pCreature->GetGUID();
                          break;
            case NPC_COMBAT_TRIGGER:
                          m_uidummyTargetGUID = pCreature->GetGUID();
                          break;
            case NPC_FROSTMOURNE_TRIGGER:
                          m_uiFrostmourneTriggerGUID = pCreature->GetGUID(); break;
            case NPC_FROSTMOURNE_HOLDER:
                          m_uiFrostmourneHolderGUID = pCreature->GetGUID(); break;
        }
    }

    void instance_icecrown_spire::OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_ICEWALL_1: 
                         m_uiIcewall1GUID = pGo->GetGUID();
                         break;
            case GO_ICEWALL_2: 
                         m_uiIcewall2GUID = pGo->GetGUID();
                         break;
            case GO_ORATORY_DOOR: 
                         m_uiOratoryDoorGUID = pGo->GetGUID();
                         break;
            case GO_DEATHWHISPER_ELEVATOR:
                         m_uiDeathWhisperElevatorGUID = pGo->GetGUID();
                         break;
            case GO_SAURFANG_DOOR: 
                         m_uiSaurfangDoorGUID = pGo->GetGUID();
                         break;
            case GO_ORANGE_PLAGUE: 
                         m_uiOrangePlagueGUID = pGo->GetGUID();
                         break;
            case GO_GREEN_PLAGUE: 
                         m_uiGreenPlagueGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR_GREEN: 
                         m_uiSDoorGreenGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR_ORANGE: 
                         m_uiSDoorOrangeGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR_COLLISION: 
                         m_uiSDoorCollisionGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR:
                         m_uiScientistDoorGUID = pGo->GetGUID();
                         break;
            case GO_CRIMSON_HALL_DOOR: 
                         m_uiCrimsonDoorGUID = pGo->GetGUID();
                         break;
            case GO_BLOODWING_DOOR: 
                         m_uiBloodwingDoorGUID = pGo->GetGUID();
                         break;
            case GO_COUNCIL_DOOR_1: 
                         m_uiCounsilDoor1GUID = pGo->GetGUID();
                         break;
            case GO_COUNCIL_DOOR_2: 
                         m_uiCounsilDoor2GUID = pGo->GetGUID();
                         break;
            case GO_FROSTWING_DOOR: 
                         m_uiFrostwingDoorGUID = pGo->GetGUID();
                         break;
            case GO_GREEN_DRAGON_DOOR_1: 
                         m_uiGreenDragonDoor1GUID = pGo->GetGUID();
                         break;
            case GO_GREEN_DRAGON_DOOR_2: 
                         m_uiGreenDragonDoor2GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_1: 
                         m_uiValithriaDoor1GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_2: 
                         m_uiValithriaDoor2GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_3: 
                         m_uiValithriaDoor3GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_4: 
                         m_uiValithriaDoor4GUID = pGo->GetGUID();
                         break;
            case GO_SINDRAGOSA_DOOR_1: 
                         m_uiSindragosaDoor1GUID = pGo->GetGUID();
                         break;
            case GO_SINDRAGOSA_DOOR_2: 
                         m_uiSindragosaDoor2GUID = pGo->GetGUID();
                         break;
            case GO_SAURFANG_CACHE_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID(); 
                                  break;
            case GO_SAURFANG_CACHE_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_SAURFANG_CACHE_10_H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_SAURFANG_CACHE_25_H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_A_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_A_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_A_10H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_A_25H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_H_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_H_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_H_10H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_H_25H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID(); 
                                  break;
            case GO_DREAMWALKER_CACHE_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiValitriaCacheGUID = pGo->GetGUID(); 
                                  break;
            case GO_DREAMWALKER_CACHE_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiValitriaCacheGUID = pGo->GetGUID();
                                  break;
            case GO_DREAMWALKER_CACHE_10_H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiValitriaCacheGUID = pGo->GetGUID();
                                  break;
            case GO_DREAMWALKER_CACHE_25_H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiValitriaCacheGUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_1: 
                                  m_uiIceShard1GUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_2: 
                                  m_uiIceShard2GUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_3: 
                                  m_uiIceShard3GUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_4: 
                                  m_uiIceShard4GUID = pGo->GetGUID();
                                  break;
            case GO_FROSTY_WIND: 
                                  m_uiFrostyWindGUID = pGo->GetGUID();
                                  break;
            case GO_FROSTY_EDGE: 
                                  m_uiFrostyEdgeGUID = pGo->GetGUID();
                                  break;
            case GO_SNOW_EDGE: 
                                  m_uiSnowEdgeGUID = pGo->GetGUID();
                                  break;
            case GO_ARTHAS_PLATFORM: 
                                  m_uiArthasPlatformGUID = pGo->GetGUID();
                                  break;
            case GO_ARTHAS_PRECIPICE: 
                                  m_uiArthasPrecipiceGUID = pGo->GetGUID();
                                  break;
            case GO_GAS_RELEASE_VALVE: 
                                  m_uiGasReleaseValveGUID = pGo->GetGUID();
                                  break;
            case NPC_BLOOD_ORB_CONTROL:
                                  m_uiBloodOrbCtrlGUID = pGo->GetGUID();
                                  break;
        }
        OpenAllDoors();
    }

    void instance_icecrown_spire::SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_TELEPORT:
                break;
            case TYPE_MARROWGAR:
                m_auiEncounter[TYPE_MARROWGAR] = uiData;
                if (uiData == DONE) 
                    {
                        OpenDoor(m_uiIcewall1GUID);
                        OpenDoor(m_uiIcewall2GUID);
                        OpenDoor( m_uiOratoryDoorGUID);
                    }
                break;
             case TYPE_DEATHWHISPER:
                m_auiEncounter[TYPE_DEATHWHISPER] = uiData; 
                if (uiData == DONE) {
                    if (GameObject* pGO = instance->GetGameObject(m_uiDeathWhisperElevatorGUID))
                        {
                              pGO->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                              pGO->SetGoState(GO_STATE_READY);
                        }
                }
                break;
             case TYPE_FLIGHT_WAR:
                if (uiData == DONE && m_auiEncounter[TYPE_FLIGHT_WAR] != DONE  ) {
                                 if (GameObject* pChest = instance->GetGameObject(m_uiGunshipArmoryAGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(7*DAY);
                                      };

                                 if (GameObject* pChest = instance->GetGameObject(m_uiGunshipArmoryHGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(7*DAY);
                                      };
                                };
                m_auiEncounter[3] = uiData; 
                break;
             case TYPE_SAURFANG:
                m_auiEncounter[TYPE_SAURFANG] = uiData; 
                if (uiData == DONE) 
                {
                    OpenDoor(m_uiSaurfangDoorGUID);
                    OpenDoor(m_uiBloodwingDoorGUID);
                    OpenDoor(m_uiFrostwingDoorGUID);

                    if (GameObject* pChest = instance->GetGameObject(m_uiSaurfangCacheGUID))
                        if (pChest && !pChest->isSpawned()) 
                        {
                            pChest->SetRespawnTime(7*DAY);
                        };
                };
                break;
             case TYPE_FESTERGUT:
                m_auiEncounter[TYPE_FESTERGUT] = uiData;
                if (uiData == IN_PROGRESS) CloseDoor(m_uiOrangePlagueGUID);
                                      else OpenDoor(m_uiOrangePlagueGUID);
                if (uiData == DONE)  {
                                     OpenDoor(m_uiSDoorOrangeGUID);
                                     if (m_auiEncounter[TYPE_ROTFACE] == DONE) 
                                         {
                                             OpenDoor(m_uiSDoorCollisionGUID);
                                             OpenDoor(m_uiGreenPlagueGUID);
                                         }
                                     }
                break;
             case TYPE_ROTFACE:
                m_auiEncounter[TYPE_ROTFACE] = uiData;
                if (uiData == IN_PROGRESS)
                    CloseDoor(m_uiGreenPlagueGUID);
                else
                    OpenDoor(m_uiGreenPlagueGUID);
                if (uiData == DONE)
                {
                    OpenDoor(m_uiSDoorGreenGUID);
                    if (m_auiEncounter[TYPE_FESTERGUT] == DONE) 
                    {
                        OpenDoor(m_uiSDoorOrangeGUID);
                        OpenDoor(m_uiSDoorCollisionGUID);
                    }
                }
                break;
             case TYPE_PUTRICIDE:
                m_auiEncounter[TYPE_PUTRICIDE] = uiData;
                if (uiData == IN_PROGRESS) 
                    CloseDoor(m_uiScientistDoorGUID);
                else
                    OpenDoor(m_uiScientistDoorGUID);
                if (uiData == DONE)
                {
                    if (m_auiEncounter[TYPE_SINDRAGOSA] == DONE
                        && m_auiEncounter[TYPE_LANATHEL] == DONE)
                        m_auiEncounter[TYPE_KINGS_OF_ICC] = DONE;
                }
                break;
             case TYPE_BLOOD_COUNCIL:
                m_auiEncounter[TYPE_BLOOD_COUNCIL] = uiData;

                if (uiData == IN_PROGRESS)
                    CloseDoor(m_uiCrimsonDoorGUID);
                else
                    OpenDoor(m_uiCrimsonDoorGUID);

                if (uiData == DONE) 
                {
                    OpenDoor(m_uiCounsilDoor1GUID);
                    OpenDoor(m_uiCounsilDoor2GUID);
                }
                break;
             case TYPE_LANATHEL:
                m_auiEncounter[TYPE_LANATHEL] = uiData;
                if (uiData == DONE)
                {
                    if (m_auiEncounter[TYPE_PUTRICIDE] == DONE
                        && m_auiEncounter[TYPE_SINDRAGOSA] == DONE)
                        m_auiEncounter[TYPE_KINGS_OF_ICC] = DONE;
                }
                break;
             case TYPE_VALITHRIA:
                m_auiEncounter[TYPE_VALITHRIA] = uiData;

                if (uiData == IN_PROGRESS)
                    CloseDoor(m_uiGreenDragonDoor1GUID);
                else
                    OpenDoor(m_uiGreenDragonDoor1GUID);

                if (uiData == DONE)
                {
                    OpenDoor(m_uiGreenDragonDoor2GUID);
                    OpenDoor(m_uiSindragosaDoor1GUID);
                    OpenDoor(m_uiSindragosaDoor2GUID);
                    if (GameObject* pChest = instance->GetGameObject(m_uiValitriaCacheGUID))
                        if (pChest && !pChest->isSpawned()) 
                        {
                            pChest->SetRespawnTime(7*DAY);
                        };
                };
                break;
             case TYPE_SINDRAGOSA:
                m_auiEncounter[TYPE_SINDRAGOSA] = uiData;
                if (uiData == DONE)
                {
                    if (m_auiEncounter[TYPE_PUTRICIDE] == DONE
                        && m_auiEncounter[TYPE_LANATHEL] == DONE)
                        m_auiEncounter[TYPE_KINGS_OF_ICC] = DONE;
                }
                break;
             case TYPE_LICH_KING:
                m_auiEncounter[TYPE_LICH_KING] = uiData;
                break;
             case TYPE_ICECROWN_QUESTS:
                m_auiEncounter[TYPE_ICECROWN_QUESTS] = uiData;
                break;
             case TYPE_COUNT:
                m_auiEncounter[TYPE_COUNT] = uiData;
                uiData = NOT_STARTED;
                break;
             case DATA_BLOOD_INVOCATION:         m_uiCouncilInvocation = uiData;
                                                 uiData = NOT_STARTED;
                                                 break;
             case DATA_DIRECTION:                m_uiDirection = uiData;
                                                 uiData = NOT_STARTED;
                                                 break;
             case TYPE_EVENT:            m_auiEvent = uiData; uiData = NOT_STARTED; break;
             case TYPE_EVENT_TIMER:      m_auiEventTimer = uiData; uiData = NOT_STARTED; break;
             case TYPE_STINKY:           m_uiStinkystate = uiData; uiData = NOT_STARTED; break;
             case TYPE_PRECIOUS:         m_uiPreciousstate = uiData; uiData = NOT_STARTED; break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
                saveStream << m_auiEncounter[i] << " ";

            strSaveData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 instance_icecrown_spire::GetData(uint32 uiType)
    {
        switch(uiType)
        {
             case TYPE_TELEPORT:
             case TYPE_MARROWGAR:
             case TYPE_DEATHWHISPER:
             case TYPE_FLIGHT_WAR:
             case TYPE_SAURFANG:
             case TYPE_FESTERGUT:
             case TYPE_ROTFACE:
             case TYPE_PUTRICIDE:
             case TYPE_BLOOD_COUNCIL:
             case TYPE_LANATHEL:
             case TYPE_VALITHRIA:
             case TYPE_SINDRAGOSA:
             case TYPE_KINGS_OF_ICC:
             case TYPE_LICH_KING:
             case TYPE_ICECROWN_QUESTS:
             case TYPE_COUNT:
                          return m_auiEncounter[uiType];

             case DATA_DIRECTION:     return m_uiDirection;
             case DATA_BLOOD_INVOCATION:         return m_uiCouncilInvocation; 
             case TYPE_STINKY:        return m_uiStinkystate;
             case TYPE_PRECIOUS:      return m_uiPreciousstate;
             case TYPE_EVENT:         return m_auiEvent;
             case TYPE_EVENT_TIMER:   return m_auiEventTimer;
             case TYPE_EVENT_NPC:     switch (m_auiEvent) 
                                         {
                                          case 12030:
                                          case 12050:
                                          case 12051:
                                          case 12052:
                                          case 12053:
                                          case 12070:
                                          case 12090:
                                          case 12110:
                                          case 12130:
                                          case 12150:
                                          case 12170:
                                          case 13110:
                                          case 13130:
                                          case 13131:
                                          case 13132:
                                          case 13150:
                                          case 13170:
                                          case 13190:
                                          case 13210:
                                          case 13230:
                                          case 13250:
                                          case 13270:
                                          case 13290:
                                          case 13310:
                                          case 13330:
                                          case 13350:
                                          case 13370:
                                          case 14010:
                                          case 14030:
                                          case 14050:
                                          case 14070:
                                                 return NPC_TIRION;
                                                 break;

                                          case 12000:
                                          case 12020:
                                          case 12040:
                                          case 12041:
                                          case 12042:
                                          case 12043:
                                          case 12060:
                                          case 12080:
                                          case 12100:
                                          case 12120:
                                          case 12200:
                                          case 13000:
                                          case 13020:
                                          case 13040:
                                          case 13060:
                                          case 13080:
                                          case 13100:
                                          case 13120:
                                          case 13140:
                                          case 13160:
                                          case 13180:
                                          case 13200:
                                          case 13220:
                                          case 13240:
                                          case 13260:
                                          case 13280:
                                          case 13300:
                                          case 14000:
                                                 return NPC_LICH_KING;
                                                 break;
                                          case 500:
                                          case 510:
                                          case 550:
                                          case 560:
                                          case 570:
                                          case 580:
                                          case 590:
                                          case 600:
                                          case 610:
                                          case 620:
                                          case 630:
                                          case 640:
                                          case 650:
                                          case 660:
                                                 return NPC_PROFESSOR_PUTRICIDE;
                                                 break;

                                          case 800:
                                          case 810:
                                          case 820:
                                                 return NPC_LANATHEL_INTRO;
                                                 break;

                                          default:
                                                 break;
                                          };

        }
        return 0;
    }

    uint64 instance_icecrown_spire::GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_LORD_MARROWGAR:          return m_uiMarrogwarGUID;
            case NPC_LADY_DEATHWHISPER:       return m_uiDeathWhisperGUID;
            case NPC_DEATHBRINGER_SAURFANG:   return m_uiSaurfangGUID;
            case NPC_FESTERGUT:               return m_uiFestergutGUID;
            case NPC_ROTFACE:                 return m_uiRotfaceGUID;
            case NPC_PROFESSOR_PUTRICIDE:     return m_uiPutricideGUID;
            case NPC_TALDARAM:                return m_uiTaldaramGUID;
            case NPC_VALANAR:                 return m_uiValanarGUID;
            case NPC_KELESETH:                return m_uiKelesethGUID;
            case NPC_LANATHEL:                return m_uiLanathelGUID;
            case NPC_LANATHEL_INTRO:          return m_uiLanathelintroGUID;
            case NPC_VALITHRIA:               return m_uiValithriaGUID;
            case NPC_VALITHRIA_QUEST:         return m_uiValithriaQuestGUID;
            case NPC_SINDRAGOSA:              return m_uiSindragosaGUID;
            case NPC_LICH_KING:               return m_uiLichKingGUID;
            case NPC_RIMEFANG:                return m_uiRimefangGUID;
            case NPC_SPINESTALKER:            return m_uiSpinestalkerGUID;
            case NPC_STINKY:                  return m_uiStinkyGUID;
            case NPC_PRECIOUS:                return m_uiPreciousGUID;
            case GO_SCIENTIST_DOOR_ORANGE:    return m_uiSDoorOrangeGUID;
            case GO_SCIENTIST_DOOR_GREEN:     return m_uiSDoorGreenGUID;
            case GO_SCIENTIST_DOOR_COLLISION: return m_uiSDoorCollisionGUID;
            case GO_BLOODWING_DOOR:           return m_uiBloodwingDoorGUID;
            case GO_FROSTWING_DOOR:           return m_uiFrostwingDoorGUID;
            case GO_VALITHRIA_DOOR_1:         return m_uiValithriaDoor1GUID;
            case GO_VALITHRIA_DOOR_2:         return m_uiValithriaDoor2GUID;
            case GO_VALITHRIA_DOOR_3:         return m_uiValithriaDoor3GUID;
            case GO_VALITHRIA_DOOR_4:         return m_uiValithriaDoor4GUID;
            case GO_ICESHARD_1:               return m_uiIceShard1GUID;
            case GO_ICESHARD_2:               return m_uiIceShard2GUID;
            case GO_ICESHARD_3:               return m_uiIceShard3GUID;
            case GO_ICESHARD_4:               return m_uiIceShard4GUID;
            case GO_FROSTY_WIND:              return m_uiFrostyWindGUID;
            case GO_FROSTY_EDGE:              return m_uiFrostyEdgeGUID;
            case GO_SNOW_EDGE:                return m_uiSnowEdgeGUID;
            case GO_ARTHAS_PLATFORM:          return m_uiArthasPlatformGUID;
            case GO_ARTHAS_PRECIPICE:         return m_uiArthasPrecipiceGUID;
            case NPC_FROSTMOURNE_TRIGGER:     return m_uiFrostmourneTriggerGUID;
            case NPC_FROSTMOURNE_HOLDER:      return m_uiFrostmourneHolderGUID;
            case NPC_COMBAT_TRIGGER:          return m_uidummyTargetGUID;
            case GO_GAS_RELEASE_VALVE:        return m_uiGasReleaseValveGUID;
            case NPC_BLOOD_ORB_CONTROL:       return m_uiBloodOrbCtrlGUID;
        }
        return 0;
    }

    void instance_icecrown_spire::Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);

        for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
        {
            loadStream >> m_auiEncounter[i];

            if (m_auiEncounter[i] == IN_PROGRESS && i >= 1)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
        OpenAllDoors();
    }

InstanceData* GetInstanceData_instance_icecrown_spire(Map* pMap)
{
    return new instance_icecrown_spire(pMap);
}


void AddSC_instance_icecrown_spire()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_icecrown_spire";
    pNewScript->GetInstanceData = &GetInstanceData_instance_icecrown_spire;
    pNewScript->RegisterSelf();
}
