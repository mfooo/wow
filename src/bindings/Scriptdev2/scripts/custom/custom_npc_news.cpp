 /* Copyright (C) 2009 - 2011 ESSENCEGAMING.ORG
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
SDName: news
SD%Complete: 100
SDComment: news NPC
SDCategory: NPC
SDAuthor: Flame
EndScriptData */

#include "precompiled.h"

#define MSG_N1		"Welcome to World of WarCraft 3.3.5!"
#define MSG_COMBAT	"You Are In Combat."

bool GossipHello_custom_npc_news(Player* pPlayer, Creature* pCreature)
{

    // Top Menu News & Updates
    pPlayer->ADD_GOSSIP_ITEM( 3, "Realm News & Updates"     , GOSSIP_SENDER_MAIN, 1000);
    // Close Menu
    pPlayer->ADD_GOSSIP_ITEM( 0, "No Thanks"                , GOSSIP_SENDER_MAIN, 10000);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

    return true;
}

void SendDefaultMenu_custom_npc_news(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{
    if (pPlayer->isInCombat())
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterSay(MSG_COMBAT, LANG_UNIVERSAL, NULL);
        return;
    }

    switch(uiAction)
    {
        case 1000: // News & Updates submenu
            pPlayer->ADD_GOSSIP_ITEM( 3, "20th July 2010"   , GOSSIP_SENDER_MAIN, 1001);
            pPlayer->ADD_GOSSIP_ITEM( 3, "No Thanks"        , GOSSIP_SENDER_MAIN, 10000);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 1001: // Actual News
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay(MSG_N1, LANG_UNIVERSAL, NULL);
        break; 

        case 10000: //Close Menu
            pPlayer->CLOSE_GOSSIP_MENU();
        break;


    }
}

bool GossipSelect_custom_npc_news(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiSender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_custom_npc_news(pPlayer, pCreature, uiAction);

    return true;
}

void AddSC_custom_npc_news()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_npc_news";
    newscript->pGossipHello     = &GossipHello_custom_npc_news;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_news;
    newscript->RegisterSelf();
}