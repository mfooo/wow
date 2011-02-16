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
SDName: Boss_Selin_Fireheart
SD%Complete: 90
SDComment: Heroic and Normal Support. Needs further testing.
SDCategory: Magister's Terrace
EndScriptData */

#include "precompiled.h"
#include "magisters_terrace.h"

#define SAY_AGGRO                       -1585000
#define SAY_ENERGY                      -1585001
#define SAY_EMPOWERED                   -1585002
#define SAY_KILL_1                      -1585003
#define SAY_KILL_2                      -1585004
#define SAY_DEATH                       -1585005
#define EMOTE_CRYSTAL                   -1585006

//Crystal effect spells
#define SPELL_FEL_CRYSTAL_COSMETIC      44374
#define SPELL_FEL_CRYSTAL_DUMMY         44329
#define SPELL_FEL_CRYSTAL_VISUAL        44355
#define SPELL_MANA_RAGE                 44320               // This spell triggers 44321, which changes scale and regens mana Requires an entry in spell_script_target

//Selin's spells
#define SPELL_DRAIN_LIFE                44294
#define SPELL_FEL_EXPLOSION             44314

#define SPELL_DRAIN_MANA                46153               // Heroic only

#define CRYSTALS_NUMBER                 5
#define DATA_CRYSTALS                   6

#define CREATURE_FEL_CRYSTAL            24722

struct MANGOS_DLL_DECL boss_selin_fireheartAI : public ScriptedAI
{
    boss_selin_fireheartAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        Crystals.clear();
        //GUIDs per instance is static, so we only need to load them once.
        if (m_pInstance)
        {
            uint32 size = m_pInstance->GetData(DATA_FEL_CRYSTAL_SIZE);
            for(uint8 i = 0; i < size; ++i)
            {
                uint64 guid = m_pInstance->GetData64(DATA_FEL_CRYSTAL);
                debug_log("SD2: Selin: Adding Fel Crystal " UI64FMTD " to list", guid);
                Crystals.push_back(guid);
            }
        }
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> Crystals;

    uint32 DrainLifeTimer;
    uint32 DrainManaTimer;
    uint32 FelExplosionTimer;
    uint32 DrainCrystalTimer;
    uint32 EmpowerTimer;

    bool IsDraining;
    bool DrainingCrystal;

    uint64 CrystalGUID;                                     // This will help us create a pointer to the crystal we are draining. We store GUIDs, never units in case unit is deleted/offline (offline if player of course).

    void Reset()
    {
        if (m_pInstance)
        {
            //for(uint8 i = 0; i < CRYSTALS_NUMBER; ++i)
            for(std::list<uint64>::iterator itr = Crystals.begin(); itr != Crystals.end(); ++itr)
            {
                //Creature* pUnit = m_creature->GetMap()->GetCreature(FelCrystals[i]);
                if (Creature* pCrystal = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (!pCrystal->isAlive())
                        pCrystal->Respawn();                // Let MaNGOS handle setting death state, etc.

                    // Only need to set unselectable flag. You can't attack unselectable units so non_attackable flag is not necessary here.
                    pCrystal->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SELIN_ENCOUNTER_DOOR)))
                pDoor->SetGoState(GO_STATE_ACTIVE);         // Open the big encounter door. Close it in Aggro and open it only in JustDied(and here)
                                                            // Small door opened after event are expected to be closed by default
            // Set Inst data for encounter
            m_pInstance->SetData(DATA_SELIN_EVENT, NOT_STARTED);
        }else error_log(ERROR_INST_DATA);

        DrainLifeTimer = urand(3000, 7000);
        DrainManaTimer = DrainLifeTimer + 5000;
        FelExplosionTimer = 2100;
        DrainCrystalTimer = urand(10000, 15000);
        DrainCrystalTimer = urand(20000, 25000);
        EmpowerTimer = 10000;

        IsDraining = false;
        DrainingCrystal = false;
        CrystalGUID = 0;
    }

    void SelectNearestCrystal()
    {
        if (Crystals.empty())
            return;

        float ShortestDistance = 0;
        CrystalGUID = 0;
        Creature* pCrystal = NULL;
        Creature* CrystalChosen = NULL;
        //for(uint8 i =  0; i < CRYSTALS_NUMBER; ++i)
        for(std::list<uint64>::iterator itr = Crystals.begin(); itr != Crystals.end(); ++itr)
        {
            pCrystal = m_creature->GetMap()->GetCreature(*itr);

            if (pCrystal && pCrystal->isAlive())
            {
                // select nearest
                if (!CrystalChosen || m_creature->GetDistanceOrder(pCrystal, CrystalChosen, false))
                {
                    CrystalGUID = pCrystal->GetGUID();
                    CrystalChosen = pCrystal;               // Store a copy of pCrystal so we don't need to recreate a pointer to closest crystal for the movement and yell.
                }
            }
        }
        if (CrystalChosen)
        {
            DoScriptText(SAY_ENERGY, m_creature);
            DoScriptText(EMOTE_CRYSTAL, m_creature);

            CrystalChosen->CastSpell(CrystalChosen, SPELL_FEL_CRYSTAL_COSMETIC, true);

            float x, y, z;                                  // coords that we move to, close to the crystal.
            CrystalChosen->GetClosePoint(x, y, z, m_creature->GetObjectBoundingRadius(), CONTACT_DISTANCE);

            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
            DrainingCrystal = true;
        }
    }

    void ShatterRemainingCrystals()
    {
        if (Crystals.empty())
            return;

        //for(uint8 i = 0; i < CRYSTALS_NUMBER; ++i)
        for(std::list<uint64>::iterator itr = Crystals.begin(); itr != Crystals.end(); ++itr)
        {
            //Creature* pCrystal = m_creature->GetMap()->GetCreature(FelCrystals[i]);
            Creature* pCrystal = m_creature->GetMap()->GetCreature(*itr);

            if (pCrystal && pCrystal->isAlive())
                pCrystal->DealDamage(pCrystal, pCrystal->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
        {
            //Close the encounter door, open it in JustDied/Reset
            if (GameObject* pEncounterDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SELIN_ENCOUNTER_DOOR)))
                pEncounterDoor->SetGoState(GO_STATE_READY);
        }
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type == POINT_MOTION_TYPE && id == 1)
        {
            Creature* CrystalChosen = m_creature->GetMap()->GetCreature(CrystalGUID);
            if (CrystalChosen && CrystalChosen->isAlive())
            {
                // Make the crystal attackable
                // We also remove NON_ATTACKABLE in case the database has it set.
                CrystalChosen->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE);
                CrystalChosen->CastSpell(m_creature, SPELL_MANA_RAGE, true);
                IsDraining = true;
            }
            else
            {
                // Make an error message in case something weird happened here
                error_log("SD2: Selin Fireheart unable to drain crystal as the crystal is either dead or despawned");
                DrainingCrystal = false;
            }
        }
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
        {
            error_log(ERROR_INST_DATA);
            return;
        }

        m_pInstance->SetData(DATA_SELIN_EVENT, DONE);         // Encounter complete!

        if (GameObject* pEncounterDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SELIN_ENCOUNTER_DOOR)))
            pEncounterDoor->SetGoState(GO_STATE_ACTIVE);    // Open the encounter door

        if (GameObject* pContinueDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SELIN_DOOR)))
            pContinueDoor->SetGoState(GO_STATE_ACTIVE);     // Open the door leading further in

        ShatterRemainingCrystals();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!DrainingCrystal)
        {
            uint32 maxPowerMana = m_creature->GetMaxPower(POWER_MANA);
            if (maxPowerMana && ((m_creature->GetPower(POWER_MANA)*100 / maxPowerMana) < 10))
            {
                if (DrainLifeTimer < diff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, SPELL_DRAIN_LIFE);

                    DrainLifeTimer = 10000;
                }else DrainLifeTimer -= diff;

                // Heroic only
                if (!m_bIsRegularMode)
                {
                    if (DrainManaTimer < diff)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                            DoCastSpellIfCan(pTarget, SPELL_DRAIN_MANA);

                        DrainManaTimer = 10000;
                    }else DrainManaTimer -= diff;
                }
            }

            if (FelExplosionTimer < diff)
            {
                if (!m_creature->IsNonMeleeSpellCasted(false))
                {
                    DoCastSpellIfCan(m_creature, SPELL_FEL_EXPLOSION);
                    FelExplosionTimer = 2000;
                }
            }else FelExplosionTimer -= diff;

            // If below 10% mana, start recharging
            maxPowerMana = m_creature->GetMaxPower(POWER_MANA);
            if (maxPowerMana && ((m_creature->GetPower(POWER_MANA)*100 / maxPowerMana) < 10))
            {
                if (DrainCrystalTimer < diff)
                {
                    SelectNearestCrystal();

                    if (m_bIsRegularMode)
                        DrainCrystalTimer = urand(20000, 25000);
                    else
                        DrainCrystalTimer = urand(10000, 15000);

                }else DrainCrystalTimer -= diff;
            }

        }else
        {
            if (IsDraining)
            {
                if (EmpowerTimer < diff)
                {
                    IsDraining = false;
                    DrainingCrystal = false;

                    DoScriptText(SAY_EMPOWERED, m_creature);

                    Creature* CrystalChosen = m_creature->GetMap()->GetCreature(CrystalGUID);
                    if (CrystalChosen && CrystalChosen->isAlive())
                        // Use Deal Damage to kill it, not SetDeathState.
                        CrystalChosen->DealDamage(CrystalChosen, CrystalChosen->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                    CrystalGUID = 0;

                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                }else EmpowerTimer -= diff;
            }
        }

        DoMeleeAttackIfReady();                             // No need to check if we are draining crystal here, as the spell has a stun.
    }
};

CreatureAI* GetAI_boss_selin_fireheart(Creature* pCreature)
{
    return new boss_selin_fireheartAI(pCreature);
};

struct MANGOS_DLL_DECL mob_fel_crystalAI : public ScriptedAI
{
    mob_fel_crystalAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() {}
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) {}

    void JustDied(Unit* killer)
    {
        if (ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData())
        {
            Creature* pSelin = m_creature->GetMap()->GetCreature(pInstance->GetData64(DATA_SELIN));

            if (pSelin && pSelin->isAlive())
            {
                boss_selin_fireheartAI* pSelinAI = dynamic_cast<boss_selin_fireheartAI*>(pSelin->AI());

                if (pSelinAI && pSelinAI->CrystalGUID == m_creature->GetGUID())
                {
                    // Set this to false if we are the creature that Selin is draining so his AI flows properly
                    pSelinAI->DrainingCrystal = false;
                    pSelinAI->IsDraining = false;
                    pSelinAI->EmpowerTimer = 10000;

                    if (pSelin->getVictim())
                    {
                        pSelin->AI()->AttackStart(pSelin->getVictim());
                        pSelin->GetMotionMaster()->MoveChase(pSelin->getVictim());
                    }
                }
            }
        }else error_log(ERROR_INST_DATA);
    }
};

CreatureAI* GetAI_mob_fel_crystal(Creature* pCreature)
{
    return new mob_fel_crystalAI(pCreature);
};

void AddSC_boss_selin_fireheart()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_selin_fireheart";
    newscript->GetAI = &GetAI_boss_selin_fireheart;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_fel_crystal";
    newscript->GetAI = &GetAI_mob_fel_crystal;
    newscript->RegisterSelf();
}
