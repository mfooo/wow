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
SDName: custom_npc_tokenvendor
SD%Complete: 100
SDComment: Token Vendor - Allows various tokens to be exchanged for extras.
SDCategory: NPC
SDAuthor: Darkrulerz @ Project Dark-iCE <http://projectSD2.clanice.com/>
SDModifier: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
WOWVersion: 3.3.2
EndScriptData */

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../config.h"
 
bool GossipHello_custom_npc_tokenvendor(Player* pPlayer, Creature* pCreature)
{
    Config SD2Config;
    if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
        error_log("SD2: Unable to open configuration file");

    pPlayer->ADD_GOSSIP_ITEM( 5, "Exchange 25 BOJ to 10 Gold" , GOSSIP_SENDER_MAIN, 2000);
    if(pPlayer->getLevel() < (SD2Config.GetFloatDefault("TokenVendor.MaxLevel",0)))
        pPlayer->ADD_GOSSIP_ITEM( 5, "40 BOJ Exchange 1 Level" , GOSSIP_SENDER_MAIN, 3000);
    pPlayer->ADD_GOSSIP_ITEM( 5, "20 BOJ Exchange 36 Slot Bag" , GOSSIP_SENDER_MAIN, 4000);
    pPlayer->ADD_GOSSIP_ITEM(5,"5 BOJ Exchange 1 Buff Spell" ,GOSSIP_SENDER_MAIN,5000);
    pPlayer->ADD_GOSSIP_ITEM(5,"10 BOJ Exchange 1 Item Summon Trainer" ,GOSSIP_SENDER_MAIN,6000);
    pPlayer->ADD_GOSSIP_ITEM(5,"10 BOJ Exchange 1 Item Summon Professional" ,GOSSIP_SENDER_MAIN,7000);
    pPlayer->ADD_GOSSIP_ITEM(5,"10 BOJ Exchange 1 Item Summon Tele NPC" ,GOSSIP_SENDER_MAIN,8000);
 
    pPlayer->SEND_GOSSIP_MENU(3961,pCreature->GetGUID());
    return true;
}
 
void SendDefaultMenu_custom_npc_tokenvendor(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{
    uint32 itemBoj = 29434; // Badge of Justice
    uint32 itemId2 = 23162; // 36 bag slot, I suppose ? (Darkiss)
    uint32 noSpaceForCount = 0;
    int32 count = 0;
    int32 count2 = 1;
 
    Item* item = NULL;
    
    ItemPosCountVec dest;
    ItemPosCountVec dest2;
    uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemBoj, 0);
    uint8 msg2 = pPlayer->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest2, 23162, 1);
 
    // Not allowed to use in combat
    if (pPlayer->isInCombat())
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterSay("You are in combat", LANG_UNIVERSAL, NULL);
        return;
    }

    switch(uiAction)
    {
        case 2000:
            if(pPlayer->HasItemCount(itemBoj, 25 , true))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->DestroyItemCount(itemBoj, 25, true);
                pPlayer->ModifyMoney(+100000);
                pCreature->MonsterWhisper("You have just exchanged 25 BOJ to 10 Gold!", pPlayer, false);
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("You don't have enough tokens!", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 3000:
            if(pPlayer->HasItemCount(itemBoj, 40, true))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->DestroyItemCount(itemBoj, 40, true);
                pPlayer->GiveLevel(pPlayer->getLevel() + 1);
                pCreature->MonsterWhisper("You have just exchanged 40 BOJ for one level!", pPlayer, false);
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("You don't have enough tokens!", LANG_UNIVERSAL, NULL);
            }
        break;

        case 4000:
            if(pPlayer->HasItemCount(itemBoj, 20, true))
            {
            if( msg != EQUIP_ERR_OK )
                count2 -= noSpaceForCount;

            if( count2 == 0 || dest.empty())
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("Error: Item count set to 0. Please inform a GM about this error.", pPlayer, false);
            }

            item = pPlayer->StoreNewItem( dest2, itemId2, true, Item::GenerateItemRandomPropertyId(itemId2));

            if(count2 > 0 && item)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->SendNewItem(item,count2,false,true);
                pPlayer->DestroyItemCount(itemBoj, 20, true);
            }

            if(noSpaceForCount > 0)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You don't have enough free inventory slots", pPlayer, false);
            }
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5000:
            pPlayer->ADD_GOSSIP_ITEM(5,"Arcane Brilliance",GOSSIP_SENDER_MAIN,5001);
            pPlayer->ADD_GOSSIP_ITEM(5,"Greater Blessing of Kings",GOSSIP_SENDER_MAIN,5002);
            pPlayer->ADD_GOSSIP_ITEM(5,"Gift of the Wild",GOSSIP_SENDER_MAIN,5003);
            pPlayer->ADD_GOSSIP_ITEM(5,"Dalaran Brilliance",GOSSIP_SENDER_MAIN,5004);
            pPlayer->ADD_GOSSIP_ITEM(5,"Greater Blessing of Might",GOSSIP_SENDER_MAIN,5005);
            pPlayer->ADD_GOSSIP_ITEM(5,"Greater Blessing of Sanctuary",GOSSIP_SENDER_MAIN,5006);
            pPlayer->ADD_GOSSIP_ITEM(5,"Greater Blessing of Wisdom",GOSSIP_SENDER_MAIN,5007);
            pPlayer->ADD_GOSSIP_ITEM(5,"Prayer of Fortitude",GOSSIP_SENDER_MAIN,5008);
            pPlayer->ADD_GOSSIP_ITEM(5,"Prayer of Shadow Protection",GOSSIP_SENDER_MAIN,5009);
            pPlayer->ADD_GOSSIP_ITEM(5,"Prayer of Spirit",GOSSIP_SENDER_MAIN,5010);
            
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        break;
         
        case 5001:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pPlayer->CastSpell(pPlayer,43002,true);
            pPlayer->DestroyItemCount(itemBoj, 5, true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5002:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pCreature->CastSpell(pPlayer,25898,false);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5003:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pPlayer->CastSpell(pPlayer,48470,true);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5004:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pPlayer->CastSpell(pPlayer,61316,true);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5005:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pCreature->CastSpell(pPlayer,48934,false);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5006:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pCreature->CastSpell(pPlayer,25899,false);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5007:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pCreature->CastSpell(pPlayer,48938,false);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5008:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pPlayer->CastSpell(pPlayer,48162,true);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5009:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pPlayer->CastSpell(pPlayer,48170,true);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 5010:
            if(pPlayer->HasItemCount(itemBoj,5,true))
            {
            pPlayer->ADD_GOSSIP_ITEM(5,"<--Back",GOSSIP_SENDER_MAIN,5000);
            pPlayer->CastSpell(pPlayer,48074,true);
            pPlayer->DestroyItemCount(itemBoj,5,true);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 6000:
            if(pPlayer->HasItemCount(itemBoj, 10, true))
            {
                if( msg != EQUIP_ERR_OK )
                    count2 -= noSpaceForCount;

                if( count2 == 0 || dest.empty())
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pCreature->MonsterWhisper("Error: Item count set to 0. Please inform a GM about this error.", pPlayer, false);
                }

                item = pPlayer->StoreNewItem( dest2, 90010, true, Item::GenerateItemRandomPropertyId(90010));

                if(count2 > 0 && item)
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pPlayer->SendNewItem(item,count2,false,true);
                    pPlayer->DestroyItemCount(itemBoj, 10, true);
                }

                if(noSpaceForCount > 0)
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pCreature->MonsterWhisper("You don't have enough free inventory slots", pPlayer, false);
                }
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;

        case 7000:
            if(pPlayer->HasItemCount(itemBoj, 10, true))
            {
                if( msg != EQUIP_ERR_OK )
                    count2 -= noSpaceForCount;

                if( count2 == 0 || dest.empty())
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pCreature->MonsterWhisper("Error: Item count set to 0. Please inform a GM about this error.", pPlayer, false);
                }

                item = pPlayer->StoreNewItem( dest2, 90005, true, Item::GenerateItemRandomPropertyId(90005));

                if(count2 > 0 && item)
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pPlayer->SendNewItem(item,count2,false,true);
                    pPlayer->DestroyItemCount(itemBoj, 10, true);
                }

                if(noSpaceForCount > 0)
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pCreature->MonsterWhisper("You don't have enough free inventory slots", pPlayer, false);
                }
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
         
        case 8000:
            if(pPlayer->HasItemCount(itemBoj, 10, true))
            {
                if( msg != EQUIP_ERR_OK )
                    count2 -= noSpaceForCount;

                if( count2 == 0 || dest.empty())
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pCreature->MonsterWhisper("Error: Item count set to 0. Please inform a GM about this error.", pPlayer, false);
                }

                item = pPlayer->StoreNewItem( dest2, 8, true, Item::GenerateItemRandomPropertyId(8));

                if(count2 > 0 && item)
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pPlayer->SendNewItem(item,count2,false,true);
                    pPlayer->DestroyItemCount(itemBoj, 10, true);
                }

                if(noSpaceForCount > 0)
                {
                    pPlayer->CLOSE_GOSSIP_MENU();
                    pCreature->MonsterWhisper("You don't have enough free inventory slots", pPlayer, false);
                }
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("You don't have enough tokens", LANG_UNIVERSAL, NULL);
            }
        break;
    }
}
 
bool GossipSelect_custom_npc_tokenvendor(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    // Main menu
    if (uiSender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_custom_npc_tokenvendor(pPlayer, pCreature, uiAction);
 
    return true;
}
 
void AddSC_custom_npc_tokenvendor()
{
    Script *newscript;
 
    newscript                   = new Script;
    newscript->Name             = "custom_npc_tokenvendor";
    newscript->pGossipHello     = &GossipHello_custom_npc_tokenvendor;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_tokenvendor;
    newscript->RegisterSelf();
}