/* Copyright (C) 2009 - 2011 Project Dark-iCE <http://projectSD2.clanice.com/>
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
SDName: custom_npc_renamecharacter
SD%Complete: 100
SDComment: Enables rename at character re-login.
SDCategory: NPC
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
EndScriptData */

#include "precompiled.h"
#include <cstring>
extern DatabaseMysql SD2Database;

bool GossipHello_custom_npc_renamecharacter(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(0, "Do you want to rename your character ?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(0, "You're not interested.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(907,pCreature->GetGUID());
    return true;
}

bool GossipSelect_custom_npc_renamecharacter(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pCreature->MonsterWhisper("You will be allowed to change your nickname at next relogin.", pPlayer, false);
        pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
        SD2Database.PExecute("UPDATE characters.characters SET at_login = at_login | '1' WHERE guid = '%u'", pPlayer->GetGUIDLow());
        pPlayer->CLOSE_GOSSIP_MENU();
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pCreature->MonsterWhisper("Mh ok. See you around!", pPlayer, false);
        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

void AddSC_custom_npc_renamecharacter()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_npc_renamecharacter";
    newscript->pGossipHello     = &GossipHello_custom_npc_renamecharacter;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_renamecharacter;
    newscript->RegisterSelf();
}