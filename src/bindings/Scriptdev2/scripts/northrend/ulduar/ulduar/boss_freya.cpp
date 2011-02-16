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
SDName: boss_freya
SD%Complete:
SDComment: aura stacking need core support after the recent aura changes
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

enum
{
    /* YELLS */
    // freya
    SAY_AGGRO               = -1603000,
    SAY_AGGRO_HARD          = -1603001,
    SAY_SUMMON1             = -1603002,
    SAY_SUMMON2             = -1603003,
    SAY_SUMMON3             = -1603004,
    SAY_SLAY1               = -1603005,
    SAY_SLAY2               = -1603006,
    SAY_DEATH               = -1603007,
    SAY_BERSERK             = -1603008,
    EMOTE_ALLIES_NATURE     = -1603362,
    EMOTE_LIFEBINDERS_GIFT  = -1603363,
    EMOTE_GROUND_TREMMOR    = -1603364,
    EMOTE_IRON_ROOTS        = -1603365,
    //brightleaf
    SAY_BRIGHTLEAF_AGGRO    = -1603160,
    SAY_BRIGHTLEAF_SLAY1    = -1603161,
    SAY_BRIGHTLEAF_SLAY2    = -1603162,
    SAY_BRIGHTLEAF_DEATH    = -1603163,
    //Ironbranch
    SAY_IRONBRANCH_AGGRO    = -1603170,
    SAY_IRONBRANCH_SLAY1    = -1603171,
    SAY_IRONBRANCH_SLAY2    = -1603172,
    SAY_IRONBRANCH_DEATH    = -1603173,
    //Stonebark
    SAY_STONEBARK_AGGRO     = -1603180,
    SAY_STONEBARK_SLAY1     = -1603181,
    SAY_STONEBARK_SLAY2     = -1603182,
    SAY_STONEBARK_DEATH     = -1603183,

    /* BOSS SPELLS */
    SPELL_ATTUNED_TO_NATURE        = 62519, //increases healing, start at 150 stacks
	SPELL_ATTUNED_10_STACKS		   = 62525,
	SPELL_ATTUNED_2_STACKS		   = 62524,
	SPELL_ATTUNED_25_STACKS		   = 62521,
    SPELL_TOUCH_OF_EONAR           = 62528, //heals Freya, 6k per second
    SPELL_TOUCH_OF_EONAR_H         = 62892, //heals Freya, 24k per second
    SPELL_SUNBEAM                  = 62623,
    SPELL_SUNBEAM_H                = 62872,
    SPELL_BERSERK                  = 47008, // 10 min

    /* HARD MODE SPELLS */
    SPELL_DRAINED_OF_POWER          = 62467,
    // brightleaf
    SPELL_UNSTABLE_ENERGY_FREYA     = 62451,
    SPELL_UNSTABLE_ENERGY_FREYA_H   = 62865,
    SPELL_BRIGHTLEAFS_ESSENCE       = 62968, //62385,
    SPELL_EFFECT_BRIGHTLEAF         = 63294,
    // ironbrach
    SPELL_STRENGHTEN_IRON_ROOTS     = 63601,
    NPC_STRENGHENED_IRON_ROOTS      = 33168,
    SPELL_IRON_ROOTS_FREYA          = 62438,
    SPELL_IRON_ROOTS_FREYA_H        = 62861,
    SPELL_IRONBRANCH_ESSENCE        = 62713, //62387,
    SPELL_EFFECT_IRONBRANCH         = 63292,
    // stonebark
    SPELL_GROUND_TREMOR_FREYA       = 62437,
    SPELL_GROUND_TREMOR_FREYA_H     = 62859,
    SPELL_STONEBARKS_ESSENCE        = 65590, //62386,
    SPELL_EFFECT_STONEBARK          = 63295,

    NPC_SUN_BEAM                    = 33170,
    NPC_UNSTABLE_SUN_BEAM           = 33050,

    // sanctuary adds 
    NPC_EONARS_GIFT             = 33228,
    SPELL_LIFEBINDERS_GIFT      = 62584,    // after 12 secs, heals Freya & her allies for 30%
    SPELL_LIFEBINDERS_GIFT_H    = 64185,    // the same but for 60%
    SPELL_PHEROMONES            = 62619,    // protects from conservators grip
    NPC_HEALTHY_SPORE           = 33215,

    /* ADDS */
    // 6 waves of adds. 1 of the 3 each min 
    NPC_DETONATING_LASHER       = 32918,    // recude 2 stacks
    // spells
    SPELL_FLAME_LASH            = 62608,
    SPELL_DETONATE              = 62598,
    SPELL_DETONATE_H            = 62937, 

    NPC_ANCIENT_CONSERVATOR     = 33203,    // reduce 30 stacks
    //spells
    SPELL_CONSERVATORS_GRIP     = 62532,
    SPELL_NATURES_FURY          = 62589,
    SPELL_NATURES_FURY_H        = 63571,

    /* elemental adds */                    // each one reduces 10 stacks
    NPC_WATER_SPIRIT            = 33202,
    // spells
    SPELL_TIDAL_WAVE            = 62653,
    SPELL_TIDAL_WAVE_H          = 62935,

    NPC_STORM_LASHER            = 32919,
    // spells
    SPELL_STORMBOLT             = 62649,
    SPELL_STORMBOLT_H           = 62938,
    SPELL_LIGHTNING_LASH        = 62648,    // 3 targets
    SPELL_LIGHTNING_LASH_H      = 62939,    // 5 targets

    NPC_SNAPLASHER              = 32916,
    // spells
    SPELL_HARDENED_BARK         = 62663,
    SPELL_HARDENED_BARK_H       = 64190,

    // nature bomb
    NPC_NATURE_BOMB             = 34129,
    GO_NATURE_BOMB              = 194902,
    SPELL_NATURE_BOMB           = 64587,
    SPELL_NATURE_BOMB_H         = 64650,

    /* ELDERS */                            // used in phase 1
    ELDER_BRIGHTLEAF            = 32915,
    ELDER_IRONBRANCH            = 32913,
    ELDER_STONEBARK             = 32914,

    // brightleaf spells
    SPELL_BRIGHTLEAF_FLUX       = 62262,
    SPELL_SOLAR_FLARE           = 62240,
    SPELL_SOLAR_FLARE_H         = 62920,
    SPELL_UNSTABLE_SUN_BEAM     = 62211,
    SPELL_UNSTABLE_SUN_BEAM_A   = 62243,
    SPELL_UNSTABLE_ENERGY       = 62217,    // cancels sun bean 
    SPELL_UNSTABLE_ENERGY_H     = 62922,
    SPELL_PHOTOSYNTHESIS        = 62209,

    // ironbrach spells
    SPELL_IMPALE                = 62310,
    SPELL_IMPALE_H              = 62928,
    SPELL_IRON_ROOTS            = 62283, 
    SPELL_IRON_ROOTS_H          = 62930,
    NPC_IRON_ROOTS              = 33088,
    SPELL_THORM_SWARM           = 62285,
    SPELL_THORM_SWARM_H         = 62931,

    // stonebark spells
    SPELL_FIST_OF_STONE         = 62344,
    SPELL_BROKEN_BONES          = 62356,
    SPELL_GROUND_TREMOR         = 62325,
    SPELL_GROUND_TREMOR_H       = 62932,
    SPELL_PETRIFIED_BARK        = 62337,
    SPELL_PETRIFIED_BARK_H      = 62933,

    // not used because summoned chest doesn't despawn after looted
    SPELL_SUMMON_CHEST_1                = 62950,
    SPELL_SUMMON_CHEST_2                = 62952,
    SPELL_SUMMON_CHEST_3                = 62953,
    SPELL_SUMMON_CHEST_4                = 62954,
    SPELL_SUMMON_CHEST_5                = 62955,
    SPELL_SUMMON_CHEST_6                = 62956,
    SPELL_SUMMON_CHEST_7                = 62957,
    SPELL_SUMMON_CHEST_8                = 62958,

    SPELL_SUMMON_ALLIES_OF_NATURE       = 62678, //better do that in sd2
    SPELL_SUMMON_LASHERS                = 62688, // lashers - broken
    SPELL_SUMMON_ELEMENTALS             = 62686, // elementals -> better in sd2
    SPELL_SUMMON_CONSERVATOR            = 62685, // conservator
    SPELL_LIFEBINDERS_GIFT_SUMMON       = 62869,
    SPELL_NATURE_BOMB_SUMMON            = 64606,

    SPELL_SPORE_SUMMON_NE               = 62591,
    SPELL_SPORE_SUMMON_SE               = 62592,
    SPELL_SPORE_SUMMON_SW               = 62593,
    SPELL_SPORE_SUMMON_NW               = 62582,

    SPELL_HEALTHY_SPORE_VISUAL          = 62538,
    SPELL_NATURE_BOMB_VISUAL            = 64604,
    SPELL_LIFEBINDERS_VISUAL            = 62579,
    SPELL_LIFEBINDER_GROW               = 44833,

    SPELL_PHEROMONES_LG                 = 62619,
    SPELL_POTENT_PHEROMONES             = 62541,

    ACHIEV_BACK_TO_NATURE           = 2982,
    ACHIEV_BACK_TO_NATURE_H         = 2983,
    ACHIEV_KNOCK_WOOD               = 3177,
    ACHIEV_KNOCK_WOOD_H             = 3185,
    ACHIEV_KNOCK_KNOCK_WOOD         = 3178,
    ACHIEV_KNOCK_KNOCK_WOOD_H       = 3186,
    ACHIEV_KNOCK_KNOCK_KNOCK_WOOD   = 3179,
    ACHIEV_KNOCK_KNOCK_KNOCK_WOOD_H = 3187, 
};

// Iron roots & stranghned iron roots
struct MANGOS_DLL_DECL mob_iron_rootsAI : public ScriptedAI
{
    mob_iron_rootsAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint64 m_uiVictimGUID;
    uint32 m_uiCreatureEntry;

    void Reset()
    {
        m_uiVictimGUID = 0;
        m_uiCreatureEntry = m_creature->GetEntry();
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = m_creature->GetMap()->GetUnit(m_uiVictimGUID))
                {
                    switch(m_uiCreatureEntry)
                    {
                    case NPC_IRON_ROOTS:
                        pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H);
                        break;
                    case NPC_STRENGHENED_IRON_ROOTS:
                        pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : SPELL_IRON_ROOTS_FREYA_H);
                        break;
                    }
                }
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) 
        {
            switch(m_uiCreatureEntry)
            {
            case NPC_IRON_ROOTS:
                pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H);
                break;
            case NPC_STRENGHENED_IRON_ROOTS:
                pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : SPELL_IRON_ROOTS_FREYA_H);
                break;
            }
        }
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = m_creature->GetMap()->GetUnit(m_uiVictimGUID))
        {
            switch(m_uiCreatureEntry)
            {
            case NPC_IRON_ROOTS:
                pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H);
                break;
            case NPC_STRENGHENED_IRON_ROOTS:
                pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : SPELL_IRON_ROOTS_FREYA_H);
                break;
            }
        }
    }

    void UpdateAI(const uint32 uiuiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

// Elder Brightleaf
struct MANGOS_DLL_DECL boss_elder_brightleafAI : public ScriptedAI
{
    boss_elder_brightleafAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 m_uiBrightleafFluxTimer;
    uint32 m_uiSolarFlareTimer;
    uint32 m_uiUnstableSunBeanTimer;
    uint32 m_uiUnstabelEnergyTimer;
    uint32 m_uiSunbeamStacks;
    uint32 m_uiHealTimer;
    bool m_bHasSunbeam;

    void Reset()
    {
        m_uiBrightleafFluxTimer     = 5000;
        m_uiSolarFlareTimer         = 10000 + urand(1000, 5000);
        m_uiUnstableSunBeanTimer    = 15000;
        m_uiUnstabelEnergyTimer     = 30000;
        m_uiSunbeamStacks           = 1;
        m_bHasSunbeam               = false;
    }

    void Aggro(Unit* pWho)
    {    
        DoScriptText(SAY_BRIGHTLEAF_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_BRIGHTLEAF_SLAY1, m_creature);
        else
            DoScriptText(SAY_BRIGHTLEAF_SLAY2, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_BRIGHTLEAF_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		// this needs core suport
        if(m_uiBrightleafFluxTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BRIGHTLEAF_FLUX);
            m_uiBrightleafFluxTimer = 5000;
        }
        else m_uiBrightleafFluxTimer -= uiDiff;

        if(m_uiSolarFlareTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SOLAR_FLARE : SPELL_SOLAR_FLARE_H);
            m_uiSolarFlareTimer = 10000 + urand(1000, 5000);
        }
        else m_uiSolarFlareTimer -= uiDiff;

		// also the following spells need some core support -> hacky way of use
		// PLEASE FIX FOR REVISION!
        if(m_uiUnstableSunBeanTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_UNSTABLE_SUN_BEAM);
            m_bHasSunbeam = true;
            m_uiHealTimer = 1000;
            m_uiUnstableSunBeanTimer = urand(7000, 12000);
        }
        else m_uiUnstableSunBeanTimer -= uiDiff;

		// cast after the unstable sun bean
        if (m_uiHealTimer < uiDiff && m_bHasSunbeam)
        {
            DoCast(m_creature, SPELL_PHOTOSYNTHESIS);
            m_bHasSunbeam = false;
        }
        else m_uiHealTimer -= uiDiff;

		// removes photosynthesis when standing inside
        if(m_uiUnstabelEnergyTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY: SPELL_UNSTABLE_ENERGY_H);
            m_creature->RemoveAurasDueToSpell(SPELL_UNSTABLE_SUN_BEAM_A);
            m_creature->RemoveAurasDueToSpell(SPELL_PHOTOSYNTHESIS);
            m_uiSunbeamStacks = 1;
            m_uiUnstabelEnergyTimer = urand(20000, 30000);
        }
        else m_uiUnstabelEnergyTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_elder_brightleaf(Creature* pCreature)
{
    return new boss_elder_brightleafAI(pCreature);
}

// Elder Ironbranch
struct MANGOS_DLL_DECL boss_elder_ironbranchAI : public ScriptedAI
{
    boss_elder_ironbranchAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 m_uiImpaleTimer;
    uint32 m_uiIronrootsTimer;
    uint32 m_uiThornSwarmTimer;

    void Reset()
    {
        m_uiImpaleTimer         = 10000 + urand (1000, 5000);
        m_uiIronrootsTimer      = 20000 + urand (1000, 7000);
        m_uiThornSwarmTimer     = 30000;
    }

    void Aggro(Unit* pWho)
    {    
        DoScriptText(SAY_IRONBRANCH_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_IRONBRANCH_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_IRONBRANCH_SLAY1, m_creature);
        else
            DoScriptText(SAY_IRONBRANCH_SLAY2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiImpaleTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
            m_uiImpaleTimer = 10000 + urand (1000, 5000);
        }
        else m_uiImpaleTimer -= uiDiff;

        if(m_uiIronrootsTimer < uiDiff)
        {
            if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCast(target, m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H);
            m_uiIronrootsTimer = 20000 + urand (1000, 7000);
        }
        else m_uiIronrootsTimer -= uiDiff;

        if(m_uiThornSwarmTimer < uiDiff)
        {
            if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCast(target, m_bIsRegularMode ? SPELL_THORM_SWARM : SPELL_THORM_SWARM_H);
            m_uiThornSwarmTimer = 30000;
        }
        else m_uiThornSwarmTimer -= uiDiff;

        DoMeleeAttackIfReady(); 
    }
};

CreatureAI* GetAI_boss_elder_ironbranch(Creature* pCreature)
{
    return new boss_elder_ironbranchAI(pCreature);
}

// Stonebark
struct MANGOS_DLL_DECL boss_elder_stonebarkAI : public ScriptedAI
{
    boss_elder_stonebarkAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 m_uiFistsOfStoneTimer;
    uint32 m_uiGroundTremorTimer;
    uint32 m_uiPetrifiedBarkTimer;

    void Reset()
    {
        m_uiFistsOfStoneTimer   = 20000;
        m_uiGroundTremorTimer   = 15000;
        m_uiPetrifiedBarkTimer  = 25000;
    }

    void Aggro(Unit* pWho)
    {    
        DoScriptText(SAY_STONEBARK_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_STONEBARK_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_STONEBARK_SLAY1, m_creature);
        else
            DoScriptText(SAY_STONEBARK_SLAY2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiFistsOfStoneTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_FIST_OF_STONE);
            m_uiFistsOfStoneTimer = 30000;
        }
        else m_uiFistsOfStoneTimer -= uiDiff;

        if(m_uiGroundTremorTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR : SPELL_GROUND_TREMOR_H);
            m_uiGroundTremorTimer = 15000 + urand (1000, 5000);
        }
        else m_uiGroundTremorTimer -= uiDiff;

        if(m_uiPetrifiedBarkTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_PETRIFIED_BARK : SPELL_PETRIFIED_BARK_H);
            m_uiPetrifiedBarkTimer = 20000 + urand (1000, 5000);
        }
        else m_uiPetrifiedBarkTimer -= uiDiff;

        DoMeleeAttackIfReady(); 
    }
};

CreatureAI* GetAI_boss_elder_stonebark(Creature* pCreature)
{
    return new boss_elder_stonebarkAI(pCreature);
}

// Freya
struct MANGOS_DLL_DECL boss_freyaAI : public ScriptedAI
{
    boss_freyaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSummonTimer;
    uint32 m_uiWaveNumber;
    uint32 m_uiWaveType;
    uint32 m_uiWaveTypeInc;
    uint32 m_uiSunbeamTimer;
    uint32 m_uiEnrageTimer;

    bool m_bIsHardMode;
	bool m_bHasAura;
    
    uint32 m_uiNatureBombTimer;
    uint32 m_uiLifebindersGiftTimer;

    bool m_bIsOutro;
    uint32 m_uiOutroTimer;
    uint32 m_uiStep;

    // hard mode timers
    uint32 m_uiUnstableEnergyTimer;
    uint32 m_uiStrenghtenIronRootsTimer;
    uint32 m_uiGroundTremorTimer;

    uint32 m_uiThreeWaveCheckTimer;
    bool m_bWaveCheck;
    uint64 m_uiWaterSpiritGUID;
    uint64 m_uiStormLasherGUID;
    uint64 m_uiSnapLasherGUID;

    bool m_bIsBrightleafAlive;
    bool m_bIsIronbranchAlive;
    bool m_bIsStonebarkAlive;

    uint32 m_uiAchievProgress;
    bool m_bNature;

    void Reset()
    {
        m_uiSummonTimer                 = 15000;
        m_uiWaveNumber                  = 0;
        m_uiWaveType                    = irand(0,2);
        m_uiWaveTypeInc                 = irand(1,2);
        m_uiSunbeamTimer                = rand()%10000;
        m_uiEnrageTimer                 = 600000; //10 minutes
        m_bIsHardMode                   = false;
        m_uiLifebindersGiftTimer        = 30000;
        m_uiUnstableEnergyTimer         = 25000;
        m_uiStrenghtenIronRootsTimer    = 25000 + urand(1000, 5000);
        m_uiGroundTremorTimer           = 20000;
        m_uiNatureBombTimer             = 7000;
        m_uiThreeWaveCheckTimer         = 1000;
        m_uiAchievProgress              = 10000;
        m_bWaveCheck                    = false;
        m_uiWaterSpiritGUID             = 0;
        m_uiStormLasherGUID             = 0;
        m_uiSnapLasherGUID              = 0;

        m_uiOutroTimer                  = 10000;
        m_uiStep                        = 1;
        m_bIsOutro                      = false;

        m_uiAchievProgress              = 0;
        m_bNature                       = false;
		m_bHasAura						= false;

        if(m_pInstance) 
        {
            // remove elder auras
            if (Creature* pBrightleaf = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_BRIGHTLEAF)))
            {
                if (pBrightleaf->isAlive())
                    pBrightleaf->RemoveAllAuras();
            }
            if (Creature* pIronbranch = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_IRONBRACH)))
            {
                if (pIronbranch->isAlive())
                    pIronbranch->RemoveAllAuras();
            }
            if (Creature* pStonebark = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_STONEBARK)))
            {
                if (pStonebark->isAlive())
                    pStonebark->RemoveAllAuras();
            }
        }
    }

    void Aggro(Unit *who) 
    {
		// aura should stack up to 150 when casted, need core support
		DoCast(m_creature, SPELL_ATTUNED_TO_NATURE);

        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FREYA, IN_PROGRESS);

            // check brightleaf
            if (Creature* pBrightleaf = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_BRIGHTLEAF)))
            {
                if (pBrightleaf->isAlive())
                {
                    pBrightleaf->CastSpell(pBrightleaf, SPELL_DRAINED_OF_POWER, false);
                    pBrightleaf->CastSpell(m_creature, SPELL_EFFECT_BRIGHTLEAF, false);
                    m_bIsBrightleafAlive = true;
                    m_uiAchievProgress += 1;
                }
                else
                    m_bIsBrightleafAlive = false;
            }

            // check ironbranch
            if (Creature* pIronbranch = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_IRONBRACH)))
            {
                if (pIronbranch->isAlive())
                {
                    pIronbranch->CastSpell(pIronbranch, SPELL_DRAINED_OF_POWER, false);
                    pIronbranch->CastSpell(m_creature, SPELL_EFFECT_IRONBRANCH, false);
                    m_bIsIronbranchAlive = true;
                    m_uiAchievProgress += 1;
                }
                else
                    m_bIsIronbranchAlive = false;
            }

            // check stonebark
            if (Creature* pStonebark = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_STONEBARK)))
            {
                if (pStonebark->isAlive())
                {
                    pStonebark->CastSpell(pStonebark, SPELL_DRAINED_OF_POWER, false);
                    pStonebark->CastSpell(m_creature, SPELL_EFFECT_STONEBARK, false);
                    m_bIsStonebarkAlive = true;
                    m_uiAchievProgress += 1;
                }
                else
                    m_bIsStonebarkAlive = false;
            }
        }

        m_bIsHardMode = CheckHardMode();

        if(!m_bIsHardMode)
            DoScriptText(SAY_AGGRO, m_creature);
        else
            DoScriptText(SAY_AGGRO_HARD, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FREYA, FAIL);
    }

    void DoOutro()
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FREYA_HARD, 0);

            // hacky way to complete achievements; use only if you have this function
            if(m_uiAchievProgress == 1)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_KNOCK_WOOD : ACHIEV_KNOCK_WOOD_H);
                m_pInstance->SetData(TYPE_FREYA_HARD, 1);
            }
            else if (m_uiAchievProgress == 2)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_KNOCK_KNOCK_WOOD : ACHIEV_KNOCK_KNOCK_WOOD_H);
                m_pInstance->SetData(TYPE_FREYA_HARD, 2);
            }
            else if (m_uiAchievProgress == 3)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_KNOCK_KNOCK_KNOCK_WOOD : ACHIEV_KNOCK_KNOCK_KNOCK_WOOD_H);
                m_pInstance->SetData(TYPE_FREYA_HARD, 3);
            }

            if (m_bNature)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_BACK_TO_NATURE : ACHIEV_BACK_TO_NATURE_H);

            m_pInstance->SetData(TYPE_FREYA, DONE);
        }

        m_creature->ForcedDespawn();
    }

    // for debug only!
    void JustDied(Unit* pKiller)
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FREYA, DONE);
            if(m_bIsHardMode)
                m_pInstance->SetData(TYPE_FREYA_HARD, DONE);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealthPercent() < 1.0f)
        {
            uiDamage = 0;
            m_bIsOutro = true;
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

	// summon 12 Lashers. Should be done by a spell which needs core fix
    void SummonLashers()
    {
        DoScriptText(SAY_SUMMON3, m_creature);
        int i;
        float x,y;
        for(i = 0; i < 12; ++i)
        {
            x = (rand_norm() * 30.0f) - 15.0f;
            y = (rand_norm() * 30.0f) - 15.0f;
            if(Creature* pLasher = DoSpawnCreature(NPC_DETONATING_LASHER, x, y, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
            {
                if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pLasher->AddThreat(pTarget, 1.0f);
            }
        }
    }

	// summon conservator. Should be done by a spell which needs core fix
    void SummonConservator()
    {
        DoScriptText(SAY_SUMMON1, m_creature);
        float x = (rand_norm() * 30.0f) - 15.0f;
        float y = (rand_norm() * 30.0f) - 15.0f;
        if(Creature* pAdd = DoSpawnCreature(NPC_ANCIENT_CONSERVATOR, x, y, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pAdd->AddThreat(pTarget, 1.0f);
        }
    }

	// summmon the 3 elementals. Should be done by a spell which needs core fix.
    void SummonElementals()
    {
        DoScriptText(SAY_SUMMON2, m_creature);
        m_bWaveCheck = true;
        m_uiThreeWaveCheckTimer = 2000;

        if(Creature* pSpirit = DoSpawnCreature(NPC_WATER_SPIRIT, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
        {
            m_uiWaterSpiritGUID = pSpirit->GetGUID();
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSpirit->AddThreat(pTarget, 1.0f);
        }

        if(Creature* pStormLasher = DoSpawnCreature(NPC_STORM_LASHER, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
        {
            m_uiStormLasherGUID = pStormLasher->GetGUID();
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pStormLasher->AddThreat(pTarget, 1.0f);
        }

        if(Creature* pSnapLasher = DoSpawnCreature(NPC_SNAPLASHER, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
        {
            m_uiSnapLasherGUID = pSnapLasher->GetGUID();
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSnapLasher->AddThreat(pTarget, 1.0f);
        }
    }

    bool CheckHardMode()
    {
        if(m_bIsBrightleafAlive && m_bIsIronbranchAlive && m_bIsStonebarkAlive)
            return true;
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_bIsOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

			// hacky way of stacking aura. Please remove when fixed in core!
			if(SpellAuraHolder* natureAura = m_creature->GetSpellAuraHolder(SPELL_ATTUNED_TO_NATURE))
			{
				if(natureAura->GetStackAmount() < 150 && !m_bHasAura)
				{
					m_bHasAura = true;
					natureAura->SetStackAmount(150);
				}
			}

			if(!m_creature->HasAura(m_bIsRegularMode ? SPELL_TOUCH_OF_EONAR : SPELL_TOUCH_OF_EONAR_H))
				DoCast(m_creature, m_bIsRegularMode ? SPELL_TOUCH_OF_EONAR : SPELL_TOUCH_OF_EONAR_H);

			// check if the 3 elementals die at the same time
            if(m_uiThreeWaveCheckTimer < uiDiff && m_bWaveCheck)
            {
                Creature* pWaterSpirit = m_pInstance->instance->GetCreature(m_uiWaterSpiritGUID);
                Creature* pStormLasher = m_pInstance->instance->GetCreature(m_uiStormLasherGUID);
                Creature* pSnapLasher = m_pInstance->instance->GetCreature(m_uiSnapLasherGUID);

                if(pWaterSpirit && pStormLasher && pSnapLasher)
                {
                    if(!pWaterSpirit->isAlive() && !pStormLasher->isAlive() && !pSnapLasher->isAlive())
                    {
                        m_bWaveCheck = false;
						if(SpellAuraHolder* natureAura = m_creature->GetSpellAuraHolder(SPELL_ATTUNED_TO_NATURE))
						{
							if(natureAura->ModStackAmount(-30))
								m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
						}
                    }
                    else
                    {
                        // respawn the dead ones
                        if(!pWaterSpirit->isAlive())
                            pWaterSpirit->Respawn();
                        if(!pSnapLasher->isAlive())
                            pSnapLasher->Respawn();
                        if(!pStormLasher->isAlive())
                            pStormLasher->Respawn();
                    }
                }
                m_uiThreeWaveCheckTimer = 2000;
            }
            else
                m_uiThreeWaveCheckTimer -= uiDiff;

            // Hardmode
            if(m_bIsBrightleafAlive)
            {
				if(!m_creature->HasAura(SPELL_BRIGHTLEAFS_ESSENCE, EFFECT_INDEX_0))
					DoCast(m_creature, SPELL_BRIGHTLEAFS_ESSENCE);

				// hacky way, should be done by spell
                if(m_uiUnstableEnergyTimer < uiDiff)
                {
                    //DoCast(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY_FREYA : SPELL_UNSTABLE_ENERGY_FREYA_H);
                    for(int8 i = 0; i < 3; ++i)
                    {
                        if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                        {
                            float x = target->GetPositionX();
                            float y = target->GetPositionY();
                            float z = target->GetPositionZ();
                            m_creature->SummonCreature(NPC_SUN_BEAM, x, y, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);
                        }
                    }
                    m_uiUnstableEnergyTimer = urand(25000, 30000);
                }
                else m_uiUnstableEnergyTimer -= uiDiff;
            }

            if(m_bIsIronbranchAlive)
            {
				if(!m_creature->HasAura(SPELL_IRONBRANCH_ESSENCE, EFFECT_INDEX_0))
					DoCast(m_creature, SPELL_IRONBRANCH_ESSENCE);

                if(m_uiStrenghtenIronRootsTimer < uiDiff)
                {
                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                    {
                        DoScriptText(EMOTE_IRON_ROOTS, m_creature, target);
                        DoCast(target, m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : SPELL_IRON_ROOTS_FREYA_H);
                    }
                    m_uiStrenghtenIronRootsTimer = 50000 + urand(10000, 20000);
                }
                else m_uiStrenghtenIronRootsTimer -= uiDiff;
            }

            if(m_bIsStonebarkAlive)
            {
				// aura doesn't work. Needs core fix
				if(!m_creature->HasAura(SPELL_STONEBARKS_ESSENCE, EFFECT_INDEX_0))
					DoCast(m_creature, SPELL_STONEBARKS_ESSENCE);

                if(m_uiGroundTremorTimer < uiDiff)
                {
                    DoScriptText(EMOTE_GROUND_TREMMOR, m_creature);
                    DoCast(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR_FREYA : SPELL_GROUND_TREMOR_FREYA_H);
                    m_uiGroundTremorTimer = 20000;
                }
                else m_uiGroundTremorTimer -= uiDiff;
            }

            //Phase 1, waves of adds
            if(m_uiWaveNumber < 6)
            {
                if(m_uiSummonTimer < uiDiff)
                {
                    DoScriptText(EMOTE_ALLIES_NATURE, m_creature);
                    switch(m_uiWaveType)
                    {
                    case 0: SummonLashers(); break;
                    case 1: SummonConservator(); break;
                    case 2: SummonElementals(); break;
                    }
                    m_uiWaveType = (m_uiWaveType + m_uiWaveTypeInc) % 3;
                    ++m_uiWaveNumber;
                    m_uiSummonTimer = 60000;
                }
                else m_uiSummonTimer -= uiDiff;
            }
            // Phase 2
            else
            {
                // nature bomb. Should be done by spell, not by summon.
                if(m_uiNatureBombTimer < uiDiff)
                {
                    DoCast(m_creature, SPELL_NATURE_BOMB_VISUAL);
                    DoCast(m_creature, SPELL_NATURE_BOMB_SUMMON);

                    int8 count = urand(8,10);
                    for(int8 i = 0; i < count; ++i)
                    {
                        float radius = 30* rand_norm_f();
                        float angle = 2.0f * M_PI_F * rand_norm_f();
                        float x = m_creature->GetPositionX() + cos(angle) * radius;
                        float y = m_creature->GetPositionY() + sin(angle) * radius;
                        float z = m_creature->GetTerrain()->GetHeight(x, y, MAX_HEIGHT);
                        m_creature->SummonCreature(NPC_NATURE_BOMB, x, y, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);
                    }
                    m_uiNatureBombTimer = urand(7000, 12000);
                }
                else m_uiNatureBombTimer -= uiDiff;
            }

            //All phases
            if(m_uiSunbeamTimer < uiDiff)
            {
                if( Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(target, m_bIsRegularMode ? SPELL_SUNBEAM : SPELL_SUNBEAM_H);
                m_uiSunbeamTimer = 6000 + rand()%10000;
            }
            else m_uiSunbeamTimer -= uiDiff;

            if(m_uiLifebindersGiftTimer < uiDiff)
            {
                DoScriptText(EMOTE_LIFEBINDERS_GIFT, m_creature);
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_LIFEBINDERS_GIFT_SUMMON);
                m_uiLifebindersGiftTimer = 30000;
            }
            else m_uiLifebindersGiftTimer -= uiDiff;

            if(m_uiEnrageTimer < uiDiff)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                DoCast(m_creature, SPELL_BERSERK);
                m_uiEnrageTimer = 30000;
            }
            else m_uiEnrageTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }

        // outro
        if(m_bIsOutro)
        {
            switch(m_uiStep)
            {
            case 1:
                if(m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
                {
                    if(m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->GetStackAmount() >= 25)
                        m_bNature = true;
                }
                m_creature->setFaction(35);
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->GetMotionMaster()->MovePoint(0, 2359.40f, -52.39f, 425.64f);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                ++m_uiStep;
                m_uiOutroTimer = 7000;
                break;
            case 3:
                DoScriptText(SAY_DEATH, m_creature);
                ++m_uiStep;
                m_uiOutroTimer = 10000;
                break;
            case 5:
                DoOutro();
                ++m_uiStep;
                m_uiOutroTimer = 10000;
                break;
            }
        }
        else return;

        if (m_uiOutroTimer <= uiDiff)
        {
            ++m_uiStep;
            m_uiOutroTimer = 330000;
        } m_uiOutroTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_freya(Creature* pCreature)
{
    return new boss_freyaAI(pCreature);
}

// Script for all the npcs found on the ground during Freya encounter
struct MANGOS_DLL_DECL mob_freya_groundAI : public ScriptedAI
{
     mob_freya_groundAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiNatureBomb_Timer;
    uint32 m_uiDieTimer;
    uint32 m_uiEonarsGift_Timer;
    uint32 m_uiNonSelectable_Timer;
    uint32 m_uiGrow_Timer;
    uint32 m_uiSunBeamDespawn_Timer;
    uint32 m_uiUnstableEnergy_Timer;
    uint32 m_uiHealthyGrow_Timer;
    uint64 m_uiNatureBombGUID;
    float m_fSize;

    bool m_bNpcNatureBomb;
    bool m_bNpcEonarsGift;
    bool m_bNpcHealthySpore;
    bool m_bNpcSunBeamFreya;
    bool m_bNpcSunBeamBright;

    bool m_bHasGrow;

    void Reset()
    {
        m_uiNatureBomb_Timer        = urand(9000,11000);
        m_uiDieTimer                = 60000;
        m_uiEonarsGift_Timer        = urand(11000,13000);
        m_uiNonSelectable_Timer     = 5000;
        m_uiUnstableEnergy_Timer    = 1000;
        m_uiGrow_Timer              = 0;
        m_uiSunBeamDespawn_Timer    = urand(10000,11000);
        m_bHasGrow                  = true;
        m_uiHealthyGrow_Timer       = urand(3000,12000);
        m_bNpcNatureBomb            = false;
        m_bNpcEonarsGift            = false;
        m_bNpcHealthySpore          = false;
        m_bNpcSunBeamFreya          = false;
        m_bNpcSunBeamBright         = false;

		// the invisible displayIds should be set in DB.
        switch(m_creature->GetEntry())
        {
            case NPC_NATURE_BOMB:
                m_bNpcNatureBomb = true;
                m_creature->setFaction(14);
                m_fSize = 1;
                m_creature->SetDisplayId(25865);     // invisible
                DoCast(m_creature, SPELL_LIFEBINDERS_VISUAL);
                break;
            case NPC_EONARS_GIFT:
                m_bNpcEonarsGift = true;
                m_fSize = float(0.1);
                DoCast(m_creature, SPELL_LIFEBINDERS_VISUAL);
                break;
            case NPC_HEALTHY_SPORE:
                m_bNpcHealthySpore = true; 
                DoCast(m_creature, SPELL_HEALTHY_SPORE_VISUAL);
                DoCast(m_creature, SPELL_POTENT_PHEROMONES);
                break;
            case NPC_SUN_BEAM:
                m_bNpcSunBeamFreya = true;
                m_creature->SetDisplayId(25865);     // invisible
                DoCast(m_creature, SPELL_LIFEBINDERS_VISUAL);
                DoCast(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY_FREYA : SPELL_UNSTABLE_ENERGY_FREYA_H);
                break;
            case NPC_UNSTABLE_SUN_BEAM:
                m_bNpcSunBeamBright = true; 
                m_creature->SetDisplayId(25865);     // invisible
                DoCast(m_creature, SPELL_LIFEBINDERS_VISUAL);
                //DoCast(m_creature, SPELL_PHOTOSYNTHESIS); // spell needs core fix, should be casted on Brighleaf!
                break;
        }

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetRespawnDelay(DAY);           
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_FREYA) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(!m_creature->isAlive())
            return;

        // NATURE BOMB
        if(m_bNpcNatureBomb)
        {
            if(m_uiNatureBomb_Timer < uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_NATURE_BOMB : SPELL_NATURE_BOMB_H);
                m_uiDieTimer = 500;
                m_uiNatureBomb_Timer = 10000;
            }else m_uiNatureBomb_Timer -= uiDiff;

            if(m_uiDieTimer < uiDiff)
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, NULL, false);
            else m_uiDieTimer -= uiDiff;
        }

        // EONAR GIFT
        if(m_bNpcEonarsGift)
        {
            if (m_uiGrow_Timer > 500 && m_fSize < 1.5)
            {
                m_fSize += float(m_uiGrow_Timer)/8000;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
                m_uiGrow_Timer = 0;
            }else m_uiGrow_Timer += uiDiff;

            if(m_uiEonarsGift_Timer < uiDiff)
            {
                if (Creature* pFreya = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_FREYA)))
                    DoCast(pFreya, m_bIsRegularMode ? SPELL_LIFEBINDERS_GIFT : SPELL_LIFEBINDERS_GIFT_H);
                m_uiEonarsGift_Timer = 1000;
            }else m_uiEonarsGift_Timer -= uiDiff;

            if(m_uiNonSelectable_Timer < uiDiff && m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoCast(m_creature, SPELL_PHEROMONES_LG);
            }else m_uiNonSelectable_Timer -= uiDiff;
        }

        // HEALTHY SPORE
        if(m_bNpcHealthySpore)
        {
            if(!m_bHasGrow && m_fSize < 0.25)
                m_creature->ForcedDespawn();

            if(m_uiHealthyGrow_Timer < uiDiff)
            {
                if(m_bHasGrow)
                {
                    m_fSize = float(urand(150,225))/100;
                    m_bHasGrow = false;
                }
                else
                    m_fSize = float(urand(1,300))/100;
                if(m_fSize < 1)
                    m_fSize = 0.1f;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
                m_uiHealthyGrow_Timer = urand(3000,5000);
            }else m_uiHealthyGrow_Timer -= uiDiff;
        }

        // SUN BEAM
        if(m_bNpcSunBeamBright)
        {
            if(m_uiUnstableEnergy_Timer < uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY : SPELL_UNSTABLE_ENERGY_H);
                m_uiUnstableEnergy_Timer = 1000;
            }else m_uiUnstableEnergy_Timer -= uiDiff;
        }

        if(m_bNpcSunBeamFreya || m_bNpcSunBeamBright)
        {
            if(m_uiSunBeamDespawn_Timer < uiDiff)
                m_creature->ForcedDespawn();
            else m_uiSunBeamDespawn_Timer -= uiDiff;
        }
    }
};

// Script for Freya's adds
struct MANGOS_DLL_DECL mob_freya_spawnedAI : public ScriptedAI
{
    mob_freya_spawnedAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bAncientConservator;
    bool m_bDetonatingLasher;
    bool m_bAncientWaterSpirit;
    bool m_bStormLasher;
    bool m_bSnaplasher;
    bool m_bHasExploded;

    uint32 m_uiDeathCountdown;
    uint32 m_uiTidalWave_Timer;
    uint32 m_uiStormbolt_Timer;
    uint32 m_uiLightningLash_Timer;
    uint32 m_uiFlameLash_Timer;
    uint32 m_uiNaturesFury_Timer;
    uint32 m_uiWave3_DeathCountdown;
    uint32 m_uiRespawnSpores_Timer;
    uint32 m_uiDieTimer;
    uint8 m_uiHealthMultiplier;

    void Reset()
    {
        m_bAncientWaterSpirit       = false;
        m_bStormLasher              = false;
        m_bSnaplasher               = false;
        m_bAncientConservator       = false;
        m_bDetonatingLasher         = false;
        m_bHasExploded              = false;
        m_uiDieTimer                = 120000;
        m_uiDeathCountdown          = 10000;
        m_uiTidalWave_Timer         = urand(2000,4000);
        m_uiStormbolt_Timer         = 1000;
        m_uiLightningLash_Timer     = urand(11000,14000);        
        m_uiFlameLash_Timer         = urand(5000,10000);
        m_uiNaturesFury_Timer       = urand(8000,10000);
        m_uiRespawnSpores_Timer     = 5000;
        m_uiHealthMultiplier        = 1;

        switch(m_creature->GetEntry())
        {
			// The Conservator's Grip needs core fix. It should be canceled by pheronomes!
        case NPC_ANCIENT_CONSERVATOR:
            m_bAncientConservator = true;
            //DoCast(m_creature, SPELL_CONSERVATORS_GRIP); //spell disabled because it isn't negated by pheronomes
            DoSpores(10);
            break;
        case NPC_DETONATING_LASHER:
            m_bDetonatingLasher = true;
            break;
        case NPC_WATER_SPIRIT:
            m_bAncientWaterSpirit = true;
            break;
        case NPC_SNAPLASHER:
            m_bSnaplasher = true;
            DoCast(m_creature, m_bIsRegularMode ? SPELL_HARDENED_BARK : SPELL_HARDENED_BARK_H);
            break;
        case NPC_STORM_LASHER:
            m_bStormLasher = true;
            break;
        }
        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit* Killer)
    {
		// remove some stacks from Freya's aura
		// hacky way. Should be done by spell which needs core support
        if (m_bAncientConservator)
		{
			if (Creature* pFreya = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_FREYA)))
			{
				if(SpellAuraHolder* natureAura = pFreya->GetSpellAuraHolder(SPELL_ATTUNED_TO_NATURE))
				{
					if(natureAura->ModStackAmount(-25))
						m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
				}
			}
		}

        if (m_bDetonatingLasher)
		{
			if (Creature* pFreya = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_FREYA)))
			{
				if(SpellAuraHolder* natureAura = pFreya->GetSpellAuraHolder(SPELL_ATTUNED_TO_NATURE))
				{
					if(natureAura->ModStackAmount(-2))
						m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
				}
			}
		}
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if (m_bDetonatingLasher && uiDamage > m_creature->GetHealth() && !m_bHasExploded)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_DETONATE : SPELL_DETONATE_H);
            uiDamage        = 0;
            m_bHasExploded  = true;
            m_uiDieTimer  = 500;
        }        
    }

    void DoSpores(int8 times)
    {
        for(int8 i = 0; i < times; ++i)
        {
            for(int8 itr = 0; i < 3; ++i)
                DoCast(m_creature, SPELL_SPORE_SUMMON_NE + itr);
            DoCast(m_creature, SPELL_SPORE_SUMMON_NW);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_FREYA) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(!m_creature->isAlive())
            return;

        // DETONATING LASHERS
        if(m_bDetonatingLasher)
        {
            if(m_uiFlameLash_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_FLAME_LASH);
                m_uiFlameLash_Timer = urand(5000,10000);
            }else m_uiFlameLash_Timer -= uiDiff;

            if(m_uiDieTimer < uiDiff)
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, NULL, false);
            else m_uiDieTimer -= uiDiff;
        }

        // CONSERVATOR
        if(m_bAncientConservator)
        {
            if(m_uiNaturesFury_Timer < uiDiff)
            {
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_NATURES_FURY : SPELL_NATURES_FURY_H);
                m_uiNaturesFury_Timer = urand(5000,6000);
            }else m_uiNaturesFury_Timer -= uiDiff;

            if(m_uiRespawnSpores_Timer < uiDiff)
            {
                DoSpores(3);
                m_uiRespawnSpores_Timer = 5000;
            }else m_uiRespawnSpores_Timer -= uiDiff;
        }

        // ELEMENTAL ADDS
        // waterspirit
        if(m_bAncientWaterSpirit && m_uiTidalWave_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_TIDAL_WAVE : SPELL_TIDAL_WAVE_H);
            m_uiTidalWave_Timer = urand(7000,9000);
        }else m_uiTidalWave_Timer -= uiDiff;

        // stormlasher
        if(m_bStormLasher)
        {
            if (m_uiLightningLash_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LIGHTNING_LASH : SPELL_LIGHTNING_LASH_H);
                m_uiLightningLash_Timer = urand(11000,14000);
            }
            else
            {
                m_uiLightningLash_Timer -= uiDiff;
                if (m_uiStormbolt_Timer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_STORMBOLT : SPELL_STORMBOLT_H);
                    m_uiStormbolt_Timer = 2000;
                }else m_uiStormbolt_Timer -= uiDiff;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_freya_ground(Creature* pCreature)
{
    return new mob_freya_groundAI(pCreature);
}

CreatureAI* GetAI_mob_freya_spawned(Creature* pCreature)
{
    return new mob_freya_spawnedAI(pCreature);
}

CreatureAI* GetAI_mob_iron_roots(Creature* pCreature)
{
    return new mob_iron_rootsAI(pCreature);
}

void AddSC_boss_freya()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_freya";
    newscript->GetAI = &GetAI_boss_freya;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elder_brightleaf";
    newscript->GetAI = &GetAI_boss_elder_brightleaf;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elder_ironbranch";
    newscript->GetAI = &GetAI_boss_elder_ironbranch;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elder_stonebark";
    newscript->GetAI = &GetAI_boss_elder_stonebark;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_freya_ground";
    newscript->GetAI = &GetAI_mob_freya_ground;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_freya_spawned";
    newscript->GetAI = &GetAI_mob_freya_spawned;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_roots";
    newscript->GetAI = &GetAI_mob_iron_roots;
    newscript->RegisterSelf();
}
