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
SDName: Boss_Doomlord_Kazzak
SD%Complete: 70
SDComment: Using incorrect spell for Mark of Kazzak
SDCategory: Hellfire Peninsula
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_INTRO                       = -1000147,
    SAY_AGGRO1                      = -1000148,
    SAY_AGGRO2                      = -1000149,
    SAY_SURPREME1                   = -1000150,
    SAY_SURPREME2                   = -1000151,
    SAY_KILL1                       = -1000152,
    SAY_KILL2                       = -1000153,
    SAY_KILL3                       = -1000154,
    SAY_DEATH                       = -1000155,
    EMOTE_GENERIC_FRENZY            = -1000002,
    SAY_RAND1                       = -1000157,
    SAY_RAND2                       = -1000158,

    SPELL_SHADOWVOLLEY              = 32963,
    SPELL_CLEAVE                    = 31779,
    SPELL_THUNDERCLAP               = 36706,
    SPELL_VOIDBOLT                  = 39329,
    SPELL_MARKOFKAZZAK              = 32960,
    SPELL_ENRAGE                    = 32964,
    SPELL_CAPTURESOUL               = 32966,
    SPELL_TWISTEDREFLECTION         = 21063
};

struct MANGOS_DLL_DECL boss_doomlordkazzakAI : public ScriptedAI
{
    boss_doomlordkazzakAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 ShadowVolley_Timer;
    uint32 Cleave_Timer;
    uint32 ThunderClap_Timer;
    uint32 VoidBolt_Timer;
    uint32 MarkOfKazzak_Timer;
    uint32 Enrage_Timer;
    uint32 Twisted_Reflection_Timer;

    void Reset()
    {
        ShadowVolley_Timer = urand(6000, 10000);
        Cleave_Timer = 7000;
        ThunderClap_Timer = urand(14000, 18000);
        VoidBolt_Timer = 30000;
        MarkOfKazzak_Timer = 25000;
        Enrage_Timer = 60000;
        Twisted_Reflection_Timer = 33000;                   // Timer may be incorrect
    }

    void JustRespawned()
    {
        DoScriptText(SAY_INTRO, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(urand(0, 1) ? SAY_AGGRO1 : SAY_AGGRO2, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
        // When Kazzak kills a player (not pets/totems), he regens some health
        if (victim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoCastSpellIfCan(m_creature,SPELL_CAPTURESOUL);

        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
            case 2: DoScriptText(SAY_KILL3, m_creature); break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //ShadowVolley_Timer
        if (ShadowVolley_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWVOLLEY);
            ShadowVolley_Timer = urand(4000, 6000);
        }else ShadowVolley_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = urand(8000, 12000);
        }else Cleave_Timer -= diff;

        //ThunderClap_Timer
        if (ThunderClap_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_THUNDERCLAP);
            ThunderClap_Timer = urand(10000, 14000);
        }else ThunderClap_Timer -= diff;

        //VoidBolt_Timer
        if (VoidBolt_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_VOIDBOLT);
            VoidBolt_Timer = urand(15000, 18000);
        }else VoidBolt_Timer -= diff;

        //MarkOfKazzak_Timer
        if (MarkOfKazzak_Timer < diff)
        {
            Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (victim)
                if (victim->GetPower(POWER_MANA))
                {
                    DoCastSpellIfCan(victim, SPELL_MARKOFKAZZAK);
                    MarkOfKazzak_Timer = 20000;
                }
        }else MarkOfKazzak_Timer -= diff;

        //Enrage_Timer
        if (Enrage_Timer < diff)
        {
            DoScriptText(EMOTE_GENERIC_FRENZY, m_creature);
            DoCastSpellIfCan(m_creature,SPELL_ENRAGE);
            Enrage_Timer = 30000;
        }else Enrage_Timer -= diff;

        if (Twisted_Reflection_Timer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_TWISTEDREFLECTION);

            Twisted_Reflection_Timer = 15000;
        }else Twisted_Reflection_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_doomlordkazzak(Creature* pCreature)
{
    return new boss_doomlordkazzakAI(pCreature);
}

void AddSC_boss_doomlordkazzak()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_doomlord_kazzak";
    newscript->GetAI = &GetAI_boss_doomlordkazzak;
    newscript->RegisterSelf();
}
