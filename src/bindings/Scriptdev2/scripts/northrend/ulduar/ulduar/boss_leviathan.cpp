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
SDName: boss_leviathan
SD%Complete: 
SDComment: needs vehicles
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"
#include "Vehicle.h"

enum say
{
    SAY_AGGRO           = -1603203,
    SAY_DEATH           = -1603202,
    SAY_SLAY            = -1603201,
    SAY_CHANGE1         = -1603204,
    SAY_CHANGE2         = -1603205,
    SAY_CHANGE3         = -1603206,
    SAY_PLAYER_ON_TOP   = -1603207,
    SAY_OVERLOAD1       = -1603208,
    SAY_OVERLOAD2       = -1603209,
    SAY_OVERLOAD3       = -1603210,
    SAY_HARD_MODE       = -1603211,
    SAY_TOWERS_DOWN     = -1603212,
    SAY_FROST_TOWER     = -1603213,
    SAY_FIRE_TOWER      = -1603214,
    SAY_ENERGY_TOWER    = -1603215,
    SAY_NATURE_TOWER    = -1603216,

    EMOTE_PURSUE        = -1603352,
};

enum spells
{
    SPELL_PURSUED           = 62374,

    SPELL_MISSILE_BARRAGE   = 62400,
    SPELL_FLAME_VENTS       = 62396,
    SPELL_BATTERING_RAM     = 62376,

    SPELL_GATHERING_SPEED   = 62375,
    // interupted by
    SPELL_OVERLOAD_CIRCUIT  = 62399,

    SPELL_SEARING_FLAME     = 62402, // used by defense turret
    // interupted by
    SPELL_SYSTEMS_SHUTDOWN  = 62475,

    SPELL_FLAME_CANNON      = 62395,
    //SPELL_FLAME_CANNON    = 64692, trigger the same spell
    SPELL_BLAZE             = 62292,

    // used by players -> to be added later
    SPELL_ELECTROSHOCK      = 62522,
    SPELL_SMOKE_TRAIL       = 63575,

    // tower of nature
    SPELL_FREYAS_WARD       = 62906,
    SPELL_TOWER_OF_LIFE     = 64482,
    // tower of flames
    SPELL_MIMIRON_INFERNO   = 62910,
    SPELL_TOWER_OF_FLAMES   = 65075,
    // tower of frost
    SPELL_HODIR_FURY        = 62297, // also + 10% hp
    // tower of storms
    SPELL_THORIMS_HAMMER    = 62912,
    SPELL_TOWER_OF_STORMS   = 65076
};

enum Mobs
{
    MOB_MECHANOLIFT     = 33214,
    MOB_LIQUID          = 33189,
    MOB_CONTAINER       = 33218,

    DEFENSE_TURRET      = 33142,
    KEEPER_OF_NORGANNON = 33686
};

enum Seats
{
    SEAT_PLAYER = 0,
    SEAT_TURRET = 1,
    SEAT_DEVICE = 2,
};


struct MANGOS_DLL_DECL boss_flame_leviathan : public ScriptedAI
{
    boss_flame_leviathan(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        // ### unit disabled, please remove if you want to test it!
//        pCreature->setFaction(35);  // remove this when vehicules fixed!
//        pCreature->SetVisibility(VISIBILITY_OFF);
        // ###
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBatteringRamTimer;
    uint32 m_uiFlameVentsTimer;
    uint32 m_uiMissileBarrageTimer;
    uint32 m_uiPursueTimer;
    uint32 m_uiGatheringSpeedTimer;
    uint32 m_uiSummonFlyerTimer;
    uint8 maxFlyers;

    bool isHardMode;
    bool isHodirsTower;
    bool isFreyasTower;
    bool isMimironsTower;
    bool isThorimsTower;

    uint32 m_uiFreyaWardTimer;
    uint32 m_uiMimironInfernoTimer;
    uint32 m_uiHodirFuryTimer;
    uint32 m_uiThorimHammerTimer;

    void Reset()
    {
        m_uiBatteringRamTimer   = 15000 + rand()%20000;
        m_uiFlameVentsTimer     = 15000 + rand()%10000;
        m_uiMissileBarrageTimer = 1000;
        m_uiPursueTimer         = 30000;
        m_uiGatheringSpeedTimer = 50000;
        m_uiSummonFlyerTimer    = 2000;
        maxFlyers = 10;

        isHardMode      = false;
        isHodirsTower   = false;
        isFreyasTower   = false;
        isMimironsTower = false;
        isThorimsTower  = false;

        m_uiFreyaWardTimer      = 40000 + urand(1000, 10000);
        m_uiMimironInfernoTimer = 40000 + urand(1000, 10000);
        m_uiHodirFuryTimer      = 40000 + urand(1000, 10000);
        m_uiThorimHammerTimer   = 40000 + urand(1000, 10000);

        m_creature->SetSpeedRate(MOVE_RUN, 0.3f);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_LEVIATHAN, IN_PROGRESS);
            if(m_pInstance->GetData(TYPE_LEVIATHAN_TP) != DONE)
                m_pInstance->SetData(TYPE_LEVIATHAN_TP, DONE);
        }

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_LEVIATHAN, DONE);
            if(isHardMode)
                m_pInstance->SetData(TYPE_LEVIATHAN_HARD, DONE);
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, FAIL);
    }

    void KilledUnit(Unit *who)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    // TODO: effect 0 and effect 1 may be on different target
    void SpellHitTarget(Unit *pTarget, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_PURSUED)
            AttackStart(pTarget);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        /*if(spell->Id == 62472)
        vehicle->InstallAllAccessories();
        else if(spell->Id == SPELL_ELECTROSHOCK)
        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);*/
    }

    void DamageTaken(Unit *pDoneBy, uint32 &uiDamage)
    {
        uiDamage *= 4;
        if(m_creature->HasAura(SPELL_SYSTEMS_SHUTDOWN, EFFECT_INDEX_0))
            uiDamage += uiDamage/2;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // pursue
        if(m_uiPursueTimer < uiDiff)
        {
            switch(urand(0, 3))
            {
                case 0: DoScriptText(SAY_CHANGE1, m_creature); break;
                case 1: DoScriptText(SAY_CHANGE2, m_creature); break;
                case 2: DoScriptText(SAY_CHANGE3, m_creature); break;
            }
            DoScriptText(EMOTE_PURSUE, m_creature);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                m_creature->AddThreat(pTarget, 100.0f);
                DoCast(pTarget, SPELL_PURSUED);
            }

            m_uiPursueTimer = 30000;
        }
        else m_uiPursueTimer -= uiDiff;

        // flame vents
        if(m_uiFlameVentsTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAME_VENTS);
            m_uiFlameVentsTimer = 30000 + rand()%20000;
        }
        else m_uiFlameVentsTimer -= uiDiff;

        // battering ram
        if(m_uiBatteringRamTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_BATTERING_RAM);
            m_uiBatteringRamTimer = 25000 + rand()%15000;
        }
        else m_uiBatteringRamTimer -= uiDiff;

        /* flyers
        it should summon some flyers. needs more research!
        if(m_uiSummonFlyerTimer < uiDiff)
        {
            m_creature->SummonCreature(MOB_MECHANOLIFT, m_creature->GetPositionX() + rand()%20, m_creature->GetPositionY() + rand()%20, m_creature->GetPositionZ() + 50, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 40000);
            m_uiSummonFlyerTimer = 2000;
        }
        else m_uiSummonFlyerTimer -= uiDiff;*/

        // missile barrage
        if(m_uiMissileBarrageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_MISSILE_BARRAGE);
            m_uiMissileBarrageTimer = 3000 + rand()%2000;
        }
        else m_uiMissileBarrageTimer -= uiDiff;

        if(m_uiGatheringSpeedTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_GATHERING_SPEED);
            m_uiGatheringSpeedTimer = urand(50000, 60000);
        }
        else m_uiGatheringSpeedTimer -= uiDiff;

        // Hard mode event. need more research and scripting
        // this part should be done in other way

        // tower of freya
        if(isFreyasTower)
        {
            DoCast(m_creature, SPELL_TOWER_OF_LIFE);

            if(m_uiFreyaWardTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_FREYAS_WARD);
                m_uiFreyaWardTimer = 40000 + urand(1000, 10000);
            }
            else m_uiFreyaWardTimer -= uiDiff;
        }

        // tower of mimiron
        if(isMimironsTower)
        {
            DoCast(m_creature, SPELL_TOWER_OF_FLAMES);

            if(m_uiMimironInfernoTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_FREYAS_WARD);
                m_uiMimironInfernoTimer = 40000 + urand(1000, 10000);
            }
            else m_uiMimironInfernoTimer -= uiDiff;
        }

        // tower of hodir
        if(isHodirsTower)
        {
            m_creature->SetHealth(m_creature->GetHealth() + 0.1* m_creature->GetHealth());

            if(m_uiHodirFuryTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_HODIR_FURY);
                m_uiHodirFuryTimer = 40000 + urand(1000, 10000);
            }
            else m_uiHodirFuryTimer -= uiDiff;
        }

        // tower of thorim
        if(isThorimsTower)
        {
            DoCast(m_creature, SPELL_TOWER_OF_STORMS);

            if(m_uiThorimHammerTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_THORIMS_HAMMER);
                m_uiThorimHammerTimer = 40000 + urand(1000, 10000);
            }
            else m_uiThorimHammerTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_defense_turretAI : public ScriptedAI
{
    mob_defense_turretAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSpell_Timer;

    void Reset()
    {
        m_uiSpell_Timer = urand(10000, 15000);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_SYSTEMS_SHUTDOWN)
            m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpell_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_SEARING_FLAME);
            m_uiSpell_Timer = urand(10000, 15000);
        }else m_uiSpell_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_defense_turret(Creature* pCreature)
{
    return new mob_defense_turretAI(pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan(Creature* pCreature)
{
    return new boss_flame_leviathan(pCreature);
}

void AddSC_boss_leviathan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_flame_leviathan";
    newscript->GetAI = &GetAI_boss_flame_leviathan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_defense_turret";
    newscript->GetAI = &GetAI_mob_defense_turret;
    newscript->RegisterSelf();
}
