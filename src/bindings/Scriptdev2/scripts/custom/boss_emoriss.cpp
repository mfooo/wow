//Emoriss Twilight Vanquisher
#include "precompiled.h"

enum
{
	SAY_AGGRO				= -2000000,		//Your intervention has been a nuisance long enough! Prepare to perish...
	SAY_KILL				= -2000001,		//Weak.. The twilight dragonflight is infinite!
	SAY_DIE				= -2000002,		//The master was right... To be concerned... He will unleash devestation upon.. you.. all..
	SAY_BERSERK				= -2000003,		// Enough.. The infinite dragonflight is unboundary in power! FEEL MY EXERTION!
	SAY_CORRUPTINGBLIGHT			= -2000005,     //Watch as your own timeline becomes deteriorated!
	REFLECTION				= 22067,		//PHASE 2    every 27 sec?
	DRAINMANA				= 46453,		//ALL PHASES, every 50 sec
	SHROUDOFSARROW			= 72982,		//Start spell
	TAILSMASH				= 71077,		// Every 40 Sec , phase 1
	SHADOWBREATH				= 59126,		// Every 30 Sec , phase 1 & 3
	STRONGCLEAVE				= 19983,		//Every 45 Sec, phase 1 & 3
	BLISTERINGCOLD		 	= 70123,		//phase 3 , 58 sec
	FROSTBOMB				= 71053,		//phase 2 , end of it
	SHADOWBOLT				= 64698,		// phase 2, every 15 sec, random target
	CORRUPTINGBLIGHT			= 60588,        // PHASE 1 & 3 , every 25 Sec, on tank
	SPELL_BERSERK				= 62555,		// berserk 1.5%

	TENEBRONID				= 30452,
	SHADRONID				= 30451,
	VESPERONID				= 30449,

};

struct MANGOS_DLL_DECL boss_emorissAI : public ScriptedAI
{
	boss_emorissAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	bool berserk;
	uint32 PHASE;
	uint32 flyTimer;

	bool tenebron;
	bool shadron;
	bool vesperon;
	uint32 boss_Timer;
	uint32 Reflection_Timer;
	uint32 Drainmana_Timer;
	uint32 Tailsmash_Timer;
	uint32 Shadowbreath_Timer;
	uint32 Strongcleave_Timer;
	uint32 Blisteringcold_Timer;
	uint32 Shadowbolt_Timer;
	uint32 Corruptingblight_Timer;

	void Reset()
	{
		PHASE			= 1;
		berserk			= false;



		Reflection_Timer = 10000;
		Drainmana_Timer		= 50000;
		Tailsmash_Timer		= urand(48000, 49000);
		Shadowbreath_Timer	= 30000;
		Strongcleave_Timer	= 45000;
		Blisteringcold_Timer = urand(56000, 58000);
		Shadowbolt_Timer	= urand(13000, 14000);
		Corruptingblight_Timer	= 25500;

		flyTimer			= 6000;
		tenebron = false;
		shadron = false;
		vesperon = false;
		boss_Timer = 80000;

	}

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);
		DoCastSpellIfCan(m_creature, SHROUDOFSARROW);
	}

	void KilledUnit(Unit* pVictim)
	{
		DoScriptText(SAY_KILL, m_creature);
	}

	void JustDied(Unit*)
	{
		DoScriptText(SAY_DIE, m_creature);
	}

	void JustSummoned(Creature *pSummoned)
	{
		if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
		pSummoned->AI()->AttackStart(pTarget);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
		return;

		if (PHASE == 1)
		{
			if(Drainmana_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), DRAINMANA);
				Drainmana_Timer		= 52000;
			}
			else
				Drainmana_Timer -= uiDiff;

			if(Tailsmash_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, TAILSMASH);
				Tailsmash_Timer		= urand(38000, 39000);
			}
			else
				Tailsmash_Timer -= uiDiff;

			if(Shadowbreath_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), SHADOWBREATH);
				Shadowbreath_Timer	= 20000;
			}
			else
				Shadowbreath_Timer -= uiDiff;

			if(Strongcleave_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), STRONGCLEAVE);
				Strongcleave_Timer	= 25000;
			}
			else
				Strongcleave_Timer -= uiDiff;

			if(Corruptingblight_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), CORRUPTINGBLIGHT);
				Corruptingblight_Timer	= 15500;
				DoScriptText(SAY_CORRUPTINGBLIGHT, m_creature);
			}
			else
				Corruptingblight_Timer -= uiDiff;

			if (m_creature->GetHealthPercent() < 65.0f)
			{
				PHASE = 2;
				SetCombatMovement(false);
				m_creature->InterruptNonMeleeSpells(false);
				m_creature->GetMotionMaster()->Clear(false);
				m_creature->GetMotionMaster()->MoveIdle();
				DoCastSpellIfCan(m_creature,57764);
				m_creature->GetMotionMaster()->MovePoint(0, 3242.49, 523.65, 77.39);	
				m_creature->SetHover(true);
			}


		}

		if(PHASE == 2)
		{

			if(flyTimer < uiDiff)
			{
				switch(urand(1, 4))
				{
					case 1:
					m_creature->GetMotionMaster()->MovePoint(0, 3242.49f, 523.65f, 77.39f);
					break;
					case 2:
					m_creature->GetMotionMaster()->MovePoint(0, 3250.58f, 550.20f, 80.56f);
					break;
					case 3:
					m_creature->GetMotionMaster()->MovePoint(0, 3264.53f, 536.81f, 76.52f);
					break;
					case 4:
					m_creature->GetMotionMaster()->MovePoint(0, 3233.67f, 524.43f, 73.70f);
					break;
				}
				flyTimer = 10000;
			}
			else
				flyTimer -= uiDiff;

			if(!tenebron)
			{
				m_creature->SummonCreature(TENEBRONID, 3243.07f, 499.38f, 59.81, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
				tenebron = true;
			}

			if(boss_Timer < uiDiff && tenebron && !shadron)
			{
				m_creature->SummonCreature(SHADRONID, 3243.07f, 499.38f, 59.81f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
				shadron = true;
				boss_Timer = 80000;
			}
			else
			{	if(tenebron && !shadron)
				boss_Timer -= uiDiff;
			}

			if(boss_Timer < uiDiff && shadron && tenebron && !vesperon)
			{
				m_creature->SummonCreature(VESPERONID, 3243.07f, 499.38f, 59.81f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
				vesperon = true;
				boss_Timer = 80000;
			}
			else
			{
				if(shadron && tenebron && !vesperon)
				boss_Timer -= uiDiff;
			}

			if(Reflection_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature,REFLECTION);
				Reflection_Timer = 10000;
			}
			else
				Reflection_Timer -= uiDiff;

			if(Drainmana_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), DRAINMANA);
				Drainmana_Timer		= 53000;
			}
			else
				Drainmana_Timer -= uiDiff;

			if(Shadowbolt_Timer < uiDiff)
			{
				if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCastSpellIfCan(target,SHADOWBOLT);

				Shadowbolt_Timer	= urand(13000, 14000);
			}
			else
				Shadowbolt_Timer -= uiDiff;

			if(boss_Timer < uiDiff && shadron && tenebron && vesperon)
			{
				PHASE = 3;
				m_creature->SetHover(false);
				m_creature->GetMotionMaster()->Clear(false);
				SetCombatMovement(true);
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				DoCastSpellIfCan(m_creature->getVictim(),FROSTBOMB);
			}
			else
			{
				if(shadron && tenebron && vesperon)
				boss_Timer -= uiDiff;
			}
		}

		if(PHASE == 3)
		{
			if (m_creature->HasAura(57764))
			m_creature->RemoveAurasDueToSpell(57764);

			if(Drainmana_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), DRAINMANA);
				Drainmana_Timer		= 43500;
			}
			else
				Drainmana_Timer -= uiDiff;
			
			if(Shadowbreath_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), SHADOWBREATH);
				Shadowbreath_Timer	= 20000;
			}
			else
				Shadowbreath_Timer -= uiDiff;

			if(Strongcleave_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), STRONGCLEAVE);
				Strongcleave_Timer	= 35000;
			}
			else
				Strongcleave_Timer -= uiDiff;

			if(Blisteringcold_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), BLISTERINGCOLD);
				Blisteringcold_Timer = urand(46000, 48000);
			}
			else
				Blisteringcold_Timer -= uiDiff;

			if(Corruptingblight_Timer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), CORRUPTINGBLIGHT);
				Corruptingblight_Timer	= 15500;
				DoScriptText(SAY_CORRUPTINGBLIGHT, m_creature);
			}
			else
				Corruptingblight_Timer -= uiDiff;

			if(m_creature->GetHealthPercent() < 1.5f && !berserk)
			{
		   		berserk = true;
				DoScriptText(SAY_AGGRO, m_creature);
				DoCastSpellIfCan(m_creature,SPELL_BERSERK);
			}	
					
		}


		if (PHASE!=2)
            DoMeleeAttackIfReady();


	}

};

CreatureAI* GetAI_boss_emoriss(Creature* pCreature)
{
    return new boss_emorissAI(pCreature);
}

void AddSC_boss_emoriss()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_emoriss";
    NewScript->GetAI = &GetAI_boss_emoriss;
    NewScript->RegisterSelf();
}
