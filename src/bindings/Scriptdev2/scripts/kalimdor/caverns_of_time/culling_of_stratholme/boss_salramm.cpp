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
    SAY_SALRAMM_AGGRO       = -1594130,
    SAY_SALRAMM_DEATH       = -1594131,
    SAY_SALRAMM_SLAY01      = -1594132,
    SAY_SALRAMM_SLAY02      = -1594133,
    SAY_SALRAMM_SLAY03      = -1594134,
    SAY_SALRAMM_STEAL01     = -1594135,
    SAY_SALRAMM_STEAL02     = -1594136,
    SAY_SALRAMM_STEAL03     = -1594137,
    SAY_SUMMON01            = -1594138,
    SAY_SUMMON02            = -1594139,
    SAY_BOOM01              = -1594140,
    SAY_BOOM02              = -1594141,

    SPELL_SB_N                        = 57725,
    SPELL_SB_H                        = 58827,
    SPELL_FLESH                       = 58845,
    SPELL_STEAL                       = 52708,
    SPELL_GNOUL_BLOW                  = 58825,
    SPELL_SUMMON_GNOUL                = 52451,

    NPC_GNOUL                         = 27733
};

struct MANGOS_DLL_DECL boss_salrammAI : public ScriptedAI
{
   boss_salrammAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroic = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        m_creature->SetActiveObjectState(true);
        Reset();
   }

   ScriptedInstance* m_pInstance;
   bool m_bIsHeroic;

   uint32 ShadowBoltTimer;
   uint32 FleshTimer;
   uint32 StealTimer;
   uint32 SummonTimer;

   void Reset() 
   {
     ShadowBoltTimer = 5000;
     FleshTimer = (urand(7000, 9000));
     StealTimer = (urand(9000, 17000));
     SummonTimer = (urand(12000, 17000));
     if(m_pInstance)
        m_pInstance->SetData64(NPC_SALRAMM, m_creature->GetGUID());
   }

   void Aggro(Unit* who)
   {
      DoScriptText(SAY_SALRAMM_AGGRO, m_creature);
   }

   void JustDied(Unit *killer)
   {
       DoScriptText(SAY_SALRAMM_DEATH, m_creature);
       if(m_pInstance)
          m_pInstance->SetData(TYPE_ENCOUNTER, DONE);
   }

   void KilledUnit(Unit* pVictim)
   {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SALRAMM_SLAY01, m_creature); break;
            case 1: DoScriptText(SAY_SALRAMM_SLAY02, m_creature); break;
            case 2: DoScriptText(SAY_SALRAMM_SLAY03, m_creature); break;
        }
   }

   void SpellHitTarget(Unit *target, const SpellEntry *spell)
   {
        if(spell->Id == SPELL_GNOUL_BLOW)
          if(target->GetTypeId() != TYPEID_PLAYER && target->GetEntry() == NPC_GNOUL)
            target->SetDisplayId(11686);
   }

   void UpdateAI(const uint32 diff)
   {

      if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
          return;

        if (ShadowBoltTimer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsHeroic ? SPELL_SB_H : SPELL_SB_N);

            ShadowBoltTimer = (urand(5000, 6000));
        }else ShadowBoltTimer -= diff;

        if (FleshTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target,SPELL_FLESH);

            FleshTimer = 7300;
        }else FleshTimer -= diff;

        if (StealTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target,SPELL_STEAL);

            switch(rand()%3)
            {
                case 0: DoScriptText(SAY_SALRAMM_STEAL01, m_creature); break;
                case 1: DoScriptText(SAY_SALRAMM_STEAL02, m_creature); break;
                case 2: DoScriptText(SAY_SALRAMM_STEAL03, m_creature); break;
            }

            StealTimer = (urand(8000, 11000));
        }else StealTimer -= diff;

        if (SummonTimer < diff)
        {
            switch(rand()%2)
            {
                case 0: DoScriptText(SAY_SUMMON01, m_creature); break;
                case 1: DoScriptText(SAY_SUMMON02, m_creature); break;
            }

            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature,SPELL_SUMMON_GNOUL);

            SummonTimer = (urand(12000, 17000));
        }else SummonTimer -= diff;

        DoMeleeAttackIfReady();
  }
};

/*###
## npc_salramm_gnoul
###*/

struct MANGOS_DLL_DECL npc_salramm_gnoulAI : public ScriptedAI
{
   npc_salramm_gnoulAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
       m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
       m_bIsHeroic = pCreature->GetMap()->IsRaidOrHeroicDungeon();
       m_creature->SetActiveObjectState(true);
       Reset();
   }

   ScriptedInstance* m_pInstance;
   bool m_bIsHeroic;

   uint32 m_uiBlowTimer;

   void Reset() 
   { 
     m_uiBlowTimer = (urand(3000, 15000));
   }

   void MoveInLineOfSight(Unit* pWho)
   {
        if (!pWho)
            return;

        if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->CanFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim())
                {
                    AttackStart(pWho);
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho, 0.0f);
                }
            }
        }
   }

   void UpdateAI(const uint32 uiDiff)
   {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiBlowTimer < uiDiff)
        {
            if(Creature* pSalramm = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_SALRAMM)))
            {
               if(pSalramm->isDead()) return;

               switch(rand()%2)
               {
                  case 0: DoScriptText(SAY_BOOM01, pSalramm); break;
                  case 1: DoScriptText(SAY_BOOM02, pSalramm); break;
               }
               pSalramm->InterruptNonMeleeSpells(false);
               pSalramm->CastSpell(m_creature, SPELL_GNOUL_BLOW, false);
            }
        }
        else m_uiBlowTimer -= uiDiff;

        DoMeleeAttackIfReady();

        return;
   }
};

CreatureAI* GetAI_boss_salramm(Creature* pCreature)
{
    return new boss_salrammAI(pCreature);
}

CreatureAI* GetAI_npc_salramm_gnoul(Creature* pCreature)
{
    return new npc_salramm_gnoulAI(pCreature);
}

void AddSC_boss_salramm()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_salramm";
    newscript->GetAI = &GetAI_boss_salramm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_salramm_gnoul";
    newscript->GetAI = &GetAI_npc_salramm_gnoul;
    newscript->RegisterSelf();
}
