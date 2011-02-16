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
SDName: custom_npc_titlemaster
SD%Complete: 100
SDComment: Title Master - Allows various tiles to be gained.
SDCategory: NPC
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
EndScriptData */

#include "precompiled.h"

#define GOSSIP_BUY_ALLIANCE_TITLES		"Buy |cffff0000Alliance|r Titles"
#define GOSSIP_BUY_HORDE_TITLES			"Buy |cffff0000Horde|r Titles"
#define GOSSIP_BUY_OTHER_TITLES			"Buy |cFF959697Other|r Titles"
#define MSG_COMBAT						"You are in combat!"
#define MSG_SHORT_ON_GOLD_OR_HONOR		"You dont have enough money or Honor Points!"
#define SD2_ERROR_LOG					"SD2: Unable to open configuration file!"

bool GossipHello_custom_npc_titlemaster(Player* pPlayer, Creature* pCreature)
{

    // Alliance Titles
    if (pPlayer->GetTeam() == ALLIANCE)
    {
        pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_ALLIANCE_TITLES, GOSSIP_SENDER_MAIN, 1000);
        // Other Tiles
        pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_OTHER_TITLES, GOSSIP_SENDER_MAIN, 3000);
    }
    else // Horde Titles
    {
        pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_HORDE_TITLES, GOSSIP_SENDER_MAIN, 2000);
        // Other Titles
        pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_OTHER_TITLES, GOSSIP_SENDER_MAIN, 3000);
    }

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

    return true;
}

void SendDefaultMenu_custom_npc_titlemaster(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

    // Not allow in combat
    if (pPlayer->isInCombat())
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterSay(MSG_COMBAT, LANG_UNIVERSAL, NULL);
        return;
    }

    switch(uiAction)
    {
        case 1000: //Alliance Titles
                pPlayer->ADD_GOSSIP_ITEM( 9, "Private. |cffff0000Costs:|r 100HP-200Gold"				, GOSSIP_SENDER_MAIN, 1001);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Corporal. |cffff0000Costs:|r 150HP-300Gold"				, GOSSIP_SENDER_MAIN, 1002);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Sergeant. |cffff0000Costs:|r 200HP-400Gold"				, GOSSIP_SENDER_MAIN, 1003);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Master Sergeant. |cffff0000Costs:|r 250HP-500Gold"		, GOSSIP_SENDER_MAIN, 1004);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Sergeant Major. |cffff0000Costs:|r 300HP-600Gold"			, GOSSIP_SENDER_MAIN, 1005);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Knight. |cffff0000Costs:|r 400HP-800Gold"					, GOSSIP_SENDER_MAIN, 1006);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Knight-Lieutenant. |cffff0000Costs:|r 500HP-1000Gold"		, GOSSIP_SENDER_MAIN, 1007);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Knight-Captain. |cffff0000Costs:|r 650HP-1300Gold"		, GOSSIP_SENDER_MAIN, 1008);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Knight-Champion. |cffff0000Costs:|r 800HP-1600Gold"		, GOSSIP_SENDER_MAIN, 1009);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Lieutenant Commander. |cffff0000Costs:|r 1000HP-2000Gold"	, GOSSIP_SENDER_MAIN, 1010);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Commander. |cffff0000Costs:|r 1200HP-2400Gold"			, GOSSIP_SENDER_MAIN, 1011);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Marshal. |cffff0000Costs:|r 1500HP-3000Gold"				, GOSSIP_SENDER_MAIN, 1012);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Field Marshal. |cffff0000Costs:|r 1800HP-3600Gold"		, GOSSIP_SENDER_MAIN, 1013);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"													, GOSSIP_SENDER_MAIN, 1500);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"												, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 1500: //Alliance Titles 2
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Marshal. |cffff0000Costs:|r 3600HP-7200Gold"		, GOSSIP_SENDER_MAIN, 1014);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of The Alliance. |cffff0000Costs:|r 5000HP-10000Gold"		, GOSSIP_SENDER_MAIN, 1015);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Justicar. |cffff0000Costs:|r 2000Gold"					, GOSSIP_SENDER_MAIN, 1016);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Flame Warden. |cffff0000Costs:|r 2000Gold"				, GOSSIP_SENDER_MAIN, 1017);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Exodar. |cffff0000Costs:|r 4000Gold"				, GOSSIP_SENDER_MAIN, 1018);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Darnassus. |cffff0000Costs:|r 4000Gold"				, GOSSIP_SENDER_MAIN, 1019);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Ironforge. |cffff0000Costs:|r 4000Gold"				, GOSSIP_SENDER_MAIN, 1020);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Stormwind. |cffff0000Costs:|r 4000Gold"				, GOSSIP_SENDER_MAIN, 1021);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Gnomeregan. |cffff0000Costs:|r 4000Gold"				, GOSSIP_SENDER_MAIN, 1022);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"													, GOSSIP_SENDER_MAIN, 1000);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"												, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 2000: //Horde Titles
                pPlayer->ADD_GOSSIP_ITEM( 9, "Scout. |cffff0000Costs:|r 100HP-200Gold"					, GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grunt. |cffff0000Costs:|r 150HP-300Gold"					, GOSSIP_SENDER_MAIN, 2002);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Sergeant. |cffff0000Costs:|r 200HP-400Gold"				, GOSSIP_SENDER_MAIN, 2003);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Senior Sergeant. |cffff0000Costs:|r 250HP-500Gold"		, GOSSIP_SENDER_MAIN, 2004);
                pPlayer->ADD_GOSSIP_ITEM( 9, "First Sergeant. |cffff0000Costs:|r 300HP-600Gold"			, GOSSIP_SENDER_MAIN, 2005);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Stone Guard. |cffff0000Costs:|r 400HP-800Gold"			, GOSSIP_SENDER_MAIN, 2006);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Blood Guard. |cffff0000Costs:|r 500HP-1000Gold"			, GOSSIP_SENDER_MAIN, 2007);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Legionnaire. |cffff0000Costs:|r 650HP-1300Gold"			, GOSSIP_SENDER_MAIN, 2008);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Centurion. |cffff0000Costs:|r 800HP-1600Gold"				, GOSSIP_SENDER_MAIN, 2009);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Champion. |cffff0000Costs:|r 1000HP-2000Gold"				, GOSSIP_SENDER_MAIN, 2010);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Lieutenant General. |cffff0000Costs:|r 1200HP-2400Gold"	, GOSSIP_SENDER_MAIN, 2011);
                pPlayer->ADD_GOSSIP_ITEM( 9, "General. |cffff0000Costs:|r 1500HP-3000Gold"				, GOSSIP_SENDER_MAIN, 2012);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Warlord. |cffff0000Costs:|r 1800HP-3600Gold"				, GOSSIP_SENDER_MAIN, 2013);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"													, GOSSIP_SENDER_MAIN, 2500);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"												, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 2500: //Horde Titles 2
                pPlayer->ADD_GOSSIP_ITEM( 9, "High Warlord. |cffff0000Costs:|r 3600HP-7200Gold"		, GOSSIP_SENDER_MAIN, 2014);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Horde. |cffff0000Costs:|r 5000HP-10000Gold"	, GOSSIP_SENDER_MAIN, 2015);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Conqueror. |cffff0000Costs:|r 2000Gold"				, GOSSIP_SENDER_MAIN, 2016);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Flame Keeper. |cffff0000Costs:|r 2000Gold"			, GOSSIP_SENDER_MAIN, 2017);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Orgrimmar. |cffff0000Costs:|r 4000Gold"			, GOSSIP_SENDER_MAIN, 2018);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Sen'jin. |cffff0000Costs:|r 4000Gold"				, GOSSIP_SENDER_MAIN, 2019);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Silvermoon. |cffff0000Costs:|r 4000Gold"			, GOSSIP_SENDER_MAIN, 2020);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Thunder Bluff. |cffff0000Costs:|r 4000Gold"		, GOSSIP_SENDER_MAIN, 2021);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Undercity. |cffff0000Costs:|r 4000Gold"		, GOSSIP_SENDER_MAIN, 2022);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"												, GOSSIP_SENDER_MAIN, 2000);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"											, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3000: //Other Titles
                pPlayer->ADD_GOSSIP_ITEM( 9, "Gladiator. |cffff0000Costs:|r 100HP-200Gold"				, GOSSIP_SENDER_MAIN, 3001);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Duelist. |cffff0000Costs:|r 200HP-400Gold"				, GOSSIP_SENDER_MAIN, 3002);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Rival. |cffff0000Costs:|r 300HP-600Gold"					, GOSSIP_SENDER_MAIN, 3003);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Challenger. |cffff0000Costs:|r 250HP-500Gold"				, GOSSIP_SENDER_MAIN, 3004);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Scarab Lord. |cffff0000Costs:|r 4000Gold"					, GOSSIP_SENDER_MAIN, 3005);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Champion of the Naaru. |cffff0000Costs:|r 4000Gold"		, GOSSIP_SENDER_MAIN, 3006);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Merciless Gladiator. |cffff0000Costs:|r 500HP-1000Gold"	, GOSSIP_SENDER_MAIN, 3007);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Shattered Sun. |cffff0000Costs:|r 4000Gold"		, GOSSIP_SENDER_MAIN, 3008);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Hand of A'dal. |cffff0000Costs:|r 4000Gold"				, GOSSIP_SENDER_MAIN, 3009);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Vengeful Gladiator. |cffff0000Costs:|r 600HP-1200Gold"	, GOSSIP_SENDER_MAIN, 3010);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Battlemaster. |cffff0000Costs:|r 15000Gold"				, GOSSIP_SENDER_MAIN, 3011);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Elder. |cffff0000Costs:|r 6000Gold"						, GOSSIP_SENDER_MAIN, 3012);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Exalted. |cffff0000Costs:|r 10000Gold"				, GOSSIP_SENDER_MAIN, 3013);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"													, GOSSIP_SENDER_MAIN, 3500);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"												, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3500: //Other Titles 2
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Explorer. |cffff0000Costs:|r 10000Gold"				, GOSSIP_SENDER_MAIN, 3014);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Diplomat. |cffff0000Costs:|r 8000Gold"				, GOSSIP_SENDER_MAIN, 3015);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Brutal Gladiator. |cffff0000Costs:|r 700HP-1400Gold"		, GOSSIP_SENDER_MAIN, 3016);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Seeker. |cffff0000Costs:|r 8000Gold"					, GOSSIP_SENDER_MAIN, 3017);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Arena Master. |cffff0000Costs:|r 15000Gold"				, GOSSIP_SENDER_MAIN, 3018);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Salty. |cffff0000Costs:|r 4000Gold"						, GOSSIP_SENDER_MAIN, 3019);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Chef. |cffff0000Costs:|r 4000Gold"						, GOSSIP_SENDER_MAIN, 3020);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Supreme. |cffff0000Costs:|r 10000Gold"				, GOSSIP_SENDER_MAIN, 3021);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Ten Storms. |cffff0000Costs:|r 8000Gold"			, GOSSIP_SENDER_MAIN, 3022);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Emerald Dream. |cffff0000Costs:|r 8000Gold"		, GOSSIP_SENDER_MAIN, 3023);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Prophet. |cffff0000Costs:|r 8000Gold"						, GOSSIP_SENDER_MAIN, 3024);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Malefic. |cffff0000Costs:|r 8000Gold"					, GOSSIP_SENDER_MAIN, 3025);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"													, GOSSIP_SENDER_MAIN, 3501);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"													, GOSSIP_SENDER_MAIN, 3000);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"												, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3501: //Other Titles 3
                pPlayer->ADD_GOSSIP_ITEM( 9, "Stalker. |cffff0000Costs:|r 8000Gold"					, GOSSIP_SENDER_MAIN, 3026);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Ebon Blade. |cffff0000Costs:|r 8000Gold"		, GOSSIP_SENDER_MAIN, 3027);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Archmage. |cffff0000Costs:|r 8000Gold"				, GOSSIP_SENDER_MAIN, 3028);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Warbringer. |cffff0000Costs:|r  8000Gold"				, GOSSIP_SENDER_MAIN, 3029);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Assassin. |cffff0000Costs:|r 8000Gold"				, GOSSIP_SENDER_MAIN, 3030);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Alchemist. |cffff0000Costs:|r 2000Gold"	, GOSSIP_SENDER_MAIN, 3031);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Blacksmith. |cffff0000Costs:|r 2000Gold"	, GOSSIP_SENDER_MAIN, 3032);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Iron Chef. |cffff0000Costs:|r 2000Gold"				, GOSSIP_SENDER_MAIN, 3033);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Enchanter. |cffff0000Costs:|r 2000Gold"	, GOSSIP_SENDER_MAIN, 3034);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Engineer. |cffff0000Costs:|r 2000Gold"	, GOSSIP_SENDER_MAIN, 3035);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Doctor. |cffff0000Costs:|r 2000Gold"					, GOSSIP_SENDER_MAIN, 3036);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Angler. |cffff0000Costs:|r 2000Gold"		, GOSSIP_SENDER_MAIN, 3037);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"												, GOSSIP_SENDER_MAIN, 3502);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"												, GOSSIP_SENDER_MAIN, 3500);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"											, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3502: //Other Titles 4
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Herbalist. |cffff0000Costs:|r 2000Gold"			, GOSSIP_SENDER_MAIN, 3038);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Scribe. |cffff0000Costs:|r 2000Gold"				, GOSSIP_SENDER_MAIN, 3039);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Jewelcrafter. |cffff0000Costs:|r 2000Gold"		, GOSSIP_SENDER_MAIN, 3040);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Leatherworker. |cffff0000Costs:|r 2000Gold"		, GOSSIP_SENDER_MAIN, 3041);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Miner. |cffff0000Costs:|r 2000Gold"				, GOSSIP_SENDER_MAIN, 3042);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Skinner. |cffff0000Costs:|r 2000Gold"			, GOSSIP_SENDER_MAIN, 3043);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Master Tailor. |cffff0000Costs:|r 2000Gold"				, GOSSIP_SENDER_MAIN, 3044);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Quel'Thalas. |cffff0000Costs:|r 4000Gold"					, GOSSIP_SENDER_MAIN, 3045);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Argus. |cffff0000Costs:|r 4000Gold"						, GOSSIP_SENDER_MAIN, 3046);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of Khaz Modan. |cffff0000Costs:|r 4000Gold"					, GOSSIP_SENDER_MAIN, 3047);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Lion Hearted. |cffff0000Costs:|r 6000Gold"				, GOSSIP_SENDER_MAIN, 3048);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Champion of Elune. |cffff0000Costs:|r 6000Gold"				, GOSSIP_SENDER_MAIN, 3049);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"														, GOSSIP_SENDER_MAIN, 3503);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"														, GOSSIP_SENDER_MAIN, 3501);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"													, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3503: //Other Titles 5
                pPlayer->ADD_GOSSIP_ITEM( 9, "Hero of Orgrimmar. |cffff0000Costs:|r 6000Gold"						, GOSSIP_SENDER_MAIN, 3050);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Plainsrunner. |cffff0000Costs:|r 6000Gold"							, GOSSIP_SENDER_MAIN, 3051);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Darkspear. |cffff0000Costs:|r 6000Gold"						, GOSSIP_SENDER_MAIN, 3052);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Forsaken. |cffff0000Costs:|r 6000Gold"							, GOSSIP_SENDER_MAIN, 3053);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Magic Seeker. |cffff0000Costs:|r 6000Gold"						, GOSSIP_SENDER_MAIN, 3054);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Twilight Vanquisher. |cffff0000Costs:|r 8000Gold"						, GOSSIP_SENDER_MAIN, 3055);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Conqueror of Naxxramas. |cffff0000Costs:|r 8000Gold"					, GOSSIP_SENDER_MAIN, 3056);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Hero of Northrend. |cffff0000Costs:|r 8000Gold"						, GOSSIP_SENDER_MAIN, 3057);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Hallowed. |cffff0000Costs:|r 6000Gold"							, GOSSIP_SENDER_MAIN, 3058);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Loremaster. |cffff0000Costs:|r 10000Gold"								, GOSSIP_SENDER_MAIN, 3059);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Flawless Victor. |cffff0000Costs:|r 100000Gold"					, GOSSIP_SENDER_MAIN, 3060);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Champion of the Frozen Wastes. |cffff0000Costs:|r 6000Gold"			, GOSSIP_SENDER_MAIN, 3061);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"																, GOSSIP_SENDER_MAIN, 3504);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"																, GOSSIP_SENDER_MAIN, 3503);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"															, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3504: //Other Titles 6
                pPlayer->ADD_GOSSIP_ITEM( 9, "Ambassador. |cffff0000Costs:|r 10000Gold"				, GOSSIP_SENDER_MAIN, 3062);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Argent Champion. |cffff0000Costs:|r 8000Gold"		, GOSSIP_SENDER_MAIN, 3063);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Guardian of Cenarius. |cffff0000Costs:|r 6000Gold"	, GOSSIP_SENDER_MAIN, 3064);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Brewmaster. |cffff0000Costs:|r 6000Gold"				, GOSSIP_SENDER_MAIN, 3065);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Merrymaker. |cffff0000Costs:|r 6000Gold"				, GOSSIP_SENDER_MAIN, 3066);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Love Fool. |cffff0000Costs:|r 6000Gold"			, GOSSIP_SENDER_MAIN, 3067);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Matron. |cffff0000Costs:|r 6000Gold"					, GOSSIP_SENDER_MAIN, 3068);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Patron. |cffff0000Costs:|r 6000Gold"					, GOSSIP_SENDER_MAIN, 3069);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Obsidian Slayer. |cffff0000Costs:|r 6000Gold"			, GOSSIP_SENDER_MAIN, 3070);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Nightfall. |cffff0000Costs:|r 6000Gold"		, GOSSIP_SENDER_MAIN, 3071);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Immortal. |cffff0000Costs:|r 20000Gold"			, GOSSIP_SENDER_MAIN, 3072);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Undying. |cffff0000Costs:|r 10000Gold"			, GOSSIP_SENDER_MAIN, 3073);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"												, GOSSIP_SENDER_MAIN, 3503);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"												, GOSSIP_SENDER_MAIN, 3505);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"											, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3505: //Other Titles 7
                pPlayer->ADD_GOSSIP_ITEM( 9, "Jenkins. |cffff0000Costs:|r 6000Gold"						, GOSSIP_SENDER_MAIN, 3074);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Bloodsail Admiral. |cffff0000Costs:|r 4000Gold"			, GOSSIP_SENDER_MAIN, 3075);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Insane. |cffff0000Costs:|r 10000Gold"					, GOSSIP_SENDER_MAIN, 3076);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Noble. |cffff0000Costs:|r 10000Gold"					, GOSSIP_SENDER_MAIN, 3077);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Crusader. |cffff0000Costs:|r 8000Gold"					, GOSSIP_SENDER_MAIN, 3078);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Deadly Gladiator. |cffff0000Costs:|r 800HP-1600Gold"		, GOSSIP_SENDER_MAIN, 3079);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Death's Demise. |cffff0000Costs:|r 8000Gold"				, GOSSIP_SENDER_MAIN, 3080);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Celestial Defender. |cffff0000Costs:|r 8000Gold"		, GOSSIP_SENDER_MAIN, 3081);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Conqueror of Ulduar. |cffff0000Costs:|r 8000Gold"			, GOSSIP_SENDER_MAIN, 3082);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Champion of Ulduar. |cffff0000Costs:|r 10000Gold"			, GOSSIP_SENDER_MAIN, 3083);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Vanquisher. |cffff0000Costs:|r 10000Gold"					, GOSSIP_SENDER_MAIN, 3084);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Starcaller. |cffff0000Costs:|r 8000Gold"					, GOSSIP_SENDER_MAIN, 3085);
                pPlayer->ADD_GOSSIP_ITEM( 7, "Next ->"													, GOSSIP_SENDER_MAIN, 3506);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"													, GOSSIP_SENDER_MAIN, 3504);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"												, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        case 3506: //Other Titles 8
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Astral Walker. |cffff0000Costs:|r 8000Gold"			, GOSSIP_SENDER_MAIN, 3086);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Herald of the Titans. |cffff0000Costs:|r 8000Gold"		, GOSSIP_SENDER_MAIN, 3087);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Furious Gladiator. |cffff0000Costs:|r 100HP-200Gold"		, GOSSIP_SENDER_MAIN, 3088);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Pilgrim. |cffff0000Costs:|r 900HP-1800Gold"			, GOSSIP_SENDER_MAIN, 3089);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Relentless Gladiator. |cffff0000Costs:|r 8000Gold"		, GOSSIP_SENDER_MAIN, 3090);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Grand Crusader. |cffff0000Costs:|r 100000Gold"			, GOSSIP_SENDER_MAIN, 3091);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Argent Defender. |cffff0000Costs:|r 8000Gold"			, GOSSIP_SENDER_MAIN, 3092);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Patient. |cffff0000Costs:|r 8000Gold"					, GOSSIP_SENDER_MAIN, 3093);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Light of Dawn. |cffff0000Costs:|r 10000Gold"			, GOSSIP_SENDER_MAIN, 3094);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Bane of the Fallen King. |cffff0000Costs:|r 800Gold"		, GOSSIP_SENDER_MAIN, 3095);
                pPlayer->ADD_GOSSIP_ITEM( 9, "the Kingslayer. |cffff0000Costs:|r 10000Gold"				, GOSSIP_SENDER_MAIN, 3096);
                pPlayer->ADD_GOSSIP_ITEM( 9, "of the Ashen Verdict. |cffff0000Costs:|r 8000Gold"		, GOSSIP_SENDER_MAIN, 3097);
                pPlayer->ADD_GOSSIP_ITEM( 9, "Wrathful Gladiator. |cffff0000Costs:|r 1200HP-2400Gold"	, GOSSIP_SENDER_MAIN, 3098);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Back"													, GOSSIP_SENDER_MAIN, 3505);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"												, GOSSIP_SENDER_MAIN, 8000);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;

        //Back To Main Menu
        case 8000:
            // Alliance Titles
            if (pPlayer->GetTeam() == ALLIANCE)
            {
                pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_ALLIANCE_TITLES, GOSSIP_SENDER_MAIN, 1000);
                // Other Tiles
                pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_OTHER_TITLES, GOSSIP_SENDER_MAIN, 3000);
            }
            else // Horde Titles
            {
                pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_HORDE_TITLES, GOSSIP_SENDER_MAIN, 2000);
                // Other Titles
                pPlayer->ADD_GOSSIP_ITEM( 9, GOSSIP_BUY_OTHER_TITLES, GOSSIP_SENDER_MAIN, 3000);
            }

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

        break;

        //////////////////////////////////////////////////Alliance Titles///////////////////////////////////////////////////////////////

        case 1001: // Private Title
                if ((pPlayer->GetHonorPoints() >= 100) && (pPlayer->GetMoney() >=2000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(1))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Private' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-2000000);
                        pPlayer->ModifyHonorPoints(-100);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Private' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1002: // Corporal Title
                if ((pPlayer->GetHonorPoints() >= 150) && (pPlayer->GetMoney() >=3000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(2))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Corporal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-3000000);
                        pPlayer->ModifyHonorPoints(-150);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Corporal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1003: // Sergeant Title
                if ((pPlayer->GetHonorPoints() >= 200) && (pPlayer->GetMoney() >=4000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(3))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-4000000);
                        pPlayer->ModifyHonorPoints(-200);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1004: // Master Sergeant Title
                if ((pPlayer->GetHonorPoints() >= 250) && (pPlayer->GetMoney() >=5000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(4))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Master Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-5000000);
                        pPlayer->ModifyHonorPoints(-250);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Master Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1005: // Sergeant Major Title
                if ((pPlayer->GetHonorPoints() >= 300) && (pPlayer->GetMoney() >=6000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(5))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Sergeant Major' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-6000000);
                        pPlayer->ModifyHonorPoints(-300);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Sergeant Major' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1006: // Knight Title
                if ((pPlayer->GetHonorPoints() >= 400) && (pPlayer->GetMoney() >=8000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(6))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Knight' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-8000000);
                        pPlayer->ModifyHonorPoints(-400);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Knight' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1007: // Knight-Lieutenant Title
                if ((pPlayer->GetHonorPoints() >= 500) && (pPlayer->GetMoney() >=10000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(7))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Knight-Lieutenant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-10000000);
                        pPlayer->ModifyHonorPoints(-500);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Knight-Lieutenant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1008: // Knight-Captain Title
                if ((pPlayer->GetHonorPoints() >= 650) && (pPlayer->GetMoney() >=13000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(8))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Knight-Captain' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-13000000);
                        pPlayer->ModifyHonorPoints(-650);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Knight-Captain' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1009: // Knight-Champion Title
                if ((pPlayer->GetHonorPoints() >= 800) && (pPlayer->GetMoney() >=16000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(9))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Knight-Champion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-16000000);
                        pPlayer->ModifyHonorPoints(-800);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Knight-Champion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1010: // Lieutenant Commander Title
                if ((pPlayer->GetHonorPoints() >= 1000) && (pPlayer->GetMoney() >=20000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(10))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Lieutenant Commander' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pPlayer->ModifyHonorPoints(-1000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Lieutenant Commander' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1011: // Commander Title
                if ((pPlayer->GetHonorPoints() >= 1200) && (pPlayer->GetMoney() >=24000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(11))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Commander' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-24000000);
                        pPlayer->ModifyHonorPoints(-1200);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Commander' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1012: // Marshal Title
                if ((pPlayer->GetHonorPoints() >= 1500) && (pPlayer->GetMoney() >=30000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(12))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Marshal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-30000000);
                        pPlayer->ModifyHonorPoints(-1500);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Marshal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1013: // Field Marshal Title
                if ((pPlayer->GetHonorPoints() >= 1800) && (pPlayer->GetMoney() >=36000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(13))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Field Marshal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-36000000);
                        pPlayer->ModifyHonorPoints(-1800);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Field Marshal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1014: // Knight-Lieutenant Title
                if ((pPlayer->GetHonorPoints() >= 3600) && (pPlayer->GetMoney() >=72000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(14))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Marshal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-72000000);
                        pPlayer->ModifyHonorPoints(-3600);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Marshal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1015: // of the Alliance Title
                if ((pPlayer->GetHonorPoints() >= 5000) && (pPlayer->GetMoney() >=100000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(126))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Alliance' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pPlayer->ModifyHonorPoints(-5000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Alliance' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1016: // Justicar Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(48))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Justicar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Justicar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1017: // Flame Warden Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(75))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Flame Warden' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Flame Warden' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1018: // of the Exodar Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(146))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Exodar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Exodar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1019: // of Darnassus Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(147))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Darnassus' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Darnassus' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1020: // of the Ironforge Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(148))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Ironforge' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Ironforge' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1021: // of Stormwind Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(149))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Stormwind' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Stormwind' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 1022: // of Gnomeregan Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(113))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Gnomeregan' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Gnomeregan' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

         pPlayer->CLOSE_GOSSIP_MENU();

        //////////////////////////////////////////////////Horde Titles//////////////////////////////////////////////////////////////////

        case 2001: // Scout Title
                if ((pPlayer->GetHonorPoints() >= 100) && (pPlayer->GetMoney() >=2000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(15))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Scout' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-2000000);
                        pPlayer->ModifyHonorPoints(-100);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Scout' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2002: // Grunt Title
                if ((pPlayer->GetHonorPoints() >= 150) && (pPlayer->GetMoney() >=3000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(16))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grunt' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-3000000);
                        pPlayer->ModifyHonorPoints(-150);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grunt' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2003: // Sergeant Title
                if ((pPlayer->GetHonorPoints() >= 200) && (pPlayer->GetMoney() >=4000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(17))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-4000000);
                        pPlayer->ModifyHonorPoints(-200);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2004: // Senior Sergeant Title
                if ((pPlayer->GetHonorPoints() >= 250) && (pPlayer->GetMoney() >=5000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(18))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Senior Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-5000000);
                        pPlayer->ModifyHonorPoints(-250);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Senior Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2005: // First Sergeant Title
                if ((pPlayer->GetHonorPoints() >= 300) && (pPlayer->GetMoney() >=6000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(19))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'First Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-6000000);
                        pPlayer->ModifyHonorPoints(-300);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'First Sergeant' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2006: // Stone Guard Title
                if ((pPlayer->GetHonorPoints() >= 400) && (pPlayer->GetMoney() >=8000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(20))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Stone Guard' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-8000000);
                        pPlayer->ModifyHonorPoints(-400);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Stone Guard' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2007: // Blood Guard Title
                if ((pPlayer->GetHonorPoints() >= 500) && (pPlayer->GetMoney() >=10000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(21))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Blood Guard' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-10000000);
                        pPlayer->ModifyHonorPoints(-500);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Blood Guard' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2008: // Legionnaire Title
                if ((pPlayer->GetHonorPoints() >= 650) && (pPlayer->GetMoney() >=13000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(22))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Legionnaire' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-13000000);
                        pPlayer->ModifyHonorPoints(-650);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Legionnaire' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2009: // Centurion Title
                if ((pPlayer->GetHonorPoints() >= 800) && (pPlayer->GetMoney() >=16000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(23))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Centurion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-16000000);
                        pPlayer->ModifyHonorPoints(-800);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Centurion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2010: // Champion Title
                if ((pPlayer->GetHonorPoints() >= 1000) && (pPlayer->GetMoney() >=20000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(24))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Champion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pPlayer->ModifyHonorPoints(-1000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Champion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2011: // Lieutenant General Title
                if ((pPlayer->GetHonorPoints() >= 1200) && (pPlayer->GetMoney() >=24000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(25))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Lieutenant General' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-24000000);
                        pPlayer->ModifyHonorPoints(-1200);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Lieutenant General' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2012: // General Title
                if ((pPlayer->GetHonorPoints() >= 1500) && (pPlayer->GetMoney() >=30000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(26))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'General' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-30000000);
                        pPlayer->ModifyHonorPoints(-1500);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'General' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2013: // Warlord Title
                if ((pPlayer->GetHonorPoints() >= 1800) && (pPlayer->GetMoney() >=36000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(27))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Warlord' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-36000000);
                        pPlayer->ModifyHonorPoints(-1800);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Warlord' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2014: // High Warlord Title
                if ((pPlayer->GetHonorPoints() >= 3600) && (pPlayer->GetMoney() >=72000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(28))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'High Warlord' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-72000000);
                        pPlayer->ModifyHonorPoints(-3600);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'High Warlord' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2015: // of the Horde Title
                if ((pPlayer->GetHonorPoints() >= 5000) && (pPlayer->GetMoney() >=100000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(127))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Horde' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pPlayer->ModifyHonorPoints(-5000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Horde' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2016: // Conqueror Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(47))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Conqueror' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Conqueror' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2017: // Flame Keeper Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(76))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Flame Keeper' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Flame Keeper' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2018: // of Orgrimmar Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(150))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Orgrimmar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Orgrimmar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2019: // of Sen'jin Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(151))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Sen'jin' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Sen'jin' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2020: // of Silvermoon Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(152))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Silvermoon' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Silvermoon' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2021: // of Thunder Bluff Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(153))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Thunder Bluff' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Thunder Bluff' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 2022: // of the Undercity Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(154))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Undercity' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Undercity' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        //////////////////////////////////////////////////Neutral Titles///////////////////////////////////////////////////////////////

        case 3001: // Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 100) && (pPlayer->GetMoney() >=2000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(42))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-2000000);
                        pPlayer->ModifyHonorPoints(-100);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3002: // Duelist Title
                if ((pPlayer->GetHonorPoints() >= 200) && (pPlayer->GetMoney() >=4000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(43))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Duelist' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-4000000);
                        pPlayer->ModifyHonorPoints(-200);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Duelist' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3003: // Rival Title
                if ((pPlayer->GetHonorPoints() >= 300) && (pPlayer->GetMoney() >=6000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(44))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Rival' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-6000000);
                        pPlayer->ModifyHonorPoints(-300);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Rival' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3004: // Challenger Title
                if ((pPlayer->GetHonorPoints() >= 400) && (pPlayer->GetMoney() >=8000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(45))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Challenger' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-8000000);
                        pPlayer->ModifyHonorPoints(-400);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Challenger' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3007: // Merciless Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 500) && (pPlayer->GetMoney() >=10000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(62))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Merciless Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-10000000);
                        pPlayer->ModifyHonorPoints(-500);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Merciless Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3010: // Vengeful Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 600) && (pPlayer->GetMoney() >=12000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(71))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Vengeful Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-12000000);
                        pPlayer->ModifyHonorPoints(-600);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Vengeful Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3016: // Brutal Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 700) && (pPlayer->GetMoney() >=14000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(80))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Brutal Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-14000000);
                        pPlayer->ModifyHonorPoints(-700);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Brutal Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3079: // Deadly Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 800) && (pPlayer->GetMoney() >=16000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(157))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Deadly Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-16000000);
                        pPlayer->ModifyHonorPoints(-800);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Deadly Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3088: // Furious Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 900) && (pPlayer->GetMoney() >=18000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(167))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Furious Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-18000000);
                        pPlayer->ModifyHonorPoints(-900);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Furious Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3090: // Relentless Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 1000) && (pPlayer->GetMoney() >=20000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(169))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Relentless Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pPlayer->ModifyHonorPoints(-1000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Relentless Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3098: // Wrathful Gladiator Title
                if ((pPlayer->GetHonorPoints() >= 1200) && (pPlayer->GetMoney() >=24000000))
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(177))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Wrathful Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-24000000);
                        pPlayer->ModifyHonorPoints(-1200);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Wrathful Gladiator' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper(MSG_SHORT_ON_GOLD_OR_HONOR, pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3005: // Scarab Lord Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(46))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Scarab Lord' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Scarab Lord' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3006: // Champion of the Naaru Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(53))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Champion of the Naaru' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Champion of the Naaru' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3008: // of the Shattered Sun Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(63))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Shattered Sun' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Shattered Sun' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3009: // Hand of A'dal Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(64))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Hand of A'dal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Hand of A'dal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3011: // Battlemaster Title
                if (pPlayer->GetMoney() >=150000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(72))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Battlemaster' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-150000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Battlemaster' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3012: // Elder Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(74))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Elder' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Elder' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3013: // the Exalted Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(77))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Exalted' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Exalted' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3014: // the Explorer Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(78))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Explorer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Explorer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3015: // the Diplomat Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(79))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Diplomat' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Diplomat' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3017: // the Seeker Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(81))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Seeker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Seeker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3018: // Arena Master Title
                if (pPlayer->GetMoney() >=150000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(82))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Arena Master' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-150000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Arena Master' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3019: // Salty Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(83))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Salty' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Salty' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3020: // Chef Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(84))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Chef' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Chef' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3021: // the Supreme Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(85))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Supreme' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Supreme' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3022: // of the Ten Storms Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(86))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Ten Storms' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Ten Storms' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3023: // of the Emerald Dream Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(87))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Emerald Dream' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Emerald Dream' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3024: // Prophet Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(89))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Prophet' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Prophet' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3025: // the Malefic Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(90))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Malefic' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Malefic' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3026: // Stalker Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(91))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Stalker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Stalker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3027: // of the Ebon Blade Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(92))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Ebon Blade' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Ebon Blade' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3028: // Archmage Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(93))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Archmage' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Archmage' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3029: // Warbringer Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(94))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Warbringer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Warbringer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3030: // Assassin Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(95))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Assassin' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Assassin' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3031: // Grand Master Alchemist Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(96))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Alchemist' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Alchemist' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3032: // Grand Master Blacksmith Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(97))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Blacksmith' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Blacksmith' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3033: // Iron Chef Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(98))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Iron Chef' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Iron Chef' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3034: // Grand Master Enchanter Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(99))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Enchanter' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Enchanter' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3035: // Grand Master Engineer Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(100))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Engineer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Engineer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3036: // Doctor Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(101))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Doctor' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Doctor' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3037: // Grand Master Angler Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(102))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Angler' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Angler' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3038: // Grand Master Herbalist Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(103))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Herbalist' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Herbalist' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3039: // Grand Master Scribe Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(104))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Scribe' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Scribe' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3040: // Grand Master Jewelcrafter Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(105))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Jewelcrafter' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Jewelcrafter' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3041: // Grand Master Leatherworker Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(106))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Leatherworker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Leatherworker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3042: // Grand Master Miner Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(107))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Miner' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Miner' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3043: // Grand Master Skinner Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(108))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Skinner' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Skinner' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3044: // Grand Master Tailor Title
                if (pPlayer->GetMoney() >=20000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(109))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Master Tailor' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-20000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Master Tailor' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3045: // of Quel'Thalas Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(110))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Quel'Thalas' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Quel'Thalas' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3046: // of Argus Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(111))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Argus' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Argus' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3047: // of Khaz Modan Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(112))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of Khaz Modan' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of Khaz Modan' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3048: // the Lion Hearted Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(114))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Lion Hearted' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Lion Hearted' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3049: // Champion of Elune Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(115))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Champion of Elune' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Champion of Elune' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3050: // Hero of Orgrimmar Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(116))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Hero of Orgrimmar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Hero of Orgrimmar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3051: // Plainsrunner Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(117))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Plainsrunner' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Plainsrunner' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3052: // of the Darkspear Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(118))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Darkspear' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Darkspear' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3053: // the Forsaken Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(119))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Forsaken' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Forsaken' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3054: // the Magic Seeker Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(120))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Magic Seeker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Magic Seeker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3055: // Twilight Vanquisher Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(121))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Twilight Vanquisher' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Twilight Vanquisher' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3056: // Conqueror of Naxxramas Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(122))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Conqueror of Naxxramas' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Conqueror of Naxxramas' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3057: // Hero of Northrend Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(123))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Hero of Northrend' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Hero of Northrend' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3058: // the Hallowed Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(124))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Hallowed' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Hallowed' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3059: // Loremaster Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(125))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Loremaster' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Loremaster' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3060: // the Flawless Victor Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(128))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Flawless Victor' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Flawless Victor' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3061: // the Flawless Victor Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(129))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Champion of the Frozen Wastes' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Champion of the Frozen Wastes' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3062: // Ambassador Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(130))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Ambassador' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Ambassador' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3063: // the Argent Champion Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(131))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Argent Champion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Argent Champion' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3064: // Guardian of Cenarius Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(132))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Guardian of Cenarius' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Guardian of Cenarius' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3065: // Brewmaster Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(133))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Brewmaster' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Brewmaster' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3066: // Merrymaker Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(134))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Merrymaker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Merrymaker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3067: // the Love Fool Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(135))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Love Fool' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Love Fool' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3068: // Matron Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(137))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Matron' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Matron' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3069: // Patron Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(138))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Patron' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Patron' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3070: // Obsidian Slayer Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(139))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Obsidian Slayer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Obsidian Slayer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3071: // of the Nightfall Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(140))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Nightfall' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Nightfall' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3072: // the Immortal Title
                if (pPlayer->GetMoney() >=200000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(141))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Immortal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-200000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Immortal' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3073: // the Undying Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(142))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Undying' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Undying' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3074: // Jenkins Title
                if (pPlayer->GetMoney() >=60000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(143))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Jenkins' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-60000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Jenkins' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3075: // Bloodsail Admiral Title
                if (pPlayer->GetMoney() >=40000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(144))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Bloodsail Admiral' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-40000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Bloodsail Admiral' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3076: // the Insane Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(145))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Insane' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Insane' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3077: // the Noble Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(155))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Noble' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Noble' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3078: // Crusader Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(156))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Crusader' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Crusader' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3080: // Death's Demise Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(158))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Death's Demise' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Death's Demise' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3081: // the Celestial Defender Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(159))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Celestial Defender' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Celestial Defender' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3082: // Conqueror of Ulduar Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(160))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Conqueror of Ulduar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Conqueror of Ulduar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3083: // Champion of Ulduar Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(161))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Champion of Ulduar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Champion of Ulduar' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3084: // Vanquisher Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(163))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Vanquisher' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Vanquisher' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3085: // Starcaller Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(164))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Starcaller' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Starcaller' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3086: // the Astral Walker Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(165))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Astral Walker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Astral Walker' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3087: // Herald of the Titans Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(166))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Herald of the Titans' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Herald of the Titans' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3089: // the Pilgrim Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(168))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Pilgrim' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Pilgrim' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3091: // Grand Crusader Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(170))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Grand Crusader' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Grand Crusader' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3092: // the Argent Defender Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(171))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Argent Defender' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Argent Defender' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3093: // the Patient Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(172))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Patient' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Patient' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3094: // the Light of Dawn Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(173))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Light of Dawn' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Light of Dawn' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3095: // Bane of the Fallen King Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(174))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'Bane of the Fallen King' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'Bane of the Fallen King' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3096: // the Kingslayer Title
                if (pPlayer->GetMoney() >=100000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(175))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'the Kingslayer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-100000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'the Kingslayer' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        case 3097: // of the Ashen Verdict Title
                if (pPlayer->GetMoney() >=80000000)
                {
                    if(CharTitlesEntry const* titleEntry = GetCharTitlesStore() -> LookupEntry(176))
                    if (pPlayer->HasTitle(titleEntry))
                    {
                        pCreature->MonsterWhisper("You already have the 'of the Ashen Verdict' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        pPlayer->SetTitle(titleEntry);
                        pPlayer->ModifyMoney(-80000000);
                        pCreature->MonsterWhisper("Congratulations! You've just gained 'of the Ashen Verdict' Title!", pPlayer, false);
                        pPlayer->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    pCreature->MonsterWhisper("Not enough Gold!", pPlayer, false);
                    pPlayer->CLOSE_GOSSIP_MENU();
                }
        break;

        pPlayer->CLOSE_GOSSIP_MENU();

    }
}

bool GossipSelect_custom_npc_titlemaster(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    // Main menu
    if (uiSender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_custom_npc_titlemaster(pPlayer, pCreature, uiAction);

    return true;
}

void AddSC_custom_npc_titlemaster()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_npc_titlemaster";
    newscript->pGossipHello     = &GossipHello_custom_npc_titlemaster;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_titlemaster;
    newscript->RegisterSelf();
}