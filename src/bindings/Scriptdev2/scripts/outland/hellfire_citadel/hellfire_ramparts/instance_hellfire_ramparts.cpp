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
SDName: Instance_Hellfire_Ramparts
SD%Complete: 50
SDComment:
SDCategory: Hellfire Ramparts
EndScriptData */

#include "precompiled.h"
#include "hellfire_ramparts.h"

struct MANGOS_DLL_DECL instance_ramparts : public ScriptedInstance
{
    instance_ramparts(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint32 m_uiSentryCounter;
    uint64 m_uiChestNGUID;
    uint64 m_uiChestHGUID;
    uint64 m_uiHeraldGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiSentryCounter = 0;
        m_uiChestNGUID = 0;
        m_uiChestHGUID = 0;
        m_uiHeraldGUID = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        if (pCreature->GetEntry() == 17307)
            m_uiHeraldGUID = pCreature->GetGUID();
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case 185168: m_uiChestNGUID = pGo->GetGUID(); break;
            case 185169: m_uiChestHGUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        debug_log("SD2: Instance Ramparts: SetData received for type %u with data %u",uiType,uiData);

        switch(uiType)
        {
            case TYPE_VAZRUDEN:
                if (uiData == DONE && m_auiEncounter[1] == DONE)
                    DoRespawnGameObject(instance->IsRegularDifficulty() ? m_uiChestNGUID : m_uiChestHGUID, HOUR*IN_MILLISECONDS);
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_NAZAN:
                if (uiData == SPECIAL)
                {
                    ++m_uiSentryCounter;

                    if (m_uiSentryCounter == 2)
                        m_auiEncounter[1] = uiData;
                }
                if (uiData == DONE && m_auiEncounter[0] == DONE)
                {
                    DoRespawnGameObject(instance->IsRegularDifficulty() ? m_uiChestNGUID : m_uiChestHGUID, HOUR*IN_MILLISECONDS);
                    m_auiEncounter[1] = uiData;
                }
                if (uiData == IN_PROGRESS)
                    m_auiEncounter[1] = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        if (uiType == TYPE_VAZRUDEN)
            return m_auiEncounter[0];

        if (uiType == TYPE_NAZAN)
            return m_auiEncounter[1];

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        if (uiData == DATA_HERALD)
            return m_uiHeraldGUID;

        return 0;
    }
};

InstanceData* GetInstanceData_instance_ramparts(Map* pMap)
{
    return new instance_ramparts(pMap);
}

void AddSC_instance_ramparts()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_ramparts";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ramparts;
    pNewScript->RegisterSelf();
}
