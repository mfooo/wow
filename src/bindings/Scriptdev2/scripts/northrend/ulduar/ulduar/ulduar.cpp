/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Ulduar teleport
SD%Complete:
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

#define REQUEST_HELP    "Help me fight Yogg-Saron!"
#define DENY_HELP       "I don't need your help."

/*
*	Here is the gossip for the keepers images
*	Each image appears in the centra chamber after the corrupted keeper is defeated
*	They should be spawned in script, but I added them into the DB by default as invisilbe
*	After the players make theyr choice if the want help or not, this option is saved and there is no turning back, until raid reset
*	If they are asked for help, at the beginning of the Yogg encounter each requested keeper will be summoned inside the chamber
*	There should be more text in the gossip menu, I couldn't find it anywhere yet
*/

/*#######
*### Keepers images
#######*/
// HODIR
bool GossipHello_hodir_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_HODIR) != DONE && m_pInstance->GetData(TYPE_KEEPER_HODIR) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_hodir_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_HODIR, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_HODIR, FAIL);
    }
    return true;
}

// FREYA
bool GossipHello_freya_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_FREYA) != DONE && m_pInstance->GetData(TYPE_KEEPER_FREYA) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_freya_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_FREYA, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_FREYA, FAIL);
    }
    return true;
}
// MIMIRON
bool GossipHello_mimiron_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_MIMIRON) != DONE && m_pInstance->GetData(TYPE_KEEPER_MIMIRON) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_mimiron_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_MIMIRON, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_MIMIRON, FAIL);
    }
    return true;
}

// THORIM
bool GossipHello_thorim_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_THORIM) != DONE && m_pInstance->GetData(TYPE_KEEPER_THORIM) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_thorim_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_THORIM, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_THORIM, FAIL);
    }
    return true;
}

void AddSC_ulduar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "hodir_image";
    newscript->pGossipHello = &GossipHello_hodir_image;
    newscript->pGossipSelect = &GossipSelect_hodir_image;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "freya_image";
    newscript->pGossipHello = &GossipHello_freya_image;
    newscript->pGossipSelect = &GossipSelect_freya_image;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "thorim_image";
    newscript->pGossipHello = &GossipHello_thorim_image;
    newscript->pGossipSelect = &GossipSelect_thorim_image;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mimiron_image";
    newscript->pGossipHello = &GossipHello_mimiron_image;
    newscript->pGossipSelect = &GossipSelect_mimiron_image;
    newscript->RegisterSelf();

}
