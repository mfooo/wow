/* Copyright (C) 2009 - 2011 Project Dark-iCE <http://projectdarkice.clanice.com/>
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
SDName: custom_npc_resetinstance
SD%Complete: 100
SDComment: Instance Reseter.
SDCategory: NPC
SDAuthor: Darkrulerz
EndScriptData */

#include "precompiled.h"
#include "../../../shared/Config/Config.h"
#include "../config.h"
#include "Database/DatabaseEnv.h"

extern DatabaseType CharacterDatabase;

bool GossipHello_custom_npc_resetinstance(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(0, "Do you want to reset all instances ?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(0, "You're not interested.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->PlayerTalkClass->SendGossipMenu(907,pCreature->GetGUID());
    return true;
}

bool GossipSelect_custom_npc_resetinstance(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    /*if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        QueryResult *result = CharacterDatabase.Query("SELECT map FROM character_instance;");
		QueryResult *raw_count = CharacterDatabase.Query("SELECT count(map) FROM character_instance;");
		uint8 count = (*raw_count)[0].GetUInt8();
        for(int i = 0; i <= count; ++i)
        {
            uint32 mapid = (*result)[i].GetUInt8();
			MapEntry const *mapEntry = sMapStore.LookupEntry(mapid);
            if (!mapEntry->Instanceable())
                return false;

            for(InstanceSaveHashMap::iterator itr = m_instanceSaveById.begin(); itr != m_instanceSaveById.end();)
            {
                if (itr->second->GetMapId() == mapid)
                    _ResetSave(itr);
                else
                    ++itr;
            }
            CharacterDatabase.BeginTransaction();
            CharacterDatabase.PExecute("DELETE FROM character_instance USING character_instance LEFT JOIN instance ON character_instance.instance = id WHERE map = '%u'", mapid);
            CharacterDatabase.PExecute("DELETE FROM group_instance USING group_instance LEFT JOIN instance ON group_instance.instance = id WHERE map = '%u'", mapid);
            CharacterDatabase.PExecute("DELETE FROM instance WHERE map = '%u'", mapid);
            CharacterDatabase.CommitTransaction();

            Map const *map = sMapMgr.FindMap(mapid);
            MapInstanced::InstancedMaps &instMaps = ((MapInstanced*)map)->GetInstancedMaps();
            MapInstanced::InstancedMaps::iterator mitr;
            for(mitr = instMaps.begin(); mitr != instMaps.end(); ++mitr)
            {
                Map *map2 = mitr->second;
                if (!map2->IsDungeon())
                    continue;

               ((InstanceMap*)map2)->Reset(INSTANCE_RESET_GLOBAL);
            }
        }
        return false;
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pCreature->MonsterWhisper("Ok, cya!", pPlayer, false);
        pPlayer->CLOSE_GOSSIP_MENU();
    }
    */
    return true;
}
void AddSC_custom_npc_resetinstance()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_npc_resetinstance";
    newscript->pGossipHello     = &GossipHello_custom_npc_resetinstance;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_resetinstance;
    newscript->RegisterSelf();
}