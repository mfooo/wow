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
SDName: Boss_Hydromancer_Thespia
SD%Complete: 80
SDComment: Needs additional adjustments (when instance script is adjusted)
SDCategory: Coilfang Resevoir, The Steamvault
EndScriptData */

/* ContentData
boss_hydromancer_thespia
mob_coilfang_waterelemental
EndContentData */

#include "precompiled.h"
#include "steam_vault.h"

#define SAY_SUMMON                  -1545000
#define SAY_AGGRO_1                 -1545001
#define SAY_AGGRO_2                 -1545002
#define SAY_AGGRO_3                 -1545003
#define SAY_SLAY_1                  -1545004
#define SAY_SLAY_2                  -1545005
#define SAY_DEAD                    -1545006

#define SPELL_LIGHTNING_CLOUD       25033
#define SPELL_LUNG_BURST            31481
#define SPELL_ENVELOPING_WINDS      31718

struct MANGOS_DLL_DECL boss_thespiaAI : public ScriptedAI
{
    boss_thespiaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 LightningCloud_Timer;
    uint32 LungBurst_Timer;
    uint32 EnvelopingWinds_Timer;

    void Reset()
    {
        LightningCloud_Timer = 15000;
        LungBurst_Timer = 7000;
        EnvelopingWinds_Timer = 9000;

        if (m_pInstance && m_creature->isAlive())
            m_pInstance->SetData(TYPE_HYDROMANCER_THESPIA,NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEAD, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HYDROMANCER_THESPIA, DONE);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HYDROMANCER_THESPIA, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //LightningCloud_Timer
        if (LightningCloud_Timer < diff)
        {
            //cast twice in Heroic mode
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_LIGHTNING_CLOUD);
            if (!m_bIsRegularMode)
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    DoCastSpellIfCan(target, SPELL_LIGHTNING_CLOUD);
            LightningCloud_Timer = urand(15000, 25000);
        }else LightningCloud_Timer -=diff;

        //LungBurst_Timer
        if (LungBurst_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_LUNG_BURST);
            LungBurst_Timer = urand(7000, 12000);
        }else LungBurst_Timer -=diff;

        //EnvelopingWinds_Timer
        if (EnvelopingWinds_Timer < diff)
        {
            //cast twice in Heroic mode
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_ENVELOPING_WINDS);
            if (!m_bIsRegularMode)
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    DoCastSpellIfCan(target, SPELL_ENVELOPING_WINDS);
            EnvelopingWinds_Timer = urand(10000, 15000);
        }else EnvelopingWinds_Timer -=diff;

        DoMeleeAttackIfReady();
    }
};

#define SPELL_WATER_BOLT_VOLLEY     34449
#define H_SPELL_WATER_BOLT_VOLLEY   37924

struct MANGOS_DLL_DECL mob_coilfang_waterelementalAI : public ScriptedAI
{
    mob_coilfang_waterelementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 WaterBoltVolley_Timer;

    void Reset()
    {
        WaterBoltVolley_Timer = urand(3000, 6000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (WaterBoltVolley_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WATER_BOLT_VOLLEY : H_SPELL_WATER_BOLT_VOLLEY);
            WaterBoltVolley_Timer = urand(7000, 12000);
        }else WaterBoltVolley_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thespiaAI(Creature* pCreature)
{
    return new boss_thespiaAI(pCreature);
}

CreatureAI* GetAI_mob_coilfang_waterelementalAI(Creature* pCreature)
{
    return new mob_coilfang_waterelementalAI(pCreature);
}

void AddSC_boss_hydromancer_thespia()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_hydromancer_thespia";
    newscript->GetAI = &GetAI_boss_thespiaAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_coilfang_waterelemental";
    newscript->GetAI = &GetAI_mob_coilfang_waterelementalAI;
    newscript->RegisterSelf();
}
