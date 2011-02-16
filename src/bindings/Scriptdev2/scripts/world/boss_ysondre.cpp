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
SDName: Ysondre
SD%Complete: 90
SDComment: Mark of Nature & Teleport missing
SDCategory: Bosses
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO               = -1000360,
    SAY_SUMMONDRUIDS        = -1000361,

    SPELL_SLEEP             = 24777,
    SPELL_NOXIOUSBREATH     = 24818,
    SPELL_TAILSWEEP         = 15847,
    //SPELL_MARKOFNATURE   = 25040,                         // Not working
    SPELL_LIGHTNINGWAVE     = 24819,
    SPELL_SUMMONDRUIDS      = 24795,

    SPELL_SUMMON_PLAYER     = 24776,

    //druid spells
    SPELL_MOONFIRE          = 21669
};

// Ysondre script
struct MANGOS_DLL_DECL boss_ysondreAI : public ScriptedAI
{
    boss_ysondreAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSleep_Timer;
    uint32 m_uiNoxiousBreath_Timer;
    uint32 m_uiTailSweep_Timer;
    //uint32 m_uiMarkOfNature_Timer;
    uint32 m_uiLightningWave_Timer;
    uint32 m_uiSummonDruidModifier;

    void Reset()
    {
        m_uiSleep_Timer = urand(15000, 20000);
        m_uiNoxiousBreath_Timer = 8000;
        m_uiTailSweep_Timer = 4000;
        //m_uiMarkOfNature_Timer = 45000;
        m_uiLightningWave_Timer = 12000;
        m_uiSummonDruidModifier = 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Sleep_Timer
        if (m_uiSleep_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SLEEP);

            m_uiSleep_Timer = urand(8000, 15000);
        }
        else
            m_uiSleep_Timer -= uiDiff;

        //NoxiousBreath_Timer
        if (m_uiNoxiousBreath_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_NOXIOUSBREATH);
            m_uiNoxiousBreath_Timer = urand(14000, 20000);
        }
        else
            m_uiNoxiousBreath_Timer -= uiDiff;

        //Tailsweep every 2 seconds
        if (m_uiTailSweep_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_TAILSWEEP);
            m_uiTailSweep_Timer = 2000;
        }
        else
            m_uiTailSweep_Timer -= uiDiff;

        //MarkOfNature_Timer
        //if (m_uiMarkOfNature_Timer < uiDiff)
        //{
        //    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MARKOFNATURE);
        //    m_uiMarkOfNature_Timer = 45000;
        //}
        //else
            //m_uiMarkOfNature_Timer -= uiDiff;

        //LightningWave_Timer
        if (m_uiLightningWave_Timer < uiDiff)
        {
            //Cast LIGHTNINGWAVE on a Random target
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_LIGHTNINGWAVE);

            m_uiLightningWave_Timer = urand(7000, 12000);
        }
        else
            m_uiLightningWave_Timer -= uiDiff;

        //Summon Druids
        if (m_creature->GetHealthPercent() <= float(100 - 25*m_uiSummonDruidModifier))
        {
            DoScriptText(SAY_SUMMONDRUIDS, m_creature);

            for(int i = 0; i < 10; ++i)
                DoCastSpellIfCan(m_creature, SPELL_SUMMONDRUIDS, CAST_TRIGGERED);

            ++m_uiSummonDruidModifier;
        }

        DoMeleeAttackIfReady();
    }
};

// Summoned druid script
struct MANGOS_DLL_DECL mob_dementeddruidsAI : public ScriptedAI
{
    mob_dementeddruidsAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiMoonFire_Timer;

    void Reset()
    {
        m_uiMoonFire_Timer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //MoonFire_Timer
        if (m_uiMoonFire_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MOONFIRE);
            m_uiMoonFire_Timer = 5000;
        }
        else
            m_uiMoonFire_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ysondre(Creature* pCreature)
{
    return new boss_ysondreAI(pCreature);
}

CreatureAI* GetAI_mob_dementeddruids(Creature* pCreature)
{
    return new mob_dementeddruidsAI(pCreature);
}

void AddSC_boss_ysondre()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ysondre";
    newscript->GetAI = &GetAI_boss_ysondre;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dementeddruids";
    newscript->GetAI = &GetAI_mob_dementeddruids;
    newscript->RegisterSelf();
}
