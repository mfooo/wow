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
SDName: Boss_Colossus
SD%Complete: 80%
SDComment:
SDAuthor: MaxXx2021 Aka Mioka.
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010,

    SPELL_EMERGE                = 54850,
    SPELL_SUMMON_ELEMENTAL      = 54851,
    SPELL_MIGHTY_BLOW           = 54719,
    SPELL_MERGE                 = 54878,
    SPELL_SURGE                 = 54801,
    SPELL_SURGE_EFFECT          = 54827,
    SPELL_MOJO_PUDDLE           = 55627,
    SPELL_MOJO_PUDDLE_H         = 58994,
    SPELL_MOJO_WAVE             = 55626,
    SPELL_MOJO_WAVE_H           = 58993,
    SPELL_FREEZE_ANIM           = 16245,

    NPC_LIVING_MOJO             = 29830,

    POINT_ID_START              = 1,
};

/*######
## boss_colossus
######*/

struct MANGOS_DLL_DECL boss_colossusAI : public ScriptedAI
{
    boss_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 uiMightyBlowTimer;
    uint32 uiStepTimer;
    uint32 m_uiStartEventTimer;

    uint8 uiStep;

    bool m_bIsElementalDead;
    bool m_bIsFirstActive;
    bool m_bIsSecondActive;
    bool m_bIsElementalEvent;
    bool m_bStartEvent;
    bool m_bEventInProgress;

    void Reset()
    {
        m_uiStartEventTimer = 3000;
        m_bStartEvent = false;
        m_bEventInProgress = false;
        m_bIsFirstActive = false;
        m_bIsSecondActive = false;
        m_bIsElementalDead = false;
        m_bIsElementalEvent = false;
        uiMightyBlowTimer = 10000;
        uiStepTimer = 2000;
        uiStep = 0;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        SetCombatMovement(true);

        if (m_pInstance)
        {
            if (Creature* pSummon = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ELEMENTAL)))
                pSummon->ForcedDespawn();
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, NOT_STARTED);

        std::list<Creature*> pMojo;
        GetCreatureListWithEntryInGrid(pMojo, m_creature, NPC_LIVING_MOJO, 12.0f);

        if (!pMojo.empty())
        {
            for (std::list<Creature*>::iterator itr = pMojo.begin(); itr != pMojo.end(); ++itr)
                (*itr)->Respawn();
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
           return;

        ScriptedAI::AttackStart(pWho);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ELEMENTAL)
        {
            pSummoned->SetInCombatWithZone();
            
            if (m_creature->GetHealthPercent() < 10.0f)
                pSummoned->SetHealth(pSummoned->GetMaxHealth() / 2);
        }
    }

    void GolemPhase()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);
        m_creature->SetInCombatWithZone();
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
    }

    void SummonElemental()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->InterruptNonMeleeSpells(true);
        m_creature->CastSpell(m_creature, SPELL_SUMMON_ELEMENTAL, true);
        m_creature->GetMotionMaster()->Clear();
        SetCombatMovement(false);
        m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
        DoCast(m_creature, SPELL_FREEZE_ANIM, true);
        m_bIsElementalEvent = false;
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage) 
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            if (!m_bIsElementalDead)
            {
                uiDamage = 0;
                m_creature->SetHealth(1);
            }
           
            if (!m_bIsSecondActive)
            {
                m_bIsSecondActive = true;
                uiStepTimer = 2900;
                m_bIsElementalEvent = true;
                DoCast(m_creature, SPELL_EMERGE);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_COLOSSUS) == IN_PROGRESS && !m_bStartEvent)
            {
                m_bStartEvent = true;

                std::list<Creature*> pMojo;
                GetCreatureListWithEntryInGrid(pMojo, m_creature, NPC_LIVING_MOJO, 12.0f);
                if (!pMojo.empty())
                {
                    for (std::list<Creature*>::iterator itr = pMojo.begin(); itr != pMojo.end(); ++itr)
                    {
                        (*itr)->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        (*itr)->GetMotionMaster()->MovePoint(POINT_ID_START, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                    }
                }
            }
        }

        if (m_bStartEvent && !m_bEventInProgress)
        {
            if (m_uiStartEventTimer <= uiDiff)
            {
                GolemPhase();
                m_bEventInProgress = true;
            }
            else
                m_uiStartEventTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiMightyBlowTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            uiMightyBlowTimer = 10000;
        }
        else
            uiMightyBlowTimer -= uiDiff;

        if (m_creature->GetHealthPercent() < 50.0f && !m_bIsFirstActive)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
            uiStepTimer = 2900;
            m_bIsFirstActive = true;
            m_bIsElementalEvent = true;
            DoCast(m_creature, SPELL_EMERGE);
        }

        if (m_bIsElementalEvent)
        {
            if (uiStepTimer <= uiDiff)
            {
                SummonElemental();
                uiStepTimer = 2000;
            }
            else
                uiStepTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_elementalAI : public ScriptedAI
{
    boss_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSurgeTimer;
    uint32 m_uiMojoPuddleTimer;

    bool bGoToColossus;

    void Reset()
    {
        m_uiSurgeTimer = 7000;
        m_uiMojoPuddleTimer = 3000;
        bGoToColossus = false;
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_MERGE && pTarget->GetEntry() == NPC_COLOSSUS)
        {
            if (Creature* pColossus = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                ((boss_colossusAI*)pColossus->AI())->GolemPhase();
                m_creature->ForcedDespawn();
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!bGoToColossus && m_creature->GetHealthPercent() < 50.0f && m_pInstance)
        {
            if (Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                if (pColossus->GetHealthPercent() > 10.0f)
                {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->InterruptNonMeleeSpells(true);
                    DoCast(pColossus, SPELL_MERGE);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
                    SetCombatMovement(false);
                    bGoToColossus = true;
                }
            }
        }

        if (m_uiSurgeTimer <= diff)
        {
            DoScriptText(EMOTE_SURGE, m_creature);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SURGE);

            m_uiSurgeTimer = 15000;
        }
        else
            m_uiSurgeTimer -= diff;

        if (m_uiMojoPuddleTimer <= diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
               DoCast(pTarget, m_bIsRegularMode ? SPELL_MOJO_PUDDLE : SPELL_MOJO_PUDDLE_H);

            m_uiMojoPuddleTimer = urand(1000, 2000);
        }
        else
            m_uiMojoPuddleTimer -= diff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            if (Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                ((boss_colossusAI*)pColossus->AI())->m_bIsElementalDead = true;
                pColossus->DealDamage(pColossus, pColossus->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }
    }
};

struct MANGOS_DLL_DECL npc_living_mojoAI : public ScriptedAI
{
    npc_living_mojoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMojoWaveTimer;
    uint32 m_uiMojoPuddleTimer;

    void Reset()
    {
        m_uiMojoWaveTimer = 2000;
        m_uiMojoPuddleTimer = 7000;
        SetCombatMovement(true);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                if (m_creature->GetDistance2d(pColossus) < 15.0f)
                {
                    SetCombatMovement(false);
                    m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
                }
            }
        }
    }
    
    void MovementInform(uint32 uiMoveType, uint32 uiPointId)
    {
        if (uiMoveType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == POINT_ID_START)
            m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_COLOSSUS) == IN_PROGRESS)
                return;
        }

        if (m_uiMojoWaveTimer <= diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_WAVE : SPELL_MOJO_WAVE_H);
            m_uiMojoWaveTimer = 15000;
        }
        else
            m_uiMojoWaveTimer -= diff;

        if (m_uiMojoPuddleTimer <= diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_PUDDLE : SPELL_MOJO_PUDDLE_H);
            m_uiMojoPuddleTimer = 18000;
        }
        else
            m_uiMojoPuddleTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

CreatureAI* GetAI_boss_elemental(Creature* pCreature)
{
    return new boss_elementalAI (pCreature);
}

CreatureAI* GetAI_npc_living_mojo(Creature* pCreature)
{
    return new npc_living_mojoAI (pCreature);
}

void AddSC_boss_colossus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_colossus";
    newscript->GetAI = &GetAI_boss_colossus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elemental";
    newscript->GetAI = &GetAI_boss_elemental;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_living_mojo";
    newscript->GetAI = &GetAI_npc_living_mojo;
    newscript->RegisterSelf();
}
