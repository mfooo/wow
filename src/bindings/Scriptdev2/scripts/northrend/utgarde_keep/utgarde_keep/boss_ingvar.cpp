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
SDName: Boss_Ingvar
SD%Complete: 20%
SDComment:
SDCategory: Utgarde Keep
EndScriptData */

#include "precompiled.h"
#include "utgarde_keep.h"

//Yells Ingvar
#define SAY_AGGRO_1                         -1574005
#define SAY_AGGRO_2                         -1574006

#define SAY_DEAD_1                          -1574007
#define SAY_DEAD_2                          -1574008

#define SAY_KILL_1                          -1574009
#define SAY_KILL_2                          -1574010

//Ingvar Spells human form
#define MOB_INGVAR_HUMAN                            23954
#define MODEL_INGVAR_HUMAN                          21953
#define SPELL_CLEAVE                                42724
#define SPELL_SMASH                                 42669
#define H_SPELL_SMASH                               59706
#define SPELL_STAGGERING_ROAR                       42708
#define H_SPELL_STAGGERING_ROAR                     59708
#define SPELL_ENRAGE                                42705
#define H_SPELL_ENRAGE                              59707

#define MOB_ANNHYLDE_THE_CALLER                     24068
#define SPELL_INGVAR_FEIGN_DEATH                    42795
#define SPELL_SUMMON_BANSHEE                        42912
#define SPELL_SCOURG_RESURRECTION_EFFEKTSPAWN       42863 //Spawn resurrecteffekt around Ingvar

//Ingvar Spells undead form
#define MOB_INGVAR_UNDEAD                           23980
#define MODEL_INGVAR_UNDEAD                         26351
#define SPELL_DARK_SMASH                            42723
#define SPELL_DREADFUL_ROAR                         42729
#define H_SPELL_DREADFUL_ROAR                       59734
#define SPELL_WOE_STRIKE                            42730
#define H_SPELL_WOE_STRIKE                          59735

#define ENTRY_THROW_TARGET                          23996
#define SPELL_SHADOW_AXE_SUMMON                     42749

struct MANGOS_DLL_DECL boss_ingvarAI : public ScriptedAI
{
    boss_ingvarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsResurrected;
    bool undead;
    bool event_inProgress;

    uint32 Cleave_Timer;
    uint32 Smash_Timer;
    uint32 Enrage_Timer;
    uint32 Roar_Timer;
    uint32 SpawnResTimer;
    uint32 wait_Timer;


    void Reset()
    {
        undead = false;
        event_inProgress = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
		//m_creature->UpdateEntry(MOB_INGVAR_HUMAN);
        m_creature->SetDisplayId(MODEL_INGVAR_HUMAN);

        Cleave_Timer = 2000;
        Smash_Timer = 5000;
        Enrage_Timer = 10000;
        Roar_Timer = 15000;

        wait_Timer = 0;
    }

    void DamageTaken(Unit *done_by, uint32 &damage) 
    {
        if (damage >= m_creature->GetHealth() && !undead)
        {
            // visuel hack
            m_creature->SetHealth(0);
            m_creature->InterruptNonMeleeSpells(true);
            m_creature->RemoveAllAuras();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MovementExpired(false);
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
            DoResetThreat();
            // visuel hack end
            
            event_inProgress = true;
	        SpawnResTimer = 3000;
            undead = true;

            DoScriptText(SAY_AGGRO_1,m_creature);
        }

		if (event_inProgress)
        {
            damage = 0;
        }
    }

    void StartZombiePhase()
    {
        undead = true;
        event_inProgress = false;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());

        DoScriptText(SAY_AGGRO_2, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO_1, m_creature);
    }

    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_DEAD_2,m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        if (undead)
        {
            DoScriptText(SAY_KILL_1,m_creature);
        }
        else
        {
            DoScriptText(SAY_KILL_2,m_creature);
        }
    }


    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
            return;

        if (event_inProgress)
        {
            if (SpawnResTimer < diff)
            {
                m_creature->SummonCreature(MOB_ANNHYLDE_THE_CALLER, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()+13.0f, 2.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 12000);
                SpawnResTimer = 999999999;
            }
            else
                SpawnResTimer -= diff;

            return;
        }

        // This is used for a spell queue ... the spells should not castet if one spell is already casting
        if (wait_Timer)
            if (wait_Timer < diff)
            {
                wait_Timer = 0;
            }
            else
                wait_Timer -= diff;

        if (Cleave_Timer < diff)
        {
            if (!wait_Timer)
            {
                if (undead)
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WOE_STRIKE : H_SPELL_WOE_STRIKE);
                else
                    DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                Cleave_Timer = rand()%5000 + 2000;

                wait_Timer = 1000;
            }
        }
        else
            Cleave_Timer -= diff;

        if (Smash_Timer < diff)
        {
            if (!wait_Timer)
            {
                if(undead)
                    DoCast(m_creature->getVictim(), SPELL_DARK_SMASH);
                else
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SMASH : H_SPELL_SMASH);
                Smash_Timer = 10000;

                wait_Timer = 5000;
            }
        }
        else
            Smash_Timer -= diff;

        if (!undead)
        {
            if (Enrage_Timer < diff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : H_SPELL_ENRAGE);
                Enrage_Timer = 10000;
            }
            else
                Enrage_Timer -= diff;
        }
        else // In Undead form used to summon weapon
        {
            if (Enrage_Timer < diff)
            {
                if (!wait_Timer)
                {
                    // Spawn target for Axe
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                    {
                        if (Creature* pThrowTarget = m_creature->SummonCreature(ENTRY_THROW_TARGET, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 2000))
                            pThrowTarget->SetDisplayId(11686);
                        DoCast(m_creature, SPELL_SHADOW_AXE_SUMMON);
                    }
                    Enrage_Timer = 30000;
                }
            }
            else
                Enrage_Timer -= diff;
        }

        if (Roar_Timer < diff)
        {
            if (!wait_Timer)
            {
                if (undead)
                    DoCast(m_creature, m_bIsRegularMode ? SPELL_DREADFUL_ROAR : H_SPELL_DREADFUL_ROAR);
                else
                    DoCast(m_creature, m_bIsRegularMode ? SPELL_STAGGERING_ROAR : H_SPELL_STAGGERING_ROAR);
                Roar_Timer = 10000;
                wait_Timer = 5000;
            }
        }
        else
            Roar_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ingvar(Creature *_Creature)
{
    return new boss_ingvarAI (_Creature);
}

//Spells for Annhylde
#define SPELL_SCOURG_RESURRECTION_HEAL              42704 //Heal Max + DummyAura
#define SPELL_SCOURG_RESURRECTION_BEAM              42857 //Channeling Beam of Annhylde
#define SPELL_SCOURG_RESURRECTION_DUMMY             42862 //Some Emote Dummy?
#define SPELL_INGVAR_TRANSFORM                      42796

struct MANGOS_DLL_DECL npc_annhyldeAI : public ScriptedAI
{
    npc_annhyldeAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    float x, y, z;
    uint32 Resurect_Timer;
    uint32 Resurect_Phase;

    void Reset()
    {
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		Resurect_Timer = 8000;
		Resurect_Phase = 1;
		if (m_pInstance)
			if (Unit* pIngvar = m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_INGVAR)))
			{
				pIngvar->RemoveAura(SPELL_SUMMON_BANSHEE, EFFECT_INDEX_0);
				pIngvar->CastSpell(pIngvar, SPELL_SCOURG_RESURRECTION_DUMMY, true);
				DoCast(pIngvar, SPELL_SCOURG_RESURRECTION_BEAM);
			}
    }

    void AttackStart(Unit* who) {}
	
    void MoveInLineOfSight(Unit* who) {}

    void UpdateAI(const uint32 diff)
    {
        if (!Resurect_Phase)
			return;
		
		if (Resurect_Timer < diff)
        {
            switch (Resurect_Phase)
			{
				case 1:
					if (m_pInstance)
					{
						if (Unit* pIngvar = m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_INGVAR)))
						{
							pIngvar->SetStandState(UNIT_STAND_STATE_STAND);
							pIngvar->CastSpell(pIngvar, SPELL_SCOURG_RESURRECTION_HEAL, false);
						}
					}
					Resurect_Timer = 3000;
					Resurect_Phase = 2;
					break;
				case 2:
	                if (m_pInstance)
						if(Creature* pIngvar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_INGVAR)))
						{
							pIngvar->RemoveAurasDueToSpell(SPELL_SCOURG_RESURRECTION_DUMMY);
                            pIngvar->SetDisplayId(MODEL_INGVAR_UNDEAD);
							((boss_ingvarAI*)(((Creature*)pIngvar)->AI()))->StartZombiePhase();
						}
					Resurect_Phase = 0;
					break;
            }
        }
        else
            Resurect_Timer -= diff;
    }
};

CreatureAI*  GetAI_npc_annhylde(Creature* pCreature)
{
    return new npc_annhyldeAI(pCreature);
}

#define SPELL_SHADOW_AXE_DAMAGE                     42750
#define H_SPELL_SHADOW_AXE_DAMAGE                   59719

struct MANGOS_DLL_DECL mob_ingvar_throw_dummyAI : public ScriptedAI
{
    mob_ingvar_throw_dummyAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 Despawn_Timer;

    void Reset()
    {
        m_creature->SetSpeedRate(MOVE_WALK, 3.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        SetEquipmentSlots(false, 33177);
        Creature* pTarget = GetClosestCreatureWithEntry(m_creature, ENTRY_THROW_TARGET, 100.0f);
        if (pTarget)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_AXE_DAMAGE : H_SPELL_SHADOW_AXE_DAMAGE);
            float x, y, z;
            pTarget->GetPosition(x, y, z);
            m_creature->GetMotionMaster()->MovePoint(0, x, y, z);
        }
        Despawn_Timer = 7000;
    }
    void AttackStart(Unit* who) {}

    void MoveInLineOfSight(Unit* who) {}

    void UpdateAI(const uint32 diff)
    {
        if (Despawn_Timer < diff)
        {
            m_creature->ForcedDespawn();
            Despawn_Timer = 0;
        }
        else
            Despawn_Timer -= diff;
    }
};

CreatureAI* GetAI_mob_ingvar_throw_dummy(Creature *_Creature)
{
    return new mob_ingvar_throw_dummyAI (_Creature);
}

void AddSC_boss_ingvar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ingvar";
    newscript->GetAI = &GetAI_boss_ingvar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_annhylde";
    newscript->GetAI = &GetAI_npc_annhylde;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ingvar_throw_dummy";
    newscript->GetAI = &GetAI_mob_ingvar_throw_dummy;
    newscript->RegisterSelf();
}
