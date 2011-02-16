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
SDName: Boss_Nethermancer_Sepethrea
SD%Complete: 90
SDComment: Need adjustments to initial summons
SDCategory: Tempest Keep, The Mechanar
EndScriptData */

#include "precompiled.h"
#include "mechanar.h"

#define SAY_AGGRO                       -1554013
#define SAY_SUMMON                      -1554014
#define SAY_DRAGONS_BREATH_1            -1554015
#define SAY_DRAGONS_BREATH_2            -1554016
#define SAY_SLAY1                       -1554017
#define SAY_SLAY2                       -1554018
#define SAY_DEATH                       -1554019

#define SPELL_SUMMON_RAGIN_FLAMES       35275

#define SPELL_FROST_ATTACK              35263
#define SPELL_ARCANE_BLAST              35314
#define SPELL_DRAGONS_BREATH            35250
#define SPELL_KNOCKBACK                 37317
#define SPELL_SOLARBURN                 35267

struct MANGOS_DLL_DECL boss_nethermancer_sepethreaAI : public ScriptedAI
{
    boss_nethermancer_sepethreaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 frost_attack_Timer;
    uint32 arcane_blast_Timer;
    uint32 dragons_breath_Timer;
    uint32 knockback_Timer;
    uint32 solarburn_Timer;

    void Reset()
    {
        frost_attack_Timer = urand(7000, 10000);
        arcane_blast_Timer = urand(12000, 18000);
        dragons_breath_Timer = urand(18000, 22000);
        knockback_Timer = urand(22000, 28000);
        solarburn_Timer = 30000;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        //Summon two guards, three in heroic
        uint8 am = (m_bIsRegularMode ? 2 : 1);
        for(int i = 0; i < am; ++i)
        {
            DoCastSpellIfCan(who,SPELL_SUMMON_RAGIN_FLAMES);
        }

        DoScriptText(SAY_SUMMON, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SEPETHREA, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Frost Attack
        if (frost_attack_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROST_ATTACK);
            frost_attack_Timer = urand(7000, 10000);
        }else frost_attack_Timer -= diff;

        //Arcane Blast
        if (arcane_blast_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ARCANE_BLAST);
            arcane_blast_Timer = 15000;
        }else arcane_blast_Timer -= diff;

        //Dragons Breath
        if (dragons_breath_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_DRAGONS_BREATH);

            if (urand(0, 1))
                DoScriptText(urand(0, 1) ? SAY_DRAGONS_BREATH_1 : SAY_DRAGONS_BREATH_2, m_creature);

            dragons_breath_Timer = urand(12000, 22000);
        }else dragons_breath_Timer -= diff;

        //Check for Knockback
        if (knockback_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KNOCKBACK);
            knockback_Timer = urand(15000, 25000);
        }else knockback_Timer -= diff;

        //Check for Solarburn
        if (solarburn_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SOLARBURN);
            solarburn_Timer = 30000;
        }else solarburn_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_nethermancer_sepethrea(Creature* pCreature)
{
    return new boss_nethermancer_sepethreaAI(pCreature);
}

#define SPELL_INFERNO                   35268
#define H_SPELL_INFERNO                 39346
#define SPELL_FIRE_TAIL                 35278

struct MANGOS_DLL_DECL mob_ragin_flamesAI : public ScriptedAI
{
    mob_ragin_flamesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 inferno_Timer;
    uint32 flame_timer;
    uint32 Check_Timer;

    bool onlyonce;

    void Reset()
    {
        inferno_Timer = 10000;
        flame_timer = 500;
        Check_Timer = 2000;
        onlyonce = false;
        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!onlyonce)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                m_creature->GetMotionMaster()->MoveChase(target);
            onlyonce = true;
        }

        if (inferno_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_INFERNO : H_SPELL_INFERNO);

            m_creature->TauntApply(m_creature->getVictim());

            inferno_Timer = 10000;
        }else inferno_Timer -= diff;

        if (flame_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FIRE_TAIL);
            flame_timer = 500;
        }else flame_timer -=diff;

        //Check_Timer
        if (Check_Timer < diff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_SEPETHREA) == DONE)
                {
                    //remove
                    m_creature->SetDeathState(JUST_DIED);
                    m_creature->RemoveCorpse();
                    return;
                }
            }

            Check_Timer = 1000;
        }else Check_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_mob_ragin_flames(Creature* pCreature)
{
    return new mob_ragin_flamesAI(pCreature);
}
void AddSC_boss_nethermancer_sepethrea()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_nethermancer_sepethrea";
    newscript->GetAI = &GetAI_boss_nethermancer_sepethrea;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ragin_flames";
    newscript->GetAI = &GetAI_mob_ragin_flames;
    newscript->RegisterSelf();
}
