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
SDName: Boss_Epoch_Hunter
SD%Complete: 60
SDComment: Missing spawns pre-event, missing speech to be coordinated with rest of escort event.
SDCategory: Caverns of Time, Old Hillsbrad Foothills
EndScriptData */

#include "precompiled.h"
#include "old_hillsbrad.h"

#define SAY_ENTER1                  -1560013
#define SAY_ENTER2                  -1560014
#define SAY_ENTER3                  -1560015
#define SAY_AGGRO1                  -1560016
#define SAY_AGGRO2                  -1560017
#define SAY_SLAY1                   -1560018
#define SAY_SLAY2                   -1560019
#define SAY_BREATH1                 -1560020
#define SAY_BREATH2                 -1560021
#define SAY_DEATH                   -1560022

#define SPELL_SAND_BREATH           31914
#define SPELL_IMPENDING_DEATH       31916
#define SPELL_MAGIC_DISRUPTION_AURA 33834
#define SPELL_WING_BUFFET           31475

struct MANGOS_DLL_DECL boss_epoch_hunterAI : public ScriptedAI
{
    boss_epoch_hunterAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 SandBreath_Timer;
    uint32 ImpendingDeath_Timer;
    uint32 WingBuffet_Timer;
    uint32 Mda_Timer;

    void Reset()
    {
        SandBreath_Timer = urand(8000, 16000);
        ImpendingDeath_Timer = urand(25000, 30000);
        WingBuffet_Timer = 35000;
        Mda_Timer = 40000;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(urand(0, 1) ? SAY_AGGRO1 : SAY_AGGRO2, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance && m_pInstance->GetData(TYPE_THRALL_EVENT) == IN_PROGRESS)
            m_pInstance->SetData(TYPE_THRALL_PART4, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Sand Breath
        if (SandBreath_Timer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SAND_BREATH);

            DoScriptText(urand(0, 1) ? SAY_BREATH1 : SAY_BREATH2, m_creature);

            SandBreath_Timer = urand(10000, 20000);
        }else SandBreath_Timer -= diff;

        if (ImpendingDeath_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_IMPENDING_DEATH);
            ImpendingDeath_Timer = urand(25000, 30000);
        }else ImpendingDeath_Timer -= diff;

        if (WingBuffet_Timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_WING_BUFFET);
            WingBuffet_Timer = urand(25000, 35000);
        }else WingBuffet_Timer -= diff;

        if (Mda_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_MAGIC_DISRUPTION_AURA);
            Mda_Timer = 15000;
        }else Mda_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_epoch_hunter(Creature* pCreature)
{
    return new boss_epoch_hunterAI(pCreature);
}

void AddSC_boss_epoch_hunter()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_epoch_hunter";
    newscript->GetAI = &GetAI_boss_epoch_hunter;
    newscript->RegisterSelf();
}
