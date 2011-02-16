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
SDName: Boss_Captain_Skarloc
SD%Complete: 75
SDComment: Missing adds, missing waypoints to move up to Thrall once spawned + speech before enter combat.
SDCategory: Caverns of Time, Old Hillsbrad Foothills
EndScriptData */

#include "precompiled.h"
#include "old_hillsbrad.h"

#define SAY_ENTER                   -1560000
#define SAY_TAUNT1                  -1560001
#define SAY_TAUNT2                  -1560002
#define SAY_SLAY1                   -1560003
#define SAY_SLAY2                   -1560004
#define SAY_DEATH                   -1560005

#define SPELL_HOLY_LIGHT            29427
#define SPELL_CLEANSE               29380
#define SPELL_HAMMER_OF_JUSTICE     13005
#define SPELL_HOLY_SHIELD           31904
#define SPELL_DEVOTION_AURA         8258
#define SPELL_CONSECRATION          38385

struct MANGOS_DLL_DECL boss_captain_skarlocAI : public ScriptedAI
{
    boss_captain_skarlocAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 Holy_Light_Timer;
    uint32 Cleanse_Timer;
    uint32 HammerOfJustice_Timer;
    uint32 HolyShield_Timer;
    uint32 DevotionAura_Timer;
    uint32 Consecration_Timer;

    void Reset()
    {
        Holy_Light_Timer = 30000;
        Cleanse_Timer = 10000;
        HammerOfJustice_Timer = 60000;
        HolyShield_Timer = 240000;
        DevotionAura_Timer = 3000;
        Consecration_Timer = 8000;
    }

    void Aggro(Unit *who)
    {
        //This is not correct. Should taunt Thrall before engage in combat
        DoScriptText(SAY_TAUNT1, m_creature);
        DoScriptText(SAY_TAUNT2, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance && m_pInstance->GetData(TYPE_THRALL_EVENT) == IN_PROGRESS)
            m_pInstance->SetData(TYPE_THRALL_PART1, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Holy_Light
        if (Holy_Light_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_HOLY_LIGHT);
            Holy_Light_Timer = urand(20000, 30000);
        }else Holy_Light_Timer -= diff;

        //Cleanse
        if (Cleanse_Timer  < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_CLEANSE);
            Cleanse_Timer = 10000;
        } else Cleanse_Timer -= diff;

        //Hammer of Justice
        if (HammerOfJustice_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMMER_OF_JUSTICE);
            HammerOfJustice_Timer = urand(20000, 35000);
        }else HammerOfJustice_Timer -= diff;

        //Holy Shield
        if (HolyShield_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_HOLY_SHIELD);
            HolyShield_Timer = 240000;
        }else HolyShield_Timer -= diff;

        //Devotion_Aura
        if (DevotionAura_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DEVOTION_AURA);
            DevotionAura_Timer = urand(45000, 55000);
        }else DevotionAura_Timer -= diff;

        //Consecration
        if (Consecration_Timer < diff)
        {
            //DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONSECRATION);
            Consecration_Timer = urand(5000, 10000);
        }else Consecration_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_captain_skarloc(Creature* pCreature)
{
    return new boss_captain_skarlocAI(pCreature);
}

void AddSC_boss_captain_skarloc()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_captain_skarloc";
    newscript->GetAI = &GetAI_boss_captain_skarloc;
    newscript->RegisterSelf();
}
