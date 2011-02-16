/* Copyright (C) 2010 - 2011 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_halion
SD%Complete: 80%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

// TODO: correct timers, Add twilight interorbs connection, sql spells, TESTING
// Attention please! This script required some core modification.

#include "precompiled.h"
#include "ruby_sanctum.h"

enum 
{
    //SPELLS
    //All
    SPELL_TWILIGHT_PRECISION                    = 78243, // Increases Halion's chance to hit by 5% and decreases all players' chance to dodge by 20%
    SPELL_BERSERK                               = 26663, // Increases the caster's attack and movement speeds by 150% and all damage it deals by 500% for 5 min.  Also grants immunity to Taunt effects.
    SPELL_START_PHASE2                          = 74808, // Phases the caster into the Twilight realm, leaving behind a large rift.
    SPELL_TWILIGHT_ENTER                        = 74807, // Phases the caster into the Twilight realm - phase 32
    SPELL_TWILIGHT_ENTER2                       = 74812, //
    SPELL_SUMMON_TWILIGHT_PORTAL                = 74809, //

    //NEED SCRIPT
    SPELL_TAIL_LASH                             = 74531, // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TWILIGHT_DIVISION                     = 75063, // Phases the caster, allowing him to exist and act simultaneously in both the material and Twilight realms.
    SPELL_TWILIGHT_CUTTER                       = 77844, // Inflicts 13,875 to 16,125 Shadow damage every second to players touched by the shadow beam
    //CORPOREALITY
    SPELL_CORPOREALITY_EVEN                     = 74826, // Deals & receives normal damage
    SPELL_CORPOREALITY_20I                      = 74827, // Damage dealt increased by 10% & Damage taken increased by 15%
    SPELL_CORPOREALITY_40I                      = 74828, // Damage dealt increased by 30% & Damage taken increased by 50%
    SPELL_CORPOREALITY_60I                      = 74829, // Damage dealt increased by 60% & Damage taken increased by 100%
    SPELL_CORPOREALITY_80I                      = 74830, // Damage dealt increased by 100% & Damage taken increased by 200%
    SPELL_CORPOREALITY_100I                     = 74831, // Damage dealt increased by 200% & Damage taken increased by 400%
    SPELL_CORPOREALITY_20D                      = 74832, // Damage dealt reduced by 10% & Damage taken reduced by 15%
    SPELL_CORPOREALITY_40D                      = 74833, // Damage dealt reduced by 30% & Damage taken reduced by 50%
    SPELL_CORPOREALITY_60D                      = 74834, // Damage dealt reduced by 60% & Damage taken reduced by 100%
    SPELL_CORPOREALITY_80D                      = 74835, // Damage dealt reduced by 100% & Damage taken reduced by 200%
    SPELL_CORPOREALITY_100D                     = 74836, // Damage dealt reduced by 200% & Damage taken reduced by 400% 
    //METEOR STRIKE
    SPELL_METEOR                                = 74637, // Script Start (summon NPC_METEOR_STRIKE)
    SPELL_METEOR_IMPACT                         = 74641, // IMPACT ZONE FOR METEOR
    SPELL_METEOR_STRIKE                         = 74648, // Inflicts 18,750 to 21,250 Fire damage to enemies within 12 yards of the targeted area. Takes about 5 seconds to land.
    SPELL_METEOR_FLAME                          = 74718, // FLAME FROM METEOR
    //N10
    SPELL_FLAME_BREATH                          = 74525, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_DARK_BREATH                           = 74806, // Inflicts 17,500 to 22,500 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD                           = 75484, // Inflicts 3,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //Combustion
    NPC_COMBUSTION                              = 40001,
    SPELL_MARK_OF_COMBUSTION                    = 74567, // Dummy effect only
    SPELL_FIERY_COMBUSTION                      = 74562, // Inflicts 4,000 Fire damage every 2 seconds for 30 seconds to a random raider. Every time Fiery Combustion does damage, it applies a stackable Mark of Combustion.
    SPELL_COMBUSTION_EXPLODE                    = 74607,
    SPELL_COMBUSTION_AURA                       = 74629,
    //Consumption
    NPC_CONSUMPTION                             = 40135,
    SPELL_MARK_OF_CONSUMPTION                   = 74795, // Dummy effect only
    SPELL_SOUL_CONSUMPTION                      = 74792, // Inflicts 4,000 Shadow damage every 2 seconds for 30 seconds to a random raider. Every time Soul Consumption does damage, it applies a stackable Mark of Consumption.
    SPELL_CONSUMPTION_EXPLODE                   = 74799,
    SPELL_CONSUMPTION_AURA                      = 74803,
    //Summons
    NPC_METEOR_STRIKE                           = 40029, //casts "impact zone" then meteor
    NPC_METEOR_STRIKE_1                         = 40041,
    NPC_METEOR_STRIKE_2                         = 40042,

    FR_RADIUS                                   = 45,

    //SAYS
    SAY_HALION_SPAWN                = -1666100, //17499 Meddlesome insects, you're too late! The Ruby Sanctum is lost.
    SAY_HALION_AGGRO                = -1666101, //17500 Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!
    SAY_HALION_SLAY_1               = -1666102, //17501 Another hero falls.
    SAY_HALION_SLAY_2               = -1666103, //17502 Ha Ha Ha!
    SAY_HALION_DEATH                = -1666104, //17503 Relish this victory mortals, for it will be your last. This world will burn with the Master's return!
    SAY_HALION_BERSERK              = -1666105, //17504 Not good enough!
    SAY_HALION_SPECIAL_1            = -1666106, //17505 The heavens burn!
    SAY_HALION_SPECIAL_2            = -1666107, //17506 Beware the shadow!
    SAY_HALION_PHASE_2              = -1666108, //17507 You will find only suffering within the realm of Twilight. Enter if you dare.
    SAY_HALION_PHASE_3              = -1666109, //17508 I am the light AND the darkness! Cower mortals before the Herald of Deathwing!
    EMOTE_WARNING                   = -1666110, //orbs charge warning
    EMOTE_REAL                      = -1666111, // To real world message
    EMOTE_TWILIGHT                  = -1666112, // To twilight world message
    EMOTE_NEITRAL                   = -1666113, // Halion reveal HP message
};

static Locations SpawnLoc[]=
{
    {3154.99f, 535.637f, 72.8887f},            // 0 - Halion spawn point (center)
};

/*######
## boss_halion_real (Physical version)
######*/
struct MANGOS_DLL_DECL boss_halion_realAI : public BSWScriptedAI
{
    boss_halion_realAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    bool intro;
    uint8 nextPoint;
    bool MovementStarted;

    void Reset()
    {
        if(!pInstance)
            return;
        m_creature->SetRespawnDelay(7*DAY);

        if (m_creature->isAlive()) 
        {
            pInstance->SetData(TYPE_HALION, NOT_STARTED);
            pInstance->SetData(TYPE_HALION_EVENT, FAIL);
        }

        resetTimers();
        setStage(0);
        nextPoint = 0;
        intro = false;
        SetCombatMovement(true);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (GameObject* pGoPortal = pInstance->instance->GetGameObject(pInstance->GetData64(GO_HALION_PORTAL_1)))
               pGoPortal->Delete();
        if (GameObject* pGoPortal = pInstance->instance->GetGameObject(pInstance->GetData64(GO_HALION_PORTAL_2)))
               pGoPortal->Delete();
        if (GameObject* pGoPortal = pInstance->instance->GetGameObject(pInstance->GetData64(GO_HALION_PORTAL_3)))
               pGoPortal->Delete();
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pInstance) return;

        if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER) return;

        if (!intro && pWho->IsWithinDistInMap(m_creature, 60.0f))
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                DoScriptText(-1666100,m_creature);
                intro = true;
                m_creature->SetActiveObjectState(true);
            }

        if (intro && !m_creature->isInCombat() && pWho->IsWithinDistInMap(m_creature, 20.0f))
            AttackStart(pWho);

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustReachedHome()
    {
        if (!pInstance) return;

        if (pInstance->GetData(TYPE_HALION_EVENT) != FAIL) return

        ScriptedAI::JustReachedHome();

//        pInstance->SetData(TYPE_HALION, FAIL);

        m_creature->SetActiveObjectState(false);
    }

    void EnterEvadeMode()
    {

        if (!pInstance) return;

        if (pInstance->GetData(TYPE_HALION_EVENT) != FAIL) return;

        ScriptedAI::EnterEvadeMode();

        m_creature->SetActiveObjectState(false);
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance)
            return;
        m_creature->SetActiveObjectState(false);

        DoScriptText(-1666104,m_creature);

        if (Creature* pclone = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_TWILIGHT)))
        {
            if (!pclone->isAlive())
            {
                pInstance->SetData(TYPE_HALION, DONE);
                m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                pInstance->SetData(TYPE_COUNTER, COUNTER_OFF);
            }
            else
            {
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1))
        {
            case 0:
                DoScriptText(-1631006,m_creature,pVictim);
                break;
            case 1:
                DoScriptText(-1631007,m_creature,pVictim);
                break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance)
            return;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        doCast(SPELL_TWILIGHT_PRECISION);
        m_creature->SetInCombatWithZone();
        pInstance->SetData(TYPE_HALION, IN_PROGRESS);
        DoScriptText(-1666101,m_creature);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (!pInstance) return;

        if (type != POINT_MOTION_TYPE || !MovementStarted) return;

        if (id == nextPoint) {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                }
    }

    void StartMovement(uint32 id)
    {
        nextPoint = id;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(id, SpawnLoc[id].x, SpawnLoc[id].y, SpawnLoc[id].z);
        MovementStarted = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (getStage())
        {
            case 0: //PHASE 1 PHYSICAL REALM
                timedCast(SPELL_FLAME_BREATH, uiDiff);
                timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                timedCast(SPELL_METEOR, uiDiff);
                if (m_creature->GetHealthPercent() < 75.0f) setStage(1);
                break;

            case 1: // Switch to phase 2
                m_creature->AttackStop();
                m_creature->InterruptNonMeleeSpells(true);
                DoScriptText(-1666108,m_creature);
                pInstance->SetData(TYPE_HALION_EVENT, NOT_STARTED);
                SetCombatMovement(false);
                StartMovement(0);
                {
                    Creature* pControl = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_CONTROL));
                    if (!pControl)
                        pControl = m_creature->SummonCreature(NPC_HALION_CONTROL, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 1000);
                    else if (!pControl->isAlive())
                        pControl->Respawn();
                    pControl->SetActiveObjectState(true);
                    pControl->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->SetInCombatWith(pControl);
                    pControl->SetInCombatWith(m_creature);
                }
                setStage(2);
                break;

            case 2:
                if (MovementStarted) return;
                doCast(SPELL_SUMMON_TWILIGHT_PORTAL);
                setStage(3);
                if (GameObject* pGoPortal = pInstance->instance->GetGameObject(pInstance->GetData64(GO_HALION_PORTAL_1)))
                      pGoPortal->SetPhaseMask(31,true);
                if (GameObject* pGoRing = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FLAME_RING)))
                      pGoRing->SetPhaseMask(65535,true);
                break;

            case 3:
                if (m_creature->IsNonMeleeSpellCasted(false)) return;
                m_creature->SetActiveObjectState(true);
                doCast(SPELL_START_PHASE2);
                setStage(4);
                break;

            case 4:
                if (!m_creature->IsNonMeleeSpellCasted(false))
                {
                    if (Creature* pControl = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_CONTROL)))
                    {
                        m_creature->SetInCombatWith(pControl);
                        pControl->SetInCombatWith(m_creature);
                    }
                    Creature* pTwilight = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_TWILIGHT));
                    if (!pTwilight)
                        pTwilight = m_creature->SummonCreature(NPC_HALION_TWILIGHT, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 1000);
                    else if (!pTwilight->isAlive())
                        pTwilight->Respawn();
                    pTwilight->SetCreatorGuid(ObjectGuid());
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    setStage(5);
                }
                break;

            case 5: // HALION awaiting end battle in TWILIGHT REALM
                if (pInstance->GetData(TYPE_HALION_EVENT) == IN_PROGRESS)
                    {
//                        pInstance->SetData(TYPE_HALION_EVENT, SPECIAL);
                        doRemove(SPELL_START_PHASE2);
                        if (Creature* pControl = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_CONTROL)))
                        {
                            m_creature->SetInCombatWith(pControl);
                            pControl->SetInCombatWith(m_creature);
                        }
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_creature->SetHealth(m_creature->GetMaxHealth()/2);
                        m_creature->SetInCombatWithZone();
                        setStage(6);
                    }
                return;

            case 6: // Switch to phase 3
//                doCast(SPELL_TWILIGHT_DIVISION);
                DoScriptText(-1666109,m_creature);
                pInstance->SetData(TYPE_HALION_EVENT, SPECIAL);
                setStage(7);
                break;

            case 7:
                if (m_creature->IsNonMeleeSpellCasted(false)) return;
                if (m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER) return;
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                setStage(8);
                break;

            case 8: //PHASE 3 BOTH REALMS
                timedCast(SPELL_FLAME_BREATH, uiDiff);
                timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                timedCast(SPELL_METEOR, uiDiff);
                break;

            default:
                break;
        }

        timedCast(SPELL_BERSERK, uiDiff);

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_halion_real(Creature* pCreature)
{
    return new boss_halion_realAI(pCreature);
}

/*######
## boss_halion_twilight (Twilight version)
######*/

struct MANGOS_DLL_DECL boss_halion_twilightAI : public BSWScriptedAI
{
    boss_halion_twilightAI(Creature* pCreature) : BSWScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint8 stage;
    bool intro;

    void Reset() 
    {
        if(!pInstance)
            return;
        m_creature->SetRespawnDelay(7*DAY);
        setStage(0);
        intro = false;
        resetTimers();
        m_creature->SetInCombatWithZone();
        if (Creature* pControl = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_CONTROL)))
        {
            m_creature->SetInCombatWith(pControl);
            pControl->SetInCombatWith(m_creature);
        }
        Creature* pFocus = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_ORB_ROTATION_FOCUS));
        if (!pFocus )
             pFocus = m_creature->SummonCreature(NPC_ORB_ROTATION_FOCUS, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 1000);
        else if (!pFocus->isAlive())
             pFocus->Respawn();

        if (Creature* pReal = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_REAL)))
            if (pReal->isAlive())
                m_creature->SetHealth(pReal->GetHealth());
        if (!hasAura(SPELL_TWILIGHT_ENTER))
             doCast(SPELL_TWILIGHT_ENTER);
    }

    void JustReachedHome()
    {
        if (!pInstance) return;

        if (pInstance->GetData(TYPE_HALION_EVENT) != FAIL || getStage() == 0)
            return;

        ScriptedAI::JustReachedHome();
    }

    void EnterEvadeMode()
    {

        if (!pInstance) return;

        if (pInstance->GetData(TYPE_HALION_EVENT) != FAIL || getStage() == 0)
            return;

        ScriptedAI::EnterEvadeMode();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (!pInstance) return;

        if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER) return;

        if (!intro && pWho->IsWithinDistInMap(m_creature, 20.0f))
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                intro = true;
                AttackStart(pWho);
                setStage(1);
                doCast(SPELL_TWILIGHT_PRECISION);
                if (Creature* pReal = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_REAL)))
                    if (pReal->isAlive())
                       m_creature->SetHealth(pReal->GetHealth());


            }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance)
            return;
        DoScriptText(-1666104,m_creature);
        doRemoveFromAll(SPELL_TWILIGHT_ENTER);
        if (Creature* pReal = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_REAL)))
            if (!pReal->isAlive())
            {
                pInstance->SetData(TYPE_HALION, DONE);
                pReal->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                pInstance->SetData(TYPE_COUNTER, COUNTER_OFF);
            }
        m_creature->ForcedDespawn();
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1))
        {
            case 0:
                DoScriptText(-1631006,m_creature,pVictim);
                break;
            case 1:
                DoScriptText(-1631007,m_creature,pVictim);
                break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance)
            return;
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!hasAura(SPELL_TWILIGHT_ENTER))
             doCast(SPELL_TWILIGHT_ENTER);

        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS || pInstance->GetData(TYPE_HALION_EVENT) == FAIL)
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;


        switch (getStage())
        {
            case 1:           //SPAWNED - Twilight realm
//                doCast(SPELL_TWILIGHT_DIVISION);
                timedCast(SPELL_DUSK_SHROUD, uiDiff);
                timedCast(SPELL_DARK_BREATH, uiDiff);
                timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                if (m_creature->GetHealthPercent() < 50.0f) setStage(2);
                break;

            case 2:           //To two realms
                pInstance->SetData(TYPE_HALION_EVENT, IN_PROGRESS);
                DoScriptText(-1666109,m_creature);
                m_creature->SummonGameobject(GO_HALION_PORTAL_3, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, 0);
                if (GameObject* pGoPortal = pInstance->instance->GetGameObject(pInstance->GetData64(GO_HALION_PORTAL_3)))
                      pGoPortal->SetPhaseMask(32,true);
                doCast(SPELL_TWILIGHT_DIVISION);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                setStage(3);
                break;

            case 3: //PHASE 3 BOTH REALMS
                timedCast(SPELL_DUSK_SHROUD, uiDiff);
                timedCast(SPELL_DARK_BREATH, uiDiff);
                timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                break;

            default:
                break;
        }

        timedCast(SPELL_BERSERK, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halion_twilight(Creature* pCreature)
{
    return new boss_halion_twilightAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_meteorAI : public BSWScriptedAI
{
    mob_halion_meteorAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        Reset();
    }

    float direction;

    void Reset()
    {
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false);
        setStage(0);
        m_creature->SetInCombatWithZone();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI(const uint32 diff)
    {
        switch (getStage())
        {
            case 0:
                 if (timedCast(SPELL_METEOR_IMPACT, diff) == CAST_OK) setStage(1);
                 break;

            case 1:
                 if (timedCast(SPELL_METEOR_STRIKE, diff) == CAST_OK) setStage(2);
                 break;

            case 2:
                 // Place summon flames there
                 {
                     direction = 2.0f*M_PI_F*((float)urand(0,15)/16.0f);
                     float x, y, radius;
                     radius = 0.0f;
                     for(uint8 i = 0; i < getSpellData(SPELL_METEOR_STRIKE); ++i)
                     {
                         radius = radius + 5.0f;
                         m_creature->GetNearPoint2D(x, y, radius, direction);
                         doSummon(NPC_METEOR_STRIKE_1, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                         m_creature->GetNearPoint2D(x, y, radius, direction+M_PI_F);
                         doSummon(NPC_METEOR_STRIKE_1, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                     }
                 };
                 {
                     direction = direction + M_PI_F/4;
                     float x, y, radius;
                     radius = 0.0f;
                     for(uint8 i = 0; i < getSpellData(SPELL_METEOR_STRIKE); ++i)
                     {
                         radius = radius + 5.0f;
                         m_creature->GetNearPoint2D(x, y, radius, direction);
                         doSummon(NPC_METEOR_STRIKE_1, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                         m_creature->GetNearPoint2D(x, y, radius, direction+M_PI_F);
                         doSummon(NPC_METEOR_STRIKE_1, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                     }

                 };
                 setStage(3);
                 break;

            case 3:
                 if (timedQuery(SPELL_METEOR_IMPACT, diff)) m_creature->ForcedDespawn();
                 break;

            default:
                 break;
        }

    }
};

CreatureAI* GetAI_mob_halion_meteor(Creature* pCreature)
{
    return new mob_halion_meteorAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_flameAI : public BSWScriptedAI
{
    mob_halion_flameAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        Reset();
    }

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false); 
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!hasAura(SPELL_METEOR_FLAME))
              doCast(SPELL_METEOR_FLAME);

    }

};

CreatureAI* GetAI_mob_halion_flame(Creature* pCreature)
{
    return new mob_halion_flameAI(pCreature);
};

struct HalionBuffLine
{
    float diff;                // Health diff in percent
    uint32 real, twilight;     // Buff pair
    uint8 disp_corp;           // Displayed Corporeality
};

static HalionBuffLine Buff[]=
{
    {-10.0f, SPELL_CORPOREALITY_100D , SPELL_CORPOREALITY_100I , 0   },
    {-8.0f,  SPELL_CORPOREALITY_80D  , SPELL_CORPOREALITY_80I  , 10  },
    {-6.0f,  SPELL_CORPOREALITY_60D  , SPELL_CORPOREALITY_60I  , 20  },
    {-4.0f,  SPELL_CORPOREALITY_40D  , SPELL_CORPOREALITY_40I  , 30  },
    {-2.0f,  SPELL_CORPOREALITY_20D  , SPELL_CORPOREALITY_20I  , 40  },
    {-1.0f,  SPELL_CORPOREALITY_EVEN , SPELL_CORPOREALITY_EVEN , 50  },
    {1.0f,   SPELL_CORPOREALITY_EVEN , SPELL_CORPOREALITY_EVEN , 50  },
    {2.0f,   SPELL_CORPOREALITY_20I  , SPELL_CORPOREALITY_20D  , 60  },
    {4.0f,   SPELL_CORPOREALITY_40I  , SPELL_CORPOREALITY_40D  , 70  },
    {6.0f,   SPELL_CORPOREALITY_60I  , SPELL_CORPOREALITY_60D  , 80  },
    {8.0f,   SPELL_CORPOREALITY_80I  , SPELL_CORPOREALITY_80D  , 90  },
    {10.0f,  SPELL_CORPOREALITY_100I , SPELL_CORPOREALITY_100D , 100 },
};

struct MANGOS_DLL_DECL mob_halion_controlAI : public BSWScriptedAI
{
    mob_halion_controlAI(Creature* pCreature) : BSWScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    Creature* pHalionReal;
    Creature* pHalionTwilight;
    uint32 m_lastBuffReal, m_lastBuffTwilight;
    bool m_detectplayers;

    void Reset()
    {
        if (!pInstance) return;
        resetTimers();
        m_detectplayers = true;
        m_creature->SetDisplayId(11686);
        m_creature->SetPhaseMask(65535, true);
//        m_creature->SetDisplayId(10045);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false);
        m_lastBuffReal = 0;
        m_lastBuffTwilight = 0;
        m_creature->SetActiveObjectState(true);
        pInstance->SetData(TYPE_COUNTER, COUNTER_OFF);
        pInstance->SetData(TYPE_HALION_EVENT, NOT_STARTED);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!pInstance) return;

        if (timedQuery(SPELL_CORPOREALITY_EVEN, diff))
        {
            if (!doSelectRandomPlayerAtRange(80.0f))
            {
                debug_log("ruby_sanctum: cannot detect players in range! ");
                if (!m_detectplayers)
                {
                    pInstance->SetData(TYPE_HALION_EVENT, FAIL);
                    pInstance->SetData(TYPE_HALION, FAIL);
                    m_creature->ForcedDespawn();
                } 
                else 
                {
                    m_detectplayers = false;
                }
            } 
            else 
            {
                m_detectplayers = true;
            }

            if (pInstance->GetData(TYPE_HALION_EVENT) != SPECIAL) return;

            pHalionReal = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_REAL));
            pHalionTwilight = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HALION_TWILIGHT));

            //pHalionReal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pHalionTwilight->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            float p_RealHP = (pHalionReal && pHalionReal->isAlive()) ? pHalionReal->GetHealthPercent() : 0.0f;
            float p_TwilightHP = (pHalionTwilight && pHalionTwilight->isAlive()) ? pHalionTwilight->GetHealthPercent() : 0.0f;

            float m_diff = (p_RealHP - p_TwilightHP);

            uint8 buffnum;
            if (m_diff <= Buff[0].diff)
            {
                buffnum = 0;
            }
            else 
            {
                for (uint8 i = 0; i < 11; i++)
                {
                    if (m_diff >= Buff[i].diff)
                    {
                        buffnum = i+1;
                    }
                    else 
                    {
                        break;
                    }
                }
            }

            if (!m_lastBuffReal || m_lastBuffReal != Buff[buffnum].real)
            {
                if (m_lastBuffReal)
                {
                    doRemove(m_lastBuffReal, pHalionReal);
                }
                doCast(Buff[buffnum].real, pHalionReal);
                m_lastBuffReal = Buff[buffnum].real;
            }

            if (!m_lastBuffTwilight || m_lastBuffTwilight != Buff[buffnum].twilight)
            {
                if (m_lastBuffTwilight) 
                {
                    doRemove(m_lastBuffTwilight, pHalionTwilight);
                }
                doCast(Buff[buffnum].twilight, pHalionTwilight);
                m_lastBuffTwilight = Buff[buffnum].twilight;
            }

            debug_log("ruby_sanctum: Buff num = %u, m_diff = %d ", buffnum, m_diff);

            pInstance->SetData(TYPE_COUNTER, (uint32)Buff[buffnum].disp_corp);

        }

    }

};

CreatureAI* GetAI_mob_halion_control(Creature* pCreature)
{
    return new mob_halion_controlAI(pCreature);
};

struct MANGOS_DLL_DECL mob_orb_rotation_focusAI : public ScriptedAI
{
    mob_orb_rotation_focusAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint32 m_timer;
    float m_direction, m_nextdirection;
    bool m_warning;

    void Reset()
    {
        m_creature->SetDisplayId(11686);
//        m_creature->SetDisplayId(10045);
        m_creature->SetRespawnDelay(7*DAY);
        m_creature->SetPhaseMask(65535, true);
        SetCombatMovement(false); 
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_direction = 0.0f;
        m_nextdirection = 0.0f;
        m_timer = 30000;
        m_warning = false;

        Creature* pPulsar1 = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_SHADOW_PULSAR_N));
        if (!pPulsar1 )
        {
            float x,y;
            m_creature->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
            pPulsar1 = m_creature->SummonCreature(NPC_SHADOW_PULSAR_N, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        } else if (!pPulsar1->isAlive())
                    pPulsar1->Respawn();

        Creature* pPulsar2 = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_SHADOW_PULSAR_S));
        if (!pPulsar2)
        {
            float x,y;
            m_creature->GetNearPoint2D(x, y, FR_RADIUS, m_direction + M_PI_F);
            pPulsar2 = m_creature->SummonCreature(NPC_SHADOW_PULSAR_S, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        } else if (!pPulsar2->isAlive())
                    pPulsar2->Respawn();
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (pInstance->GetData(DATA_ORB_S) == DONE && pInstance->GetData(DATA_ORB_N) == DONE)
        {
            m_direction = m_nextdirection;
            m_nextdirection = (m_direction - M_PI_F/64.0f);
            if (m_nextdirection < 0.0f ) m_nextdirection = m_nextdirection + 2.0f*M_PI_F;
            pInstance->SetData(DATA_ORB_DIRECTION, (uint32)(m_nextdirection*1000));
            pInstance->SetData(DATA_ORB_N, SPECIAL);
            pInstance->SetData(DATA_ORB_S, SPECIAL);
            debug_log("EventMGR: creature %u send direction %u ",m_creature->GetEntry(),pInstance->GetData(DATA_ORB_DIRECTION));
        }

        if (m_timer - 6000 <= uiDiff && !m_warning)
        {
            DoScriptText(-1666110,m_creature);
            m_warning = true;
        }

        if (m_timer <= uiDiff)
        {
            float x,y;
            m_creature->GetNearPoint2D(x, y, FR_RADIUS, m_nextdirection);
            m_creature->SummonCreature(NPC_ORB_CARRIER, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
            m_timer = 30000;
            m_warning = false;
        }   else m_timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_orb_rotation_focus(Creature* pCreature)
{
    return new mob_orb_rotation_focusAI(pCreature);
};

struct MANGOS_DLL_DECL mob_halion_orbAI : public BSWScriptedAI
{
    mob_halion_orbAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
    float m_direction,m_delta;
    uint32 m_flag;
    uint32 m_flag1;
    bool MovementStarted;
    Creature* focus;
    uint32 nextPoint;

    void Reset()
    {
        if (!pInstance) return;
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false);
        m_creature->SetPhaseMask(32, true);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if (m_creature->GetEntry() == NPC_SHADOW_PULSAR_N)
        {
            m_flag = DATA_ORB_N;
            m_delta = 0.0f;
        } else if (m_creature->GetEntry() == NPC_SHADOW_PULSAR_S)
        {
            m_flag = DATA_ORB_S;
            m_delta = M_PI_F;
        };
        m_direction = 0.0f;
        nextPoint = 0;
        MovementStarted = false;
        pInstance->SetData(m_flag, DONE);
        debug_log("EventMGR: creature %u assume m_flag %u ",m_creature->GetEntry(),m_flag);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (!pInstance) return;

        if (type != POINT_MOTION_TYPE || !MovementStarted) return;

        if (id == nextPoint) {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                pInstance->SetData(m_flag, DONE);
                }
    }

    void StartMovement(uint32 id)
    {
        if (!pInstance) return;
        nextPoint = id;
        float x,y;
        pInstance->SetData(m_flag, IN_PROGRESS);
        MovementStarted = true;
        m_direction = ((float)pInstance->GetData(DATA_ORB_DIRECTION)/1000 + m_delta);
        if (m_direction > 2.0f*M_PI_F) m_direction = m_direction - 2.0f*M_PI_F;
        if (focus = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_ORB_ROTATION_FOCUS)))
            focus->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
            else m_creature->ForcedDespawn();
//        debug_log("EventMGR: creature %u go to move point %u ",m_creature->GetEntry(),id);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(id, x, y,  m_creature->GetPositionZ());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (Unit* pTarget = doSelectRandomPlayerAtRange(2.0f))
            doCast(SPELL_TWILIGHT_CUTTER, pTarget);

        if (!MovementStarted && pInstance->GetData(m_flag) == SPECIAL)
        {
//            debug_log("EventMGR: creature %u get direction %u ",m_creature->GetEntry(),pInstance->GetData(DATA_ORB_DIRECTION));
            StartMovement(1);
        }

    }
};

CreatureAI* GetAI_mob_halion_orb(Creature* pCreature)
{
    return new mob_halion_orbAI(pCreature);
}

struct MANGOS_DLL_DECL mob_orb_carrierAI : public BSWScriptedAI
{
    mob_orb_carrierAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;
    bool MovementStarted;

    void Reset()
    {
//        m_creature->SetDisplayId(10045);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false); 
        m_creature->SetPhaseMask(32, true);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        MovementStarted = false;
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->SetSpeedRate(MOVE_RUN, 6.0f);
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (!pInstance) return;

        if (type != POINT_MOTION_TYPE || !MovementStarted) return;

        if (id == 1) {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                m_creature->ForcedDespawn();
                }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS)
              m_creature->ForcedDespawn();

        if (Unit* pTarget = doSelectRandomPlayerAtRange(2.0f))
            doCast(SPELL_TWILIGHT_CUTTER, pTarget);

        if (!MovementStarted)
        {
            float x,y;
            float m_direction = ((float)pInstance->GetData(DATA_ORB_DIRECTION)/1000.0f + M_PI_F - M_PI_F/32.0f);
            if (m_direction > 2.0f*M_PI_F) m_direction = m_direction - 2.0f*M_PI_F;
            if (Creature* focus = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_ORB_ROTATION_FOCUS)))
                focus->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
            else m_creature->ForcedDespawn();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MovePoint(1, x, y,  m_creature->GetPositionZ());
            MovementStarted = true;
        }

    }

};

CreatureAI* GetAI_mob_orb_carrier(Creature* pCreature)
{
    return new mob_orb_carrierAI(pCreature);
};

struct MANGOS_DLL_DECL mob_soul_consumptionAI : public BSWScriptedAI
{
    mob_soul_consumptionAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    float m_Size0;
    float m_Size;

    void Reset()
    {
        if (!isHeroic()) m_creature->SetPhaseMask(32,true);
            else m_creature->SetPhaseMask(65535,true);
        SetCombatMovement(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        doCast(SPELL_CONSUMPTION_AURA);
        m_Size0 = m_creature->GetObjectScale();
        m_Size = m_Size0;
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_HALION) != IN_PROGRESS)
            m_creature->ForcedDespawn();

//        if (!hasAura(SPELL_TWILIGHT_ENTER))
//             doCast(SPELL_TWILIGHT_ENTER);

        if (timedQuery(SPELL_CONSUMPTION_AURA, uiDiff))
            m_creature->ForcedDespawn();

        if (doSelectRandomPlayerAtRange(m_Size * 3.0f) && m_Size <= m_Size0 * 3.0f) {
                m_Size = m_Size*1.01;
                m_creature->SetObjectScale(m_Size);
                }
    }
};

CreatureAI* GetAI_mob_soul_consumption(Creature* pCreature)
{
    return new mob_soul_consumptionAI(pCreature);
};

struct MANGOS_DLL_DECL mob_fiery_combustionAI : public BSWScriptedAI
{
    mob_fiery_combustionAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    float m_Size0;
    float m_Size;

    void Reset()
    {
        if (!isHeroic()) m_creature->SetPhaseMask(31,true);
            else m_creature->SetPhaseMask(65535,true);
        SetCombatMovement(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        doCast(SPELL_COMBUSTION_AURA);
        m_Size0 = m_creature->GetObjectScale();
        m_Size = m_Size0;
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_HALION) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (timedQuery(SPELL_COMBUSTION_AURA, uiDiff))
            m_creature->ForcedDespawn();

        if (doSelectRandomPlayerAtRange(m_Size * 3.0f) && m_Size <= m_Size0 * 3.0f) {
                m_Size = m_Size*1.01;
                m_creature->SetObjectScale(m_Size);
                }
    }

};

CreatureAI* GetAI_mob_fiery_combustion(Creature* pCreature)
{
    return new mob_fiery_combustionAI(pCreature);
};


bool GOUse_go_halion_portal_twilight(Player *player, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if(!pInstance) return false;
    player->CastSpell(player,SPELL_TWILIGHT_ENTER,false);
    return true;
}

bool GOUse_go_halion_portal_real(Player *player, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if(!pInstance) return false;
    player->RemoveAurasDueToSpell(SPELL_TWILIGHT_ENTER);
    return true;
}

void AddSC_boss_halion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_halion_real";
    newscript->GetAI = &GetAI_boss_halion_real;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_halion_twilight";
    newscript->GetAI = &GetAI_boss_halion_twilight;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_meteor";
    newscript->GetAI = &GetAI_mob_halion_meteor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_flame";
    newscript->GetAI = &GetAI_mob_halion_flame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_orb";
    newscript->GetAI = &GetAI_mob_halion_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_control";
    newscript->GetAI = &GetAI_mob_halion_control;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_orb_rotation_focus";
    newscript->GetAI = &GetAI_mob_orb_rotation_focus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_orb_carrier";
    newscript->GetAI = &GetAI_mob_orb_carrier;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_soul_consumption";
    newscript->GetAI = &GetAI_mob_soul_consumption;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_fiery_combustion";
    newscript->GetAI = &GetAI_mob_fiery_combustion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_halion_portal_twilight";
    newscript->pGOUse  = &GOUse_go_halion_portal_twilight;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_halion_portal_real";
    newscript->pGOUse  = &GOUse_go_halion_portal_real;
    newscript->RegisterSelf();
}
