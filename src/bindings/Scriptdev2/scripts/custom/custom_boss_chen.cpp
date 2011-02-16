/* ScriptData
SDName: custom_boss_chen
SD%Complete: who knows
SDComment: biym
SDCategory: Lore shiz
EndScriptData */

#include "precompiled.h"

enum
{
	SPELL_WHIRLWIND        = 41056, // 33238
	SPELL_KICK             = 24671,
	SPELL_SHADOWLANCE      = 69058, // unlimited range
	SPELL_RAINOFFIRE       = 57757, // 40 yd
	SPELL_ARCANEVOLLEY     = 59483, // 20 yd
	SPELL_DRAINLIFE        = 70213, // 20 yd
	SPELL_ENRAGE           = 28131,
	SPELL_BERSERK          = 47008,

	NPC_BOSS               = 7105000,
	NPC_STORM_FURY         = 7105001,
	NPC_FROSTBLAZE         = 7105002,
	NPC_DANGERZONE         = 7105003,

	SPELL_AURA_WHIRL     = 38464,
	SPELL_AURA_SPARK     = 54141,
	SPELL_AURA_SPARK_2   = 55845,
//	SPELL_BOOM_VISUAL    = 71495,
	SPELL_BOOM_DAMAGE    = 52717, // 53071,

	SPELL_VISUAL_TELEPORT = 41232,
	SPELL_VISUAL_PILLAR   = 51892,
	SPELL_VISUAL_MARK     = 51939,
	SPELL_VISUAL_SPHERE   = 56075,

	DISPLAYID_INACTIVE  = 11686,
	DISPLAYID_ACTIVE    = 28988,

	WHIRLWIND_CD		 = 10000,
	KICK_CD				 = 7000,
	SHADOWLANCE_CD       = 10000,
	DRAINLIFE_CD		 = 25000,
	RAINOFFIRE_CD        = 30000,
	ARCANEVOLLEY_CD      = 35000,
	SUMMON_STORMFURY_CD  = 30000,
	SUMMON_DANGERZONE_CD = 60000,

	BERSERK_CD          = 10*MINUTE*IN_MILLISECONDS
};

struct MANGOS_DLL_DECL custom_boss_chenAI : public ScriptedAI
{
	custom_boss_chenAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    uint32 Whirlwind_Timer;
	uint32 Kick_Timer;
	uint32 Shadowlance_Timer;
	uint32 Drainlife_Timer;
	uint32 Rainoffire_Timer;
	uint32 Arcanevolley_Timer;
	uint32 Summon_Stormfury_Timer;
	uint32 Summon_Dangerzone_Timer;
    uint32 Berserk_Timer;
	bool Enraged;
	bool Berserk;
	bool Aggroed;
	uint32 Formation;
	uint8 stage;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);

		Whirlwind_Timer = urand(9000,11000);
		Kick_Timer = urand(12000,14000);
		Shadowlance_Timer = urand(500,1000);
		Drainlife_Timer = urand(28000,32000);
		Rainoffire_Timer = urand(10000,15000);
		Arcanevolley_Timer = urand(19000,21000);
		Summon_Stormfury_Timer = urand(5000,6000);
		Summon_Dangerzone_Timer = urand(29000,31000);
        Berserk_Timer = BERSERK_CD;

		Enraged = false;
		Berserk = false;
		Aggroed = false;
		
		stage = 0;
    }
/*
	void JustReachedHome()
	{
		Aggroed = false;
		return;
	}
*/
	void Aggro(Unit *who) 
    {
        // if (!Aggroed)
			DoScriptText(-1568017,m_creature,who);
		// Aggroed = true;
		return;
    }
/*
	void JustDied(Unit *)
	{
		Aggroed = false;
		return;
	}
*/
    void UpdateAI(const uint32 diff)
    {
        //return if no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		if (stage == 0 && m_creature->GetHealthPercent() < 50.0f) { stage = 1; }

			//whirlwind near main target
		    if (Whirlwind_Timer < diff)
			{
				DoCastSpellIfCan(m_creature->getVictim(),SPELL_WHIRLWIND);
				Whirlwind_Timer = WHIRLWIND_CD + urand(0,2000);
			}else Whirlwind_Timer -= diff;

			//kick on random melee
			if (Kick_Timer < diff)
			{
				if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
					if (!m_creature->IsWithinDistInMap(target, 5.0f))
					{
						DoCastSpellIfCan(target,SPELL_KICK);
						Kick_Timer = KICK_CD + urand(0,2000);
					}
			}else if (Kick_Timer > 0) Kick_Timer -= diff;

			//shadowlance on random attacker more than 10 yd away if hp > 50%
			if (stage==0 && Shadowlance_Timer < diff)
			{
				if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
					if (!m_creature->IsWithinDistInMap(target, 8.0f))
				{
					DoCastSpellIfCan(target,SPELL_SHADOWLANCE);
					Shadowlance_Timer = SHADOWLANCE_CD + urand(0,200);
				}
			}else if (Shadowlance_Timer > 0) Shadowlance_Timer -= diff;

			//rain of fire on random attacker more than 10 yd away
			if (Rainoffire_Timer < diff)
			{
				if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
					if (!m_creature->IsWithinDistInMap(target, 10.0f))
					{
						DoCastSpellIfCan(target,SPELL_RAINOFFIRE);
						Rainoffire_Timer = RAINOFFIRE_CD + urand(0,100);
					}
			}else if (Rainoffire_Timer > 0) Rainoffire_Timer -= diff;

			//arcane volley on all enemies
			if (Arcanevolley_Timer < diff)
			{
				DoCastSpellIfCan(m_creature->getVictim(),SPELL_ARCANEVOLLEY);
				Arcanevolley_Timer = ARCANEVOLLEY_CD + urand(0,2000);
			}else Arcanevolley_Timer -= diff;

			//drain random attacker if hp < 50%
			if (stage == 1 && Drainlife_Timer < diff)
			{
				if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				{
					DoCastSpellIfCan(target,SPELL_DRAINLIFE);
					Drainlife_Timer = DRAINLIFE_CD + urand(-2000,2000);
				}
			}else Drainlife_Timer -= diff;

			//soft enrage at 5% once
			if (!Enraged)
			{
				if (m_creature->GetHealthPercent() < 5.0f)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
					{
						Enraged = true;
					}
				}
			}

			//berserk once
			if (!Berserk)
			{
				if (Berserk_Timer < diff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
					{
						Berserk = true;
					}
				}
				else
					Berserk_Timer -= diff;
			}

			// summon tornado chasing random target
/*			if (Summon_Stormfury_Timer < diff)
			{
                {
					DoScriptText(-1565016,m_creature);
                    float fPosX, fPosY, fPosZ;
                    m_creature->GetPosition(fPosX, fPosY, fPosZ);
					float fSpawnX, fSpawnY;
					fSpawnX = fPosX;
					fSpawnY = fPosY;
					m_creature->SummonCreature(NPC_STORM_FURY, fSpawnX, fSpawnY, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_STORMFURY_CD);
					m_creature->SummonCreature(NPC_STORM_FURY, fSpawnX+10, fSpawnY+10, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_STORMFURY_CD);
                }
				Summon_Stormfury_Timer = SUMMON_STORMFURY_CD;

			} else Summon_Stormfury_Timer -= diff;
*/

			if (Summon_Dangerzone_Timer < diff)
			{
                {
					DoScriptText(-1565016,m_creature);
                    float fPosX, fPosY, fPosZ;
                    m_creature->GetPosition(fPosX, fPosY, fPosZ);
					float fSpawnX, fSpawnY;
					fSpawnX = fPosX;
					fSpawnY = fPosY;
					Formation = urand(0,1);
					if (!Formation)
					{
						m_creature->SummonCreature(NPC_DANGERZONE, fSpawnX, fSpawnY-10, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_DANGERZONE_CD);
						m_creature->SummonCreature(NPC_DANGERZONE, fSpawnX+7, fSpawnY+7, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_DANGERZONE_CD);
						m_creature->SummonCreature(NPC_DANGERZONE, fSpawnX-7, fSpawnY+7, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_DANGERZONE_CD);
					}
					else
					{
						m_creature->SummonCreature(NPC_DANGERZONE, fSpawnX+7, fSpawnY+7, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_DANGERZONE_CD);
						m_creature->SummonCreature(NPC_DANGERZONE, fSpawnX-7, fSpawnY+7, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_DANGERZONE_CD);
						m_creature->SummonCreature(NPC_DANGERZONE, fSpawnX+7, fSpawnY-7, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_DANGERZONE_CD);
						m_creature->SummonCreature(NPC_DANGERZONE, fSpawnX-7, fSpawnY-7, fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, SUMMON_DANGERZONE_CD);
					}
                }
				Summon_Dangerzone_Timer = SUMMON_DANGERZONE_CD;

			} else Summon_Dangerzone_Timer -= diff;

			DoMeleeAttackIfReady();
		}
};

CreatureAI* GetAI_custom_boss_chen(Creature* pCreature)
{
    return new custom_boss_chenAI(pCreature);
}

struct MANGOS_DLL_DECL custom_mob_storm_furyAI : public ScriptedAI
{
	custom_mob_storm_furyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

	Unit* pChased;
    uint32 m_uiRangeCheck_Timer;
	Creature* pBoss;
	uint32 Activate_Timer;
	bool Active;
	bool Chasing;

	void Reset()
    {
		Active = false;
		m_creature->SetDisplayId(DISPLAYID_INACTIVE);
		Activate_Timer = 5000;

		pBoss = GetClosestCreatureWithEntry(m_creature, NPC_BOSS, 50.0f);
		if (pBoss) pChased = pBoss->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
		if (pChased) m_creature->GetMotionMaster()->MoveChase(pChased);
		Chasing = false;

		if (pBoss) pBoss->_AddAura(51939,SUMMON_STORMFURY_CD);
		if (pChased) pChased->_AddAura(51939,SUMMON_STORMFURY_CD);

//		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		m_creature->_AddAura(SPELL_AURA_WHIRL);
		m_creature->SetRespawnDelay(SUMMON_STORMFURY_CD);
//		m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->SetSpeedRate(MOVE_RUN, 0.3f);

		m_uiRangeCheck_Timer = 500;
	}

	void UpdateAI(const uint32 uiDiff)
    {
		if (!Active)
		{
			if (Activate_Timer < uiDiff)
			{
				Active = true;
				m_creature->SetDisplayId(28988);
				m_creature->_AddAura(SPELL_AURA_SPARK);
				m_creature->_AddAura(SPELL_AURA_SPARK_2);
			}
			else Activate_Timer -= uiDiff;
			return; // don't start chasing if not active
		}

		pBoss = GetClosestCreatureWithEntry(m_creature, NPC_BOSS, 50.0f);
		if (pBoss) pChased = pBoss->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
		if (pChased) m_creature->GetMotionMaster()->MoveChase(pChased);

		if (!Chasing)
		{
			if (pChased)
			{
				Chasing = true; // turn on chasing after being activated
//				m_creature->_AddAura(51939,SUMMON_STORMFURY_CD);
			}
			return; // don't check proximity if not chasing
		}

		if (m_uiRangeCheck_Timer < uiDiff)
		{
			if (pChased->IsWithinDistInMap(m_creature, 2.0f))
			{
				DoCastSpellIfCan(m_creature, SPELL_BOOM_DAMAGE);
				m_creature->ForcedDespawn(250);
		    }
			else m_creature->GetMotionMaster()->MoveChase(pChased);

		    m_uiRangeCheck_Timer = 500;
		}
		else m_uiRangeCheck_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_custom_mob_storm_fury(Creature* pCreature)
{
    return new custom_mob_storm_furyAI (pCreature);
}

struct MANGOS_DLL_DECL custom_mob_danger_zoneAI : public ScriptedAI
{
	custom_mob_danger_zoneAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

	uint32 AoE_Timer;
	uint32 Activate_Timer;
	bool Active;

	void Reset()
    {
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		m_creature->_AddAura(SPELL_VISUAL_PILLAR);
		m_creature->SetRespawnDelay(SUMMON_DANGERZONE_CD);
        m_creature->SetSpeedRate(MOVE_RUN, 0.0f);
		Active = false;
		Activate_Timer = 10000;
		AoE_Timer = 1000;
	}

	void UpdateAI(const uint32 uiDiff)
    {
		if (!Active)
		{
			if (Activate_Timer < uiDiff)
			{
				Active = true;
				m_creature->RemoveAllAuras();
				m_creature->_AddAura(SPELL_VISUAL_SPHERE);
				DoCastSpellIfCan(m_creature,SPELL_VISUAL_TELEPORT);
			}
			else Activate_Timer -= uiDiff;
			return;
		}

		if (AoE_Timer < uiDiff)
		{
			DoCastSpellIfCan(m_creature,SPELL_BOOM_DAMAGE);
			AoE_Timer = 1000;
		}
		else AoE_Timer -= uiDiff;

    }
};

CreatureAI* GetAI_custom_mob_danger_zone(Creature* pCreature)
{
    return new custom_mob_danger_zoneAI (pCreature);
}

void AddSC_custom_boss_chen()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "custom_boss_chen";
    newscript->GetAI = &GetAI_custom_boss_chen;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "custom_mob_storm_fury";
    newscript->GetAI = &GetAI_custom_mob_storm_fury;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "custom_mob_danger_zone";
    newscript->GetAI = &GetAI_custom_mob_danger_zone;
    newscript->RegisterSelf();
}
