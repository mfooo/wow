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
SDName: Instance_Dire_Maul
SD%Complete:
SDComment: 
SDCategory: Dire Maul
EndScriptData */

#include "precompiled.h"
#include "dire_maul.h"

instance_dire_maul::instance_dire_maul(Map* pMap) : ScriptedInstance(pMap),
m_uiConservatoryDoorGUID(0)
{
    Initialize();
}

void instance_dire_maul::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_dire_maul::OnCreatureCreate(Creature* pCreature)
{
    /*switch (pCreature->GetEntry())
    {
        default: break;
    }*/
}

void instance_dire_maul::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_CONSERVATORY_DOOR:
            if (m_auiEncounter[3] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiConservatoryDoorGUID = pGo->GetGUID();
            break;
    }
}

void instance_dire_maul::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ZERVIM:
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_HYDROSPAWN:
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_LETHTENDRIS:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_IRONBARK:
            if (uiData == SPECIAL)
            {
                DoUseDoorOrButton(m_uiConservatoryDoorGUID);
                uiData = DONE;
            }
            m_auiEncounter[3] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        strInstData = saveStream.str();
        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_dire_maul::Load(const char* chrIn)
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

uint32 instance_dire_maul::GetData(uint32 uiType)
{
    switch (uiType)
    {
        case TYPE_ZERVIM:       return m_auiEncounter[0]; break;
        case TYPE_HYDROSPAWN:   return m_auiEncounter[1]; break;
        case TYPE_LETHTENDRIS:  return m_auiEncounter[2]; break;
        case TYPE_IRONBARK:     return m_auiEncounter[3]; break;
    }
    return 0;
}

InstanceData* GetInstanceData_instance_dire_maul(Map* pMap)
{
    return new instance_dire_maul(pMap);
}

void AddSC_instance_dire_maul()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "instance_dire_maul";
    newscript->GetInstanceData = &GetInstanceData_instance_dire_maul;
    newscript->RegisterSelf();
}
