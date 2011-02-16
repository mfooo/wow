//Halion, The Twilight Destroyer

#include "precompiled.h"

enum
{
	SAY_AGGRO				= -2000000,		//Your intervention has been a nuisance long enough! Prepare to perish...
	SAY_KILL				= -2000001,		//Weak.. The twilight dragonflight is infinite!
	SAY_DIE					= -2000002,		//The master was right... To be concerned... He will unleash devestation upon.. you.. all..
	SAY_BERSERK				= -2000003,		// Enough.. The infinite dragonflight is unboundary in power! FEEL MY EXERTION!
	SAY_CURSEOFMENDING		= -2000004,     // Pity.. Even your futile attacks betray you!
	SAY_CORRUPTINGBLIGHT	= -2000005,     //Watch as your own timeline becomes deteriorated!

	SPELL_SHADOWBREATH		= 57570,
	SPELL_SHROUDOFSORROW		= 72981,
	SPELL_TAILSWEEP			= 15847,
	SPELL_WELLOFCORRUPTION	= 72362,
	SPELL_CORRUPTINGBLIGHT	= 60588,
	SPELL_CURSEOFEXERTION	= 52772,
	SPELL_CURSEOFMENDING	= 39647,
	SPELL_TWILIGHTBLOODBOLT	= 71446,
	SPELL_DELIRIOUSSLASH	= 71623,
	SPELL_BERSERK			= 62555
};

struct MANGOS_DLL_DECL boss_halionAI : public ScriptedAI
{
	boss_halionAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 m_uiSpellShadowBreathTimer;
	uint32 m_uiSpellTailSweepTimer;
	uint32 m_uiSpellWellOfCorruptionTimer;
	uint32 m_uiSpellCorruptingBlightTimer;
	uint32 m_uiSpellCurseOfExertionTimer;
	uint32 m_uiSpellCurseOfMendingTimer;
	uint32 m_uiSpellTwilightBloodBoltTimer;
	uint32 m_uiSpellDeliriousslashTimer;
	uint32 m_uiSpellBerserkTimer;

	void Reset()
	{
		m_uiSpellShadowBreathTimer = 25000;
		m_uiSpellTailSweepTimer = 10000;
		m_uiSpellWellOfCorruptionTimer = 45000;
		m_uiSpellCorruptingBlightTimer = 20000;
		m_uiSpellCurseOfExertionTimer = 25000;
		m_uiSpellCurseOfMendingTimer = 60000;
		m_uiSpellTwilightBloodBoltTimer = 30000;
		m_uiSpellDeliriousslashTimer = 17000;
		m_uiSpellBerserkTimer = 400000;

	}

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);
		DoCast(m_creature, SPELL_SHROUDOFSORROW);
	}

	void KilledUnit(Unit* pVictim)
	{
		DoScriptText(SAY_KILL, m_creature);
	}

	void JustDied(Unit*) 
	{
		DoScriptText(SAY_DIE, m_creature);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiSpellShadowBreathTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_SHADOWBREATH);
			m_uiSpellShadowBreathTimer = 25000;
		}
		else
			m_uiSpellShadowBreathTimer -= uiDiff;

		if(m_uiSpellTailSweepTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_TAILSWEEP);
			m_uiSpellTailSweepTimer = 10000;
		}
		else
			m_uiSpellTailSweepTimer -= uiDiff;

		if(m_uiSpellWellOfCorruptionTimer < uiDiff)
		{
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCast(target, SPELL_WELLOFCORRUPTION);
				
				m_uiSpellWellOfCorruptionTimer = 45000;
		}
		else
			m_uiSpellWellOfCorruptionTimer -= uiDiff;

		if(m_uiSpellCorruptingBlightTimer < uiDiff)
		{	
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			DoCast(target, SPELL_CORRUPTINGBLIGHT);
			DoScriptText(SAY_CORRUPTINGBLIGHT, m_creature);

			m_uiSpellCorruptingBlightTimer = 20000;

		}
		else
			m_uiSpellCorruptingBlightTimer -= uiDiff;

		if(m_uiSpellCurseOfExertionTimer < uiDiff)
		{
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			DoCast(target, SPELL_CURSEOFEXERTION); 

			m_uiSpellCurseOfExertionTimer = 25000;

		}
		else
			m_uiSpellCurseOfExertionTimer -= uiDiff;

		if(m_uiSpellCurseOfMendingTimer < uiDiff)
		{
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			DoCast(target, SPELL_CURSEOFMENDING);
			DoScriptText(SAY_CURSEOFMENDING, m_creature);

			m_uiSpellCurseOfMendingTimer = 60000;
		}
		else
			m_uiSpellCurseOfMendingTimer -= uiDiff;

		if(m_uiSpellTwilightBloodBoltTimer < uiDiff)
		{
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			DoCast(target, SPELL_TWILIGHTBLOODBOLT);

			m_uiSpellTwilightBloodBoltTimer = 30000;
		}
		else
			m_uiSpellTwilightBloodBoltTimer -= uiDiff;

		if(m_uiSpellDeliriousslashTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_DELIRIOUSSLASH);
			m_uiSpellDeliriousslashTimer = 17000;
		}
		else
			m_uiSpellDeliriousslashTimer -= uiDiff;

		if(m_uiSpellBerserkTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_BERSERK);
			DoScriptText(SAY_BERSERK, m_creature);
			m_uiSpellBerserkTimer = 400000;
		}
		else
			m_uiSpellBerserkTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}

};

CreatureAI* GetAI_boss_halion(Creature* pCreature)
{
    return new boss_halionAI(pCreature);
}

void AddSC_boss_halion()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_halion";
    newscript->GetAI = &GetAI_boss_halion;
    newscript->RegisterSelf();
}