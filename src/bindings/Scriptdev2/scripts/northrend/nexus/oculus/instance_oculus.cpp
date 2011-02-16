/* Copyright (C) 2008 - 2011 ScriptDev2 & TrinityCore <http://www.trinitycore.org>
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
SDName: instance_oculus
SD%Complete: 70%
SDComment:
SDAuthor: originally from TC, reworked by MaxXx2021 Aka Mioka, corrected by /dev/rsa && FallenAngelX
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

/* The Occulus encounters:
0 - Drakos the Interrogator
1 - Varos Cloudstrider
2 - Mage-Lord Urom
3 - Ley-Guardian Eregos */

enum
{
    SAY_VAROS_SPAWN        = -1578029,
};

struct MANGOS_DLL_DECL instance_oculus : public ScriptedInstance
{
    instance_oculus(Map* pMap) : ScriptedInstance(pMap)
    {
        m_bIsRegularMode = pMap->IsRegularDifficulty();
        Initialize();
    };

    uint64 m_uiDrakosGUID;
    uint64 m_uiVarosGUID;
    uint64 m_uiUromGUID;
    uint64 m_uiEregosGUID;
    std::list<uint64> m_uiCageDoorList;
    uint64 uiProect;
    uint64 uiCacheEregosGUID;
    uint64 uiCacheEregosHGUID;
    uint64 m_uiSpotLightGUID;

    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];

    std::string strSaveData;
    bool m_bIsRegularMode;

    void Initialize()
    {
        for (uint8 i = 0; i < MAX_ENCOUNTERS+1; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        uiCacheEregosGUID = 0;
        m_uiSpotLightGUID = 0;
        uiProect = 0;
        m_uiDrakosGUID = 0;
        m_uiVarosGUID = 0;
        m_uiUromGUID = 0;
        m_uiEregosGUID = 0;
        m_uiCageDoorList.clear();
        m_auiEncounter[TYPE_ROBOTS] = 10;
        m_auiEncounter[TYPE_UROM_PHASE] = 0;
    }

    void OnObjectCreate(GameObject* pGO)
    {
        switch(pGO->GetEntry())
        {
            case GO_DRAGON_CAGE_DOOR:
                m_uiCageDoorList.push_back(pGO->GetGUID());
                break;
            case GO_EREGOS_CACHE:
                uiCacheEregosGUID = pGO->GetGUID();
                break;
            case GO_EREGOS_CACHE_H:
                uiCacheEregosHGUID = pGO->GetGUID();
                break;
            case GO_SPOTLIGHT:
                m_uiSpotLightGUID = pGO->GetGUID();
                break;
        }
    }

    void OpenCageDoors()
    {
        if(m_uiCageDoorList.empty())
            return;

        for(std::list<uint64>::const_iterator itr = m_uiCageDoorList.begin(); itr != m_uiCageDoorList.end(); ++itr)
        {
            GameObject* pDoor = instance->GetGameObject(*itr);
            if(pDoor)
               DoUseDoorOrButton(*itr);
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_DRAKOS:
                m_uiDrakosGUID = pCreature->GetGUID();
                break;
            case NPC_VAROS:
                m_uiVarosGUID = pCreature->GetGUID();
                break;
            case NPC_UROM:
                m_uiUromGUID = pCreature->GetGUID();
                break;
            case NPC_EREGOS:
                m_uiEregosGUID = pCreature->GetGUID();
                break;
            case NPC_BALGAR_IMAGE:
                uiProect = pCreature->GetGUID();
                break;
        }
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_DRAKOS:
                m_auiEncounter[type] = data;
                if (data == DONE)
                OpenCageDoors();
                break;
            case TYPE_VAROS:
            case TYPE_UROM:
                m_auiEncounter[type] = data;
                break;
            case TYPE_EREGOS:
                m_auiEncounter[type] = data;
                if (data == DONE)
                {
                    DoRespawnGameObject((m_bIsRegularMode ? uiCacheEregosGUID : uiCacheEregosHGUID), HOUR);
                    DoRespawnGameObject(m_uiSpotLightGUID, HOUR);
                }
                break;
            case TYPE_ROBOTS:
                m_auiEncounter[type] = m_auiEncounter[type] - data;
                if(m_auiEncounter[type] == 0)
                {
                    if(Creature* pVaros = instance->GetCreature(m_uiVarosGUID))
                    {
                        DoScriptText(SAY_VAROS_SPAWN, pVaros);
                        pVaros->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pVaros->InterruptNonMeleeSpells(false);
                        pVaros->RemoveAurasDueToSpell(50053);
                    }
                }
                data = NOT_STARTED;
                break;
            case TYPE_UROM_PHASE:
                m_auiEncounter[type] = data;
                data = NOT_STARTED;
                break;
        }

        if (data == DONE)
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

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_DRAKOS:
            case TYPE_VAROS:
            case TYPE_UROM:
            case TYPE_EREGOS:
            case TYPE_ROBOTS:
            case TYPE_UROM_PHASE:
                return m_auiEncounter[type];
            default:
                return 0;
        }
        return 0;
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case DATA_DRAKOS:                 return m_uiDrakosGUID;
            case NPC_VAROS:                   return m_uiVarosGUID;
            case NPC_UROM:                    return m_uiUromGUID;
            case NPC_EREGOS:                  return m_uiEregosGUID;
        }
        return 0;
    }

    const char* Save()
    {
        return strSaveData.c_str();
    }

    void Load(const char* chrIn)
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

            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        m_auiEncounter[TYPE_ROBOTS] = 10;
        m_auiEncounter[TYPE_UROM_PHASE] = 0;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_oculus(Map* pMap)
{
    return new instance_oculus(pMap);
}

bool GOUse_go_oculus_portal(Player* pPlayer, GameObject* pGo)
{
	switch(pGo->GetEntry())
	{
	case GO_ORB_OF_NEXUS:
		pPlayer->TeleportTo(571,3876.159912f,6984.439941f,106.32f,6.279f);
		return true;
	case GO_NEXUS_PORTAL:
		pPlayer->NearTeleportTo(1001.3419f,1051.692f,359.476776f,3.189315f);
		return true;
	}

	return false;
}

void AddSC_instance_oculus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_oculus";
    newscript->GetInstanceData = &GetInstanceData_instance_oculus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_oculus_portal";
    newscript->pGOUse = GOUse_go_oculus_portal;
    newscript->RegisterSelf();
}
