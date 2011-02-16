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
SDName: Boss_Darkweaver_Syth
SD%Complete: 85
SDComment: Shock spells/times need more work.
SDCategory: Auchindoun, Sethekk Halls
EndScriptData */

#include "precompiled.h"

#define SAY_SUMMON                  -1556000

#define SAY_AGGRO_1                 -1556001
#define SAY_AGGRO_2                 -1556002
#define SAY_AGGRO_3                 -1556003

#define SAY_SLAY_1                  -1556004
#define SAY_SLAY_2                  -1556005

#define SAY_DEATH                   -1556006

#define SPELL_FROST_SHOCK           37865
#define SPELL_FLAME_SHOCK           34354
#define SPELL_SHADOW_SHOCK          30138
#define SPELL_ARCANE_SHOCK          37132

#define SPELL_CHAIN_LIGHTNING       39945

#define SPELL_SUMMON_SYTH_FIRE      33537                   // Spawns 19203
#define SPELL_SUMMON_SYTH_ARCANE    33538                   // Spawns 19205
#define SPELL_SUMMON_SYTH_FROST     33539                   // Spawns 19204
#define SPELL_SUMMON_SYTH_SHADOW    33540                   // Spawns 19206

#define SPELL_FLAME_BUFFET          33526
#define H_SPELL_FLAME_BUFFET        38141
#define SPELL_ARCANE_BUFFET         33527
#define H_SPELL_ARCANE_BUFFET       38138
#define SPELL_FROST_BUFFET          33528
#define H_SPELL_FROST_BUFFET        38142
#define SPELL_SHADOW_BUFFET         33529
#define H_SPELL_SHADOW_BUFFET       38143

struct MANGOS_DLL_DECL boss_darkweaver_sythAI : public ScriptedAI
{
    boss_darkweaver_sythAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 flameshock_timer;
    uint32 arcaneshock_timer;
    uint32 frostshock_timer;
    uint32 shadowshock_timer;
    uint32 chainlightning_timer;

    bool summon90;
    bool summon50;
    bool summon10;

    void Reset()
    {
        flameshock_timer = 2000;
        arcaneshock_timer = 4000;
        frostshock_timer = 6000;
        shadowshock_timer = 8000;
        chainlightning_timer = 15000;

        summon90 = false;
        summon50 = false;
        summon10 = false;
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
        if (urand(0, 1))
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustSummoned(Creature *summoned)
    {
        if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            summoned->AI()->AttackStart(target);
    }

    void SythSummoning()
    {
        DoScriptText(SAY_SUMMON, m_creature);

        if (m_creature->IsNonMeleeSpellCasted(false))
            m_creature->InterruptNonMeleeSpells(false);

        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_ARCANE, CAST_TRIGGERED);//front
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_FIRE,   CAST_TRIGGERED);//back
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_FROST,  CAST_TRIGGERED);//left
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SYTH_SHADOW, CAST_TRIGGERED);//right
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() < 90.0f && !summon90)
        {
            SythSummoning();
            summon90 = true;
        }

        if (m_creature->GetHealthPercent() < 50.0f && !summon50)
        {
            SythSummoning();
            summon50 = true;
        }

        if (m_creature->GetHealthPercent() < 10.0f && !summon10)
        {
            SythSummoning();
            summon10 = true;
        }

        if (flameshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_FLAME_SHOCK);

            flameshock_timer = urand(10000, 15000);
        } else flameshock_timer -= diff;

        if (arcaneshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_ARCANE_SHOCK);

            arcaneshock_timer = urand(10000, 15000);
        } else arcaneshock_timer -= diff;

        if (frostshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_FROST_SHOCK);

            frostshock_timer = urand(10000, 15000);
        } else frostshock_timer -= diff;

        if (shadowshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_SHADOW_SHOCK);

            shadowshock_timer = urand(10000, 15000);
        } else shadowshock_timer -= diff;

        if (chainlightning_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_CHAIN_LIGHTNING);

            chainlightning_timer = 25000;
        } else chainlightning_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_darkweaver_syth(Creature* pCreature)
{
    return new boss_darkweaver_sythAI(pCreature);
}

/* ELEMENTALS */

struct MANGOS_DLL_DECL mob_syth_fireAI : public ScriptedAI
{
    mob_syth_fireAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 flameshock_timer;
    uint32 flamebuffet_timer;


    void Reset()
    {
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
        flameshock_timer = 2500;
        flamebuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (flameshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_FLAME_SHOCK);

            flameshock_timer = 5000;
        }else flameshock_timer -= diff;

        if (flamebuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
             DoCastSpellIfCan(target, m_bIsRegularMode ? H_SPELL_FLAME_BUFFET : SPELL_FLAME_BUFFET);

            flamebuffet_timer = 5000;

        }else flamebuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_fire(Creature* pCreature)
{
    return new mob_syth_fireAI(pCreature);
}

struct MANGOS_DLL_DECL mob_syth_arcaneAI : public ScriptedAI
{
    mob_syth_arcaneAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 arcaneshock_timer;
    uint32 arcanebuffet_timer;

    void Reset()
    {
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, true);
        arcaneshock_timer = 2500;
        arcanebuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (arcaneshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_ARCANE_SHOCK);

            arcaneshock_timer = 5000;
        }else arcaneshock_timer -= diff;

        if (arcanebuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? H_SPELL_ARCANE_BUFFET : SPELL_ARCANE_BUFFET);

            arcanebuffet_timer = 5000;
        }else arcanebuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_arcane(Creature* pCreature)
{
    return new mob_syth_arcaneAI(pCreature);
}

struct MANGOS_DLL_DECL mob_syth_frostAI : public ScriptedAI
{
    mob_syth_frostAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 frostshock_timer;
    uint32 frostbuffet_timer;

    void Reset()
    {
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
        frostshock_timer = 2500;
        frostbuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (frostshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_FROST_SHOCK);

            frostshock_timer = 5000;
        }else frostshock_timer -= diff;

        if (frostbuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? H_SPELL_FROST_BUFFET : SPELL_FROST_BUFFET);

            frostbuffet_timer = 5000;
        }else frostbuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_frost(Creature* pCreature)
{
    return new mob_syth_frostAI(pCreature);
}

struct MANGOS_DLL_DECL mob_syth_shadowAI : public ScriptedAI
{
    mob_syth_shadowAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 shadowshock_timer;
    uint32 shadowbuffet_timer;

    void Reset()
    {
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, true);
        shadowshock_timer = 2500;
        shadowbuffet_timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (shadowshock_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_SHADOW_SHOCK);

            shadowshock_timer = 5000;
        }else shadowshock_timer -= diff;

        if (shadowbuffet_timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? H_SPELL_SHADOW_BUFFET : SPELL_SHADOW_BUFFET);

            shadowbuffet_timer = 5000;
        }else shadowbuffet_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_syth_shadow(Creature* pCreature)
{
    return new mob_syth_shadowAI(pCreature);
}

void AddSC_boss_darkweaver_syth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_darkweaver_syth";
    newscript->GetAI = &GetAI_boss_darkweaver_syth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_syth_fire";
    newscript->GetAI = &GetAI_mob_syth_fire;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_syth_arcane";
    newscript->GetAI = &GetAI_mob_syth_arcane;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_syth_frost";
    newscript->GetAI = &GetAI_mob_syth_frost;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_syth_shadow";
    newscript->GetAI = &GetAI_mob_syth_shadow;
    newscript->RegisterSelf();
}