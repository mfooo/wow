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
SDName: npc_arena_honor
SD%Complete: 100%
SDComment: by tempura, corrected by /dev/rsa
SDCategory: custom
EndScriptData */
#include "precompiled.h"
#include "sc_creature.h"
#include "sc_gossip.h"

#define GOSSIP_ITEM_ARENA_TO_HONOR  -3000770
#define GOSSIP_ITEM_ARENA_TO_HONOR1 -3000771
#define GOSSIP_ITEM_HONOR_TO_ARENA  -3000772
#define GOSSIP_ITEM_HONOR_TO_ARENA1 -3000773

#define UNSUCCESSFUL_HONOR  -1001007
#define UNSUCCESSFUL_ARENA  -1001008

bool GossipHello_npc_arena_honor(Player* pPlayer, Creature *pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_HONOR_TO_ARENA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_HONOR_TO_ARENA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARENA_TO_HONOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARENA_TO_HONOR1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    pPlayer->SEND_GOSSIP_MENU(3961,pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_arena_honor(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        if (pPlayer->GetHonorPoints() >= 1000)
        {
            pPlayer->ModifyHonorPoints(-1000);
            pPlayer->ModifyArenaPoints(+50);
        }
        else
            DoScriptText(UNSUCCESSFUL_HONOR, pCreature);
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        if (pPlayer->GetHonorPoints() >= 10000)
        {
            pPlayer->ModifyHonorPoints(-10000);
            pPlayer->ModifyArenaPoints(+500);
        }
        else
            DoScriptText(UNSUCCESSFUL_HONOR, pCreature);
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 3)
    {
        if (pPlayer->GetArenaPoints() >= 100)
        {
            pPlayer->ModifyArenaPoints(-100);
            pPlayer->ModifyHonorPoints(+2000);
        }
        else
            DoScriptText(UNSUCCESSFUL_ARENA, pCreature);
    }
    if (action == GOSSIP_ACTION_INFO_DEF + 4)
    {
        if (pPlayer->GetArenaPoints() >= 1000)
        {
            pPlayer->ModifyArenaPoints(-1000);
            pPlayer->ModifyHonorPoints(+20000);
        }
        else
            DoScriptText(UNSUCCESSFUL_ARENA, pCreature);
    }
    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

void AddSC_npc_arena_honor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_arena_honor";
    newscript->pGossipHello =  &GossipHello_npc_arena_honor;
    newscript->pGossipSelect = &GossipSelect_npc_arena_honor;
    newscript->RegisterSelf();
}
