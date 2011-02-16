//Custom boss PVP

#include "precompiled.h"


enum
{
	SAY_AGGRO       = -1999900,
	SAY_KILL		= -1249001,
	SAY_OVERLOAD	= -2000010,				// COME TO ME!!!


	SPELL_FELFIREBALL		= 66532,		//FEL FIREBALL	alle phases *
	SPELL_NETHERPOWER		= 67108,		// BUFF NETHER POWER	*
	SPELL_FROSTBURN			= 23187,        // FROST BURN    *
	SPELL_BALLLIGHTNING		= 59800,		// BALL LIGHTNING  *
	SPELL_SONICBOOM		    = 38897,		//SONIC BOOM , Against CAsters		*
	SPELL_SHADOWBLAST		= 70866,		//SHADOW BLAST , GOod for melee knock back *
	SPELL_OVERLOAD		    = 63481,		//OVERLOAD , Niet te vaak  bij 80% 60% 40% 20% *
	SPELL_FRENZY	        = 68335,		//FRENZY   30% *
	SPELL_HEAL		        = 71131,		//GREAT HEAL  PHASE 2			*
	SPELL_DIVINESHIELD		= 642,			// DIVINE SHIELD			*

	PHASE_ONE				= 1,
	PHASE_TWO				= 2,
	PHASE_THREE				= 3

};

struct MANGOS_DLL_DECL boss_pvpAI : public ScriptedAI

{
	boss_pvpAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 m_uiPhase;

	uint32 m_uiSpellFelFireballTimer;
	uint32 m_uiSpellNetherpowerTimer;
	uint32 m_uiSpellFrenzyTimer;
	uint32 m_uiSpellHealTimer;
	uint32 m_uiSpellSonicBoomTimer;
	uint32 m_uiSpellDivineShieldTimer;
	uint32 m_uiSpellBallLightningTimer;
	uint32 m_uiSpellFrostBurnTimer;
	uint32 m_uiSpellShadowBlastTimer;

	void Reset()
    {

		m_uiPhase = PHASE_ONE;

		m_uiSpellFelFireballTimer = urand(10000, 12000);
		m_uiSpellNetherpowerTimer = urand(7000, 9000);
		m_uiSpellHealTimer		  = urand(12000, 13000);
		m_uiSpellSonicBoomTimer	  = urand(4000, 4500);
		m_uiSpellDivineShieldTimer = urand(30000, 31000);
		m_uiSpellBallLightningTimer = urand(17000, 19000);
		m_uiSpellFrostBurnTimer     = urand(28000, 30000);
		m_uiSpellShadowBlastTimer   = urand(22000, 24000); 
		m_uiSpellFrenzyTimer        = urand(10000,12000);

	}

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);
	}

	void KilledUnit(Unit* pVictim)
	{
		DoScriptText(SAY_KILL, m_creature);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;


		if(m_uiSpellFelFireballTimer < uiDiff)
		{
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCastSpellIfCan(target, SPELL_FELFIREBALL);
				
				m_uiSpellFelFireballTimer = urand(10000, 12000);
		}
		else
			m_uiSpellFelFireballTimer -= uiDiff;

		if(m_uiSpellFrostBurnTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBURN);
			
			m_uiSpellFrostBurnTimer     = urand(28000, 30000);
		}
		else
			m_uiSpellFrostBurnTimer -= uiDiff;


		if(m_uiSpellSonicBoomTimer < uiDiff)
		{
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCastSpellIfCan(target, SPELL_SONICBOOM);
				
				m_uiSpellSonicBoomTimer = urand(4000, 4500);
		}
		else
			m_uiSpellSonicBoomTimer -= uiDiff;		

		if(m_uiPhase == PHASE_ONE || m_uiPhase == PHASE_THREE)
		{

			if(m_uiSpellNetherpowerTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_NETHERPOWER);

			    m_uiSpellNetherpowerTimer = urand(7000, 9000);
			}
			else
				m_uiSpellNetherpowerTimer -= uiDiff;

			if(m_uiSpellShadowBlastTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_SHADOWBLAST);
			    m_uiSpellShadowBlastTimer   = urand(22000, 24000);
			}
			else
				m_uiSpellShadowBlastTimer -= uiDiff;


			if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() == 79 || m_creature->GetHealth()*100 / m_creature->GetMaxHealth() == 19)
			{
				DoScriptText(SAY_OVERLOAD, m_creature);
				DoCastSpellIfCan(m_creature, SPELL_OVERLOAD);
			}

			if(m_uiPhase == PHASE_THREE && m_uiSpellFrenzyTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_FRENZY);

			    m_uiSpellFrenzyTimer      = urand(20000, 22000);
			}
			else
				m_uiSpellFrenzyTimer -= uiDiff;

			if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 10 && m_uiSpellFrenzyTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_FRENZY);

				m_uiSpellFrenzyTimer = (10000,12000);
			}
			else
				m_uiSpellFrenzyTimer -= uiDiff;




			if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 60 && m_uiPhase == PHASE_ONE)
			{
				DoScriptText(SAY_OVERLOAD, m_creature);
				DoCastSpellIfCan(m_creature, SPELL_OVERLOAD);

				m_uiPhase = PHASE_TWO;

				return;
			}
			DoMeleeAttackIfReady();

		}
		else
		{
			if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 30)
			{

				m_uiPhase = PHASE_THREE;
				return;
			}
			else //spells of phase 2 here
			{
				if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() == 40)
				{
					DoScriptText(SAY_OVERLOAD, m_creature);
					DoCastSpellIfCan(m_creature, SPELL_OVERLOAD);
				}

				if(m_uiSpellHealTimer < uiDiff)
				{
					DoCastSpellIfCan(m_creature, SPELL_HEAL);

					m_uiSpellHealTimer = urand(12000, 13000);
				}
				else
					m_uiSpellHealTimer -= uiDiff;

				if(m_uiSpellDivineShieldTimer < uiDiff)
				{
					DoCastSpellIfCan(m_creature, SPELL_DIVINESHIELD);

					m_uiSpellDivineShieldTimer = urand(30000, 31000);
				}
				else
					m_uiSpellDivineShieldTimer -= uiDiff;

				if(m_uiSpellBallLightningTimer < uiDiff)
				{
					if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
					DoCastSpellIfCan(pTarget, SPELL_BALLLIGHTNING);
				
					m_uiSpellBallLightningTimer = urand(17000, 19000);
				}
				else
					m_uiSpellBallLightningTimer -= uiDiff;
			}
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_pvpAI(Creature* pCreature)
{
    return new boss_pvpAI(pCreature);
}

void AddSC_boss_pvp()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_pvp";
    newscript->GetAI = &GetAI_boss_pvpAI;
    newscript->RegisterSelf();
}


