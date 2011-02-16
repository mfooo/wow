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
SDName: Western_Plaguelands
SD%Complete: 90
SDComment: Quest support: 5216,5219,5222,5225,5229,5231,5233,5235. To obtain Vitreous Focuser (could use more spesifics about gossip items)
SDCategory: Western Plaguelands
EndScriptData */

/* ContentData
npcs_dithers_and_arbington
npc_myranda_hag
npc_the_scourge_cauldron
EndContentData */

#include "precompiled.h"

/*######
## npcs_dithers_and_arbington
######*/

bool GossipHello_npcs_dithers_and_arbington(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
    if (pCreature->isVendor())
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if (pPlayer->GetQuestRewardStatus(5237) || pPlayer->GetQuestRewardStatus(5238))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What does the Felstone Field Cauldron need?",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What does the Dalson's Tears Cauldron need?",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What does the Writhing Haunt Cauldron need?",      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What does the Gahrron's Withering Cauldron need?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
        pPlayer->SEND_GOSSIP_MENU(3985, pCreature->GetGUID());
    }else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npcs_dithers_and_arbington(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_TRADE:
            pPlayer->SEND_VENDORLIST(pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(3980, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(3981, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(3982, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Thanks, i need a Vitreous Focuser", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(3983, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer, 17529, false);
            break;
    }
    return true;
}

/*######
## npc_myranda_the_hag
######*/

enum
{
    QUEST_SUBTERFUGE        = 5862,
    QUEST_IN_DREAMS         = 5944,
    SPELL_SCARLET_ILLUSION  = 17961
};

#define GOSSIP_ITEM_ILLUSION    "I am ready for the illusion, Myranda."

bool GossipHello_npc_myranda_the_hag(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_SUBTERFUGE) == QUEST_STATUS_COMPLETE &&
        !pPlayer->GetQuestRewardStatus(QUEST_IN_DREAMS) && !pPlayer->HasAura(SPELL_SCARLET_ILLUSION))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ILLUSION, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(4773, pCreature->GetGUID());
        return true;
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_myranda_the_hag(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, SPELL_SCARLET_ILLUSION, false);
    }
    return true;
}

/*######
## npc_the_scourge_cauldron
######*/

struct MANGOS_DLL_DECL npc_the_scourge_cauldronAI : public ScriptedAI
{
    npc_the_scourge_cauldronAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() {}

    void DoDie()
    {
        //summoner dies here
        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        //override any database `spawntimesecs` to prevent duplicated summons
        uint32 rTime = m_creature->GetRespawnDelay();
        if (rTime<600)
            m_creature->SetRespawnDelay(600);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || who->GetTypeId() != TYPEID_PLAYER)
            return;

        if (who->GetTypeId() == TYPEID_PLAYER)
        {
            switch(m_creature->GetAreaId())
            {
                case 199:                                   //felstone
                    if (((Player*)who)->GetQuestStatus(5216) == QUEST_STATUS_INCOMPLETE ||
                        ((Player*)who)->GetQuestStatus(5229) == QUEST_STATUS_INCOMPLETE)
                    {
                        m_creature->SummonCreature(11075, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        DoDie();
                    }
                    break;
                case 200:                                   //dalson
                    if (((Player*)who)->GetQuestStatus(5219) == QUEST_STATUS_INCOMPLETE ||
                        ((Player*)who)->GetQuestStatus(5231) == QUEST_STATUS_INCOMPLETE)
                    {
                        m_creature->SummonCreature(11077, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        DoDie();
                    }
                    break;
                case 201:                                   //gahrron
                    if (((Player*)who)->GetQuestStatus(5225) == QUEST_STATUS_INCOMPLETE ||
                        ((Player*)who)->GetQuestStatus(5235) == QUEST_STATUS_INCOMPLETE)
                    {
                        m_creature->SummonCreature(11078, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000);
                        DoDie();
                    }
                    break;
                case 202:                                   //writhing
                    if (((Player*)who)->GetQuestStatus(5222) == QUEST_STATUS_INCOMPLETE ||
                        ((Player*)who)->GetQuestStatus(5233) == QUEST_STATUS_INCOMPLETE)
                    {
                        m_creature->SummonCreature(11076, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000);
                        DoDie();
                    }
                    break;
            }
        }
    }
};
CreatureAI* GetAI_npc_the_scourge_cauldron(Creature* pCreature)
{
    return new npc_the_scourge_cauldronAI(pCreature);
}

/*######
##
######*/

void AddSC_western_plaguelands()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npcs_dithers_and_arbington";
    newscript->pGossipHello = &GossipHello_npcs_dithers_and_arbington;
    newscript->pGossipSelect = &GossipSelect_npcs_dithers_and_arbington;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_myranda_the_hag";
    newscript->pGossipHello = &GossipHello_npc_myranda_the_hag;
    newscript->pGossipSelect = &GossipSelect_npc_myranda_the_hag;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_the_scourge_cauldron";
    newscript->GetAI = &GetAI_npc_the_scourge_cauldron;
    newscript->RegisterSelf();
}
