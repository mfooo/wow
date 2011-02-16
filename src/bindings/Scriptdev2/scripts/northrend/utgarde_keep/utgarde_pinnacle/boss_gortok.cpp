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
SDName: Boss_Gortok
SD%Complete: %
SDComment: 
SDAuthor: /// dev FallenAngelX ///
SDCategory: Utgarde Pinnacle
TODO:
EndScriptData */

/*
ACID support for mini boses
*/

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    SAY_AGGRO               = -1575015,
    SAY_SLAY_1              = -1575016,
    SAY_SLAY_2              = -1575017,
    SAY_DEATH               = -1575018,

    SPELL_FREEZE_ANIM       = 16245,

    SPELL_IMPALE            = 48261,
    SPELL_IMPALE_H          = 59268,

    SPELL_WITHERING_ROAR    = 48256,
    SPELL_WITHERING_ROAR_H  = 59267,

    SPELL_ARCING_SMASH      = 48260,
};

uint32 m_uiCreature[4] = {NPC_FURBOLG, NPC_WORGEN, NPC_JORMUNGAR, NPC_RHINO};

float fSpawn[4][2] =
{
    {292.419098f, -462.365509f},
    {290.920288f, -441.411682f},
    {261.907288f, -440.493561f},
    {260.422180f, -463.380402f}
};

/*######
## boss_gortok
######*/

struct MANGOS_DLL_DECL boss_gortokAI : public ScriptedAI
{
    boss_gortokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool   m_bIsEventNow;
    uint64 m_uiAddGUID[4];
    uint32 m_uiRoarTimer;
    uint32 m_uiImpaleTimer;
    uint32 m_uiArcingSmashTimer;
    uint8  m_uiPhase;

    void Reset()
    {
        m_bIsEventNow        = true;
        m_uiPhase            = 0;
        m_uiRoarTimer        = urand(22000,28000);
        m_uiImpaleTimer      = urand(15000,25000);
        m_uiArcingSmashTimer = urand(10000,30000);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); 

        for(uint8 i=0; i<4; ++i)
            m_uiAddGUID[i] = 0;
    }

    void Aggro(Unit* pWho)
    {
        if(!pWho)
            return;

        StartAttack(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GORTOK, DONE);
    }

    void StartAttack(Unit* pVictim)
    {
        //uint8 m_uiCreatureNumber = urand(0,3);
        //losowanie adda
        Creature* pAdd = m_creature->SummonCreature(m_uiCreature[m_uiPhase], fSpawn[m_uiPhase][0], fSpawn[m_uiPhase][1], m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
        if(pAdd)
        {
            m_uiAddGUID[m_uiPhase] = pAdd->GetGUID();
            pAdd->setFaction(974);
            pAdd->AI()->AttackStart(pVictim);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_bIsEventNow)
        {
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();

            Unit* pAdd = m_creature->GetMap()->GetCreature(m_uiAddGUID[m_uiPhase]);
            if(pAdd && !pAdd->isAlive()) 
            {
                if((m_bIsRegularMode && m_uiPhase == 1) || m_uiPhase == 3)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); 
                    if(m_creature->getVictim())
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    DoScriptText(SAY_AGGRO, m_creature);

                    m_bIsEventNow = false;
                    return;
                }
                ++m_uiPhase;

                if(m_creature->getVictim())
                    StartAttack(m_creature->getVictim());
            }
            return;
        }

        if(m_uiRoarTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WITHERING_ROAR : SPELL_WITHERING_ROAR_H, false);
            m_uiRoarTimer = urand(22000,28000);
        }
        else
            m_uiRoarTimer -= uiDiff;

        if(m_uiImpaleTimer < uiDiff)
        {
            if(Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(pUnit, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
            m_uiImpaleTimer = urand(15000,25000);
        }
        else
            m_uiImpaleTimer -= uiDiff;

        if(m_uiArcingSmashTimer < uiDiff)   
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_ARCING_SMASH, false);
            m_uiArcingSmashTimer = urand(10000,30000);
        }
        else
            m_uiArcingSmashTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gortok(Creature* pCreature)
{
    return new boss_gortokAI(pCreature);
}

void AddSC_boss_gortok()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_gortok";
    newscript->GetAI = &GetAI_boss_gortok;
    newscript->RegisterSelf();
}
