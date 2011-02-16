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
SDName: custom_npc_guildmaster
SD%Complete: 100
SDComment: Guildmaster - Used for Guild Halls
SDCategory: NPC
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
SDTodo: Revamp this script.
EndScriptData */

#include "precompiled.h"

extern DatabaseMysql SD2Database;

#define MSG_GOSSIP_TELE          "Teleport to Guild Hall."
#define MSG_GOSSIP_BUY           "Buy Guild Hall (10000 Gold)."
#define MSG_GOSSIP_SELL          "Sell Guild Hall (5000 Gold)."
#define MSG_GOSSIP_NEXTPAGE      "Next -->"
#define MSG_INCOMBAT             "Your in combat!"
#define MSG_NOGUILDHOUSE         "Your Guild doesn't have a Guild Hall."
#define MSG_NOFREEGH             "Sorry, but all Guild Halls are taken."
#define MSG_ALREADYHAVEGH        "Sorry, but you already have a Guild Hall."
#define MSG_NOTENOUGHMONEY       "Sorry, but you dont have enough gold."
#define MSG_GHOCCUPIED           "Sorry, but this Guild Hall is already taken."
#define MSG_CONGRATULATIONS      "You have been teleported to your Guild Hall."
#define MSG_SOLD                 "You have sold your Guild Hall, here is your gold."
#define MSG_NOTINGUILD           "You dont have a guild! Join or create new one."
#define MSG_HASGUILDHOUSE        "Your guild already has a Guild Hall, get lost n00b!"

#define OFFSET_GH_ID_TO_ACTION   1500
#define OFFSET_SHOWBUY_FROM      10000

#define ACTION_TELE              1001
#define ACTION_SHOW_BUYLIST      1002
#define ACTION_SELL_GUILDHOUSE   1003

#define ICON_GOSSIP_BALOON       0
#define ICON_GOSSIP_WING         2
#define ICON_GOSSIP_BOOK         3
#define ICON_GOSSIP_WHEEL1       4
#define ICON_GOSSIP_WHEEL2       5
#define ICON_GOSSIP_GOLD         6
#define ICON_GOSSIP_BALOONDOTS   7
#define ICON_GOSSIP_TABARD       8
#define ICON_GOSSIP_XSWORDS      9

#define COST_GH_BUY              100000000  //10000 G.
#define COST_GH_SELL             50000000   //5000 G.

#define GOSSIP_COUNT_MAX         10

bool isPlayerGuildLeader(Player *player)
{
    return (player->GetRank() == 0) && (player->GetGuildId() != 0);
}

bool getGuildHouseCoords(uint32 guildId, float &x, float &y, float &z, uint32 &map)
{
    if (guildId == 0)
    {
        //if player has no guild
        return false;
    }

    QueryResult *result;
    result = SD2Database.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhalls` WHERE `guildId` = %u", guildId);
    if(result)
    {
        Field *fields = result->Fetch();
        x = fields[0].GetFloat();
        y = fields[1].GetFloat();
        z = fields[2].GetFloat();
        map = fields[3].GetUInt32();
        delete result;
        return true;
    }
    return false;
}

void teleportPlayerToGuildHouse(Player *player, Creature *_creature)
{
    if (player->GetGuildId() == 0)
    {
        //if player has no guild
        _creature->MonsterWhisper(MSG_NOTINGUILD, player, false);
        return;
    }

    if (!player->getAttackers().empty())
    {
        //if player in combat
        _creature->MonsterSay(MSG_INCOMBAT, LANG_UNIVERSAL, player);
        return;
    }

    float x, y, z;
    uint32 map;

    if (getGuildHouseCoords(player->GetGuildId(), x, y, z, map))
    {
        //teleport player to the specified location
        player->TeleportTo(map, x, y, z, 0.0f);
    }
    else
        _creature->MonsterWhisper(MSG_NOGUILDHOUSE, player, false);

}

bool showBuyList(Player *player, Creature *_creature, uint32 showFromId = 0)
{
    //show not occupied guildhalls

    QueryResult *result;
    result = SD2Database.PQuery("SELECT `id`, `comment` FROM `guildhalls` WHERE `guildId` = 0 AND `id` > %u ORDER BY `id` ASC LIMIT %u", showFromId, GOSSIP_COUNT_MAX);

    if (result)
    {
        uint32 guildhouseId = 0;
        std::string comment = "";
        do {

            Field *fields = result->Fetch();

            guildhouseId = fields[0].GetInt32();
            comment = fields[1].GetString();
            
            //send comment as a gossip item
            //transmit guildhouseId in Action variable
            player->ADD_GOSSIP_ITEM(ICON_GOSSIP_TABARD, comment, GOSSIP_SENDER_MAIN,
                guildhouseId + OFFSET_GH_ID_TO_ACTION);

        } while (result->NextRow());

        if (result->GetRowCount() == GOSSIP_COUNT_MAX)
        {
            //assume that we have additional page
            //add link to next GOSSIP_COUNT_MAX items
            player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOONDOTS, MSG_GOSSIP_NEXTPAGE, GOSSIP_SENDER_MAIN, 
                guildhouseId + OFFSET_SHOWBUY_FROM);
        }

        delete result;

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());

        return true;
    }
    else
    {
        if (showFromId = 0)
        {
            //all guildhalls are occupied
            _creature->MonsterWhisper(MSG_NOFREEGH, player, false);
            player->CLOSE_GOSSIP_MENU();
        }
        else
        {
            //this condition occurs when COUNT(guildhalls) % GOSSIP_COUNT_MAX == 0
            //just show GHs from beginning
            showBuyList(player, _creature, 0);
        }
    }
    return false;
}

bool isPlayerHasGuildhouse(Player *player, Creature *_creature, bool whisper = false)
{

    QueryResult *result;

    result = SD2Database.PQuery("SELECT `comment` FROM `guildhalls` WHERE `guildId` = %u", player->GetGuildId());

    if (result)
    {
        if (whisper)
        {
            //whisper to player "already have etc..."
            Field *fields = result->Fetch();
            char msg[100];
            sprintf(msg, MSG_ALREADYHAVEGH, fields[0].GetString());
            _creature->MonsterWhisper(msg, player, false);
        }
        delete result;
        return true;
    }
    return false;
}

void buyGuildhouse(Player *player, Creature *_creature, uint32 guildhouseId)
{
    if (player->GetMoney() < COST_GH_BUY)
    {
        //show how much money player need to buy GH (in gold)
        char msg[100];
        sprintf(msg, MSG_NOTENOUGHMONEY, COST_GH_BUY / 10000);
        _creature->MonsterWhisper(msg, player, false);
        return;
    }

    if (isPlayerHasGuildhouse(player, _creature, true))
    {
        //show how much money player need to buy GH (in gold)
        char msg[100];
        sprintf(msg, MSG_HASGUILDHOUSE);
        _creature->MonsterWhisper(msg, player, false);
        return;
    }

    QueryResult *result;

    //check if somebody already occupied this GH
    result = SD2Database.PQuery("SELECT `id` FROM `guildhalls` WHERE `id` = %u AND `guildId` <> 0", guildhouseId);

    if (result)
    {
        delete result;
        _creature->MonsterWhisper(MSG_GHOCCUPIED, player, false);
        return;
    }

    //update DB
    result = SD2Database.PQuery("UPDATE `guildhalls` SET `guildId` = %u WHERE `id` = %u", player->GetGuildId(), guildhouseId);

    if (result)
        delete result;

    player->ModifyMoney(-COST_GH_BUY);
    _creature->MonsterSay(MSG_CONGRATULATIONS, LANG_UNIVERSAL, player);
    
}

void sellGuildhouse(Player *player, Creature *_creature)
{
    if (isPlayerHasGuildhouse(player, _creature))
    {
        QueryResult *result;

        result = SD2Database.PQuery("UPDATE `guildhalls` SET `guildId` = 0 WHERE `guildId` = %u", player->GetGuildId());
        
        if (result)
            delete result;

        player->ModifyMoney(COST_GH_SELL);

        //display message e.g. "here your money etc."
        char msg[100];
        sprintf(msg, MSG_SOLD, COST_GH_SELL / 10000);
        _creature->MonsterWhisper(msg, player, false);
    }
}

bool GossipHello_custom_npc_guildmaster(Player *player, Creature *_creature)
{
    player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOON, MSG_GOSSIP_TELE, GOSSIP_SENDER_MAIN, ACTION_TELE);

    if (isPlayerGuildLeader(player))
    {
        //show additional menu for guild leader
        player->ADD_GOSSIP_ITEM(ICON_GOSSIP_GOLD, MSG_GOSSIP_BUY, GOSSIP_SENDER_MAIN, ACTION_SHOW_BUYLIST);
        if (isPlayerHasGuildhouse(player, _creature))
        {
            //and additional for guildhouse owner
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(ICON_GOSSIP_GOLD, MSG_GOSSIP_SELL, GOSSIP_SENDER_MAIN, ACTION_SELL_GUILDHOUSE, 0, true);
        }
    }
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
    return true;
}

bool GossipSelect_custom_npc_guildmaster(Player *player, Creature *_creature, uint32 sender, uint32 action )
{
    if (sender != GOSSIP_SENDER_MAIN)
        return false;

    switch (action)
    {
        case ACTION_TELE:
        {
            //teleport player to GH
            player->CLOSE_GOSSIP_MENU();
            teleportPlayerToGuildHouse(player, _creature);
            break;
        }
        case ACTION_SHOW_BUYLIST:
        {
            //show list of GHs which currently not occupied
            showBuyList(player, _creature);
            break;
        }
        case ACTION_SELL_GUILDHOUSE:
        {
            sellGuildhouse(player, _creature);
            player->CLOSE_GOSSIP_MENU();
            break;
        }
        default:
        {
            if (action > OFFSET_SHOWBUY_FROM)
            {
                showBuyList(player, _creature, action - OFFSET_SHOWBUY_FROM);
            }
            else if (action > OFFSET_GH_ID_TO_ACTION)
            {
                //player clicked on buy list
                player->CLOSE_GOSSIP_MENU();
                //get guildhouseId from action
                //guildhouseId = action - OFFSET_GH_ID_TO_ACTION
                buyGuildhouse(player, _creature, action - OFFSET_GH_ID_TO_ACTION);
            }
            break;
        }
    }
    return true;
}

void AddSC_custom_npc_guildmaster()
{
    Script *newscript;
    newscript                   = new Script;
    newscript->Name             = "custom_npc_guildmaster";
    newscript->pGossipHello     = &GossipHello_custom_npc_guildmaster;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_guildmaster;
    newscript->RegisterSelf();
}