/* Custom boss Razoz */

#include "precompiled.h"

enum
{
	SAY_AGGRO		= -1999900,
	SAY_APOCALYPSE  = -2000004,
	SAY_PHASEONE	= -2000002,
	SAY_PHASETWO	= -2000003,
	SAY_PHASETHREE  = -1999907,
	SAY_KILL		= -1249001,

	SPELL_FLAMEBREATH		        = 70116,		//Frost Breath
	SPELL_HATEDGROUND		= 72905,		//FRostbolt Voley
	SPELL_APOCALYPSE		= 53210,		// Apocalypse
	SPELL_ENRAGE		    = 68335,		//Enrage , increase damage by 50%
	//SPELL_CLEAVE		    = 19983,		//CLEAVE
	SPELL_HEAL		        = 71131,		//GREAT HEAL  PHASE 2
	SPELL_PYROBLAST			= 61603,		// death and decay
	SPELL_FEAR				= 65809,        // FEAR
	SPELL_BONESTORM		    = 63784,		// PHASE 2
	SPELL_SHOCKWAVE 		= 63783,		// PHASE 2

	PHASE_ONE		= 1,
	PHASE_TWO		= 2,
	PHASE_THREE		= 3
};

struct MANGOS_DLL_DECL boss_razorAI : public ScriptedAI
{
	boss_razorAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}
	uint32 m_uiPhase;

    uint32 m_uiFlameBreathTimer;                               
    uint32 m_uiSpellHatedGroundTimer;								
    uint32 m_uiSpellApocalypseTimer;
	uint32 m_uiSpellDivineTimer;
	uint32 m_uiSpellEnrageTimer;
	//uint32 m_uiSpellCleaveTimer;
	uint32 m_uiSpellHealTimer;
	uint32 m_uiSpellPyroblastTimer;
	uint32 m_uiSpellBoneStormTimer;
	uint32 m_uiSpellShockwaveTimer;
	uint32 m_uiSpellFearTimer;

	void Reset()
    {

		m_uiPhase = PHASE_ONE;

		m_uiFlameBreathTimer = urand(10000, 20000);
		m_uiSpellHatedGroundTimer = urand(15000, 18000);
		m_uiSpellApocalypseTimer = urand(60000, 70000);
		m_uiSpellEnrageTimer = 15000;
		//m_uiSpellCleaveTimer = urand(8000, 10000);
		m_uiSpellHealTimer = urand(8000,11000);
		m_uiSpellPyroblastTimer = urand(10000, 11000);
		m_uiSpellBoneStormTimer = urand(15000, 16000);
		m_uiSpellShockwaveTimer = urand(11000, 13000);
		m_uiSpellFearTimer = urand(13000, 14000);

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

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		if (m_uiPhase == PHASE_ONE || m_uiPhase == PHASE_THREE)
        {
			if (m_uiFlameBreathTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAMEBREATH);
				m_uiFlameBreathTimer = urand(10000, 20000);
			}
			else
				m_uiFlameBreathTimer -= uiDiff;

			if (m_uiSpellHatedGroundTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_HATEDGROUND);
				m_uiSpellHatedGroundTimer = urand(15000, 18000);
			}
			else
				m_uiSpellHatedGroundTimer -= uiDiff;

			if (m_uiSpellApocalypseTimer < uiDiff)
			{
				DoScriptText(SAY_APOCALYPSE, m_creature);
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_APOCALYPSE);
			    m_uiSpellApocalypseTimer = urand(60000, 70000);
			}
			else
				m_uiSpellApocalypseTimer -= uiDiff;
				
			/*if (m_uiSpellCleaveTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
			    m_uiSpellCleaveTimer = urand(8000, 10000);
			}
			else
				m_uiSpellCleaveTimer -= uiDiff;*/

			if (m_uiSpellPyroblastTimer < uiDiff)
			{
				if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                            DoCastSpellIfCan(target, SPELL_PYROBLAST);
						
						m_uiSpellPyroblastTimer = urand(10000, 11000);
			}
			else 
				m_uiSpellPyroblastTimer -= uiDiff;

			if (m_uiSpellFearTimer < uiDiff)
			{
				if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                        DoCastSpellIfCan(target, SPELL_FEAR);
						
						m_uiSpellFearTimer = urand(13000, 14000);
			}
			else 
				m_uiSpellFearTimer -= uiDiff;
		
			if (m_uiPhase == PHASE_THREE)  //phase 3
			{
				if (m_uiSpellEnrageTimer < uiDiff)
				{
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_ENRAGE);
				m_uiSpellEnrageTimer = 15000;
				}
				else
					m_uiSpellEnrageTimer -= uiDiff ;

				if (m_uiSpellBoneStormTimer < uiDiff)
				{
					DoCastSpellIfCan(m_creature, SPELL_BONESTORM);
					m_uiSpellBoneStormTimer = urand(15000, 16000);
				}
				else
					m_uiSpellBoneStormTimer -= uiDiff;

			}
			else
			{
				if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 30)
				{
					m_uiPhase = PHASE_TWO;

					DoScriptText(SAY_PHASETWO, m_creature);
					return;

				}
			}

			DoMeleeAttackIfReady();
		}
		else				//PHASE 2
		{
			if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 20)   // PHASE3
			{
				m_uiPhase = PHASE_THREE;
				DoScriptText(SAY_PHASETHREE, m_creature);

				return;
			}

			if (m_uiSpellHealTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_HEAL);
				m_uiSpellHealTimer = urand(8000,11000);
			}
			else
				m_uiSpellHealTimer -= uiDiff;
			
			if (m_uiSpellShockwaveTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHOCKWAVE);
				m_uiSpellShockwaveTimer = urand(11000, 13000);
			}
			else
				m_uiSpellShockwaveTimer -= uiDiff;

			if (m_uiSpellBoneStormTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_BONESTORM);
				m_uiSpellBoneStormTimer = urand(15000, 16000);
			}
			else
				m_uiSpellBoneStormTimer -= uiDiff;
		}

		DoMeleeAttackIfReady();
	}
	
};

CreatureAI* GetAI_boss_razorAI(Creature* pCreature)
{
    return new boss_razorAI(pCreature);
}

void AddSC_boss_razor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_razor";
    newscript->GetAI = &GetAI_boss_razorAI;
    newscript->RegisterSelf();
}

	






						
						
						



			




	


