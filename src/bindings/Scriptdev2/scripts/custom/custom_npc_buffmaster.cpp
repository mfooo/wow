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
SDName: custom_npc_buffmaster
SD%Complete: 100
SDComment: Buff Master - Buffs characters with various buffs.
SDCategory: NPC
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
EndScriptData */

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../config.h"

bool GossipHello_custom_npc_buffmaster(Player* pPlayer, Creature* pCreature)
{
    Config SD2Config;
	if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
		error_log("SD2: Unable to open configuration file");

	bool SmallBuffsEnabled = SD2Config.GetBoolDefault("BuffsMaster.SmallBuffsEnabled", true);
	bool GreatBuffsEnabled = SD2Config.GetBoolDefault("BuffsMaster.GreatBuffsEnabled", true);
	bool GameMasterBuffsEnabled = SD2Config.GetBoolDefault("BuffsMaster.GameMasterBuffsEnabled", true);
	bool PlayerToolsEnabled = SD2Config.GetBoolDefault("BuffsMaster.PlayerToolsEnabled", true);
	bool GoldSpellEnabled = SD2Config.GetBoolDefault("BuffsMaster.GoldSpellEnabled", false);

    if (pPlayer->GetTeam() == ALLIANCE)
	{
	    if(SmallBuffsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "Small Buffs ->"              , GOSSIP_SENDER_MAIN, 1000);
	    if(GreatBuffsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "Great Buffs ->"              , GOSSIP_SENDER_MAIN, 2000);
	    if(GameMasterBuffsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "GM Buffs ->"                 , GOSSIP_SENDER_MAIN, 3000);
        if(PlayerToolsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
	}
	else
	{
	    if(SmallBuffsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "Small Buffs ->"              , GOSSIP_SENDER_MAIN, 1000);
	    if(GreatBuffsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "Great Buffs ->"              , GOSSIP_SENDER_MAIN, 2000);
	    if(GameMasterBuffsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "GM Buffs ->"                 , GOSSIP_SENDER_MAIN, 3000);
        if(PlayerToolsEnabled)
            pPlayer->ADD_GOSSIP_ITEM( 7, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
	}
    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
    return true;
}

void SendDefaultMenu_custom_npc_buffmaster(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

    //Combat Check
    if (pPlayer->isInCombat())
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
        return;
    }

    Config SD2Config;
	if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
		error_log("SD2: Unable to open configuration file");
        
    //Money Check
    if (pPlayer->GetMoney() < (SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)))
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterWhisper("You don't have enough money.", pPlayer, false);
        return;
    }

	bool SmallBuffsEnabled = SD2Config.GetBoolDefault("BuffsMaster.SmallBuffsEnabled", true);
	bool GreatBuffsEnabled = SD2Config.GetBoolDefault("BuffsMaster.GreatBuffsEnabled", true);
	bool GameMasterBuffsEnabled = SD2Config.GetBoolDefault("BuffsMaster.GameMasterBuffsEnabled", true);
	bool PlayerToolsEnabled = SD2Config.GetBoolDefault("BuffsMaster.PlayerToolsEnabled", true);
	bool GoldSpellEnabled = SD2Config.GetBoolDefault("BuffsMaster.GoldSpellEnabled", false);

    switch(uiAction)
    {
        case 1000:
        {
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Mark of the Wild"                , GOSSIP_SENDER_MAIN, 1001);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Amplify Magic"                   , GOSSIP_SENDER_MAIN, 1010);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Arcane Intellect"                , GOSSIP_SENDER_MAIN, 1015);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Dalaran Intellect"               , GOSSIP_SENDER_MAIN, 1020);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Dampen Magic"                    , GOSSIP_SENDER_MAIN, 1025);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Blessing of Kings"               , GOSSIP_SENDER_MAIN, 1030);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Blessing of Might"               , GOSSIP_SENDER_MAIN, 1035);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Blessing of Wisdom"              , GOSSIP_SENDER_MAIN, 1040);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Divine Spirit"                   , GOSSIP_SENDER_MAIN, 1045);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Power Word: Fortitude"           , GOSSIP_SENDER_MAIN, 1050);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Shadow Protection"               , GOSSIP_SENDER_MAIN, 1055);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

                pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
                break;
        }
        case 2000:
        {
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Gift of the Wild"                , GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Arcane Brilliance"               , GOSSIP_SENDER_MAIN, 2005);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Dalaran Brilliance"              , GOSSIP_SENDER_MAIN, 2010);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Kings"       , GOSSIP_SENDER_MAIN, 2015);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Might"       , GOSSIP_SENDER_MAIN, 2020);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Sanctuary"   , GOSSIP_SENDER_MAIN, 2025);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Wisdom"      , GOSSIP_SENDER_MAIN, 2030);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Prayer of Fortitude"             , GOSSIP_SENDER_MAIN, 2035);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Prayer of Shadow Protection"     , GOSSIP_SENDER_MAIN, 2040);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Prayer of Spirit"                , GOSSIP_SENDER_MAIN, 2045);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

                pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
                break;
        }
        case 3000:
        {
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Agamaggan's Agility"             , GOSSIP_SENDER_MAIN, 3001);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Agamaggan's Strength"            , GOSSIP_SENDER_MAIN, 3005);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Armor Magic"                     , GOSSIP_SENDER_MAIN, 3010);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Armor Penetration"               , GOSSIP_SENDER_MAIN, 3015);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Increased Stamina"               , GOSSIP_SENDER_MAIN, 3020);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Razorhide"                       , GOSSIP_SENDER_MAIN, 3025);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Rising Spirit"                   , GOSSIP_SENDER_MAIN, 3030);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Spirit of the Wind"              , GOSSIP_SENDER_MAIN, 3035);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Wisdom of Agamaggan"             , GOSSIP_SENDER_MAIN, 3040);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

                pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
                break;
        }
        case 4000:
        {
                if(GoldSpellEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 5, "Give me Gold"                            , GOSSIP_SENDER_MAIN, 4001);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Give me Soul Shards"                     , GOSSIP_SENDER_MAIN, 4005);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Heal me Please"                          , GOSSIP_SENDER_MAIN, 4010);
                pPlayer->ADD_GOSSIP_ITEM( 5, "Heal me and party members Please"        , GOSSIP_SENDER_MAIN, 4015);
                pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

                pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
                break;
        }
        case 5005:
        {
            if (pPlayer->GetTeam() == ALLIANCE)
            {
                if(SmallBuffsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "Small Buffs ->"              , GOSSIP_SENDER_MAIN, 1000);
                if(GreatBuffsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "Great Buffs ->"              , GOSSIP_SENDER_MAIN, 2000);
                if(GameMasterBuffsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "GM Buffs ->"                 , GOSSIP_SENDER_MAIN, 3000);
                if(PlayerToolsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
            }
            else
            {
                if(SmallBuffsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "Small Buffs ->"              , GOSSIP_SENDER_MAIN, 1000);
                if(GreatBuffsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "Great Buffs ->"              , GOSSIP_SENDER_MAIN, 2000);
                if(GameMasterBuffsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "GM Buffs ->"                 , GOSSIP_SENDER_MAIN, 3000);
                if(PlayerToolsEnabled)
                    pPlayer->ADD_GOSSIP_ITEM( 7, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
            }
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            break;
        }
        case 1001: // Buff me Mark of the Wild
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48469,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1010: // Buff me Amplify Magic
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,43017,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1015: // Buff me Arcane Intellect
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,42995,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1020: // Buff me Dalaran Intellect
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,61024,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1025: // Buff me Dampen Magic
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,43015,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1030: // Buff me Blessing of Kings
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,20217,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1035: // Buff me Blessing of Might
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48932,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1040: // Buff me Blessing of Wisdom
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48936,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1045: // Buff me Divine Spirit
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48073,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1050: // Buff me Power Word: Fortitude
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48161,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 1055: // Buff me Shadow Protection
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48169,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        //////////////////////////////////////////////////Great Buff///////////////////////////////////////////////////////////////

        case 2001: // Buff me Gift of the Wild
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,48470,true);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2005: // Buff me Arcane Brilliance
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,43002,true);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2010: // Buff me Dalaran Brilliance
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,61316,true);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2015: // Buff me Greater Blessing of Kings
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,25898,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2020: // Buff me Greater Blessing of Might
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48934,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2025: // Buff me Greater Blessing of Sanctuary
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,25899,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2030: // Buff me Greater Blessing of Wisdom
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,48938,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2035: // Buff me Prayer of Fortitude
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,48162,true);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2040: // Buff me Prayer of Shadow Protection
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,48170,true);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 2045: // Buff me Prayer of Spirit
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,48074,true);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        //////////////////////////////////////////////////GM Buff///////////////////////////////////////////////////////////////

        case 3001: // Buff me Agamaggan's Agility
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,17013,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3005: // Buff me Agamaggan's Strength
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,16612,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3010: // Buff me Armor Magic
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,58453,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3015: // Buff me Armor Penetration
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,34106,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3020: // Buff me Increased Stamina
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,25661,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3025: // Buff me Razorhide
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,16610,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3030: // Buff me Rising Spirit
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,10767,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3035: // Buff me Spirit of the Wind
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,16618,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 3040: // Buff me Wisdom of Agamaggan
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,7764,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        //////////////////////////////////////////////////Player Tools///////////////////////////////////////////////////////////////

        case 4001://Give me Gold
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,46642,false); // 5000 gold
            break;
        }
        case 4005://Give me Soul Shards
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,24827,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 4010: // Heal me please
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer,38588,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 4015: // Heal me and party members Please
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,53251,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 4020: // Conjure Refreshment
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,42956,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        case 4025: // Conjure Mana Gem
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer,42985,false);
            pPlayer->ModifyMoney(-(SD2Config.GetFloatDefault("BuffsMaster.BuffCost",0)));
            break;
        }
        pPlayer->CLOSE_GOSSIP_MENU();
    }
}

bool GossipSelect_custom_npc_buffmaster(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	// Main menu
	if (uiSender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_custom_npc_buffmaster(pPlayer, pCreature, uiAction);

    return true;
}

void AddSC_custom_npc_buffmaster()
{
    Script *newscript;

	newscript                   = new Script;
    newscript->Name             = "custom_npc_buffmaster";
    newscript->pGossipHello     = &GossipHello_custom_npc_buffmaster;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_buffmaster;
    newscript->RegisterSelf();
}