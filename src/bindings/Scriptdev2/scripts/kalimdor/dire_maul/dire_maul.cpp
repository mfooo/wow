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
SDName: Dire Maul
SD%Complete: 
SDComment: 
SDCategory: Dire Maul
EndScriptData */

/* ContentData
npc_old_ironbark                and door opening event. waypoint stored in creature_movement, this is not expected to be EscortAI
EndContentData */

#include "precompiled.h"
#include "dire_maul.h"

enum Ironbark
{
    SAY_START_WALKING   = -1998992,
    NPC_TEXT_IRONBARK   = 6695
};

#define OLD_IRONBARK_GOSSIP "Thank you, Ironbark. We are ready for you to open the door."

struct MANGOS_DLL_DECL npc_old_ironbarkAI : public ScriptedAI
{
    npc_old_ironbarkAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_dire_maul*)pCreature->GetInstanceData();
        Reset();
    }

    instance_dire_maul* m_pInstance;

    bool m_bIsInLastPoint;
    uint32 m_uiLastPoint;

    void Reset() 
    {
        m_creature->GetMotionMaster()->MoveIdle();
        m_uiLastPoint = 9;                                  // taken from creature_movement
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != WAYPOINT_MOTION_TYPE || uiPointId != m_uiLastPoint || !m_pInstance)
            return;

        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ATTACKUNARMED);
        m_pInstance->SetData(TYPE_IRONBARK, SPECIAL);
        m_creature->ForcedDespawn(8000);
    }
};

CreatureAI* GetAI_npc_old_ironbark(Creature* pCreature)
{
    return new npc_old_ironbarkAI(pCreature);
}

bool GossipHello_npc_old_ironbark(Player* pPlayer, Creature* pCreature)
{
    instance_dire_maul* m_pInstance = (instance_dire_maul*)pCreature->GetInstanceData();
    if (m_pInstance && m_pInstance->GetData(TYPE_ZERVIM) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, OLD_IRONBARK_GOSSIP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_IRONBARK, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_old_ironbark(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            DoScriptText(SAY_START_WALKING, pCreature, pPlayer);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->GetMotionMaster()->MovementExpired();
            break;
    }
    return true;
}

void AddSC_dire_maul()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_old_ironbark";
    newscript->GetAI = &GetAI_npc_old_ironbark;
    newscript->pGossipHello =  &GossipHello_npc_old_ironbark;
    newscript->pGossipSelect = &GossipSelect_npc_old_ironbark;
    newscript->RegisterSelf();
}
