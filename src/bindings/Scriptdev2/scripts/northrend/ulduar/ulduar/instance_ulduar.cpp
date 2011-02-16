/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Instance_Ulduar
SD%Complete: 
SDComment: 
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

struct MANGOS_DLL_DECL instance_ulduar : public ScriptedInstance
{
    instance_ulduar(Map* pMap) : ScriptedInstance(pMap) 
    { 
        Regular = pMap->IsRegularDifficulty();
        Initialize(); 
    }

    bool Regular;

	// initialize the encouter variables
    std::string m_strInstData;
    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint32 m_auiHardBoss[HARD_ENCOUNTER];
    uint32 m_auiUlduarKeepers[KEEPER_ENCOUNTER];
    uint32 m_auiUlduarTeleporters[3];
    uint32 m_auiMiniBoss[6];

	// boss phases which need to be used inside the instance script
    uint32 m_uiMimironPhase;
    uint32 m_uiYoggPhase;
    uint32 m_uiVisionPhase;

	// creature guids
    uint64 m_uiLeviathanGUID;
    uint64 m_uiIgnisGUID;
    uint64 m_uiRazorscaleGUID;
    uint64 m_uiCommanderGUID;
    uint64 m_uiXT002GUID;
    uint64 m_auiAssemblyGUIDs[3];
    uint64 m_uiKologarnGUID;
    uint64 m_uiAuriayaGUID;
    uint64 m_uiMimironGUID;
    uint64 m_uiHodirGUID;
    uint64 m_uiThorimGUID;
    uint64 m_uiFreyaGUID;
    uint64 m_uiVezaxGUID;
    uint64 m_uiYoggSaronGUID;
    uint64 m_uiAlgalonGUID;
    uint64 m_uiRightArmGUID;
    uint64 m_uiLeftArmGUID;
    uint64 m_uiFeralDefenderGUID;
    uint64 m_uiElderBrightleafGUID;
    uint64 m_uiElderStonebarkGUID;
    uint64 m_uiElderIronbrachGUID;
    uint64 m_uiSaroniteAnimusGUID;
    uint64 m_uiRunicColossusGUID;
    uint64 m_uiRuneGiantGUID;
    uint64 m_uiJormungarGUID;
    uint64 m_uiLeviathanMkGUID;
    uint64 m_uiHodirImageGUID;
    uint64 m_uiFreyaImageGUID;
    uint64 m_uiThorimImageGUID;
    uint64 m_uiMimironImageGUID;
    uint64 m_uiSaraGUID;
    uint64 m_uiYoggBrainGUID;

    //doors & objects
    // The siege
    uint64 m_uiShieldWallGUID;
    uint64 m_uiLeviathanGateGUID;
    uint64 m_uiXT002GateGUID;
    uint64 m_uiBrokenHarpoonGUID;
    // Archivum
    uint64 m_uiIronCouncilDoorGUID;
    uint64 m_uiArchivumDoorGUID;
    uint64 m_uiArchivumConsoleGUID;
    uint64 m_uiUniverseFloorArchivumGUID;
    // Celestial planetarium
    uint64 m_uiCelestialDoorGUID;
    uint64 m_uiCelestialConsoleGUID;
    uint64 m_uiUniverseFloorCelestialGUID;
    uint64 m_uiAzerothGlobeGUID;
    // Kologarn
    uint64 m_uiShatteredHallsDoorGUID;
    uint64 m_uiKologarnBridgeGUID;
    // Hodir
    uint64 m_uiHodirEnterDoorGUID;
    uint64 m_uiHodirWallGUID;
    uint64 m_uiHodirExitDoorGUID;
    // Mimiron
    uint64 m_uiMimironTramGUID;
    uint64 m_uiMimironButtonGUID;
    uint64 m_uiMimironDoor1GUID;
    uint64 m_uiMimironDoor2GUID;
    uint64 m_uiMimironDoor3GUID;
    uint64 m_uiMimironElevatorGUID;
    uint64 m_uiMimironTelGUID[9];
    // Thorim
    uint64 m_uiArenaEnterDoorGUID;
    uint64 m_uiArenaExitDoorGUID;
    uint64 m_uiHallwayDoorGUID;
    uint64 m_uiThorimEnterDoorGUID;
    uint64 m_uiThorimLeverGUID;
    // Prison
    uint64 m_uiAncientGateGUID;
    uint64 m_uiVezaxGateGUID;
    uint64 m_uiYoggGateGUID;
    uint64 m_uiBrainDoor1GUID;
    uint64 m_uiBrainDoor2GUID;
    uint64 m_uiBrainDoor3GUID;

    // chests
    uint64 m_uiKologarnLootGUID;
    uint64 m_uiHodirLootGUID;
    uint64 m_uiHodirRareLootGUID;
    uint64 m_uiThorimLootGUID;
    uint64 m_uiThorimRareLootGUID;
    uint64 m_uiFreyaLootGUID;
    uint64 m_uiFreyaLoot1GUID;
    uint64 m_uiFreyaLoot2GUID;
    uint64 m_uiFreyaLoot3GUID;
    uint64 m_uiMimironLootGUID;
    uint64 m_uiMimironHardLootGUID;
    uint64 m_uiAlagonLootGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        memset(&m_auiHardBoss, 0, sizeof(m_auiHardBoss));
        memset(&m_auiUlduarKeepers, 0, sizeof(m_auiUlduarKeepers));
        memset(&m_auiUlduarTeleporters, 0, sizeof(m_auiUlduarTeleporters));

        for(uint8 i = 0; i < 6; i++)
            m_auiMiniBoss[i] = NOT_STARTED;

        for(uint8 i = 0; i < 9; i++)
            m_uiMimironTelGUID[i] = 0;

        m_uiMimironPhase        = 0;
        m_uiYoggPhase           = 0;
        m_uiVisionPhase         = 0;

        m_uiLeviathanGUID       = 0;
        m_uiIgnisGUID           = 0;
        m_uiRazorscaleGUID      = 0;
        m_uiCommanderGUID       = 0;
        m_uiXT002GUID           = 0;
        m_uiKologarnGUID        = 0;
        m_uiAuriayaGUID         = 0;
        m_uiMimironGUID         = 0;
        m_uiHodirGUID           = 0;
        m_uiThorimGUID          = 0;
        m_uiFreyaGUID           = 0;
        m_uiVezaxGUID           = 0;
        m_uiYoggSaronGUID       = 0;
        m_uiAlgalonGUID         = 0;
        m_uiRightArmGUID		= 0;
        m_uiLeftArmGUID			= 0;
        m_uiFeralDefenderGUID	= 0;
        m_uiElderBrightleafGUID = 0;
        m_uiElderStonebarkGUID  = 0;
        m_uiElderIronbrachGUID  = 0;
        m_uiSaroniteAnimusGUID  = 0;
        m_uiRunicColossusGUID   = 0;
        m_uiRuneGiantGUID       = 0;
        m_uiJormungarGUID		= 0;
        m_uiLeviathanMkGUID     = 0;
        m_uiHodirImageGUID      = 0;
        m_uiFreyaImageGUID      = 0;
        m_uiThorimImageGUID     = 0;
        m_uiMimironImageGUID    = 0;
        m_uiSaraGUID            = 0;
        m_uiYoggBrainGUID       = 0;

        // loot
        m_uiKologarnLootGUID	= 0;
        m_uiHodirLootGUID       = 0;
        m_uiHodirRareLootGUID   = 0;
        m_uiThorimLootGUID      = 0;
        m_uiThorimRareLootGUID  = 0;
        m_uiFreyaLootGUID       = 0;
        m_uiFreyaLoot1GUID      = 0;
        m_uiFreyaLoot2GUID      = 0; 
        m_uiFreyaLoot3GUID      = 0;
        m_uiMimironLootGUID     = 0;
        m_uiMimironHardLootGUID = 0;
        m_uiAlagonLootGUID      = 0;

        // doors
        // The siege
        m_uiShieldWallGUID      = 0;
        m_uiLeviathanGateGUID   = 0;
        m_uiXT002GateGUID       = 0;
        m_uiBrokenHarpoonGUID   = 0;
        // Archivum
        m_uiIronCouncilDoorGUID = 0;
        m_uiArchivumDoorGUID    = 0;
        m_uiArchivumConsoleGUID = 0;
        m_uiUniverseFloorArchivumGUID = 0;
        // Celestial planetarium
        m_uiCelestialDoorGUID   = 0;
        m_uiCelestialConsoleGUID = 0;
        m_uiUniverseFloorCelestialGUID = 0;
        m_uiAzerothGlobeGUID    = 0;
        // Kologarn
        m_uiShatteredHallsDoorGUID = 0;
        m_uiKologarnBridgeGUID  = 0;
        // Hodir
        m_uiHodirEnterDoorGUID  = 0;
        m_uiHodirWallGUID       = 0;
        m_uiHodirExitDoorGUID   = 0;
        // Mimiron
        m_uiMimironTramGUID     = 0;
        m_uiMimironButtonGUID   = 0;
        m_uiMimironDoor1GUID    = 0;
        m_uiMimironDoor2GUID    = 0;
        m_uiMimironDoor3GUID    = 0;
        m_uiMimironElevatorGUID = 0;
        // Thorim
        m_uiArenaEnterDoorGUID  = 0;
        m_uiArenaExitDoorGUID   = 0;
        m_uiHallwayDoorGUID     = 0;
        m_uiThorimEnterDoorGUID = 0;
        m_uiThorimLeverGUID     = 0;
        // Prison
        m_uiAncientGateGUID     = 0;
        m_uiVezaxGateGUID       = 0;
        m_uiYoggGateGUID        = 0;
        m_uiBrainDoor1GUID      = 0;
        m_uiBrainDoor2GUID      = 0;
        m_uiBrainDoor3GUID      = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        }

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
        case NPC_LEVIATHAN:
            m_uiLeviathanGUID = pCreature->GetGUID();
            break;
        case NPC_IGNIS:
            m_uiIgnisGUID = pCreature->GetGUID();
            break;
        case NPC_RAZORSCALE:
            m_uiRazorscaleGUID = pCreature->GetGUID();
            break;
        case NPC_COMMANDER:
            m_uiCommanderGUID = pCreature->GetGUID();
            break;
        case NPC_XT002:
            m_uiXT002GUID = pCreature->GetGUID();
            break;

            // Assembly of Iron
        case NPC_STEELBREAKER:
            m_auiAssemblyGUIDs[0] = pCreature->GetGUID();
            break;
        case NPC_MOLGEIM:
            m_auiAssemblyGUIDs[1] = pCreature->GetGUID();
            break;
        case NPC_BRUNDIR:
            m_auiAssemblyGUIDs[2] = pCreature->GetGUID();
            break;

        case NPC_KOLOGARN:
            m_uiKologarnGUID = pCreature->GetGUID();
            break;
        case NPC_RIGHT_ARM:
            m_uiRightArmGUID = pCreature->GetGUID();
            break;
        case NPC_LEFT_ARM:
            m_uiLeftArmGUID = pCreature->GetGUID();
            break;
        case NPC_AURIAYA:
            m_uiAuriayaGUID = pCreature->GetGUID();
            break;
        case NPC_FERAL_DEFENDER:
            m_uiFeralDefenderGUID = pCreature->GetGUID();
            break;
        case NPC_MIMIRON:
            m_uiMimironGUID = pCreature->GetGUID();
            break;
        case NPC_LEVIATHAN_MK:
            m_uiLeviathanMkGUID = pCreature->GetGUID();
            break;
        case NPC_HODIR:
            m_uiHodirGUID = pCreature->GetGUID();
            break;
        case NPC_THORIM:
            m_uiThorimGUID = pCreature->GetGUID();
            break;
        case NPC_RUNIC_COLOSSUS:
            m_uiRunicColossusGUID = pCreature->GetGUID();
            break;
        case NPC_RUNE_GIANT:
            m_uiRuneGiantGUID = pCreature->GetGUID();
            break;
        case NPC_JORMUNGAR_BEHEMOTH:
            m_uiJormungarGUID = pCreature->GetGUID();
            break;
        case NPC_FREYA:
            m_uiFreyaGUID = pCreature->GetGUID();
            break;
        case NPC_BRIGHTLEAF:
            m_uiElderBrightleafGUID = pCreature->GetGUID();
            break;
        case NPC_IRONBRACH:
            m_uiElderIronbrachGUID = pCreature->GetGUID();
            break;
        case NPC_STONEBARK:
            m_uiElderStonebarkGUID = pCreature->GetGUID();
            break;
        case NPC_VEZAX:
            m_uiVezaxGUID = pCreature->GetGUID();
            break;
        case NPC_ANIMUS:
            m_uiSaroniteAnimusGUID = pCreature->GetGUID();
            break;
        case NPC_YOGGSARON:
            m_uiYoggSaronGUID = pCreature->GetGUID();
            break;
        case NPC_SARA:
            m_uiSaraGUID = pCreature->GetGUID();
            break;
        case NPC_YOGG_BRAIN:
            m_uiYoggBrainGUID = pCreature->GetGUID();
            break;
        case NPC_ALGALON:
            m_uiAlgalonGUID = pCreature->GetGUID();
            break;
            // used to handle the keepers images
            // set to invisible by default and only made visible if the encounter is done
        case HODIR_IMAGE:
            m_uiHodirImageGUID = pCreature->GetGUID();
            pCreature->SetVisibility(VISIBILITY_OFF);
            if(m_auiEncounter[8] == DONE)
                pCreature->SetVisibility(VISIBILITY_ON);
            break;
        case FREYA_IMAGE:
            m_uiFreyaImageGUID = pCreature->GetGUID();
            pCreature->SetVisibility(VISIBILITY_OFF);
            if(m_auiEncounter[10] == DONE)
                pCreature->SetVisibility(VISIBILITY_ON);
            break;
        case THORIM_IMAGE:
            m_uiThorimImageGUID = pCreature->GetGUID();
            pCreature->SetVisibility(VISIBILITY_OFF);
            if(m_auiEncounter[9] == DONE)
                pCreature->SetVisibility(VISIBILITY_ON);
            break;
        case MIMIRON_IMAGE:
            m_uiMimironImageGUID = pCreature->GetGUID();
            pCreature->SetVisibility(VISIBILITY_OFF);
            if(m_auiEncounter[7] == DONE)
                pCreature->SetVisibility(VISIBILITY_ON);
            break;
        }
    }

    void OnObjectCreate(GameObject *pGo)
    {
        switch(pGo->GetEntry())
        {
            // doors & other
            // The siege
        case GO_SHIELD_WALL:
            m_uiShieldWallGUID = pGo->GetGUID();
            break;
        case GO_LEVIATHAN_GATE:
            m_uiLeviathanGateGUID = pGo->GetGUID();
            if(m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_XT002_GATE:
            pGo->SetGoState(GO_STATE_READY);
            if(m_auiEncounter[3] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            if(m_auiEncounter[1] == DONE && m_auiEncounter[2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiXT002GateGUID = pGo->GetGUID();
            break;
        case GO_BROKEN_HARPOON:
            m_uiBrokenHarpoonGUID = pGo->GetGUID();
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;

            // Archivum
        case GO_IRON_ENTRANCE_DOOR:
            m_uiIronCouncilDoorGUID = pGo->GetGUID();
            break;
        case GO_ARCHIVUM_DOOR:
            m_uiArchivumDoorGUID = pGo->GetGUID();
            if(m_auiEncounter[4])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARCHIVUM_CONSOLE:
            m_uiArchivumConsoleGUID = pGo->GetGUID();
            break;
        case GO_UNIVERSE_FLOOR_ARCHIVUM:
            m_uiUniverseFloorArchivumGUID = pGo->GetGUID();
            break;
            // Celestial Planetarium
        case GO_CELESTIAL_ACCES:
            m_uiCelestialConsoleGUID = pGo->GetGUID();
            break;
        case GO_CELESTIAL_DOOR:
            m_uiCelestialDoorGUID = pGo->GetGUID();
            break;
        case GO_UNIVERSE_FLOOR_CELESTIAL:
            m_uiUniverseFloorCelestialGUID = pGo->GetGUID();
            break;
        case GO_AZEROTH_GLOBE:
            m_uiAzerothGlobeGUID = pGo->GetGUID();
            break;
            // Shattered Hallway
        case GO_KOLOGARN_BRIDGE:
            m_uiKologarnBridgeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            if(m_auiEncounter[5] == DONE)
            {
                pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                pGo->SetGoState(GO_STATE_READY);
            }
            break;
        case GO_SHATTERED_DOOR:
            m_uiShatteredHallsDoorGUID = pGo->GetGUID();
            break;

            // The keepers
            // Hodir
        case GO_HODIR_EXIT:
            m_uiHodirExitDoorGUID = pGo->GetGUID();
            if(m_auiEncounter[8])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ICE_WALL:
            m_uiHodirWallGUID = pGo->GetGUID();
            if(m_auiEncounter[8])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HODIR_ENTER:
            m_uiHodirEnterDoorGUID = pGo->GetGUID();
            break;
            // Mimiron
        case GO_MIMIRON_TRAM:
            m_uiMimironTramGUID = pGo->GetGUID();
            if (m_auiEncounter[6] == DONE)
            {
                pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                pGo->SetGoState(GO_STATE_READY);
            }
            break;
        case GO_MIMIRON_BUTTON:
            m_uiMimironButtonGUID = pGo->GetGUID();
            if (m_auiEncounter[7] == NOT_STARTED)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case GO_MIMIRON_DOOR_1:
            m_uiMimironDoor1GUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_DOOR_2:
            m_uiMimironDoor2GUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_DOOR_3:
            m_uiMimironDoor3GUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_ELEVATOR:
            m_uiMimironElevatorGUID = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL1:
            m_uiMimironTelGUID[0] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL2:
            m_uiMimironTelGUID[1] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL3:
            m_uiMimironTelGUID[2] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL4:
            m_uiMimironTelGUID[3] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL5:
            m_uiMimironTelGUID[4] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL6:
            m_uiMimironTelGUID[5] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL7:
            m_uiMimironTelGUID[6] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL8:
            m_uiMimironTelGUID[7] = pGo->GetGUID();
            break;
        case GO_MIMIRON_TEL9:
            m_uiMimironTelGUID[8] = pGo->GetGUID();
            break;
            // Thorim
        case GO_DARK_IRON_PORTCULIS:
            m_uiArenaExitDoorGUID = pGo->GetGUID();
            break;
        case GO_RUNED_STONE_DOOR:
            m_uiHallwayDoorGUID = pGo->GetGUID();
            break;
        case GO_THORIM_STONE_DOOR:
            m_uiThorimEnterDoorGUID = pGo->GetGUID();
            break;
        case GO_LIGHTNING_FIELD:
            m_uiArenaEnterDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_LEVER:
            m_uiThorimLeverGUID = pGo->GetGUID();
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;

            // Prison
        case GO_ANCIENT_GATE:
            m_uiAncientGateGUID = pGo->GetGUID();
            OpenMadnessDoor();
            break;
        case GO_VEZAX_GATE:
            m_uiVezaxGateGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_READY);
            if(m_auiEncounter[11])
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_YOGG_GATE:
            m_uiYoggGateGUID = pGo->GetGUID();
            break;
        case GO_BRAIN_DOOR1:
            m_uiBrainDoor1GUID = pGo->GetGUID();
            break;
        case GO_BRAIN_DOOR2:
            m_uiBrainDoor2GUID = pGo->GetGUID();
            break;
        case GO_BRAIN_DOOR3:
            m_uiBrainDoor3GUID = pGo->GetGUID();
            break;

            // loot
            // Kologarn
        case GO_CACHE_OF_LIVING_STONE:
            if(Regular)
                m_uiKologarnLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_LIVING_STONE_H:
            if(!Regular)
                m_uiKologarnLootGUID = pGo->GetGUID();
            break;

            // Hodir
        case GO_CACHE_OF_WINTER:
            if(Regular)
                m_uiHodirLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_WINTER_H:
            if(!Regular)
                m_uiHodirLootGUID = pGo->GetGUID();
            break;
            // Hodir rare
        case GO_CACHE_OF_RARE_WINTER:
            if(Regular)
                m_uiHodirRareLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_RARE_WINTER_H:
            if(!Regular)
                m_uiHodirRareLootGUID = pGo->GetGUID();
            break;

            // Freya
        case GO_FREYA_GIFT:
            if(Regular)
                m_uiFreyaLootGUID = pGo->GetGUID();
            break;
        case GO_FREYA_GIFT_H:
            if(!Regular)
                m_uiFreyaLootGUID = pGo->GetGUID();
            break;
            // Freya rare
        case GO_FREYA_GIFT_1:
            if(Regular)
                m_uiFreyaLoot1GUID = pGo->GetGUID();
            break;
        case GO_FREYA_GIFT_H_1:
            if(!Regular)
                m_uiFreyaLoot1GUID = pGo->GetGUID();
            break;
        case GO_FREYA_GIFT_2:
            if(Regular)
                m_uiFreyaLoot2GUID = pGo->GetGUID();
            break;
        case GO_FREYA_GIFT_H_2:
            if(!Regular)
                m_uiFreyaLoot2GUID = pGo->GetGUID();
            break;
        case GO_FREYA_GIFT_3:
            if(Regular)
                m_uiFreyaLoot3GUID = pGo->GetGUID();
            break;
        case GO_FREYA_GIFT_H_3:
            if(!Regular)
                m_uiFreyaLoot3GUID = pGo->GetGUID();
            break;

            // Thorim
        case GO_CACHE_OF_STORMS:
            if(Regular)
                m_uiThorimLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_STORMS_H:
            if(!Regular)
                m_uiThorimLootGUID = pGo->GetGUID();
            break;
            // Thorim rare
        case GO_CACHE_OF_RARE_STORMS:
            if(Regular)
                m_uiThorimRareLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_RARE_STORMS_H:
            if(!Regular)
                m_uiThorimRareLootGUID = pGo->GetGUID();
            break;

            // Mimiron
        case GO_CACHE_OF_INOV:
            if(Regular)
                m_uiMimironLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_INOV_H:
            if(!Regular)
                m_uiMimironLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_INOV_HARD:
            if(Regular)
                m_uiMimironHardLootGUID = pGo->GetGUID();
            break;
        case GO_CACHE_OF_INOV_HARD_H:
            if(!Regular)
                m_uiMimironHardLootGUID = pGo->GetGUID();
            break;

            // Alagon
        case GO_GIFT_OF_OBSERVER:
            if(Regular)
                m_uiAlagonLootGUID = pGo->GetGUID();
            break;
        case GO_GIFT_OF_OBSERVER_H:
            if(!Regular)
                m_uiAlagonLootGUID = pGo->GetGUID();
            break;
        }
    }

	// functions to open or close some doors
    void OpenDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_ACTIVE);
    }

    void CloseDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_READY);
    }

    // used in order to unlock the door to Vezax and make vezax attackable
    void OpenMadnessDoor()
    {
        if(m_auiEncounter[7] == DONE && m_auiEncounter[8] == DONE && m_auiEncounter[9] == DONE && m_auiEncounter[10] == DONE)
            OpenDoor(m_uiAncientGateGUID);
            OpenDoor(m_uiAncientGateGUID);
    }

    // used to open the door to XT (custom script because Leviathan is disabled)
    // this will be removed when the Leviathan will be implemented
    void OpenXtDoor()
    {
        if(m_auiEncounter[1] == DONE && m_auiEncounter[2] == DONE)
            OpenDoor(m_uiXT002GateGUID);
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
        case TYPE_LEVIATHAN:
            m_auiEncounter[0] = uiData;
            DoUseDoorOrButton(m_uiShieldWallGUID);
            if (uiData == DONE)
            {
                OpenDoor(m_uiXT002GateGUID);
                OpenDoor(m_uiLeviathanGateGUID);
            }
            break;
        case TYPE_IGNIS:
            m_auiEncounter[1] = uiData;
            OpenXtDoor();       // remove when leviathan implemented
            break;
        case TYPE_RAZORSCALE:
            m_auiEncounter[2] = uiData;
            OpenXtDoor();       // remove when leviathan implemented
            break;
        case TYPE_XT002:
            m_auiEncounter[3] = uiData;
            if (uiData == DONE)
                OpenDoor(m_uiXT002GateGUID);
            else if (uiData == IN_PROGRESS)
                CloseDoor(m_uiXT002GateGUID);
            break;
        case TYPE_ASSEMBLY:
            m_auiEncounter[4] = uiData;
            if (uiData == DONE)
            {
                OpenDoor(m_uiIronCouncilDoorGUID);
                OpenDoor(m_uiArchivumDoorGUID);
                CheckIronCouncil();		// used for a hacky achiev, remove for revision!
            } else if (uiData == IN_PROGRESS)
                CloseDoor(m_uiIronCouncilDoorGUID);
            break;
        case TYPE_KOLOGARN:
            m_auiEncounter[5] = uiData;
            if (uiData == DONE)
            {
                DoRespawnGameObject(m_uiKologarnLootGUID, 30*MINUTE);
                if(m_auiEncounter[5] == DONE)
                if (GameObject* pGo = instance->GetGameObject(m_uiKologarnBridgeGUID))
                {
                    pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                    pGo->SetGoState(GO_STATE_READY);
                }
            }
            break;
        case TYPE_AURIAYA:
            m_auiEncounter[6] = uiData;
            if (uiData == DONE)
            {
//                CheckIronCouncil();		// used for a hacky achiev, remove for revision!
                if (GameObject* pGO = instance->GetGameObject(m_uiMimironTramGUID))
                {
                    pGO->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                    pGO->SetGoState(GO_STATE_READY);
                }
            }
            break;

            // Keepers
        case TYPE_MIMIRON:
            m_auiEncounter[7] = uiData;
            DoUseDoorOrButton(m_uiMimironDoor1GUID);
            DoUseDoorOrButton(m_uiMimironDoor2GUID);
            DoUseDoorOrButton(m_uiMimironDoor3GUID);
            if (uiData == DONE)
            {
                if(m_auiHardBoss[3] != DONE)
                    DoRespawnGameObject(m_uiMimironLootGUID, 30*MINUTE);
                // used to make the friendly keeper visible
                if(Creature* pImage = instance->GetCreature(m_uiMimironImageGUID))
                    pImage->SetVisibility(VISIBILITY_ON);
                OpenMadnessDoor();
                CheckKeepers();		// used for a hacky achiev, remove for revision!
            }
            break;
        case TYPE_HODIR:
            m_auiEncounter[8] = uiData;
            DoUseDoorOrButton(m_uiHodirEnterDoorGUID);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiHodirWallGUID);
                DoUseDoorOrButton(m_uiHodirExitDoorGUID);
                DoRespawnGameObject(m_uiHodirLootGUID, 30*MINUTE);
                // used to make the friendly keeper visible
                if(Creature* pImage = instance->GetCreature(m_uiHodirImageGUID))
                    pImage->SetVisibility(VISIBILITY_ON);
                OpenMadnessDoor();
                CheckKeepers();		// used for a hacky achiev, remove for revision!
            }
            break;
        case TYPE_THORIM:
            m_auiEncounter[9] = uiData;   
            DoUseDoorOrButton(m_uiArenaEnterDoorGUID);
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(m_uiArenaExitDoorGUID);
            if (uiData == DONE)
            {
                if(m_auiHardBoss[5] != DONE)
                    DoRespawnGameObject(m_uiThorimLootGUID, 30*MINUTE);
                // used to make the friendly keeper visible
                if(Creature* pImage = instance->GetCreature(m_uiThorimImageGUID))
                    pImage->SetVisibility(VISIBILITY_ON);
                OpenMadnessDoor();
                CheckKeepers();		// used for a hacky achiev, remove for revision!
            }
            break;
        case TYPE_FREYA:
            m_auiEncounter[10] = uiData;
            if (uiData == DONE)
            {
                // do this in order to see how many elders were alive and spawn the correct chest
                if(m_auiHardBoss[6] == 0)
                    DoRespawnGameObject(m_uiFreyaLootGUID, 30*MINUTE);
                else if(m_auiHardBoss[6] == 1)
                    DoRespawnGameObject(m_uiFreyaLoot1GUID, 30*MINUTE);
                else if(m_auiHardBoss[6] == 2)
                    DoRespawnGameObject(m_uiFreyaLoot2GUID, 30*MINUTE);
                else if(m_auiHardBoss[6] == 3)
                    DoRespawnGameObject(m_uiFreyaLoot3GUID, 30*MINUTE);
                // used to make the friendly keeper visible
                if(Creature* pImage = instance->GetCreature(m_uiFreyaImageGUID))
                    pImage->SetVisibility(VISIBILITY_ON);
                OpenMadnessDoor();
                CheckKeepers();		// used for a hacky achiev, remove for revision!
            }
            break;

            // Prison
        case TYPE_VEZAX:
            m_auiEncounter[11] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiVezaxGateGUID);
            break;
        case TYPE_YOGGSARON:
            m_auiEncounter[12] = uiData;
            DoUseDoorOrButton(m_uiYoggGateGUID);
            break;

            // Celestial Planetarium
        case TYPE_ALGALON:
            m_auiEncounter[13] = uiData;
            DoUseDoorOrButton(m_uiCelestialDoorGUID);
            DoUseDoorOrButton(m_uiUniverseFloorCelestialGUID);
            if (uiData == DONE)
                DoRespawnGameObject(m_uiAlagonLootGUID, 30*MINUTE);
            break;

            // Hard modes
        case TYPE_LEVIATHAN_HARD:
            m_auiHardBoss[0] = uiData;  // todo: add extra loot
            break;
        case TYPE_XT002_HARD:
            m_auiHardBoss[1] = uiData;  // hard mode loot in sql -> hacky way
            break;
        case TYPE_HODIR_HARD:
            m_auiHardBoss[4] = uiData;
            if(uiData == DONE)
                DoRespawnGameObject(m_uiHodirRareLootGUID, 30*MINUTE);
            break;
        case TYPE_ASSEMBLY_HARD:
            m_auiHardBoss[2] = uiData;  // hard mode loot in sql
            break;
        case TYPE_FREYA_HARD:
            m_auiHardBoss[6] = uiData;  // hard mode loot in the script above
            break;
        case TYPE_THORIM_HARD:
            m_auiHardBoss[5] = uiData;
            if(uiData == DONE)
                DoRespawnGameObject(m_uiThorimRareLootGUID, 30*MINUTE);
            break;
        case TYPE_MIMIRON_HARD:
            m_auiHardBoss[3] = uiData;
            if(uiData == DONE)
                DoRespawnGameObject(m_uiMimironHardLootGUID, 30*MINUTE);
            break;
        case TYPE_VEZAX_HARD:
            m_auiHardBoss[7] = uiData;  // hard mode loot in sql -> hacky way
            break;
        case TYPE_YOGGSARON_HARD:
            m_auiHardBoss[8] = uiData;  // todo: add extra loot
            break;

            // Ulduar keepers
        case TYPE_KEEPER_HODIR:
            m_auiUlduarKeepers[0] = uiData;
            break;
        case TYPE_KEEPER_THORIM:
            m_auiUlduarKeepers[1] = uiData;
            break;
        case TYPE_KEEPER_FREYA:
            m_auiUlduarKeepers[2] = uiData;
            break;
        case TYPE_KEEPER_MIMIRON:
            m_auiUlduarKeepers[3] = uiData;
            break;

            // teleporters
        case TYPE_LEVIATHAN_TP:
            m_auiUlduarTeleporters[0] = uiData;
            break;
        case TYPE_XT002_TP:
            m_auiUlduarTeleporters[1] = uiData;
            break;
        case TYPE_MIMIRON_TP:
            m_auiUlduarTeleporters[2] = uiData;
            break;

            // mini boss
        case TYPE_RUNIC_COLOSSUS:
            m_auiMiniBoss[0] = uiData;
            if (uiData == DONE)
                OpenDoor(m_uiHallwayDoorGUID);
            else
                CloseDoor(m_uiHallwayDoorGUID);
            break;
        case TYPE_RUNE_GIANT:
            m_auiMiniBoss[1] = uiData;
            if (uiData == DONE)
                OpenDoor(m_uiThorimEnterDoorGUID);
            else
                CloseDoor(m_uiThorimEnterDoorGUID);
            break;
        case TYPE_LEVIATHAN_MK:
            m_auiMiniBoss[2] = uiData;
            break;
        case TYPE_VX001:
            m_auiMiniBoss[3] = uiData;
            if (uiData == DONE)     // just for animation :)
            {
                for(uint8 i = 0; i < 9; i++)
                    DoUseDoorOrButton(m_uiMimironTelGUID[i]);
            }
            break;
        case TYPE_AERIAL_UNIT:
            m_auiMiniBoss[4] = uiData;
            break;
        case TYPE_YOGG_BRAIN:
            m_auiMiniBoss[5] = uiData;
            break;

            //phases
        case TYPE_MIMIRON_PHASE:
            m_uiMimironPhase = uiData;
            break;
        case TYPE_YOGG_PHASE:
            m_uiYoggPhase = uiData;
            break;
        case TYPE_VISION_PHASE:
            m_uiVisionPhase = uiData;
            break;
        }

        if (uiData == DONE || uiData == FAIL)
        {
            OUT_SAVE_INST_DATA;

            // save all encounters, hard bosses and keepers
            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
                << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
                << m_auiEncounter[12] << " " << m_auiEncounter[13] << " " << m_auiHardBoss[0] << " " 
                << m_auiHardBoss[1] << " " << m_auiHardBoss[2] << " " << m_auiHardBoss[2] << " "
                << m_auiHardBoss[4] << " " << m_auiHardBoss[5] << " " << m_auiHardBoss[6] << " "
                << m_auiHardBoss[7] << " " << m_auiHardBoss[8] << " " << m_auiUlduarKeepers[0] << " "
                << m_auiUlduarKeepers[1] << " " << m_auiUlduarKeepers[2] << " " << m_auiUlduarKeepers[3] << " "
                << m_auiUlduarTeleporters[0] << " " << m_auiUlduarTeleporters[1] << " " << m_auiUlduarTeleporters[2];

            m_strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            // Siege
        case NPC_LEVIATHAN:
            return m_uiLeviathanGUID;
        case NPC_IGNIS:
            return m_uiIgnisGUID;
        case NPC_RAZORSCALE:
            return m_uiRazorscaleGUID;
        case NPC_COMMANDER:
            return m_uiCommanderGUID;
        case NPC_XT002:
            return m_uiXT002GUID;
            // Antechamber
        case NPC_STEELBREAKER:
            return m_auiAssemblyGUIDs[0];
        case NPC_MOLGEIM:
            return m_auiAssemblyGUIDs[1];
        case NPC_BRUNDIR:
            return m_auiAssemblyGUIDs[2];
        case NPC_KOLOGARN:
            return m_uiKologarnGUID;
        case NPC_LEFT_ARM:
            return m_uiLeftArmGUID;
        case NPC_RIGHT_ARM:
            return m_uiRightArmGUID;
        case NPC_AURIAYA:
            return m_uiAuriayaGUID;
            // Keepers
        case NPC_MIMIRON:
            return m_uiMimironGUID;
        case NPC_LEVIATHAN_MK:
            return m_uiLeviathanMkGUID;
        case NPC_HODIR:
            return m_uiMimironGUID;
        case NPC_THORIM:
            return m_uiThorimGUID;
        case NPC_RUNE_GIANT:
            return m_uiRuneGiantGUID;
        case NPC_RUNIC_COLOSSUS:
            return m_uiRunicColossusGUID;
        case NPC_JORMUNGAR_BEHEMOTH:
            return m_uiJormungarGUID;
        case NPC_FREYA:
            return m_uiFreyaGUID;
        case NPC_BRIGHTLEAF:
            return m_uiElderBrightleafGUID;
        case NPC_IRONBRACH:
            return m_uiElderIronbrachGUID;
        case NPC_STONEBARK:
            return m_uiElderStonebarkGUID;
        case NPC_VEZAX:
            return m_uiVezaxGUID;
        case NPC_YOGGSARON:
            return m_uiYoggSaronGUID;
        case NPC_SARA:
            return m_uiSaraGUID;
        case NPC_YOGG_BRAIN:
            return m_uiYoggBrainGUID;
        case NPC_ALGALON:
            return m_uiAlgalonGUID;

            // mimiron hard  mode button
        case GO_MIMIRON_BUTTON:
            return m_uiMimironButtonGUID;
            // thorim encounter starter lever
        case GO_DOOR_LEVER:
            return m_uiThorimLeverGUID;
            // celestial door
        case GO_CELESTIAL_DOOR:
            return m_uiCelestialDoorGUID;
        }

        return 0;
    }

	// TODO: implement all achievs here!
    bool CheckAchievementCriteriaMeet(uint32 criteria_id, const Player *source)
    {
        switch(criteria_id)
        {
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
            break;
        }
        return false;
    }

    // TODO: implement all hard mode loot here!
    bool CheckConditionCriteriaMeet(Player const* source, uint32 map_id, uint32 instance_condition_id)
    {
        if (map_id != instance->GetId())
            return false;
        switch (instance_condition_id)
        {
           case TYPE_XT002_HARD:
               break;
        }
        return true;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
        case TYPE_LEVIATHAN:
            return m_auiEncounter[0];
        case TYPE_IGNIS:
            return m_auiEncounter[1];
        case TYPE_RAZORSCALE:
            return m_auiEncounter[2];
        case TYPE_XT002:
            return m_auiEncounter[3];
        case TYPE_ASSEMBLY:
            return m_auiEncounter[4];
        case TYPE_KOLOGARN:
            return m_auiEncounter[5];
        case TYPE_AURIAYA:
            return m_auiEncounter[6];
        case TYPE_MIMIRON:
            return m_auiEncounter[7];
        case TYPE_HODIR:
            return m_auiEncounter[8];
        case TYPE_THORIM:
            return m_auiEncounter[9];
        case TYPE_FREYA:
            return m_auiEncounter[10];
        case TYPE_VEZAX:
            return m_auiEncounter[11];
        case TYPE_YOGGSARON:
            return m_auiEncounter[12];
        case TYPE_ALGALON:
            return m_auiEncounter[13];

        // hard modes
        case TYPE_LEVIATHAN_HARD:
            return m_auiHardBoss[0];
        case TYPE_XT002_HARD:
            return m_auiHardBoss[1];
        case TYPE_ASSEMBLY_HARD:
            return m_auiHardBoss[2];
        case TYPE_MIMIRON_HARD:
            return m_auiHardBoss[3];
        case TYPE_HODIR_HARD:
            return m_auiHardBoss[4];
        case TYPE_THORIM_HARD:
            return m_auiHardBoss[5];
        case TYPE_FREYA_HARD:
            return m_auiHardBoss[6];
        case TYPE_VEZAX_HARD:
            return m_auiHardBoss[7];
        case TYPE_YOGGSARON_HARD:
            return m_auiHardBoss[8];

            // ulduar keepers
        case TYPE_KEEPER_HODIR:
            return m_auiUlduarKeepers[0];
        case TYPE_KEEPER_THORIM:
            return m_auiUlduarKeepers[1];
        case TYPE_KEEPER_FREYA:
            return m_auiUlduarKeepers[2];
        case TYPE_KEEPER_MIMIRON:
            return m_auiUlduarKeepers[3];

            // teleporters
        case TYPE_LEVIATHAN_TP:
            return m_auiUlduarTeleporters[0];
        case TYPE_XT002_TP:
            return m_auiUlduarTeleporters[1];
        case TYPE_MIMIRON_TP:
            return m_auiUlduarTeleporters[2];

            // mini boss
        case TYPE_RUNE_GIANT:
            return m_auiMiniBoss[1];
        case TYPE_RUNIC_COLOSSUS:
            return m_auiMiniBoss[0];
        case TYPE_LEVIATHAN_MK:
            return m_auiMiniBoss[2];
        case TYPE_VX001:
            return m_auiMiniBoss[3];
        case TYPE_AERIAL_UNIT:
            return m_auiMiniBoss[4];
        case TYPE_YOGG_BRAIN:
            return m_auiMiniBoss[5];

        case TYPE_MIMIRON_PHASE:
            return m_uiMimironPhase;
        case TYPE_YOGG_PHASE:
            return m_uiYoggPhase;
        case TYPE_VISION_PHASE:
            return m_uiVisionPhase;
        }

        return 0;
    }

    const char* Save()
    {
        return m_strInstData.c_str();
    }

    void Load(const char* strIn)
    {
        if (!strIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(strIn);

        std::istringstream loadStream(strIn);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
        >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11]
        >> m_auiEncounter[12] >> m_auiEncounter[13] >> m_auiHardBoss[0] >> m_auiHardBoss[1]
        >> m_auiHardBoss[2] >> m_auiHardBoss[3] >> m_auiHardBoss[4] >> m_auiHardBoss[5]
        >> m_auiHardBoss[6] >> m_auiHardBoss[7] >> m_auiHardBoss[8] >> m_auiUlduarKeepers[0]
        >> m_auiUlduarKeepers[1] >> m_auiUlduarKeepers[2] >> m_auiUlduarKeepers[3] >> m_auiUlduarTeleporters[0]
        >> m_auiUlduarTeleporters[1] >> m_auiUlduarTeleporters[2];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    // Hacky way of completing some achievs
    // PLEASE REMOVE FOR REVISION!
    void CheckIronCouncil()
    {
        // check if the other bosses in the antechamber are dead
        // hacky way to complete achievements; use only if you have this function
        if(m_auiEncounter[4] == DONE && m_auiEncounter[5] == DONE && m_auiEncounter[6] == DONE)
            DoCompleteAchievement(instance->IsRegularDifficulty() ? ACHIEV_IRON_COUNCIL : ACHIEV_IRON_COUNCIL_H);
    }

    void CheckKeepers()
    {
        // check if the other bosses in the antechamber are dead
        // hacky way to complete achievements; use only if you have this function
        if(m_auiEncounter[7] == DONE && m_auiEncounter[8] == DONE && m_auiEncounter[9] == DONE && m_auiEncounter[10] == DONE)
            DoCompleteAchievement(instance->IsRegularDifficulty() ? ACHIEV_KEEPERS : ACHIEV_KEEPERS_H);
    }
};

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

void AddSC_instance_ulduar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_ulduar";
    newscript->GetInstanceData = &GetInstanceData_instance_ulduar;
    newscript->RegisterSelf();
}
