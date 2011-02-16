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
SDName: Forge Of The Souls
SD%Complete: 90%
SDComment: event script! Need Add 6 Coliseum Champion after devourer death.
SDCategory: forge_of_the_souls
SDAuthor: MaxXx2021 aka Mioka
EndScriptData */

#include "precompiled.h"
#include "def_forge.h"

enum
{
  SAY_JAINA_FS01             = -1632040,
  SAY_JAINA_FS02             = -1632041,
  SAY_JAINA_FS03             = -1632042,
  SAY_JAINA_FS04             = -1632043,
  SAY_JAINA_FS05             = -1632044,
  SAY_JAINA_FS06             = -1632045,
  SAY_JAINA_FS07             = -1632046,
  SAY_JAINA_FS08             = -1632047,
  SAY_JAINA_FS09_EXTRO       = -1632029,

  SAY_SYLVANA_FS01           = -1632050,
  SAY_SYLVANA_FS02           = -1632051,
  SAY_SYLVANA_FS03           = -1632052,
  SAY_SYLVANA_FS04           = -1632053,
  SAY_SYLVANA_FS05           = -1632054,
  SAY_SYLVANA_FS06           = -1632055,
  SAY_SYLVANA_FS07_EXTRO     = -1632030,

  GOSSIP_SPEECHINTRO         = 13525,
};

struct MANGOS_DLL_DECL npc_jaina_and_sylvana_FSintroAI : public ScriptedAI
{
    npc_jaina_and_sylvana_FSintroAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
   }

ScriptedInstance* m_pInstance;

uint32 StepTimer;
uint32 Step;
bool StartEvent;

    void Reset()
    {
            if(m_pInstance) 
               if(m_pInstance->GetData(TYPE_DEVOURER) == DONE)
                   m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            Step = 0;
            StepTimer = 100;
            if(m_pInstance->GetData(TYPE_INTRO) == DONE)
            {
              m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
              Step = 8;
            }
    }

   void UpdateAI(const uint32 diff)
    {
         DoMeleeAttackIfReady();

         if(!m_pInstance) return;

         if(StartEvent != true) return;

         if(StepTimer < diff)
           {
             switch(Step)
               {
                case 0:
                    m_pInstance->SetData(TYPE_INTRO, DONE);
                    m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS01, m_creature);
                       StepTimer = 7000;
                      }
                    if(m_creature->GetEntry() == npc_sylvana)
                       StepTimer = 100;
                    ++Step;
                    break;
                case 1:
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS02, m_creature);
                       StepTimer = 9000;
                      } 
                    if(m_creature->GetEntry() == npc_sylvana)
                      {
                       DoScriptText(SAY_SYLVANA_FS01, m_creature);
                       StepTimer = 12000;
                      }
                    ++Step;
                    break;
                case 2:
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS03, m_creature);
                       StepTimer = 8000;
                      }
                    if(m_creature->GetEntry() == npc_sylvana)
                      {
                       DoScriptText(SAY_SYLVANA_FS02, m_creature);
                       StepTimer = 11000;
                      }
                    ++Step;
                    break;
                case 3:
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS04, m_creature);
                       StepTimer = 10000;
                      }
                    if(m_creature->GetEntry() == npc_sylvana)
                      {
                       DoScriptText(SAY_SYLVANA_FS03, m_creature);
                       StepTimer = 11000;
                      }
                    ++Step;
                    break;
                case 4:
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS05, m_creature);
                       StepTimer = 8000;
                      }
                    if(m_creature->GetEntry() == npc_sylvana)
                      {
                       DoScriptText(SAY_SYLVANA_FS04, m_creature);
                       StepTimer = 12000;
                      }
                    ++Step;
                    break;
                case 5:
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS06, m_creature);
                       StepTimer = 12000;
                      }
                    if(m_creature->GetEntry() == npc_sylvana)
                      {
                       DoScriptText(SAY_SYLVANA_FS05, m_creature);
                       StepTimer = 7000;
                      }
                    ++Step;
                    break;
                case 6:
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS07, m_creature);
                       StepTimer = 7000;
                      }
                    if(m_creature->GetEntry() == npc_sylvana)
                      {
                       DoScriptText(SAY_SYLVANA_FS06, m_creature);
                       StepTimer = 4000;
                      }
                    ++Step;
                    break;
                case 7:
                    if(m_creature->GetEntry() == npc_jaina)
                      {
                       DoScriptText(SAY_JAINA_FS08, m_creature);
                       StepTimer = 5000;
                      }                    
                    ++Step;
                    break;
               }
            } else StepTimer -= diff;
    }
};

bool GossipHello_npc_jaina_and_sylvana_FSintro(Player* pPlayer, Creature* pCreature)
{
           if (pCreature->isQuestGiver())
             pPlayer->PrepareQuestMenu( pCreature->GetGUID());
            switch(pCreature->GetEntry())
              {
                case 37597:
                  if(((npc_jaina_and_sylvana_FSintroAI*)pCreature->AI())->StartEvent != true)
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What would you have of me, My Lady?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                  break;
                case 37596:
                  if(((npc_jaina_and_sylvana_FSintroAI*)pCreature->AI())->StartEvent != true)
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What would you have of me, Banshee Queen?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                  break;
               }

    pPlayer->PlayerTalkClass->SendGossipMenu(907,pCreature->GetGUID()); //907
    return true;
}

bool GossipSelect_npc_jaina_and_sylvana_FSintro(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
      pPlayer->CLOSE_GOSSIP_MENU();
      ((npc_jaina_and_sylvana_FSintroAI*)pCreature->AI())->StartEvent = true;

    return true;
}

struct MANGOS_DLL_DECL npc_jaina_and_sylvana_FSextroAI : public ScriptedAI
{
    npc_jaina_and_sylvana_FSextroAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
   }

ScriptedInstance* m_pInstance;

uint32 StepTimer;
uint32 Step;
uint64 m_uiLiderGUID;
uint32 uiSummon_counter;

    void Reset()
    {
            if (m_pInstance)
               if (m_pInstance->GetData(TYPE_DEVOURER) != DONE)
                  {
                   m_pInstance->SetData(TYPE_DEVOURER, NOT_STARTED);
                   Step = 0;
                   m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                   m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                  } else Step = 10;
            StepTimer = 100;
            m_creature->SetVisibility(VISIBILITY_OFF);
    }

   void UpdateAI(const uint32 diff)
    {
        DoMeleeAttackIfReady();

        if (!m_pInstance) return;

        if (m_pInstance->GetData(TYPE_DEVOURER) == DONE)
        {

         if(m_pInstance->GetData64(DATA_LIDER) == 1 && m_creature->GetEntry() == npc_sylvana_extro) return;

         if(m_pInstance->GetData64(DATA_LIDER) == 2 && m_creature->GetEntry() == npc_jaina_extro) return;

         if(StepTimer < diff)
           {
             switch(Step)
               {
                case 0:
                  m_creature->SetVisibility(VISIBILITY_ON);
                  m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                  m_creature->GetMotionMaster()->MovePoint(0, 5653.337f, 2496.407f, 708.829f);
                  uiSummon_counter = 0;
                  StepTimer = 400;
                  ++Step;
                  break;
                case 1:
                    if(m_creature->GetEntry() == npc_jaina_extro)
                    {
                        StepTimer = 100;
                        if (uiSummon_counter < 4)
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(npc_cc_a_01,5623.609f,2457.946f,705.891f,1.37f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                            {
                                pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                switch(uiSummon_counter)
                                 {
                                   case 0:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5659.251f, 2472.017f, 708.696f);
                                     break;
                                   case 1:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5627.611f, 2501.972f, 708.696f);
                                     break;
                                   case 2:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5680.920f, 2482.998f, 708.696f);
                                     break;
                                   case 3:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5641.398f, 2523.911f, 708.696f);
                                     break;
                                 }
                                uiSummon_counter++;
                            }
                        }
                        else
                        {
                            uiSummon_counter = 0;
                            Step++;
                        }
                    }
                    if(m_creature->GetEntry() == npc_sylvana_extro)
                    {
                        StepTimer = 100;
                        if (uiSummon_counter < 4)
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(npc_cc_h_01,5623.609f,2457.946f,705.891f,1.37f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                            {
                                pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                switch(uiSummon_counter)
                                 {
                                   case 0:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5659.251f, 2472.017f, 708.696f);
                                     break;
                                   case 1:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5627.611f, 2501.972f, 708.696f);
                                     break;
                                   case 2:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5680.920f, 2482.998f, 708.696f);
                                     break;
                                   case 3:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5641.398f, 2523.911f, 708.696f);
                                     break;
                                 }
                                uiSummon_counter++;
                            }
                        }
                        else
                        {
                            uiSummon_counter = 0;
                            Step++;
                        }
                    }
                    break;
                case 2:
                    if(m_creature->GetEntry() == npc_jaina_extro)
                    {
                        StepTimer = 100;
                        if (uiSummon_counter < 4)
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(npc_cc_a_02,5623.609f,2457.946f,705.891f,1.37f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                            {
                                pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                switch(uiSummon_counter)
                                 {
                                   case 0:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5661.508f, 2473.066f, 708.696f);
                                     break;
                                   case 1:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5627.146f, 2499.098f, 708.696f);
                                     break;
                                   case 2:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5682.331f, 2485.985f, 708.696f);
                                     break;
                                   case 3:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5639.636f, 2521.228f, 708.696f);
                                     break;
                                 }
                                uiSummon_counter++;
                            }
                        }
                        else
                        {
                            uiSummon_counter = 0;
                            Step++;
                        }
                    }
                    if(m_creature->GetEntry() == npc_sylvana_extro)
                    {
                        StepTimer = 100;
                        if (uiSummon_counter < 4)
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(npc_cc_h_02,5623.609f,2457.946f,705.891f,1.37f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                            {
                                pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                switch(uiSummon_counter)
                                 {
                                   case 0:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5661.508f, 2473.066f, 708.696f);
                                     break;
                                   case 1:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5627.146f, 2499.098f, 708.696f);
                                     break;
                                   case 2:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5682.331f, 2485.985f, 708.696f);
                                     break;
                                   case 3:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5639.636f, 2521.228f, 708.696f);
                                     break;
                                 }
                                uiSummon_counter++;
                            }
                        }
                        else
                        {
                            uiSummon_counter = 0;
                            Step++;
                        }
                    }
                    break;
                case 3:
                    if(m_creature->GetEntry() == npc_jaina_extro)
                    {
                        StepTimer = 100;
                        if (uiSummon_counter < 4)
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(npc_cc_a_03,5623.609f,2457.946f,705.891f,1.37f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                            {
                                pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                switch(uiSummon_counter)
                                 {
                                   case 0:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5664.265f, 2473.974f, 708.696f);
                                     break;
                                   case 1:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5626.768f, 2496.534f, 708.696f);
                                     break;
                                   case 2:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5683.696f, 2489.598f, 708.696f);
                                     break;
                                   case 3:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5637.580f, 2518.249f, 708.696f);
                                     break;
                                 }
                                uiSummon_counter++;
                            }
                        }
                        else
                        {
                            uiSummon_counter = 0;
                            StepTimer = 8000;
                            Step++;
                        }
                    }
                    if(m_creature->GetEntry() == npc_sylvana_extro)
                    {
                        StepTimer = 100;
                        if (uiSummon_counter < 4)
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(npc_cc_h_03,5623.609f,2457.946f,705.891f,1.37f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000))
                            {
                                pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                switch(uiSummon_counter)
                                 {
                                   case 0:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5664.265f, 2473.974f, 708.696f);
                                     break;
                                   case 1:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5626.768f, 2496.534f, 708.696f);
                                     break;
                                   case 2:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5683.696f, 2489.598f, 708.696f);
                                     break;
                                   case 3:
                                     pTemp->GetMotionMaster()->MovePoint(0, 5637.580f, 2518.249f, 708.696f);
                                     break;
                                 }
                                uiSummon_counter++;
                            }
                        }
                        else
                        {
                            uiSummon_counter = 0;
                            StepTimer = 8000;
                            Step++;
                        }
                    }
                    break;
                case 4:
                  if(m_creature->GetEntry() == npc_jaina_extro)
                  {
                    DoScriptText(SAY_JAINA_FS09_EXTRO, m_creature);
                    StepTimer = 6000;
                  }
                  if(m_creature->GetEntry() == npc_sylvana_extro)
                  {
                    DoScriptText(SAY_SYLVANA_FS07_EXTRO, m_creature);
                    StepTimer = 3000;
                  }
                  ++Step;
                  break;
                case 5:
                  m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                  m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                  ++Step;
                  break;
               }
            } else StepTimer -= diff;
          }
        return;
    }
};

CreatureAI* GetAI_npc_jaina_and_sylvana_FSintro(Creature* pCreature)
{
    return new npc_jaina_and_sylvana_FSintroAI(pCreature);
}

CreatureAI* GetAI_npc_jaina_and_sylvana_FSextro(Creature* pCreature)
{
    return new npc_jaina_and_sylvana_FSextroAI(pCreature);
}

void AddSC_forge_of_souls()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_jaina_and_sylvana_FSintro";
    newscript->GetAI = &GetAI_npc_jaina_and_sylvana_FSintro;
    newscript->pGossipHello = &GossipHello_npc_jaina_and_sylvana_FSintro;
    newscript->pGossipSelect = &GossipSelect_npc_jaina_and_sylvana_FSintro;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_jaina_and_sylvana_FSextro";
    newscript->GetAI = &GetAI_npc_jaina_and_sylvana_FSextro;
    newscript->RegisterSelf();
}