#include "precompiled.h"

enum
{	
    SAY_AGGRO       = -1999900,
    SAY_BESERK      = -1999906,
    SAY_PHASE       = -1999907,
	SPELL_ONE	    = 64698,			//Pyroblast
	SPELL_BESERK    = 32309,
	SPELL_TWO       = 57668,		//frost nova
	SPELL_THREE     = 41524,		//arcane explosion
	SPELL_FOUR		= 59159,		//thunderstorm
	SPELL_ENRAGE    = 23537,
    SPELL_FIVE		= 67996,	    // ARcane barrage
	SPELL_SIX		= 33129,		//	DOT
	SPELL_SEVEN	    = 41410,				// DEBUFF
	SPELL_EIGHT		= 41937					// LOT OF DAMAGE!
};

struct MANGOS_DLL_DECL custom_bossAI : public ScriptedAI
{
	custom_bossAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}
    //Timer for random chat
    uint32 m_uiRebuff_Timer;                                //Timer for rebuffing
    uint32 m_uiSpell_1_Timer;                               //Timer for spell 1 when in combat
    uint32 m_uiSpell_2_Timer;                               //Timer for spell 1 when in combat
    uint32 m_uiSpell_3_Timer;								//Timer for spell 1 when in combat
    uint32 m_uiSpell_4_Timer;
	uint32 m_uiSpell_5_Timer;
	uint32 m_uiSpell_6_Timer;
	uint32 m_uiSpell_7_Timer;
	uint32 m_uiSpell_8_Timer;
	uint32 m_uiBeserk_Timer;                                //Timer until we go into Beserk (enraged) mode
    uint32 m_uiPhase;                                       //The current battle phase we are in
    uint32 m_uiPhase_Timer;                                 //Timer until phase transition

	void Reset()
    {
        m_uiPhase = 1;                                      //Start in phase 1
        m_uiPhase_Timer = 100000;                            //100 seconds
        m_uiSpell_1_Timer = 11000;                           //11 seconds
		m_uiSpell_2_Timer = 21000;							// 21 sec
		m_uiSpell_3_Timer = 14000;							// 14 sec
		m_uiSpell_4_Timer = 16000;
		m_uiSpell_5_Timer = 17000;
		m_uiSpell_6_Timer = 10000;
		m_uiSpell_7_Timer = 28000;
		m_uiSpell_8_Timer = 30000;
		m_uiBeserk_Timer = 300000;                          //120sec
    }

	void Aggro(Unit* pWho)
    {
        //Say some stuff
        DoScriptText(SAY_AGGRO, m_creature, pWho);
    }
        

		//*** HANDLED FUNCTION ***
    //Update AI is called Every single map update (roughly once every 100ms if a player is within the grid)
    void UpdateAI(const uint32 uiDiff)
    {
		//Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Spell 1 timer
        if (m_uiSpell_1_Timer < uiDiff)
        {
            //Cast spell one on our current target.
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ONE);
            m_uiSpell_1_Timer = 11000;
        }
        else
            m_uiSpell_1_Timer -= uiDiff;

		//Spell 2 timer
        if (m_uiSpell_2_Timer < uiDiff)
        {
            //Cast spell one on our current target.
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_TWO);
            m_uiSpell_2_Timer = 21000;
        }
        else
            m_uiSpell_2_Timer -= uiDiff;

		//Spell 3 timer
        if (m_uiSpell_3_Timer < uiDiff)
        {
            //Cast spell one on our current target.
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_THREE);
            m_uiSpell_3_Timer = 14000;
        }
        else
            m_uiSpell_3_Timer -= uiDiff;

		//Spell 4 timer
        if (m_uiSpell_4_Timer < uiDiff)
        {
            //Cast spell one on our current target.
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FOUR);
            m_uiSpell_4_Timer = 16000;
        }
        else
            m_uiSpell_4_Timer -= uiDiff;
		
		//Spell 5 timer
        if (m_uiSpell_5_Timer < uiDiff)
        {
            //Cast spell one on our current target.
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIVE);
            m_uiSpell_5_Timer = 17000;
        }
        else
            m_uiSpell_5_Timer -= uiDiff;

				//Spell 6 timer
        if (m_uiSpell_6_Timer < uiDiff)
        {
            //Cast spell one on our current target.
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SIX);
            m_uiSpell_6_Timer = 10000;
        }
        else
            m_uiSpell_6_Timer -= uiDiff;
				
		//Spell 7 timer
        if (m_uiSpell_7_Timer < uiDiff)
        {
            //Cast spell one on our current target.
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SEVEN);
            m_uiSpell_7_Timer = 28000;
        }
        else
            m_uiSpell_7_Timer -= uiDiff;

		//Beserk timer
        if (m_uiPhase > 1)
        {
            //Spell Thunderstorm timer
            if (m_uiSpell_3_Timer < uiDiff)
            {
                //Cast spell one on our current target.
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_EIGHT);

                m_uiSpell_3_Timer = 30000;
            }
            else
                m_uiSpell_3_Timer -= uiDiff;

            if (m_uiBeserk_Timer < uiDiff)
            {
                //Say our line then cast uber death spell
                DoScriptText(SAY_BESERK, m_creature, m_creature->getVictim());
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_BESERK);

                //Cast our beserk spell agian in 120 seconds if we didn't kill everyone
                m_uiBeserk_Timer = 300000;
            }
            else
                m_uiBeserk_Timer -= uiDiff;
        }
        else if (m_uiPhase == 1)                            //Phase timer
        {
            if (m_uiPhase_Timer < uiDiff)
            {
                //Go to next phase
                ++m_uiPhase;
                DoScriptText(SAY_PHASE, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            }
            else
                m_uiPhase_Timer -= uiDiff;
        }


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_custom_bossAI(Creature* pCreature)
{
    return new custom_bossAI(pCreature);
}

void AddSC_custom_boss()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "custom_boss";
    newscript->GetAI = &GetAI_custom_bossAI;
    newscript->RegisterSelf();
}