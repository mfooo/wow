/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Loatheb
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SPELL_BERSERK               = 26662,
    SPELL_DEATHBLOOM            = 29865,
    SPELL_DEATHBLOOM_H          = 55053,
    SPELL_INEVITABLE_DOOM       = 29204,
    SPELL_INEVITABLE_DOOM_H     = 55052,
    SPELL_NECROTIC_AURA         = 55593,
    SPELL_FUNGAL_CREEP          = 29232,

    NPC_SPORE                   = 16286
};

float m_afSporesLoc[4][3] = {
    {2880.58f, -3968.67f, 273.63f},
    {2938.41f, -3968.55f, 273.59f},
    {2938.46f, -4027.03f, 273.62f},
    {2880.89f, -4026.87f, 273.62f}
};

struct MANGOS_DLL_DECL boss_loathebAI : public ScriptedAI
{
    boss_loathebAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Deathbloom_Timer;
    uint32 NecroticAura_Timer;
    uint32 InevitableDoom_Timer;
    uint8 InevitableDoom_Count;
    uint32 Summon_Timer;
    uint32 Enrage_Timer;

    void Reset()
    {
        Deathbloom_Timer = 6000;
        NecroticAura_Timer = 11000;
        InevitableDoom_Timer = 120000;
        InevitableDoom_Count = 0;
        Summon_Timer = 13000;
        Enrage_Timer = 720000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, FAIL);
    }

    void Aggro(Unit *who)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, IN_PROGRESS);
    }

    void JustDied(Unit* Killer)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Deathbloom_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DEATHBLOOM : SPELL_DEATHBLOOM_H);
            Deathbloom_Timer = 30000;
        }
        else
            Deathbloom_Timer -= diff;

        if (NecroticAura_Timer < diff)
        {
            m_creature->MonsterTextEmote("An aura of necrotic energy blocks all healing!", 0, true);
            DoCastSpellIfCan(m_creature, SPELL_NECROTIC_AURA);
            NecroticAura_Timer = 20000;
        }
        else
            NecroticAura_Timer -= diff;

        if (InevitableDoom_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_INEVITABLE_DOOM);
            InevitableDoom_Count++;
            if (InevitableDoom_Count < 7)
                InevitableDoom_Timer = 30000;
            else
                InevitableDoom_Timer = 15000;
        }
        else
            InevitableDoom_Timer -= diff;

        if (Summon_Timer < diff)
        {
            uint8 uiRnd = urand(0, 3);
            if (Creature* pSpore = m_creature->SummonCreature(NPC_SPORE, m_afSporesLoc[uiRnd][0], m_afSporesLoc[uiRnd][1], m_afSporesLoc[uiRnd][2], 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                pSpore->AI()->AttackStart(m_creature->getVictim());

            Summon_Timer = 30000;
        }
        else
            Summon_Timer -= diff;

        if (Enrage_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BERSERK);
            Enrage_Timer = 300000;
        }
        else
            Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_loatheb_sporesAI : public ScriptedAI
{
    npc_loatheb_sporesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 DieDelay_Timer;

    void Reset()
    {
        DieDelay_Timer = 0;
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth() && !DieDelay_Timer)
        {
            m_creature->CastSpell(m_creature, SPELL_FUNGAL_CREEP, true);
            DieDelay_Timer = 500;
        }
        if (DieDelay_Timer)
        {
            damage = 0;
            return;
        }
    }

    void JustDied(Unit* Killer) {}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (DieDelay_Timer)
            if (DieDelay_Timer < diff)
            {
                m_creature->ForcedDespawn();
                DieDelay_Timer = 0;
            }else DieDelay_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_loatheb(Creature* pCreature)
{
    return new boss_loathebAI(pCreature);
}

CreatureAI* GetAI_npc_loatheb_spores(Creature* pCreature)
{
    return new npc_loatheb_sporesAI(pCreature);
}

void AddSC_boss_loatheb()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_loatheb";
    newscript->GetAI = &GetAI_boss_loatheb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_loatheb_spores";
    newscript->GetAI = &GetAI_npc_loatheb_spores;
    newscript->RegisterSelf();
}
