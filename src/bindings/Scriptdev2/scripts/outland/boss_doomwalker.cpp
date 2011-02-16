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
SDName: Boss_Doomwalker
SD%Complete: 100
SDComment:
SDCategory: Shadowmoon Valley
EndScriptData */

#include "precompiled.h"

#define SAY_AGGRO                   -1000159
#define SAY_EARTHQUAKE_1            -1000160
#define SAY_EARTHQUAKE_2            -1000161
#define SAY_OVERRUN_1               -1000162
#define SAY_OVERRUN_2               -1000163
#define SAY_SLAY_1                  -1000164
#define SAY_SLAY_2                  -1000165
#define SAY_SLAY_3                  -1000166
#define SAY_DEATH                   -1000167

#define SPELL_EARTHQUAKE            32686
#define SPELL_SUNDER_ARMOR          33661
#define SPELL_CHAIN_LIGHTNING       33665
#define SPELL_OVERRUN               32636
#define SPELL_ENRAGE                33653
#define SPELL_MARK_DEATH            37128
#define SPELL_AURA_DEATH            37131

struct MANGOS_DLL_DECL boss_doomwalkerAI : public ScriptedAI
{
    boss_doomwalkerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Chain_Timer;
    uint32 Enrage_Timer;
    uint32 Overrun_Timer;
    uint32 Quake_Timer;
    uint32 Armor_Timer;

    bool InEnrage;

    void Reset()
    {
        Enrage_Timer    = 0;
        Armor_Timer     = urand(5000, 13000);
        Chain_Timer     = urand(10000, 30000);
        Quake_Timer     = urand(25000, 35000);
        Overrun_Timer   = urand(30000, 45000);

        InEnrage = false;
    }

    void KilledUnit(Unit* Victim)
    {

        Victim->CastSpell(Victim, SPELL_MARK_DEATH, true);

        if (urand(0, 4))
            return;

        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }

    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }


    void MoveInLineOfSight(Unit *who)
    {
        if (who && who->GetTypeId() == TYPEID_PLAYER && m_creature->IsHostileTo(who))
        {
            if (who->HasAura(SPELL_MARK_DEATH, EFFECT_INDEX_0))
            {
                who->CastSpell(who,SPELL_AURA_DEATH,true);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Spell Enrage, when hp <= 20% gain enrage
        if (m_creature->GetHealthPercent() <= 20.0f)
        {
            if (Enrage_Timer < diff)
            {
                DoCastSpellIfCan(m_creature,SPELL_ENRAGE);
                Enrage_Timer = 6000;
                InEnrage = true;
            }else Enrage_Timer -= diff;
        }

        //Spell Overrun
        if (Overrun_Timer < diff)
        {
            DoScriptText(urand(0, 1) ? SAY_OVERRUN_1 : SAY_OVERRUN_2, m_creature);
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_OVERRUN);
            Overrun_Timer = urand(25000, 40000);
        }else Overrun_Timer -= diff;

        //Spell Earthquake
        if (Quake_Timer < diff)
        {
            if (urand(0, 1))
                return;

            DoScriptText(urand(0, 1) ? SAY_EARTHQUAKE_1 : SAY_EARTHQUAKE_2, m_creature);

            //remove enrage before casting earthquake because enrage + earthquake = 16000dmg over 8sec and all dead
            if (InEnrage)
                m_creature->RemoveAurasDueToSpell(SPELL_ENRAGE);

            DoCastSpellIfCan(m_creature,SPELL_EARTHQUAKE);
            Quake_Timer = urand(30000, 55000);
        }else Quake_Timer -= diff;

        //Spell Chain Lightning
        if (Chain_Timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1);

            if (!target)
                target = m_creature->getVictim();

            if (target)
                DoCastSpellIfCan(target,SPELL_CHAIN_LIGHTNING);

            Chain_Timer = urand(7000, 27000);
        }else Chain_Timer -= diff;

        //Spell Sunder Armor
        if (Armor_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SUNDER_ARMOR);
            Armor_Timer = urand(10000, 25000);
        }else Armor_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_doomwalker(Creature* pCreature)
{
    return new boss_doomwalkerAI(pCreature);
}

void AddSC_boss_doomwalker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_doomwalker";
    newscript->GetAI = &GetAI_boss_doomwalker;
    newscript->RegisterSelf();
}
