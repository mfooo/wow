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
    SPELL_COURSE                   = 52772,
    SPELL_TIME_STOP                = 58848,
    SPELL_TIME_WARP                = 52766,
    SPELL_SPIKE_N                  = 52771,
    SPELL_SPIKE_H                  = 58830,  

    SAY_EPOCH_DEATH        = -1594119,
    SAY_EPOCH_SLAY01       = -1594120,
    SAY_EPOCH_SLAY02       = -1594121,
    SAY_EPOCH_SLAY03       = -1594122,
    SAY_EPOCH_WARP01       = -1594123,
    SAY_EPOCH_WARP02       = -1594124,
    SAY_EPOCH_WARP03       = -1594125
};

struct MANGOS_DLL_DECL boss_lord_epochAI : public ScriptedAI
{
   boss_lord_epochAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroic = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        m_creature->SetActiveObjectState(true);
        Reset();
   }

   ScriptedInstance* m_pInstance;
   bool m_bIsHeroic;

   uint32 Spike_Timer;
   uint32 Warp_Timer;
   uint32 Stop_Timer;
   uint32 Course_Timer;
   uint64 m_uiArthasGUID;

   void Reset() 
   {
     Course_Timer = 9300;
     Stop_Timer = 21300;
     Warp_Timer = 25300;
     Spike_Timer = 5300;
   }

   void JustDied(Unit *killer)
   {
       DoScriptText(SAY_EPOCH_DEATH, m_creature);
   }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_EPOCH_SLAY01, m_creature); break;
            case 1: DoScriptText(SAY_EPOCH_SLAY02, m_creature); break;
            case 2: DoScriptText(SAY_EPOCH_SLAY03, m_creature); break;
        }
    }

   void UpdateAI(const uint32 diff)
   {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();

        if (Course_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target, SPELL_COURSE);

            Course_Timer = 9300;
        }else Course_Timer -= diff;

        if (Spike_Timer < diff)
        {

            DoCast(m_creature->getVictim(),m_bIsHeroic ? SPELL_SPIKE_H : SPELL_SPIKE_N);

            Spike_Timer = 5300;
        }else Spike_Timer -= diff;

        if (Stop_Timer < diff)
        {
             if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target, SPELL_TIME_STOP);

            Stop_Timer = 21300;
        }else Stop_Timer -= diff;

        if (Warp_Timer < diff)
        {
             if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(target, SPELL_TIME_WARP);
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_EPOCH_WARP01, m_creature); break;
            case 1: DoScriptText(SAY_EPOCH_WARP02, m_creature); break;
            case 2: DoScriptText(SAY_EPOCH_WARP03, m_creature); break;
        }

            Warp_Timer = 25300;
        }else Warp_Timer -= diff;

  }
};

CreatureAI* GetAI_boss_lord_epoch(Creature* pCreature)
{
    return new boss_lord_epochAI(pCreature);
}

void AddSC_boss_lord_epoch()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_lord_epoch";
    newscript->GetAI = &GetAI_boss_lord_epoch;
    newscript->RegisterSelf();
}
