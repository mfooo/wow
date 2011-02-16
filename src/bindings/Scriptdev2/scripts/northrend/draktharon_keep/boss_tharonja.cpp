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
SDName: Boss_Tharonja
SD%Complete: 80%
SDComment: Timers, end event with drakuru
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

enum
{
    SAY_AGGRO                       = -1600012,
    SAY_KILL_1                      = -1600013,
    SAY_KILL_2                      = -1600014,
    SAY_FLESH_1                     = -1600015,
    SAY_FLESH_2                     = -1600016,
    SAY_SKELETON_1                  = -1600017,
    SAY_SKELETON_2                  = -1600018,
    SAY_DEATH                       = -1600019,

    //Phase 1 (Skeleton) Spells
    SPELL_CURSE_OF_LIFE                = 49527,
    H_SPELL_CURSE_OF_LIFE            = 59972,

    SPELL_SHADOW_VOLLEY                = 49528,
    H_SPELL_SHADOW_VOLLEY            = 59973,
    SPELL_RAIN_OF_FIRE                = 49518,
    H_SPELL_RAIN_OF_FIRE            = 59971,
    
    SPELL_DECAY_FLESH                = 49356,            //not working

    //Phase 2 (Flesh) Spells
    SPELL_GIFT_OF_THARONJA            = 52509,

    SPELL_LIGHTNING_BREATH            = 49537,
    H_SPELL_LIGHTNING_BREATH        = 59936,
    SPELL_EYE_BEAM                    = 49544,
    H_SPELL_EYE_BEAM                = 59965,

    SPELL_POISON_CLOUD                = 49548,
    H_SPELL_POSION_CLOUD            = 59969,
    
    SPELL_RETURN_FLESH                = 53463,                //not working

    //achie hacks
    ACHIEVEMENT_NORMAL                = 482,
    ACHIEVEMENT_HEROIC                = 493

};

//Phasses
enum Phase
{
    PHASE_SKELETON                    =   0,
    PHASE_INTOFLESH                    =   1,
    PHASE_FLESH                        =   2,
    PHASE_INTOSKELETON                =   3
};

enum PhaseChangeTimer
{
    PHASE_CHANGE_SKELETON            = 12000,
    PHASE_CHANGE_REAL                = 6000,
    PHASE_CHANGE_FLESH                = 20000
};
/*######
## boss_tharonja
######*/

struct MANGOS_DLL_DECL boss_tharonjaAI : public ScriptedAI
{
    boss_tharonjaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 PhaseChangeTimer;
    uint32 Phase;
    uint32 CurseOfLife_Timer;
    uint32 SkeletonSpells_Timer;
    uint32 PoisonCloud_Timer;
    uint32 FleshSpells_Timer;

    void Reset()
    {
        PhaseChangeTimer = PHASE_CHANGE_SKELETON;
        Phase = PHASE_SKELETON;
        SkeletonSpells_Timer = urand (5000, 10000);
        CurseOfLife_Timer = urand (5000, 10000);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THARONJA, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THARONJA, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THARONJA, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Phase == PHASE_SKELETON)
        {
            if (CurseOfLife_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CURSE_OF_LIFE : H_SPELL_CURSE_OF_LIFE);
                CurseOfLife_Timer = urand (5000, 10000);
            }else CurseOfLife_Timer -= uiDiff;

            if (SkeletonSpells_Timer < uiDiff)
            {     
                switch(urand(0, 1))
                {
                    case 0: 
                        DoCast(m_creature->getVictim(),  m_bIsRegularMode ? SPELL_SHADOW_VOLLEY : H_SPELL_SHADOW_VOLLEY);
                    case 1: 
                        DoCast(m_creature->getVictim(),  m_bIsRegularMode ? SPELL_RAIN_OF_FIRE : H_SPELL_RAIN_OF_FIRE);
                }
                SkeletonSpells_Timer = urand (5000, 10000);
            }else SkeletonSpells_Timer -= uiDiff;

            if (PhaseChangeTimer < uiDiff)
            {
                m_creature->CastSpell(m_creature, SPELL_DECAY_FLESH, true);
                PhaseChangeTimer = PHASE_CHANGE_REAL;
                Phase = PHASE_INTOFLESH;
            }else PhaseChangeTimer -= uiDiff;
            
            DoMeleeAttackIfReady();
        }
        
        if (Phase == PHASE_INTOFLESH)
        {
            if (PhaseChangeTimer < uiDiff)
            {
                m_creature->CastSpell(m_creature, SPELL_GIFT_OF_THARONJA, true);
                m_creature->SetDisplayId(27073);
                PhaseChangeTimer = PHASE_CHANGE_FLESH;
                Phase = PHASE_FLESH;
                FleshSpells_Timer = 1500;
                PoisonCloud_Timer = 10000;
            }else PhaseChangeTimer -= uiDiff;
        }
        
        if (Phase == PHASE_FLESH)
        {
            if (PhaseChangeTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_RETURN_FLESH);
                PhaseChangeTimer = PHASE_CHANGE_REAL;
                Phase = PHASE_INTOSKELETON;
            }else PhaseChangeTimer -= uiDiff;

            if (PoisonCloud_Timer < uiDiff)
            {
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_POISON_CLOUD : H_SPELL_POSION_CLOUD);
                PoisonCloud_Timer = 10000;
            }else PoisonCloud_Timer -= uiDiff;

            if (FleshSpells_Timer < uiDiff)
            {     
                switch(urand(0, 3))
                {
                    case 0:
                    case 1:
                    case 2:
                        DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_LIGHTNING_BREATH : H_SPELL_LIGHTNING_BREATH);
                    case 3: 
                        DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_EYE_BEAM : SPELL_EYE_BEAM);
                }
                FleshSpells_Timer = 1500;
            }else FleshSpells_Timer -= uiDiff;
        }
        
        if (Phase == PHASE_INTOSKELETON)
        {
            if (PhaseChangeTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_DECAY_FLESH);
                m_creature->SetDisplayId(27072);
                PhaseChangeTimer = PHASE_CHANGE_SKELETON;
                Phase = PHASE_SKELETON;
                SkeletonSpells_Timer = urand (5000, 10000);
                CurseOfLife_Timer = urand (5000, 10000);
            }else PhaseChangeTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_boss_tharonja(Creature* pCreature)
{
    return new boss_tharonjaAI(pCreature);
}

void AddSC_boss_tharonja()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_tharonja";
    pNewScript->GetAI = &GetAI_boss_tharonja;
    pNewScript->RegisterSelf();
}
