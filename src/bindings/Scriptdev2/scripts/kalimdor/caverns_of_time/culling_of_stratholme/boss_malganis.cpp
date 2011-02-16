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

enum
{
    SAY_MALGANIS_AGGRO       = -1594170,
    SAY_MALGANIS_SLAY01      = -1594172,
    SAY_MALGANIS_SLAY02      = -1594173,
    SAY_MALGANIS_SLAY03      = -1594174,
    SAY_MALGANIS_SLAY04      = -1594175,
    SAY_MALGANIS_SLAY05      = -1594176,
    SAY_MALGANIS_SLAY06      = -1594177,
    SAY_MALGANIS_SLAY07      = -1594166,
    SAY_MALGANIS_SLEEP01     = -1594185,
    SAY_MALGANIS_SLEEP02     = -1594186,
    SAY_MALGANIS_Sleep       = -1594178,
    SAY_MALGANIS_15HP        = -1594179,

    SPELL_SWAMP_N                          = 52720,
    SPELL_SWAMP_H                          = 58852,
    SPELL_MIND_BLAST_N                     = 52722,
    SPELL_MIND_BLAST_H                     = 58850,
    SPELL_SLEEP_N                          = 52721,
    SPELL_SLEEP_H                          = 58849,
    SPELL_VAMPIRE                          = 52723
};

struct MANGOS_DLL_DECL boss_malganisAI : public ScriptedAI
{
   boss_malganisAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroic = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        m_creature->SetActiveObjectState(true);
        Reset();
   }

   ScriptedInstance* m_pInstance;
   bool m_bIsHeroic;

  Unit* pTarget;
  bool Sleep;
  bool Vampire;
  uint32 Phase;
  Creature* Malganis;
  Creature* Arthas;

  uint32 Swamp_Timer;
  uint32 MindBlast_Timer;
  uint32 Sleep_Timer;
  uint32 Vampire_Timer;

   void Reset() 
   { 
     Sleep = false;
     Vampire = false;
     Swamp_Timer = 6300;
     MindBlast_Timer = 11300;
     Sleep_Timer = 17300;
     Vampire_Timer = 30000;
   }

   void AttackStart(Unit* who)
   {
        if(m_pInstance->GetData(TYPE_PHASE) > 9) return;

        if(m_pInstance->GetData(TYPE_MALGANIS) != IN_PROGRESS) return;

        if(!who || who == m_creature)
            return;

        ScriptedAI::AttackStart(who);
   }

   void KillCreditMalganis()
   {
         Map *map = m_creature->GetMap();
         Map::PlayerList const& players = map->GetPlayers();
         if (!players.isEmpty() && map->IsDungeon())
         {
           for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
           {
             if(Player* pPlayer = itr->getSource()) 
               pPlayer->KilledMonsterCredit(31006, m_creature->GetGUID());
           }
         }
   }

   void EnterEvadeMode()
   {
      m_creature->RemoveAllAuras();
      m_creature->DeleteThreatList();
      m_creature->CombatStop(true);
      m_creature->LoadCreatureAddon();

      if(m_pInstance->GetData(TYPE_PHASE) > 9)
      {
        KillCreditMalganis();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        if (Creature* pArthas = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_ARTHAS)))
           m_creature->SetInCombatWith(pArthas);
      }
      else
         m_creature->RemoveFromWorld();

      m_creature->SetLootRecipient(NULL);
    }

   void Aggro(Unit* who)
   {
     if(m_pInstance->GetData(TYPE_PHASE) > 9) return;

     DoScriptText(SAY_MALGANIS_AGGRO, m_creature);
   }

   void KilledUnit(Unit* pVictim)
   {
        switch(rand()%7)
        {
            case 0: DoScriptText(SAY_MALGANIS_SLAY01, m_creature); break;
            case 1: DoScriptText(SAY_MALGANIS_SLAY02, m_creature); break;
            case 2: DoScriptText(SAY_MALGANIS_SLAY03, m_creature); break;
            case 3: DoScriptText(SAY_MALGANIS_SLAY04, m_creature); break;
            case 4: DoScriptText(SAY_MALGANIS_SLAY05, m_creature); break;
            case 5: DoScriptText(SAY_MALGANIS_SLAY06, m_creature); break;
            case 6: DoScriptText(SAY_MALGANIS_SLAY07, m_creature); break;
        }
   }

   void UpdateAI(const uint32 diff)
   {
        if(m_pInstance->GetData(TYPE_PHASE) > 9) return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();

        if (Swamp_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target, m_bIsHeroic ? SPELL_SWAMP_H : SPELL_SWAMP_N);

            Swamp_Timer = 7300;
        }else Swamp_Timer -= diff;

        if (MindBlast_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target, m_bIsHeroic ? SPELL_MIND_BLAST_H : SPELL_MIND_BLAST_N);

            MindBlast_Timer = 11300;
        }else MindBlast_Timer -= diff;

   if(m_creature->GetHealthPercent() < 40.0f)
   {
      if(Sleep == false)
      {
        Sleep = true;
        DoScriptText(SAY_MALGANIS_Sleep, m_creature); 
      }

        if (Sleep_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target, m_bIsHeroic ? SPELL_SLEEP_H : SPELL_SLEEP_N);
            switch(rand()%2)
            {
                case 0: DoScriptText(SAY_MALGANIS_SLEEP01, m_creature); break;
                case 1: DoScriptText(SAY_MALGANIS_SLEEP02, m_creature); break;
            }

            Sleep_Timer = 17300;
        }else Sleep_Timer -= diff;
   }

   if(m_creature->GetHealthPercent() < 25.0f)
   {
        if(Vampire == false)
        {
           Vampire = true;
           DoScriptText(SAY_MALGANIS_15HP, m_creature); 
           DoCast(m_creature, SPELL_VAMPIRE);
        }

        if (Vampire_Timer < diff)
        {
                DoCast(m_creature, SPELL_VAMPIRE);

            Vampire_Timer = 30000;
        }else Vampire_Timer -= diff;

   }

   if(m_creature->GetHealthPercent() < 5.0f)
   { 
      m_pInstance->SetData(TYPE_PHASE, 10);
      m_pInstance->SetData(TYPE_MALGANIS, DONE);
      EnterEvadeMode();
   }

  }
};

CreatureAI* GetAI_boss_malganis(Creature* pCreature)
{
    return new boss_malganisAI(pCreature);
}

void AddSC_boss_malganis()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_malganis";
    newscript->GetAI = &GetAI_boss_malganis;
    newscript->RegisterSelf();

}