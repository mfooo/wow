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
SDName: Boss_Felblood_Kaelthas
SD%Complete: 80
SDComment: Normal and Heroic Support. Issues: Arcane Spheres do not initially follow targets.
SDCategory: Magisters' Terrace
EndScriptData */

#include "precompiled.h"
#include "magisters_terrace.h"
#include "WorldPacket.h"

#define SAY_AGGRO                   -1585023                //This yell should be done when the room is cleared. For now, set it as a movelineofsight yell.
#define SAY_PHOENIX                 -1585024
#define SAY_FLAMESTRIKE             -1585025
#define SAY_GRAVITY_LAPSE           -1585026
#define SAY_TIRED                   -1585027
#define SAY_RECAST_GRAVITY          -1585028
#define SAY_DEATH                   -1585029

/*** Spells ***/

// Phase 1 spells
#define SPELL_FIREBALL_NORMAL         44189                 // Deals 2700-3300 damage at current target
#define SPELL_FIREBALL_HEROIC         46164                 //       4950-6050

#define SPELL_PHOENIX                 44194                 // Summons a phoenix (Doesn't work?)
#define SPELL_PHOENIX_BURN            44197                 // A spell Phoenix uses to damage everything around
#define SPELL_REBIRTH_DMG             44196                 // DMG if a Phoenix rebirth happen

#define SPELL_FLAME_STRIKE_DUMMY      44191                 // Flamestrike indicator before the damage
#define SPELL_FLAME_STRIKE            44192                 // Summons the trigger + animation (projectile)

#define SPELL_SHOCK_BARRIER           46165                 // Heroic only; 10k damage shield, followed by Pyroblast
#define SPELL_PYROBLAST               36819                 // Heroic only; 45-55k fire damage

// Phase 2 spells
#define SPELL_GRAVITY_LAPSE_INITIAL   44224                 // Cast at the beginning of every Gravity Lapse
#define SPELL_GRAVITY_LAPSE_CHANNEL   44251                 // Channeled; blue beam animation to every enemy in range
#define SPELL_TELEPORT_CENTER         44218                 // Should teleport people to the center. Requires DB entry in spell_target_position.
#define SPELL_GRAVITY_LAPSE_FLY       44227                 // Hastens flyspeed and allows flying for 1 minute. For some reason removes 44226.
#define SPELL_GRAVITY_LAPSE_DOT       44226                 // Knocks up in the air and applies a 300 DPS DoT.
#define SPELL_ARCANE_SPHERE_PASSIVE   44263                 // Passive auras on Arcane Spheres
#define SPELL_POWER_FEEDBACK          44233                 // Stuns him, making him take 50% more damage for 10 seconds. Cast after Gravity Lapse

/*** Creatures ***/
#define NPC_FLAME_STRIKE_TRIGGER      24666
#define CREATURE_PHOENIX              24674
#define CREATURE_PHOENIX_EGG          24675
#define CREATURE_ARCANE_SPHERE        24708

/** Locations **/
float KaelLocations[3][2]=
{
    {148.744659f, 181.377426f},
    {140.823883f, 195.403046f},
    {156.574188f, 195.650482f},
};

#define LOCATION_Z                  -16.727455f

struct MANGOS_DLL_DECL boss_felblood_kaelthasAI : public ScriptedAI
{
    boss_felblood_kaelthasAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 FireballTimer;
    uint32 PhoenixTimer;
    uint32 FlameStrikeTimer;
    uint32 CombatPulseTimer;

    //Heroic only
    uint32 PyroblastTimer;

    uint32 GravityLapseTimer;
    uint32 GravityLapsePhase;
    // 0 = No Gravity Lapse
    // 1 = Casting Gravity Lapse visual
    // 2 = Teleported people to self
    // 3 = Knocked people up in the air
    // 4 = Applied an aura that allows them to fly, channeling visual, relased Arcane Orbs.

    bool FirstGravityLapse;
    bool HasTaunted;

    uint8 Phase;
    // 0 = Not started
    // 1 = Fireball; Summon Phoenix; Flamestrike
    // 2 = Gravity Lapses

    void Reset()
    {
        // TODO: Timers
        FireballTimer = 0;
        PhoenixTimer = 10000;
        FlameStrikeTimer = 25000;
        CombatPulseTimer = 0;

        PyroblastTimer = 60000;

        GravityLapseTimer = 0;
        GravityLapsePhase = 0;

        FirstGravityLapse = true;
        HasTaunted = false;

        Phase = 0;

        if (m_pInstance)
        {
            m_pInstance->SetData(DATA_KAELTHAS_EVENT, NOT_STARTED);

            if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_KAEL_DOOR)))
                pDoor->SetGoState(GO_STATE_ACTIVE);         // Open the big encounter door. Close it in Aggro and open it only in JustDied(and here)
                                                            // Small door opened after event are expected to be closed by default
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        if (GameObject* pEncounterDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_KAEL_DOOR)))
            pEncounterDoor->SetGoState(GO_STATE_ACTIVE);    // Open the encounter door
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth())
            RemoveGravityLapse();                           // Remove Gravity Lapse so that players fall to ground if they kill him when in air.
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance)
            return;

        if (GameObject* pEncounterDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_KAEL_DOOR)))
            pEncounterDoor->SetGoState(GO_STATE_READY);     //Close the encounter door, open it in JustDied/Reset
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!HasTaunted && m_creature->IsWithinDistInMap(who, 40.0))
        {
            DoScriptText(SAY_AGGRO, m_creature);
            HasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(who);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_FLAME_STRIKE_TRIGGER)
            pSummoned->CastSpell(pSummoned, SPELL_FLAME_STRIKE_DUMMY, false, NULL, NULL, m_creature->GetGUID());
    }

    void SetThreatList(Creature* SummonedUnit)
    {
        if (!SummonedUnit)
            return;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());
            if (pUnit && pUnit->isAlive())
            {
                float threat = m_creature->getThreatManager().getThreat(pUnit);
                SummonedUnit->AddThreat(pUnit, threat);
            }
        }
    }

    void TeleportPlayersToSelf()
    {
        float x = KaelLocations[0][0];
        float y = KaelLocations[0][1];

        DoCastSpellIfCan(m_creature, SPELL_TELEPORT_CENTER, CAST_TRIGGERED);

        std::vector<ObjectGuid> vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        for (std::vector<ObjectGuid>::const_iterator i = vGuids.begin();i != vGuids.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit(*i);

            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->CastSpell(pUnit, SPELL_TELEPORT_CENTER, true);
        }
    }

    void CastGravityLapseKnockUp()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());

            // Knockback into the air
            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE_DOT, true, 0, 0, m_creature->GetGUID());
        }
    }

    // players can't cast "fly" spells unless in map 530. Has to be done a while after they get knocked into the air...
    void CastGravityLapseFly()
    {
        std::vector<ObjectGuid> vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        for (std::vector<ObjectGuid>::const_iterator i = vGuids.begin();i != vGuids.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit(*i);

            // Also needs an exception in spell system.
            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE_FLY, true, 0, 0, m_creature->GetGUID());
        }
    }

    void RemoveGravityLapse()
    {
        std::vector<ObjectGuid> vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        for (std::vector<ObjectGuid>::const_iterator i = vGuids.begin();i != vGuids.end(); ++i)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit(*i);

            if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
            {
                pUnit->RemoveAurasDueToSpell(SPELL_GRAVITY_LAPSE_FLY);
                pUnit->RemoveAurasDueToSpell(SPELL_GRAVITY_LAPSE_DOT);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(Phase)
        {
            case 0:
            {
                // *Heroic mode only:
                if (!m_bIsRegularMode)
                {
                    if (PyroblastTimer < diff)
                    {
                        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
                        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                        DoCastSpellIfCan(m_creature, SPELL_SHOCK_BARRIER, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature->getVictim(), SPELL_PYROBLAST);
                        PyroblastTimer = 60000;
                    }else PyroblastTimer -= diff;
                }

                if (FireballTimer < diff)
                {
                    DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FIREBALL_NORMAL : SPELL_FIREBALL_HEROIC);
                    FireballTimer = urand(2000, 6000);
                }else FireballTimer -= diff;

                if (PhoenixTimer < diff)
                {

                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                    {
                        uint32 random = urand(1, 2);
                        float x = KaelLocations[random][0];
                        float y = KaelLocations[random][1];

                        if (Creature* Phoenix = m_creature->SummonCreature(CREATURE_PHOENIX, x, y, LOCATION_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000))
                        {
                            Phoenix->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_NON_ATTACKABLE);
                            SetThreatList(Phoenix);
                            Phoenix->AI()->AttackStart(pTarget);
                            DoScriptText(SAY_PHOENIX, m_creature);
                        }
                    }

                    PhoenixTimer = 60000;
                }else PhoenixTimer -= diff;

                if (FlameStrikeTimer < diff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (m_creature->IsNonMeleeSpellCasted(false))
                            m_creature->InterruptNonMeleeSpells(false);

                        DoCastSpellIfCan(pTarget, SPELL_FLAME_STRIKE);
                        DoScriptText(SAY_FLAMESTRIKE, m_creature);
                    }
                    FlameStrikeTimer = urand(15000, 25000);
                }else FlameStrikeTimer -= diff;

                // Below 50%
                if (m_creature->GetHealthPercent() < 50.0f)
                {
                    m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                    m_creature->StopMoving();
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveIdle();
                    GravityLapseTimer = 0;
                    GravityLapsePhase = 0;
                    Phase = 1;
                }

                DoMeleeAttackIfReady();
            }
            break;

            case 1:
            {
                if (GravityLapseTimer < diff)
                {
                    switch(GravityLapsePhase)
                    {
                        case 0:
                            if (FirstGravityLapse)          // Different yells at 50%, and at every following Gravity Lapse
                            {
                                DoScriptText(SAY_GRAVITY_LAPSE, m_creature);
                                FirstGravityLapse = false;

                                if (m_pInstance)
                                {
                                    if (GameObject* pKaelLeft = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_KAEL_STATUE_LEFT)))
                                        pKaelLeft->SetGoState(GO_STATE_ACTIVE);

                                    if (GameObject* pKaelRight = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_KAEL_STATUE_RIGHT)))
                                        pKaelRight->SetGoState(GO_STATE_ACTIVE);
                                }
                            }
                            else
                            {
                                DoScriptText(SAY_RECAST_GRAVITY, m_creature);
                            }

                            DoCastSpellIfCan(m_creature, SPELL_GRAVITY_LAPSE_INITIAL);
                            GravityLapseTimer = 2000 + diff;// Don't interrupt the visual spell
                            GravityLapsePhase = 1;
                            break;

                        case 1:
                            TeleportPlayersToSelf();
                            GravityLapseTimer = 1000;
                            GravityLapsePhase = 2;
                            break;

                        case 2:
                            CastGravityLapseKnockUp();
                            GravityLapseTimer = 1000;
                            GravityLapsePhase = 3;
                            break;

                        case 3:
                            CastGravityLapseFly();
                            GravityLapseTimer = 30000;
                            GravityLapsePhase = 4;


                            for(uint8 i = 0; i < 3; ++i)
                            {
                                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                                {
                                    if (Creature* Orb = DoSpawnCreature(CREATURE_ARCANE_SPHERE, 5, 5, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                                        Orb->AI()->AttackStart(pTarget);
                                }
                            }

                            DoCastSpellIfCan(m_creature, SPELL_GRAVITY_LAPSE_CHANNEL);
                            break;

                        case 4:
                            m_creature->InterruptNonMeleeSpells(false);
                            DoScriptText(SAY_TIRED, m_creature);
                            DoCastSpellIfCan(m_creature, SPELL_POWER_FEEDBACK);
                            RemoveGravityLapse();
                            GravityLapseTimer = 10000;
                            GravityLapsePhase = 0;
                            break;
                    }
                }else GravityLapseTimer -= diff;
            }
            break;
        }
    }
};

struct MANGOS_DLL_DECL mob_felkael_phoenixAI : public ScriptedAI
{
    mob_felkael_phoenixAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 BurnTimer;
    uint32 Death_Timer;
    bool Rebirth;
    bool FakeDeath;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_NON_ATTACKABLE);
        m_creature->CastSpell(m_creature,SPELL_PHOENIX_BURN,true);

        BurnTimer = 2000;
        Death_Timer = 3000;
        Rebirth = false;
        FakeDeath = false;
    }

    void DamageTaken(Unit* pKiller, uint32 &damage)
    {
        if (damage < m_creature->GetHealth())
            return;

        //Prevent glitch if in fake death
        if (FakeDeath)
        {
            damage = 0;
            return;

        }
        //Don't really die in all phases of Kael'Thas
        if (m_pInstance && m_pInstance->GetData(DATA_KAELTHAS_EVENT) == 0)
        {
            //prevent death
            damage = 0;
            FakeDeath = true;

            m_creature->InterruptNonMeleeSpells(false);
            m_creature->SetHealth(0);
            m_creature->StopMoving();
            m_creature->ClearComboPointHolders();
            m_creature->RemoveAllAurasOnDeath();
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->ClearAllReactives();
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET,0);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

       }

    }

    void JustDied(Unit* slayer)
    {
        m_creature->SummonCreature(CREATURE_PHOENIX_EGG, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
    }

    void UpdateAI(const uint32 diff)
    {

        //If we are fake death, we cast revbirth and after that we kill the phoenix to spawn the egg.
        if (FakeDeath)
        {
            if (!Rebirth)
            {
                DoCastSpellIfCan(m_creature, SPELL_REBIRTH_DMG);
                Rebirth = true;
            }

            if (Rebirth)
            {

                if (Death_Timer < diff)
                {
                    m_creature->SummonCreature(CREATURE_PHOENIX_EGG, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
                    m_creature->SetDeathState(JUST_DIED);
                    m_creature->RemoveCorpse();
                    Rebirth = false;
                }else Death_Timer -= diff;
            }


        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (BurnTimer < diff)
        {
            //spell Burn should possible do this, but it doesn't, so do this for now.
            uint32 dmg = urand(1650,2050);
            m_creature->DealDamage(m_creature, dmg, 0, DOT, SPELL_SCHOOL_MASK_FIRE, NULL, false);
            BurnTimer += 2000;
        } BurnTimer -= diff;


        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_felkael_phoenix_eggAI : public ScriptedAI
{
    mob_felkael_phoenix_eggAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 HatchTimer;

    void Reset()
    {
        HatchTimer = 10000;

    }

    void MoveInLineOfSight(Unit* who) {}

    void UpdateAI(const uint32 diff)
    {
        if (HatchTimer < diff)
        {
            m_creature->SummonCreature(CREATURE_PHOENIX, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else HatchTimer -= diff;

    }
};

struct MANGOS_DLL_DECL mob_arcane_sphereAI : public ScriptedAI
{
    mob_arcane_sphereAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 DespawnTimer;
    uint32 ChangeTargetTimer;

    void Reset()
    {
        DespawnTimer = 30000;
        ChangeTargetTimer = urand(6000, 12000);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoCastSpellIfCan(m_creature, SPELL_ARCANE_SPHERE_PASSIVE, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 diff)
    {
        if (DespawnTimer < diff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else DespawnTimer -= diff;

        if (!m_creature->getVictim() || !m_creature->SelectHostileTarget())
            return;

        if (ChangeTargetTimer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                m_creature->TauntApply(pTarget);
                AttackStart(pTarget);
            }

            ChangeTargetTimer = urand(5000, 15000);
        }else ChangeTargetTimer -= diff;
    }
};

CreatureAI* GetAI_boss_felblood_kaelthas(Creature* pCreature)
{
    return new boss_felblood_kaelthasAI(pCreature);
}

CreatureAI* GetAI_mob_arcane_sphere(Creature* pCreature)
{
    return new mob_arcane_sphereAI(pCreature);
}

CreatureAI* GetAI_mob_felkael_phoenix(Creature* pCreature)
{
    return new mob_felkael_phoenixAI(pCreature);
}

CreatureAI* GetAI_mob_felkael_phoenix_egg(Creature* pCreature)
{
    return new mob_felkael_phoenix_eggAI(pCreature);
}

void AddSC_boss_felblood_kaelthas()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_felblood_kaelthas";
    newscript->GetAI = &GetAI_boss_felblood_kaelthas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_arcane_sphere";
    newscript->GetAI = &GetAI_mob_arcane_sphere;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_felkael_phoenix";
    newscript->GetAI = &GetAI_mob_felkael_phoenix;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_felkael_phoenix_egg";
    newscript->GetAI = &GetAI_mob_felkael_phoenix_egg;
    newscript->RegisterSelf();
}
