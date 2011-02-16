/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: oculus
SD%Complete: 90%
SDComment: // dev // FallenAngelX
SDCategory:Mage-Lord Urom
ToDo:: FIX TELEPORT PART OF EVENT
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

/*###
#Mage_lord_urom
####*/
enum
{
	SPELL_SHIELD_CHANNEL     = 53813,  //Dummy --> Channeled, shields the caster from damage.
	SPELL_SUMMON_MENAGERIE_1 = 50476,   //Summons an assortment of creatures and teleports the caster to ring 1.
	SPELL_SUMMON_MENAGERIE_2 = 50495,   //Summons an assortment of creatures and teleports the caster to ring 2.
	SPELL_SUMMON_MENAGERIE_3 = 50496,   //Summons an assortment of creatures and teleports the caster to ring 3.
	SPELL_TELEPORT           = 51112, //Teleports to the center of Oculus  in the center of ring 3

	SPELL_FROSTBOMB          = 51103,  //Urom throws a bomb, hitting its target with the highest aggro which inflict directly 650 frost damage and drops a frost zone on the ground. This zone deals 650 frost damage per second and reduce the movement speed by 35%. Lasts 1 minute.
	SPELL_TIMEBOMB           = 51121,   //Deals arcane damage to a random player, and after 6 seconds, deals zone damage to nearby equal to the health missing of the target afflicted by the debuff.
	SPELL_TIMEBOMB_H         = 59376,
	SPELL_ARCANE_EXPLOSION   = 51110,
	SPELL_ARCANE_EXPLOSION_H = 59377,

	SPELL_TIMEBOMB_DETONATE  = 51132,

	SAY_SUMMON1              = -1578019,
	SAY_SUMMON2              = -1578020,
	SAY_SUMMON3              = -1578021,
	SAY_AGGRO                = -1578012,
	SAY_EXPL1                = -1578017,
	SAY_EXPL2                = -1578018,
	SAY_KILL1                = -1578013,
	SAY_KILL2                = -1578014,
	SAY_KILL3                = -1578015,
	SAY_DEATH                = -1578016,
};

enum FightPhases
{
	RING_ONE,
	RING_TWO,
	RING_THREE,
	CENTER
};

struct MANGOS_DLL_DECL boss_uromAI: public ScriptedAI
{
	boss_uromAI(Creature* c): ScriptedAI(c)
	{
		m_pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		m_bIsRegularMode = m_pInstance->instance->IsRegularDifficulty();
		Reset();
	}

	ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

	uint8 FightPhase;
	bool isEscaping;
	bool CenterEngaged;

	uint32 TeleportTimer;
	uint32 CanCatchCombatTimer;

	uint32 FrostBombTimer;
	uint32 TimeBombTimer;
	uint32 CentralTeleportTimer;
	uint32 ArcaneExplosionTimer;

	Unit* TimeBomb;
	uint32 TimeBombDetonateTimer;

	void Reset()
	{
		m_creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);

		TeleportTimer = 0;
		CanCatchCombatTimer = 0;
		isEscaping = false;
		CenterEngaged = false;
		FightPhase = RING_ONE;
		m_creature->CastSpell(m_creature,SPELL_SHIELD_CHANNEL,false);

		FrostBombTimer = 3000;
		TimeBombTimer = 6500;
		CentralTeleportTimer = 9000;
		ArcaneExplosionTimer = 0;

		TimeBomb = NULL;
		TimeBombDetonateTimer = 0;
	}

	void Aggro(Unit*)
	{
		DoScriptText(SAY_AGGRO,m_creature);
	}

	void AttackStart(Unit* pWho)
	{
		if(FightPhase != CENTER || m_creature->GetPositionZ() > 516.0f || CanCatchCombatTimer) //hack
			return;

		m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
		switch(pWho->GetEntry())
		{
		case 27692:
		case 27755:
		case 27756:
			m_creature->DealDamage(pWho,pWho->GetHealth(),0,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,0,false);
			return;
		}
		CenterEngaged = true;
		ScriptedAI::AttackStart(pWho);
	}

	void MoveInLineOfSight(Unit* pWho)
	{
		if(isEscaping || CanCatchCombatTimer || m_creature->getVictim())
			return;

		if(pWho->GetTypeId() == TYPEID_PLAYER && pWho->IsWithinDistInMap(m_creature,20.0f))
		{
			m_creature->InterruptNonMeleeSpells(false);
			m_creature->SetFacingToObject(pWho);

			switch(FightPhase)
			{
			case RING_ONE:
				DoScriptText(SAY_SUMMON1,m_creature);
				m_creature->GetMotionMaster()->MoveIdle();
				m_creature->CastSpell(m_creature,SPELL_SUMMON_MENAGERIE_1,false);
				TeleportTimer = 3900;
				CanCatchCombatTimer = 6000;
				isEscaping = true;
				break;
			case RING_TWO:
				if(m_creature->GetPositionX() < 1000.0f) //hack
				{
					DoScriptText(SAY_SUMMON2,m_creature);
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->CastSpell(m_creature,SPELL_SUMMON_MENAGERIE_2,false);
					TeleportTimer = 3900;
					CanCatchCombatTimer = 6000;
					isEscaping = true;
				}
				break;
			case RING_THREE:
				if(m_creature->GetPositionX() > 1000.0f) //hack
				{
					DoScriptText(SAY_SUMMON3,m_creature);
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->CastSpell(m_creature,SPELL_SUMMON_MENAGERIE_3,false);
					TeleportTimer = 3900;
					CanCatchCombatTimer = 6000;
					isEscaping = true;
				}
				break;
			case CENTER:
				if(m_creature->GetPositionZ() < 516.0f) //hack
				{
					ScriptedAI::MoveInLineOfSight(pWho);
					AttackStart(pWho);
				}
				break;
			}
		}
	}

	void TimeBombDetonate()
	{
		if(TimeBomb && TimeBomb->isAlive() && m_pInstance)
		{
			TimeBomb->CastSpell(TimeBomb,SPELL_TIMEBOMB_DETONATE,true);

			uint32 damage = TimeBomb->GetMaxHealth() - TimeBomb->GetHealth();

			Map::PlayerList const& PlayerList = m_pInstance->instance->GetPlayers();

			if(PlayerList.isEmpty())
				return;

			for(Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
			{
				if(itr->getSource() && !(TimeBomb->GetTypeId() == TYPEID_PLAYER && ((Player*)TimeBomb) == itr->getSource()) )
				{
					Player* Target = itr->getSource();
					if(Target->isAlive() && Target->IsWithinDistInMap(TimeBomb,10.0f))
						TimeBomb->DealDamage(Target,damage,0,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,0,false);
				}
			}
			TimeBomb = NULL;
		}
	}

	void JustDied(Unit* pKiller)
	{
		if(m_pInstance)
			m_pInstance->SetData(TYPE_UROM,DONE);

		DoScriptText(SAY_DEATH,m_creature);
	}

	void KilledUnit(Unit* pVictim)
	{
		switch(urand(1,3))
		{
		case 1: DoScriptText(SAY_KILL1,m_creature); break;
		case 2: DoScriptText(SAY_KILL2,m_creature); break;
		case 3: DoScriptText(SAY_KILL3,m_creature); break;
		}
	}

	void UpdateAI(const uint32 diff)
	{
		if(isEscaping)
		{
			if(TeleportTimer <= diff)
			{
				switch(FightPhase)
				{
				case RING_ONE:
					m_creature->SummonCreature(27650, 1177.505f,  937.96f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					m_creature->SummonCreature(27651, 1177.505f,  937.96f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					m_creature->SummonCreature(27651, 1177.505f,  937.96f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					m_creature->SummonCreature(27653, 1177.505f,  937.96f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					FightPhase = RING_TWO;
					break;
				case RING_TWO:
					m_creature->SummonCreature(27645, 968.941f, 1045.45f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					m_creature->SummonCreature(27642, 968.941f, 1042.45f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					m_creature->SummonCreature(27644, 966.941f, 1045.45f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					FightPhase = RING_THREE;
					break;
				case RING_THREE:
					m_creature->SummonCreature(27649, 1163.637f, 1170.95f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					m_creature->SummonCreature(27648, 1163.637f, 1170.95f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					m_creature->SummonCreature(27647, 1163.637f, 1170.95f, 527.32f,0,TEMPSUMMON_DEAD_DESPAWN,0);
					FightPhase = CENTER;
					break;
				}
				TeleportTimer = 0;
				isEscaping = false;
			} else TeleportTimer -= diff;
		}

		if(CanCatchCombatTimer)
		{
			if(CanCatchCombatTimer <= diff)
			{
				CanCatchCombatTimer = 0;
			} else CanCatchCombatTimer -= diff;
		}

		if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
		{
			if((FightPhase == CENTER) && CenterEngaged)
				EnterEvadeMode();
			return;
		}

		if(FightPhase == CENTER)
		{
			if(FrostBombTimer <= diff)
			{
				if(DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), SPELL_FROSTBOMB) == CAST_OK)
					FrostBombTimer = urand(40000,50000);
			} else FrostBombTimer -= diff;

			if(TimeBombTimer <= diff)
			{
				TimeBomb = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
				if(DoCastSpellIfCan(TimeBomb, m_bIsRegularMode? SPELL_TIMEBOMB : SPELL_TIMEBOMB_H) == CAST_OK)
				{
					TimeBombDetonateTimer = 6000;
					TimeBombTimer = urand(8000,12000);
				}
			} else TimeBombTimer -= diff;

			if(TimeBombDetonateTimer)
			{
				if(TimeBombDetonateTimer <= diff)
				{
					TimeBombDetonate();
					TimeBombDetonateTimer = 0;
				} else TimeBombDetonateTimer -= diff;
			}

			if(CentralTeleportTimer <= diff)
			{
				//DoCastSpellIfCan(m_creature,SPELL_TELEPORT);
				//m_creature->GetMotionMaster()->MoveIdle();
				ArcaneExplosionTimer = 2000;
				CentralTeleportTimer = urand(28000,38000);
				FrostBombTimer += 10000;
				TimeBombTimer += 10000;
			} else CentralTeleportTimer -= diff;

			if(ArcaneExplosionTimer)
			{
				if(ArcaneExplosionTimer <= diff)
				{
					DoScriptText( urand(0,1)? SAY_EXPL1 : SAY_EXPL2, m_creature);
					//m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());  -- this make event start over right after text
					DoCastSpellIfCan(m_creature, m_bIsRegularMode? SPELL_ARCANE_EXPLOSION : SPELL_ARCANE_EXPLOSION_H);
					ArcaneExplosionTimer = 0;
				} else ArcaneExplosionTimer -= diff;
			}
			DoMeleeAttackIfReady();
		}
	}
};

CreatureAI* GetAI_boss_urom(Creature* pCreature)
{
	return new boss_uromAI(pCreature);
}

void AddSC_boss_urom()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "boss_urom";
	newscript->GetAI = &GetAI_boss_urom;
	newscript->RegisterSelf();
}
