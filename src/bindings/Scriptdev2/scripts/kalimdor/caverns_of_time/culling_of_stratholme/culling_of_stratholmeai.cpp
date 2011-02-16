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
SDName: instance_culling_of_stratholme
SD%Complete: ?%
SDComment: by MaxXx2021
SDCategory: Culling of Stratholme
EndScriptData */

#include "precompiled.h"
#include "def_culling_of_stratholme.h"
#include "escort_ai.h"
#include "WorldPacket.h"

/*###
## npc_chromi_start
###*/

#define GOSSIP_ITEM_CHROMI1 "Why have I been sent back to this particular place and time?"
#define GOSSIP_ITEM_CHROMI2 "What was this decision?"
#define GOSSIP_ITEM_CHROMI3 "So how does the infinite Dragonflight plan to Interfere?"

enum
{
  GOSSIP_TEXTID_CHROMI1            = 12939,
  GOSSIP_TEXTID_CHROMI2            = 12949,
  GOSSIP_TEXTID_CHROMI3            = 12950,
  GOSSIP_TEXTID_CHROMI4            = 12952
};

bool GossipHello_npc_chromi_start(Player* pPlayer, Creature* pCreature)
{
    if(pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    if (pPlayer && pPlayer->GetQuestStatus(QUEST_DISPELLING_ILLUSIONS) == QUEST_STATUS_COMPLETE && pInstance && pInstance->GetData(TYPE_QUEST) == NOT_STARTED)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_CHROMI1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_CHROMI1, pCreature->GetGUID()); 

    return true; 
}

bool GossipSelect_npc_chromi_start(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1) 
    {
       pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_CHROMI2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2); 

       pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_CHROMI2, pCreature->GetGUID()); 
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2) 
    {
       pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_CHROMI3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3); 

       pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_CHROMI3, pCreature->GetGUID()); 
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+3) 
    {
       // START COUNTER HERE 
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
        {
            pInstance->DoUpdateWorldState(WORLD_STATE_COS_CRATE_ON, 1);
            pInstance->SetData(TYPE_QUEST, IN_PROGRESS);
        }

        if (pPlayer)
            if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(ITEM_ARCANE_DISRUPTOR, 1)) 
                pPlayer->SendNewItem(pItem, 1, true, false); 

       pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_CHROMI4, pCreature->GetGUID()); 
    }

    return true;
}

struct MANGOS_DLL_DECL npc_chromi_startAI : public ScriptedAI
{
    npc_chromi_startAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        m_bCounterHere = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bCounterHere;

    void Reset()
    {
        m_bCounterHere = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bCounterHere && m_pInstance && pWho && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(pWho) <= 15 && ((Player*)pWho)->GetQuestStatus(QUEST_DISPELLING_ILLUSIONS) == QUEST_STATUS_INCOMPLETE)
        {
            m_pInstance->DoUpdateWorldState(WORLD_STATE_COS_CRATE_ON, 1);
            m_pInstance->SetData(TYPE_QUEST, IN_PROGRESS);
            m_bCounterHere = true;
        }
    }
};

/*###
## npc_mike
###*/

enum
{
   SAY_MIKE01                = -1557270,
   SAY_FORRESTER02           = -1557271,
   SAY_JAMES03               = -1557272,
   SAY_SIABI04               = -1557273,
   SAY_MIKE05                = -1557274,
   SAY_CORICKS06             = -1557275,
   SAY_GRIAN07               = -1557276,
   SAY_CORICKS08             = -1557277,
   SAY_JAMES09               = -1557278,
   SAY_FORRESTER10           = -1557279,

   EMOTE_SHOT                = 5,
   EMOTE_TALK                = 1,
   EMOTE_POINT               = 25,
   EMOTE_NO                  = 274,
   EMOTE_LAUGH               = 11
};

struct MANGOS_DLL_DECL npc_mikeAI : public ScriptedAI
{
    npc_mikeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    uint64 m_uiForesterGUID;
    uint64 m_uiJamesGUID;
    uint64 m_uiSiabiGUID;
    uint64 m_uiCorricksGUID;
    uint64 m_uiGryanGUID;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void MoveInLineOfSight(Unit* who)
    {
      if (!who)
          return;

      if(!m_pInstance) return;

           if (who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(who) <= 15 && who->GetPositionZ() > 99.50f && m_uiPhase == 0)
           {
             m_creature->SetUInt64Value(UNIT_FIELD_TARGET, who->GetGUID());
             m_uiPhase = 1;
           }
    }

    void TavernEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_MIKE01, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 1:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              m_uiStepTimer = 5000;
              break;
           case 2:
              m_uiForesterGUID = m_pInstance->GetData64(NPC_FORRESTER);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiForesterGUID))
                 DoScriptText(SAY_FORRESTER02, pTemp);
              m_uiStepTimer = 6000;
              break;
           case 3:
              m_uiJamesGUID = m_pInstance->GetData64(NPC_JAMES);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiJamesGUID))
                 DoScriptText(SAY_JAMES03, pTemp);
              m_uiStepTimer = 5000;
              break;
           case 4:
              m_uiSiabiGUID = m_pInstance->GetData64(NPC_FRAS_FRASIABI);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiSiabiGUID))
                 DoScriptText(SAY_SIABI04, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 5:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiSiabiGUID))
                 pTemp->HandleEmoteCommand(EMOTE_SHOT);
              m_uiStepTimer = 5000;
              break;
           case 6:
              m_creature->GetMotionMaster()->MovePoint(0, 1554.849f, 588.465f, 99.775f);
              m_uiStepTimer = 3000;
              break;
           case 7:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiSiabiGUID))
                 pTemp->HandleEmoteCommand(EMOTE_LAUGH);
              m_uiStepTimer = 3000;
              break;
           case 8:
              DoScriptText(SAY_MIKE05, m_creature);
              m_uiStepTimer = 2000;
              break;
           case 9:
              m_creature->HandleEmoteCommand(EMOTE_SHOT);
              m_uiStepTimer = 1000;
              break;
           case 10:
              m_uiCorricksGUID = m_pInstance->GetData64(NPC_MAL_CORICS);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiCorricksGUID))
                 DoScriptText(SAY_CORICKS06, pTemp);
              m_uiStepTimer = 4000;
              break;
           case 11:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiCorricksGUID))
                 pTemp->HandleEmoteCommand(EMOTE_TALK);
              m_uiGryanGUID = m_pInstance->GetData64(NPC_GRIAN_STONE);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiGryanGUID))
                 DoScriptText(SAY_GRIAN07, pTemp);
              m_uiStepTimer = 11000;
              break;
           case 12:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiCorricksGUID))
                 DoScriptText(SAY_CORICKS08, pTemp);
              m_creature->GetMotionMaster()->MovePoint(0, 1549.609f, 575.544f, 100.052f);
              m_uiStepTimer = 2000;
              break;
           case 13:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiJamesGUID))
                 DoScriptText(SAY_JAMES09, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 14:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiJamesGUID))
                 pTemp->HandleEmoteCommand(EMOTE_TALK);
              m_uiStepTimer = 5000;
              break;
           case 15:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiForesterGUID))
                 DoScriptText(SAY_FORRESTER10, pTemp);
              m_uiPhase = 2;
              break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               TavernEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_roger
###*/

enum
{
  SAY_ROGER01                     = -1557280,
  SAY_ROGER02                     = -1557281,
  SAY_ROGER03                     = -1557282,
  SAY_ROGER04                     = -1557283,
};

struct MANGOS_DLL_DECL npc_rogerAI : public ScriptedAI
{
    npc_rogerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartRoger()
    {
      m_uiPhase = 1;
    }

    void FirstCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 7000;
              break;
           case 1:
              MoveToPoint(m_creature, 1590.055f, 615.727f, 99.795f, 7000);
              m_uiStepTimer = 6900;
              break;
           case 2:
              MoveToPoint(m_creature, 1584.039f, 622.049f, 99.907f, 4000);
              m_uiStepTimer = 3900;
              break;
           case 3:
              MoveToPoint(m_creature, 1578.787f, 623.924f, 99.855f, 2000);
              m_uiStepTimer = 2500;
              break;
           case 4:
              DoScriptText(SAY_ROGER01, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 5:
              MoveToPoint(m_creature, 1579.393f, 624.018f, 99.886f, 900);
              m_uiStepTimer = 2000;
              break;
           case 6:
              DoScriptText(SAY_ROGER02, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 7:
              MoveToPoint(m_creature, 1579.387f, 623.198f, 99.837f, 300);
              m_uiStepTimer = 1000;
              break;
           case 8:
              DoScriptText(SAY_ROGER03, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 9:
              MoveToPoint(m_creature, 1575.576f, 619.935f, 99.422f, 1500);
              m_uiStepTimer = 2000;
              break;
           case 10:
              MoveToPoint(m_creature, 1575.833f, 620.471f, 99.466f, 300);
              m_uiStepTimer = 1000;
              break;
           case 11:
              DoScriptText(SAY_ROGER04, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 12:
              MoveToPoint(m_creature, 1580.215f, 624.368f, 99.924f, 2000);
              m_uiStepTimer = 1900;
              break;
           case 13:
              MoveToPoint(m_creature, 1587.471f, 618.181f, 99.850f, 4000);
              m_uiStepTimer = 3900;
              break;
           case 14:
              MoveToPoint(m_creature, 1592.646f, 590.888f, 99.151f, 11000);
              m_uiStepTimer = 13000;
              break;
           case 15:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMotionMaster()->MovementExpired(false);
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               FirstCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_morigan
###*/

enum
{
  SAY_MORIGAN01                     = -1557284,
  SAY_PERELLI02                     = -1557285,
  SAY_MORIGAN03                     = -1557286,
  SAY_PERELLI04                     = -1557287,
  SAY_MORIGAN05                     = -1557288,
  SAY_PERELLI06                     = -1557289,
  SAY_MORIGAN07                     = -1557290,
};

struct MANGOS_DLL_DECL npc_moriganAI : public ScriptedAI
{
    npc_moriganAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiPerelliGUID;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartMorigan()
    {
      m_uiPhase = 1;
    }

    void SecondCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_MORIGAN01, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 1:
              m_uiPerelliGUID = m_pInstance->GetData64(NPC_PERELLI);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 DoScriptText(SAY_PERELLI02, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 2:
              m_creature->GetMotionMaster()->MovePoint(0, 1564.138f, 668.343f, 102.058f);
              m_uiStepTimer = 2000;
              break;
           case 3:
              m_creature->GetMotionMaster()->MovePoint(0, 1567.956f, 667.776f, 102.094f);
              m_uiStepTimer = 1500;
              break;
           case 4:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1569.615f, 668.859f, 102.180f);
              m_uiStepTimer = 2000;
              break;
           case 5:
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              m_uiStepTimer = 1000;
              break;
           case 6:
              DoScriptText(SAY_MORIGAN03, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 7:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
              {
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
              }
              m_uiStepTimer = 3000;
              break;
           case 8:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 DoScriptText(SAY_PERELLI04, pTemp);
              m_uiStepTimer = 3000;
              break;
           case 9:
              DoScriptText(SAY_MORIGAN05, m_creature);
              m_uiStepTimer = 9000;
              break;
           case 10:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 DoScriptText(SAY_PERELLI06, pTemp);
              m_uiStepTimer = 6000;
              break;
           case 11:
              DoScriptText(SAY_MORIGAN07, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 12:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1576.119f, 657.675f, 102.09f, 5000);
              m_uiStepTimer = 4900;
              break;
           case 13:
              MoveToPoint(m_creature, 1586.040f, 646.113f, 100.910f, 6000);
              m_uiStepTimer = 5900;
              break;
           case 14:
              MoveToPoint(m_creature, 1609.189f, 697.134f, 106.902f, 23000);
              m_uiStepTimer = 24900;
              break;
           case 15:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               SecondCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};
     
/*###
## npc_jena
###*/

enum
{
  SAY_JENA01           = -1557291,
  SAY_MARTHA02         = -1557292,
  SAY_JENA03           = -1557293,
  SAY_JENA04           = -1557294,
  SAY_MARTHA05         = -1557295,

};

struct MANGOS_DLL_DECL npc_jenaAI : public ScriptedAI
{
    npc_jenaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiMarthaGUID;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartJena()
    {
      m_uiPhase = 1;
    }

    void ThirdCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1605.546f, 744.869f, 114.731f);
              m_uiStepTimer = 1900;
              break;
           case 1:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1614.967f, 743.673f, 114.063f);
              m_uiStepTimer = 4900;
              break;
           case 2:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1623.848f, 729.251f, 112.410f);
              m_uiStepTimer = 6900;
              break;
           case 3:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1633.460f, 726.261f, 113.518f);
              m_uiStepTimer = 4000;
              break;
           case 4:
              m_uiMarthaGUID = m_pInstance->GetData64(NPC_MARTHA);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_JENA01, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 5:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->RemoveAurasDueToSpell(58925);
                 pTemp->GetMotionMaster()->MovePoint(0, 1635.918f, 724.357f, 113.561f);
              }
              m_uiStepTimer = 1000;
              break;
           case 6:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->GetMotionMaster()->MovementExpired(false);
                 pTemp->GetMotionMaster()->MovePoint(0, 1636.721f, 725.88f, 113.561f);
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
              }
              m_uiStepTimer = 1000;
              break;
           case 7:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 DoScriptText(SAY_MARTHA02, pTemp);
              m_uiStepTimer = 4000;
              break;
           case 8:
              DoScriptText(SAY_JENA03, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 9:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              m_creature->GetMotionMaster()->MovePoint(0, 1629.278f, 727.894f, 112.636f);
              m_uiStepTimer = 1500;
              break;
           case 10:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                 pTemp->GetMap()->CreatureRelocation(pTemp, 1640.089f, 725.766f, 113.561f, 4.77f);
                 pTemp->SendMonsterMove(1640.089f, 725.766f, 113.561f, SPLINETYPE_NORMAL , pTemp->GetSplineFlags(), 1500);
              }
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1629.452f, 729.416f, 112.712f);
              m_uiStepTimer = 1000;
              break;
           case 11:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 pTemp->GetMotionMaster()->MovePoint(0, 1640.103f, 725.522f, 113.561f);
              m_uiStepTimer = 500;
              break;
           case 12:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 pTemp->CastSpell(pTemp, 58925, false);
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              m_uiStepTimer = 1500;
              break;
           case 13:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 1000;
              break;
           case 14:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_JENA04, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 15:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->RemoveAurasDueToSpell(58925);
                 DoScriptText(SAY_MARTHA05, pTemp);
                 MoveToPoint(pTemp, 1638.196f, 726.171f, 113.561f, 1000);
              }
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1615.590f, 719.509f, 110.311f, 2000);
              m_uiStepTimer = 2000;
              break;
           case 16:
              MoveToPoint(m_creature, 1596.436f, 670.809f, 103.747f, 7000);
              m_uiStepTimer = 6900;
              break;
           case 17:
              MoveToPoint(m_creature, 1571.549f, 609.837f, 99.767f, 9000);
              m_uiStepTimer = 11000;
              break;
           case 18:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               ThirdCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_malcolm
###*/

enum
{
  SOUND_ID_DOG_GROWL    = 1108,
  SOUND_ID_DOG_HOWL     = 1018,
  EMOTE_DOG_HOWL        = 393,

  SAY_MALCOLM01         = -1557296,
  SAY_MALCOLM02         = -1557297,
  SAY_MALCOLM03         = -1557298,
  SAY_MALCOLM04         = -1557299,
};

struct MANGOS_DLL_DECL npc_malcolmAI : public ScriptedAI
{
    npc_malcolmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiDogGUID;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartMalcolm()
    {
      m_uiPhase = 1;
    }

    void FourCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              MoveToPoint(m_creature, 1614.066f, 796.722f, 121.739f, 5500);
              m_uiDogGUID = m_pInstance->GetData64(NPC_DOG);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1611.459f, 793.274f, 121.928f, 5500);
              m_uiStepTimer = 5400;
              break;
           case 1:
              DoScriptText(SAY_MALCOLM01, m_creature);
              MoveToPoint(m_creature, 1622.820f, 798.816f, 120.570f, 3500);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1621.467f, 794.323f, 120.323f, 3500);
              m_uiStepTimer = 3400;
              break;
           case 2:
              MoveToPoint(m_creature, 1626.574f, 806.781f, 120.270f, 3500);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1629.232f, 803.629f, 120.011f, 3500);
              m_uiStepTimer = 3400;
              break;
           case 3:
              MoveToPoint(m_creature, 1622.782f, 808.533f, 121.249f, 1500);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1629.265f, 805.245f, 120.070f, 300);
              m_uiStepTimer = 300;
              break;
           case 4:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 pTemp->PlayDirectSound(SOUND_ID_DOG_GROWL);
              m_uiStepTimer = 500;
              break;
           case 5:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_MALCOLM02, m_creature);
              m_uiStepTimer = 2000;
              break;
           case 6:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1629.163f, 809.738f, 120.369f, 1500);
              m_uiStepTimer = 2000;
              break;
           case 7:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
              {
                 pTemp->HandleEmoteCommand(EMOTE_DOG_HOWL);
                 pTemp->PlayDirectSound(SOUND_ID_DOG_HOWL);
              }
              m_uiStepTimer = 4000;
              break;
           case 8:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1629.922f, 807.799f, 120.122f, 3000);
              m_uiStepTimer = 2900;
              break;
           case 9:
              MoveToPoint(m_creature, 1632.169f, 809.851f, 120.047f, 1000);
              m_uiStepTimer = 900;
              break;
           case 10:
              MoveToPoint(m_creature, 1630.651f, 811.149f, 120.307f, 800);
              m_uiStepTimer = 800;
              break;
           case 11:
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              DoScriptText(SAY_MALCOLM03, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 12:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_MALCOLM04, m_creature);
              m_uiStepTimer = 7000;
              break;
           case 13:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1630.692f, 808.011f, 120.083f, 400);
              m_uiStepTimer = 600;
              break;
           case 14:
             if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                pTemp->SetStandState(UNIT_STAND_STATE_SIT);
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1641.452f, 812.600f, 119.948f, 4000);
              m_uiStepTimer = 3900;
              break;
           case 15:
              MoveToPoint(m_creature, 1657.975f, 857.352f, 119.097f, 18000);
              m_uiStepTimer = 17900;
              break;
           case 16:
              MoveToPoint(m_creature, 1679.852f, 912.245f, 120.533f, 23000);
              m_uiStepTimer = 22900;
              break;
           case 17:
              MoveToPoint(m_creature, 1699.915f, 967.110f, 121.643f, 23000);
              m_uiStepTimer = 22900;
              break;
           case 18:
              MoveToPoint(m_creature, 1678.393f, 1026.890f, 125.431f, 25000);
              m_uiStepTimer = 24900;
              break;
           case 19:
              MoveToPoint(m_creature, 1678.943f, 1093.130f, 126.899f, 26000);
              m_uiStepTimer = 25900;
              break;
           case 20:
              MoveToPoint(m_creature, 1700.042f, 1103.880f, 130.872f, 9000);
              m_uiStepTimer = 10900;
              break;
           case 21:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               FourCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_bartleby
###*/

enum
{
  SAY_BARTLEBY01         = -1557300,
  SAY_BARTLEBY02         = -1557301, 
  SAY_BARTLEBY03         = -1557302,
  SAY_BARTLEBY04         = -1557303,
  SAY_BARTLEBY05         = -1557304,
};

struct MANGOS_DLL_DECL npc_bartleby_csAI : public ScriptedAI
{
    npc_bartleby_csAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 4)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void MoveInLineOfSight(Unit* who)
    {
      if (!who)
          return;

      if(!m_pInstance) return;

           if (who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(who) <= 20 && m_uiPhase == 0)
           {
             m_uiPhase = 1;
           }
    }

    void StartBartleby()
    {
      m_uiPhase = 3;
    }

    void FifthCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_BARTLEBY03, m_creature);
              MoveToPoint(m_creature, 1672.539f, 872.277f, 120.113f, 1000); 
              m_uiStepTimer = 1000;
              break;
           case 1:
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              m_uiStepTimer = 3000;
              break;
           case 2:
              DoScriptText(SAY_BARTLEBY04, m_creature);
              m_uiStepTimer = 7000; 
              break;
           case 3:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 1000;
              break;
           case 4:
              DoScriptText(SAY_BARTLEBY05, m_creature);
              MoveToPoint(m_creature, 1663.054f, 869.959f, 119.734f, 3000);
              m_uiStepTimer = 2900; 
              break;
           case 5:
              MoveToPoint(m_creature, 1640.732f, 812.422f, 119.933f, 24000);
              m_uiStepTimer = 23900; 
              break;
           case 6:
              MoveToPoint(m_creature, 1623.704f, 755.741f, 115.710f, 23000);
              m_uiStepTimer = 22900; 
              break;
           case 7:
              MoveToPoint(m_creature, 1607.108f, 699.637f, 106.971f, 23000);
              m_uiStepTimer = 22900; 
              break;
           case 8:
              MoveToPoint(m_creature, 1587.750f, 646.929f, 100.990f, 21000);
              m_uiStepTimer = 20900; 
              break;
           case 9:
              MoveToPoint(m_creature, 1571.103f, 660.949f, 102.084f, 8000);
              m_uiStepTimer = 10900; 
              break;
           case 10:
              m_uiPhase = 4;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }

    void SpeechEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_BARTLEBY01, m_creature);
              m_uiStepTimer = 5000;
              break;
           case 1:
              DoScriptText(SAY_BARTLEBY02, m_creature);
              if(m_uiPhase == 1)
                 m_uiPhase = 2;
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 3)
       {
            if(m_uiStepTimer < uiDiff)
            {
               FifthCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               SpeechEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_crates
###*/

enum
{
    SPELL_LIGHT            = 49590
};

struct MANGOS_DLL_DECL npc_stratholme_cratesAI : public ScriptedAI
{
    npc_stratholme_cratesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool Active;

    void Reset()
    {
        Active = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(!m_pInstance) return;

       if(m_creature->HasAura(SPELL_LIGHT) && Active != true)
       {
            if(Creature* pRoger = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_ROGER)))
            {
               if(m_creature->GetDistance2d(pRoger->GetPositionX(), pRoger->GetPositionY()) < 50.0f)
               {
                   ((npc_rogerAI*)pRoger->AI())->StartRoger();
               }
            }

            if(Creature* pMorigan = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MORIGAN)))
            { 
               if(m_creature->GetDistance2d(pMorigan->GetPositionX(), pMorigan->GetPositionY()) < 50.0f)
               {
                  ((npc_moriganAI*)pMorigan->AI())->StartMorigan();
               }
            }

            if(Creature* pJena = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_JENA)))
            { 
               if(m_creature->GetDistance2d(pJena->GetPositionX(), pJena->GetPositionY()) < 50.0f)
               {
                   ((npc_jenaAI*)pJena->AI())->StartJena();
               }
            }

            if(Creature* pMalcolm = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MALCOLM)))
            { 
               if(m_creature->GetDistance2d(pMalcolm->GetPositionX(), pMalcolm->GetPositionY()) < 50.0f)
               {
                   ((npc_malcolmAI*)pMalcolm->AI())->StartMalcolm();
               }
            }

            if(Creature* pBartleby = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_BARTLEBY)))
            { 
               if(m_creature->GetDistance2d(pBartleby->GetPositionX(), pBartleby->GetPositionY()) < 50.0f)
               {
                   ((npc_bartleby_csAI*)pBartleby->AI())->StartBartleby();
               }
            }

            m_pInstance->SetData(TYPE_CRATES_COUNT, 1);
            if(GameObject* pLight = GetClosestGameObjectWithEntry(m_creature, GO_CRATE_LIGHT, 5.0f))
               pLight->SetPhaseMask(0, true);
            //m_creature->SetPhaseMask(0, true);
            Active = true;
       }

       return;
    }
};

CreatureAI* GetAI_npc_chromi_start(Creature* pCreature)
{
    return new npc_chromi_startAI(pCreature);
}

CreatureAI* GetAI_npc_mike(Creature* pCreature)
{
    return new npc_mikeAI(pCreature);
}

CreatureAI* GetAI_npc_roger(Creature* pCreature)
{
    return new npc_rogerAI(pCreature);
}

CreatureAI* GetAI_npc_morigan(Creature* pCreature)
{
    return new npc_moriganAI(pCreature);
}

CreatureAI* GetAI_npc_jena(Creature* pCreature)
{
    return new npc_jenaAI(pCreature);
}

CreatureAI* GetAI_npc_malcolm(Creature* pCreature)
{
    return new npc_malcolmAI(pCreature);
}

CreatureAI* GetAI_npc_bartleby_cs(Creature* pCreature)
{
    return new npc_bartleby_csAI(pCreature);
}

CreatureAI* GetAI_npc_stratholme_crates(Creature* pCreature)
{
    return new npc_stratholme_cratesAI(pCreature);
}

void AddSC_culling_of_stratholmeAI()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_chromi_start";
    newscript->pGossipHello =  &GossipHello_npc_chromi_start;
    newscript->pGossipSelect = &GossipSelect_npc_chromi_start;
    newscript->GetAI = &GetAI_npc_chromi_start;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_mike";
    newscript->GetAI = &GetAI_npc_mike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_roger";
    newscript->GetAI = &GetAI_npc_roger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_morigan";
    newscript->GetAI = &GetAI_npc_morigan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_jena";
    newscript->GetAI = &GetAI_npc_jena;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_malcolm";
    newscript->GetAI = &GetAI_npc_malcolm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_bartleby_cs";
    newscript->GetAI = &GetAI_npc_bartleby_cs;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_stratholme_crates";
    newscript->GetAI = &GetAI_npc_stratholme_crates;
    newscript->RegisterSelf();
}
