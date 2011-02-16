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
SDName: Boss_Gahz'ranka
SD%Complete: 85
SDComment: Massive Geyser with knockback not working. Spell buggy.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"

#define SPELL_FROSTBREATH            16099
#define SPELL_MASSIVEGEYSER          22421                  //Not working. Cause its a summon...
#define SPELL_SLAM                   24326

struct MANGOS_DLL_DECL boss_gahzrankaAI : public ScriptedAI
{
    boss_gahzrankaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}
    uint32 Frostbreath_Timer;
    uint32 MassiveGeyser_Timer;
    uint32 Slam_Timer;

    void Reset()
    {
        Frostbreath_Timer = 8000;
        MassiveGeyser_Timer = 25000;
        Slam_Timer = 17000;
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Frostbreath_Timer
        if (Frostbreath_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTBREATH);
            Frostbreath_Timer = urand(7000, 11000);
        }else Frostbreath_Timer -= diff;

        //MassiveGeyser_Timer
        if (MassiveGeyser_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MASSIVEGEYSER);
            DoResetThreat();

            MassiveGeyser_Timer = urand(22000, 32000);
        }else MassiveGeyser_Timer -= diff;

        //Slam_Timer
        if (Slam_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SLAM);
            Slam_Timer = urand(12000, 20000);
        }else Slam_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_gahzranka(Creature* pCreature)
{
    return new boss_gahzrankaAI(pCreature);
}

void AddSC_boss_gahzranka()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gahzranka";
    newscript->GetAI = &GetAI_boss_gahzranka;
    newscript->RegisterSelf();
}
