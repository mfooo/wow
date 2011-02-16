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
SDName: boss_kologarn
SD%Complete:
SDComment: stone grip and arms need vehicles
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

enum
{
    //yells
    SAY_AGGRO           = -1603150,
    SAY_SHOCKWEAVE      = -1603151,
    SAY_GRAB            = -1603152,
    SAY_LEFT_ARM_LOST   = -1603153,
    SAY_RIGHT_ARM_LOST  = -1603154,
    SAY_SLAY1           = -1603155,
    SAY_SLAY2           = -1603156,
    SAY_BERSERK         = -1603157,
    SAY_DEATH           = -1603158,
    EMOTE_RIGHT_ARM     = -1603355,
    EMOTE_LEFT_ARM      = -1603356,
    EMOTE_STONE_GRIP    = -1603357,

    //kologarn
    SPELL_OVERHEAD_SMASH		= 63356,
    SPELL_OVERHEAD_SMASH_H		= 64003,
    SPELL_ONE_ARMED_SMASH		= 63573,
    SPELL_ONE_ARMED_SMASH_H		= 64006,
    SPELL_STONE_SHOUT			= 63716,
    SPELL_STONE_SHOUT_H			= 64005,
    SPELL_PETRIFYING_BREATH		= 62030,
    SPELL_PETRIFYING_BREATH_H	= 63980,
    SPELL_FOCUSED_EYEBEAM       = 63346,
    SPELL_FOCUSED_EYEBEAM_H     = 63976,
    SPELL_FOCUSED_EYEBEAM_TRIG  = 63369,
    SPELL_FOCUSED_EYEBEAM_VISUAL= 63368,
    SPELL_ENRAGE                = 26662,
    //left arm
    SPELL_SHOCKWAVE				= 63783,
    SPELL_SHOCKWAVE_H			= 63982,
    //right arm
    SPELL_STONE_GRIP_GRAB       = 63981,    // not working
    SPELL_STONE_GRIP			= 64290,
    SPELL_STONE_GRIP_H			= 64292,
    //both
    SPELL_ARM_VISUAL			= 64753,
    //rubble
    SPELL_RUMBLE				= 63818,    // on 10 man
    SPELL_STONE_NOVA			= 63978,    // on 25 man
    //NPC ids
    MOB_RUBBLE					= 33768, 

    ACHIEV_RUBBLE_AND_ROLL      = 2959,
    ACHIEV_RUBBLE_AND_ROLL_H    = 2960,
    ACHIEV_WITH_OPEN_ARMS       = 2951,
    ACHIEV_WITH_OPEN_ARMS_H     = 2952,
    ACHIEV_DISARMED             = 2953,
    ACHIEV_DISARMED_H           = 2954,
    ACHIEV_IF_LOOKS_COULD_KILL  = 2955,
    ACHIEV_IF_LOOKS_COULD_KILL_H= 2956,
};

float LeftArm[3] = {1784.742f, -39.962f, 448.805f}; 
float RightArm[3] = {1785.615f, -5.516f, 448.810f};
const float KoloFront[3] = {1771.683f, -24.230f, 448.806f};

// Rubble
struct MANGOS_DLL_DECL mob_ulduar_rubbleAI : public ScriptedAI
{
    mob_ulduar_rubbleAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStone_Nova_Timer;

    void Reset()
    {
        m_uiStone_Nova_Timer = urand(8000, 12000);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_KOLOGARN) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStone_Nova_Timer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_RUMBLE : SPELL_STONE_NOVA);
            m_uiStone_Nova_Timer = urand(7000, 9000);
        }else m_uiStone_Nova_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ulduar_rubble(Creature* pCreature)
{
    return new mob_ulduar_rubbleAI(pCreature);
}

// Left Arm
struct MANGOS_DLL_DECL boss_left_armAI : public ScriptedAI
{
    boss_left_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShockwave_Timer;

    void Reset()
    {
        m_uiShockwave_Timer = 30000;
        DoCast(m_creature, SPELL_ARM_VISUAL);
        m_creature->SetRespawnDelay(DAY);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_RIGHT_ARM)))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_KOLOGARN)))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_KOLOGARN)))
        {
            DoScriptText(SAY_LEFT_ARM_LOST, pTemp);
            if (pTemp->isAlive())
                pTemp->DealDamage(pTemp, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShockwave_Timer < diff)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_KOLOGARN)))
                DoScriptText(SAY_SHOCKWEAVE, pTemp);

            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHOCKWAVE : SPELL_SHOCKWAVE_H);
            m_uiShockwave_Timer = 17000;
        }else m_uiShockwave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_left_arm(Creature* pCreature)
{
    return new boss_left_armAI(pCreature);
}

// Right Arm
struct MANGOS_DLL_DECL boss_right_armAI : public ScriptedAI
{
    boss_right_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStone_Grip_Timer;
    uint32 m_uiFreeDamage;
    uint32 m_uiMaxDamage;
    uint64 m_uiGripTargetGUID[3];
    uint8 m_uiMaxTargets;

    void Reset()
    {
        m_uiStone_Grip_Timer    = 20000;
        m_uiMaxTargets          = m_bIsRegularMode ? 1 : 3;
        for(int i = 0; i < m_uiMaxTargets; i++)
            m_uiGripTargetGUID[i] = 0;
        m_uiFreeDamage          = 0;
        m_uiMaxDamage           = m_bIsRegularMode ? 100000 : 480000;

        DoCast(m_creature, SPELL_ARM_VISUAL);
        m_creature->SetRespawnDelay(DAY);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEFT_ARM)))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_KOLOGARN)))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) 
            pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_STONE_GRIP : SPELL_STONE_GRIP_H);
    }

    void JustReachedHome()
    {
        //if (m_pInstance)
            //m_pInstance->DoRemoveAurasDueToSpellOnPlayers(m_bIsRegularMode ? SPELL_STONE_GRIP : SPELL_STONE_GRIP_H);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        m_uiFreeDamage += uiDamage;
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_KOLOGARN)))
        {
            DoScriptText(SAY_RIGHT_ARM_LOST, pTemp);
            if (pTemp->isAlive())
                pTemp->DealDamage(pTemp, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        for(int i = 0; i < m_uiMaxTargets; i++)
        {
            if (Unit* pVictim = m_creature->GetMap()->GetUnit( m_uiGripTargetGUID[i]))
                pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_STONE_GRIP : SPELL_STONE_GRIP_H);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiFreeDamage > m_uiMaxDamage)
        {
            m_uiFreeDamage = 0;
            for(int i = 0; i < m_uiMaxTargets; i++)
            {
                if (Unit* pVictim = m_creature->GetMap()->GetUnit( m_uiGripTargetGUID[i]))
                    pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_STONE_GRIP : SPELL_STONE_GRIP_H);
            }
        }

        if (m_uiStone_Grip_Timer < diff)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_KOLOGARN)))
                DoScriptText(SAY_GRAB, pTemp);

            DoScriptText(EMOTE_STONE_GRIP, m_creature);

			// this needs vehicles!
            for(int i = 0; i < m_uiMaxTargets; i++)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)){
                    pTarget->CastSpell(pTarget, m_bIsRegularMode ? SPELL_STONE_GRIP : SPELL_STONE_GRIP_H, false);
                    m_uiGripTargetGUID[i] = pTarget->GetGUID();
                }
            }
            m_uiStone_Grip_Timer = 30000;
        }else m_uiStone_Grip_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_right_arm(Creature* pCreature)
{
    return new boss_right_armAI(pCreature);
}

// Kologarn
struct MANGOS_DLL_DECL boss_kologarnAI : public ScriptedAI
{
    boss_kologarnAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpell_Timer;
    uint32 m_uiCheck_Timer;
    uint32 m_uiEyebeah_Timer;
    uint32 m_uiRespawnRightTimer;
    uint32 m_uiRespawnLeftTimer;
    uint32 m_uiEnrageTimer;

    uint32 m_uiRubbleNo;
    bool m_bHasLeftDied;
    bool m_bHasRightDied;
    uint32 m_uiDisarmedTimer;
    bool m_bOpenArms;

    bool m_bIsRightDead;
    bool m_bIsLeftDead;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

        m_uiSpell_Timer     = 10000;
        m_uiCheck_Timer     = 6300;
        m_uiEnrageTimer     = 600000;
        m_uiEyebeah_Timer   = 10000 + urand(1000, 5000);
        m_bIsRightDead      = false;
        m_bIsLeftDead       = false;

        m_uiRubbleNo        = 0;
        m_bHasLeftDied      = false;
        m_bHasRightDied     = false;
        m_bOpenArms         = true;
        m_uiDisarmedTimer   = 0;
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        // hacky way to complete achievements; use only if you have this function
        // Rubble and roll
        if (m_uiRubbleNo >= 25)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_RUBBLE_AND_ROLL : ACHIEV_RUBBLE_AND_ROLL_H);
        }

        // With open arms
        if (m_bOpenArms)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_WITH_OPEN_ARMS : ACHIEV_WITH_OPEN_ARMS_H);
        }

        // Disarmed
        if (m_bHasLeftDied && m_bHasRightDied && m_uiDisarmedTimer <= 12000)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_DISARMED : ACHIEV_DISARMED_H);
        }

        //death yell
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KOLOGARN, DONE);
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEFT_ARM)))
            {
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
            }
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_RIGHT_ARM)))
            {
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KOLOGARN, IN_PROGRESS);

            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEFT_ARM)))
            {
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            }
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_RIGHT_ARM)))
            {
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            }
        }
        //aggro yell
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KOLOGARN, FAIL);
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEFT_ARM)))
            {
                if (!pTemp->isAlive())
                    pTemp->Respawn();
            }
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_RIGHT_ARM)))
            {
                if (!pTemp->isAlive())
                    pTemp->Respawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpell_Timer < uiDiff)
        {
            if (!m_bIsRightDead && !m_bIsLeftDead)
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OVERHEAD_SMASH : SPELL_OVERHEAD_SMASH_H);
            else if (m_bIsRightDead && m_bIsLeftDead)
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_STONE_SHOUT : SPELL_STONE_SHOUT_H);
            else
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ONE_ARMED_SMASH : SPELL_ONE_ARMED_SMASH_H);
            m_uiSpell_Timer = 20000;
        }else m_uiSpell_Timer -= uiDiff;   

        // to be fixed -> only damage, no animation
        if (m_uiEyebeah_Timer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(target, SPELL_FOCUSED_EYEBEAM_VISUAL);
                DoCast(target, m_bIsRegularMode ? SPELL_FOCUSED_EYEBEAM : SPELL_FOCUSED_EYEBEAM_H, true);
            }
            m_uiEyebeah_Timer = 20000;
        }else m_uiEyebeah_Timer -= uiDiff;

		// respawn arms
        if (m_uiRespawnLeftTimer < uiDiff && m_bIsLeftDead)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEFT_ARM)))
            {
                if (!pTemp->isAlive())
                {
                    pTemp->Respawn();
                    m_bIsLeftDead   = false;
                    m_bHasLeftDied  = false;
                    DoScriptText(EMOTE_LEFT_ARM, m_creature);
                }
            }
        }else m_uiRespawnLeftTimer -= uiDiff;  

        if (m_uiRespawnRightTimer < uiDiff && m_bIsRightDead)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_RIGHT_ARM)))
            {
                if (!pTemp->isAlive())
                {
                    pTemp->Respawn();
                    m_bIsRightDead  = false;
                    m_bHasRightDied = false;
                    DoScriptText(EMOTE_RIGHT_ARM, m_creature);
                }
            }
        }else m_uiRespawnRightTimer -= uiDiff; 

		// check if arms are dead and if there is no player in melee range
        if (m_uiCheck_Timer < uiDiff)
        {
            if (Creature* lArm = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEFT_ARM)))
            {
                if (!lArm->isAlive() && !m_bIsLeftDead)
                {
                    m_bHasLeftDied  = true;
                    m_bOpenArms     = false;
                    m_uiDisarmedTimer = 0;

                    for(uint8 i = 0; i < 5; i ++)
                    {
                        if(Creature* pRubble = m_creature->SummonCreature(MOB_RUBBLE, LeftArm[0] - urand(0, 5), LeftArm[1] + urand(0, 10), LeftArm[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                        {
                            pRubble->GetMotionMaster()->MovePoint(0, KoloFront[0], KoloFront[1], KoloFront[2]);

                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                pRubble->AddThreat(pTarget,0.0f);
                                pRubble->AI()->AttackStart(pTarget);
                                pRubble->SetInCombatWithZone();
                            }

                            m_uiRubbleNo += 1;
                        }
                    }
                    m_bIsLeftDead = true;
                    m_uiRespawnLeftTimer = 47000;
                }
            }
            if (Creature* rArm = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_RIGHT_ARM)))
            {
                if (!rArm->isAlive() && !m_bIsRightDead)
                {
                    m_bHasRightDied = true;
                    m_bOpenArms     = false;
                    m_uiDisarmedTimer = 0;

                    for(uint8 i = 0; i < 5; i ++)
                    {
                        if(Creature* pRubble = m_creature->SummonCreature(MOB_RUBBLE, RightArm[0] - urand(0, 5), RightArm[1] + urand(0, 10), RightArm[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                        {
                            pRubble->GetMotionMaster()->MovePoint(0, KoloFront[0], KoloFront[1], KoloFront[2]);

                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                pRubble->AddThreat(pTarget,0.0f);
                                pRubble->AI()->AttackStart(pTarget);
                                pRubble->SetInCombatWithZone();
                            }

                            m_uiRubbleNo += 1;
                        }
                    }
                    m_bIsRightDead = true;
                    m_uiRespawnRightTimer = 47000;
                }
            }

            //Petrifying breath
            if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), 5))
                DoCast(m_creature, m_bIsRegularMode ? SPELL_PETRIFYING_BREATH : SPELL_PETRIFYING_BREATH_H);

            m_uiCheck_Timer = 500;
        }else m_uiCheck_Timer -= uiDiff;

        // disarmed achiev check
        if (m_bHasLeftDied || m_bHasRightDied)
            m_uiDisarmedTimer += uiDiff;

        if(m_uiEnrageTimer < uiDiff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_ENRAGE);
            m_uiEnrageTimer = 30000;
        }
        else m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kologarn(Creature* pCreature)
{
    return new boss_kologarnAI(pCreature);
}

void AddSC_boss_kologarn()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_kologarn";
    NewScript->GetAI = GetAI_boss_kologarn;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_ulduar_rubble";
    NewScript->GetAI = &GetAI_mob_ulduar_rubble;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_left_arm";
    NewScript->GetAI = &GetAI_boss_left_arm;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_right_arm";
    NewScript->GetAI = &GetAI_boss_right_arm;
    NewScript->RegisterSelf();
}