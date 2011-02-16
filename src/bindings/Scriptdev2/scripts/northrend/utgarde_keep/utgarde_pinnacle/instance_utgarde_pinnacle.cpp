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
SDName: instance_pinnacle
SD%Complete: %
SDComment:
SDCategory: Utgarde Pinnacle
TODO::
// dev // FallenAngelX
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

struct MANGOS_DLL_DECL instance_pinnacle : public ScriptedInstance
{
    instance_pinnacle(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiSkadiDoorGUID;
    uint64 m_uYmironDoorGUID;

    uint64 m_uiRhinoGUID;
    uint64 m_uiJormungarGUID;
    uint64 m_uiWorgenGUID;
    uint64 m_uiFurbolgGUID;
    uint8  m_uiHarpoonsUsed;
    uint64 m_uiHarpoon1GUID;
    uint64 m_uiHarpoon2GUID;
    uint64 m_uiHarpoon3GUID;
    uint64 m_uiBjornGUID;
    uint64 m_uiHaldorGUID;
    uint64 m_uiRanulfGUID;
    uint64 m_uiTorgynGUID;
    uint64 m_uiYmironGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiRhinoGUID       = 0;
        m_uiJormungarGUID   = 0;
        m_uiWorgenGUID      = 0;
        m_uiFurbolgGUID     = 0;
        m_uiSkadiDoorGUID   = 0;
        m_uiHarpoonsUsed    = 0;
        m_uiHarpoon1GUID    = 0;
        m_uiHarpoon2GUID    = 0;
        m_uiHarpoon3GUID    = 0;
        m_uYmironDoorGUID   = 0;
        m_uiBjornGUID       = 0;
        m_uiHaldorGUID      = 0;
        m_uiRanulfGUID      = 0;
        m_uiTorgynGUID      = 0;
        m_uiYmironGUID      = 0;

    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_RHINO:         m_uiRhinoGUID = pCreature->GetGUID();           break;
            case NPC_JORMUNGAR:     m_uiJormungarGUID = pCreature->GetGUID();       break;
            case NPC_WORGEN :       m_uiWorgenGUID = pCreature->GetGUID();          break;
            case NPC_FURBOLG:       m_uiFurbolgGUID = pCreature->GetGUID();         break;
            case NPC_BJORN:         m_uiBjornGUID = pCreature->GetGUID();           break;
            case NPC_HALDOR:        m_uiHaldorGUID = pCreature->GetGUID();          break;
            case NPC_RANULF:        m_uiRanulfGUID = pCreature->GetGUID();          break;
            case NPC_TORGYN:        m_uiTorgynGUID = pCreature->GetGUID();          break;
            case NPC_YMIRON:        m_uiYmironGUID = pCreature->GetGUID();          break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_DOOR_SKADI:
                m_uiSkadiDoorGUID = pGo->GetGUID();

                if (m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);

                break;
            case GO_DOOR_YMIRON: m_uYmironDoorGUID = pGo->GetGUID(); break; 
            case GO_HARPOON1: m_uiHarpoon1GUID = pGo->GetGUID(); break;
            case GO_HARPOON2: m_uiHarpoon2GUID = pGo->GetGUID(); break;
            case GO_HARPOON3: m_uiHarpoon3GUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        debug_log("SD2: Instance Pinnacle: SetData received for type %u with data %u", uiType, uiData);

        switch(uiType)
        {
            case TYPE_SVALA:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_GORTOK:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_SKADI:
                if (uiData == DONE)
                    instance->GetGameObject(m_uiSkadiDoorGUID)->SetGoState(GO_STATE_ACTIVE);

                m_auiEncounter[2] = uiData;
                break;
            case TYPE_YMIRON:
                if (uiData == DONE)
                    instance->GetGameObject(m_uYmironDoorGUID)->SetGoState(GO_STATE_ACTIVE);

                m_auiEncounter[3] = uiData;
                break;
            case TYPE_HARPOONLUNCHER:
                if (uiData == IN_PROGRESS)
                    if (m_uiHarpoonsUsed < 7)
                        ++m_uiHarpoonsUsed;
                if (m_uiHarpoonsUsed == 4)
                    m_auiEncounter[4] = DONE;
                if (m_uiHarpoonsUsed == 6)
                    m_auiEncounter[4] = SPECIAL;
                break;

            default:
                error_log("SD2: Instance Pinnacle: SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
                break;
        }

        //saving also SPECIAL for this instance
        if (uiData == DONE || uiData == SPECIAL)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_SVALA:
                return m_auiEncounter[0];
            case TYPE_GORTOK:
                return m_auiEncounter[1];
            case TYPE_SKADI:
                return m_auiEncounter[2];
            case TYPE_YMIRON:
                return m_auiEncounter[3];
            case TYPE_HARPOONLUNCHER:
                return m_auiEncounter[4];
        }

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_RHINO:         return m_uiRhinoGUID;       
            case NPC_JORMUNGAR:     return m_uiJormungarGUID; 
            case NPC_WORGEN :       return m_uiWorgenGUID;  
            case NPC_FURBOLG:       return m_uiFurbolgGUID;
            case DATA_BJORN:        return m_uiBjornGUID;
            case DATA_HALDOR:       return m_uiHaldorGUID;
            case DATA_RANULF:       return m_uiRanulfGUID;
            case DATA_TORGYN:       return m_uiTorgynGUID;
            case DATA_YMIRON:       return m_uiYmironGUID;
            case GO_HARPOON1:       return m_uiHarpoon1GUID;
            case GO_HARPOON2:       return m_uiHarpoon2GUID;
            case GO_HARPOON3:       return m_uiHarpoon3GUID;
        }
        return 0;
    }

    const char* Save()
    {
        return strInstData.c_str();
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_pinnacle(Map* pMap)
{
    return new instance_pinnacle(pMap);
}

void AddSC_instance_pinnacle()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "instance_pinnacle";
    newscript->GetInstanceData = &GetInstanceData_instance_pinnacle;
    newscript->RegisterSelf();
}
