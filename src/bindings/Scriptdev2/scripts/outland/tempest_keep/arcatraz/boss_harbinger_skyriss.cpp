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
SDName: Boss_Harbinger_Skyriss
SD%Complete: 45
SDComment: CombatAI not fully implemented. Timers will need adjustments. Need more docs on how event fully work. Reset all event and force start over if fail at one point?
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

/* ContentData
boss_harbinger_skyriss
boss_harbinger_skyriss_illusion
EndContentData */

#include "precompiled.h"
#include "arcatraz.h"

#define SAY_INTRO               -1552000
#define SAY_AGGRO               -1552001
#define SAY_KILL_1              -1552002
#define SAY_KILL_2              -1552003
#define SAY_MIND_1              -1552004
#define SAY_MIND_2              -1552005
#define SAY_FEAR_1              -1552006
#define SAY_FEAR_2              -1552007
#define SAY_IMAGE               -1552008
#define SAY_DEATH               -1552009

#define SPELL_FEAR              39415

#define SPELL_MIND_REND         36924
#define H_SPELL_MIND_REND       39017

#define SPELL_DOMINATION        37162
#define H_SPELL_DOMINATION      39019

#define H_SPELL_MANA_BURN       39020

#define SPELL_66_ILLUSION       36931                       //entry 21466
#define SPELL_33_ILLUSION       36932                       //entry 21467

struct MANGOS_DLL_DECL boss_harbinger_skyrissAI : public ScriptedAI
{
    boss_harbinger_skyrissAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Intro = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool Intro;
    bool IsImage33;
    bool IsImage66;

    uint32 Intro_Phase;
    uint32 Intro_Timer;
    uint32 MindRend_Timer;
    uint32 Fear_Timer;
    uint32 Domination_Timer;
    uint32 ManaBurn_Timer;

    void Reset()
    {
        IsImage33 = false;
        IsImage66 = false;

        Intro_Phase = 1;
        Intro_Timer = 5000;
        MindRend_Timer = 3000;
        Fear_Timer = 15000;
        Domination_Timer = 30000;
        ManaBurn_Timer = 25000;
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!Intro)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void AttackStart(Unit* who)
    {
        if (!Intro)
            return;

        ScriptedAI::AttackStart(who);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HARBINGERSKYRISS,DONE);
    }

    void KilledUnit(Unit* victim)
    {
        //won't yell killing pet/other unit
        if (victim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustSummoned(Creature *summoned)
    {
        summoned->AI()->AttackStart(m_creature->getVictim());
    }

    void DoSplit()
    {
        if (m_creature->IsNonMeleeSpellCasted(false))
            m_creature->InterruptNonMeleeSpells(false);

        DoScriptText(SAY_IMAGE, m_creature);

        DoCastSpellIfCan(m_creature, IsImage33 ? SPELL_33_ILLUSION : SPELL_66_ILLUSION);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Intro && !m_creature->isInCombat())
        {
            if (!m_pInstance)
                return;

            if (Intro_Timer < diff)
            {
                switch(Intro_Phase)
                {
                    case 1:
                        DoScriptText(SAY_INTRO, m_creature);
                        if (GameObject* pSphere = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SPHERE_SHIELD)))
                            pSphere->SetGoState(GO_STATE_ACTIVE);
                        ++Intro_Phase;
                        Intro_Timer = 25000;
                        break;
                    case 2:
                        DoScriptText(SAY_AGGRO, m_creature);
                        if (Creature *mellic = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_MELLICHAR)))
                        {
                            //should have a better way to do this. possibly spell exist.
                            mellic->SetDeathState(JUST_DIED);
                            mellic->SetHealth(0);
                        }
                        ++Intro_Phase;
                        Intro_Timer = 3000;
                        break;
                    case 3:
                        Intro = true;
                        break;
                }
            }else Intro_Timer -=diff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!IsImage66 && m_creature->GetHealthPercent() <= 66.0f)
        {
            IsImage66 = true;
            DoSplit();
        }

        if (!IsImage33 && m_creature->GetHealthPercent() <= 33.0f)
        {
            IsImage33 = true;
            DoSplit();
        }

        if (MindRend_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_MIND_REND : H_SPELL_MIND_REND);
            else
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MIND_REND : H_SPELL_MIND_REND);

            MindRend_Timer = 8000;
        }else MindRend_Timer -=diff;

        if (Fear_Timer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                return;

            DoScriptText(urand(0, 1) ? SAY_FEAR_1 : SAY_FEAR_2, m_creature);

            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                DoCastSpellIfCan(target,SPELL_FEAR);
            else
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_FEAR);

            Fear_Timer = 25000;
        }else Fear_Timer -=diff;

        if (Domination_Timer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                return;

            DoScriptText(urand(0, 1) ? SAY_MIND_1 : SAY_MIND_2, m_creature);

            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_DOMINATION : H_SPELL_DOMINATION);
            else
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DOMINATION : H_SPELL_DOMINATION);

            Domination_Timer = urand(16000, 32000);
        }else Domination_Timer -=diff;

        if (!m_bIsRegularMode)
        {
            if (ManaBurn_Timer < diff)
            {
                if (m_creature->IsNonMeleeSpellCasted(false))
                    return;

                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                    DoCastSpellIfCan(target,H_SPELL_MANA_BURN);

                ManaBurn_Timer = urand(16000, 32000);
            }else ManaBurn_Timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_harbinger_skyriss(Creature* pCreature)
{
    return new boss_harbinger_skyrissAI(pCreature);
}

#define SPELL_MIND_REND_IMAGE   36929
#define H_SPELL_MIND_REND_IMAGE 39021

struct MANGOS_DLL_DECL boss_harbinger_skyriss_illusionAI : public ScriptedAI
{
    boss_harbinger_skyriss_illusionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() { }
};

CreatureAI* GetAI_boss_harbinger_skyriss_illusion(Creature* pCreature)
{
    return new boss_harbinger_skyriss_illusionAI(pCreature);
}

void AddSC_boss_harbinger_skyriss()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_harbinger_skyriss";
    newscript->GetAI = &GetAI_boss_harbinger_skyriss;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_harbinger_skyriss_illusion";
    newscript->GetAI = &GetAI_boss_harbinger_skyriss_illusion;
    newscript->RegisterSelf();
}
