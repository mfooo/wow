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
SDName: Boss_Razuvious
SD%Complete: 75%
sd author: FallenangelX
SDComment: TODO: need confirmation
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1               = -1533125,
    SAY_AGGRO2               = -1533126,
    SAY_AGGRO3               = -1533128,
    SAY_AGGRO4               = -1533123,
    SAY_COMMAND1             = -1533121,
    SAY_COMMAND2             = -1533122,
    SAY_COMMAND3             = -1533124,
    SAY_COMMAND4             = -1533127,
    SAY_COMMAND5             = -1533120,
    SAY_DEATH                = -1533129,

    SPELL_UNBALANCING_STRIKE = 55470,
    SPELL_DISRUPTING_SHOUT   = 55543,
    SPELL_DISRUPTING_SHOUT_H = 29107,
    SPELL_JAGGED_KNIFE       = 55550,
    SPELL_HOPELESS           = 29125,
    SPELL_FORCE_OBEDIENCE    = 55479,

    NPC_DEATH_KNIGHT_UNDERSTUDY = 16803
};

bool GossipHello_npc_obedience_crystal(Player* pPlayer, Creature* pCreature)
{
    std::list<Creature*> lDeathKnightList;
    Unit* pTarget = NULL;

    GetCreatureListWithEntryInGrid(lDeathKnightList, pCreature, NPC_DEATH_KNIGHT_UNDERSTUDY, 100.0f);

    if (!lDeathKnightList.empty())
    {
        pTarget = *(lDeathKnightList.begin());
        for (std::list<Creature*>::iterator itr = lDeathKnightList.begin(); itr != lDeathKnightList.end(); ++itr)
        {
            if ((*itr)->isDead() || (*itr)->HasAura(SPELL_FORCE_OBEDIENCE))
                continue;

            if ((pCreature->GetDistance2d(*itr) < pCreature->GetDistance2d(pTarget)) || pTarget->isDead())
                pTarget = *itr;
        }
    }

    if (pTarget && pTarget->isAlive() && !pTarget->HasAura(SPELL_FORCE_OBEDIENCE))
        pPlayer->CastSpell(pTarget, SPELL_FORCE_OBEDIENCE, true);

    return true;
}

struct MANGOS_DLL_DECL boss_razuviousAI : public ScriptedAI
{
    boss_razuviousAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    std::list<Creature*> m_lDeathKnightList;

    uint32 UnbalancingStrike_Timer;
    uint32 DisruptingShout_Timer;
    uint32 JaggedKnife_Timer;
    uint32 CommandSound_Timer;

    void Reset()
    {
        UnbalancingStrike_Timer = urand(7000, 8000);
        DisruptingShout_Timer = 15000;
        JaggedKnife_Timer = 10000;
        CommandSound_Timer = 30000;
        m_lDeathKnightList.clear();
        GetCreatureListWithEntryInGrid(m_lDeathKnightList, m_creature, NPC_DEATH_KNIGHT_UNDERSTUDY, 50.0f);

        if (!m_lDeathKnightList.empty())
            for (std::list<Creature*>::iterator itr = m_lDeathKnightList.begin(); itr != m_lDeathKnightList.end(); ++itr)
                (*itr)->Respawn();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZUVIOUS, FAIL);
    }

    void Aggro(Unit *pWho)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
            case 3: DoScriptText(SAY_AGGRO4, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZUVIOUS, IN_PROGRESS);

        if (!m_lDeathKnightList.empty())
            for(std::list<Creature*>::iterator itr = m_lDeathKnightList.begin(); itr != m_lDeathKnightList.end(); ++itr)
            {
                (*itr)->AI()->AttackStart(pWho);
                (*itr)->SetPvP(true);
            }

        m_creature->SetInCombatWithZone();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZUVIOUS, DONE);

        DoCastSpellIfCan(m_creature, SPELL_HOPELESS, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (UnbalancingStrike_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_UNBALANCING_STRIKE);
            UnbalancingStrike_Timer = urand(7000, 8000);;
        }
        else
            UnbalancingStrike_Timer -= diff;

        if (DisruptingShout_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DISRUPTING_SHOUT : SPELL_DISRUPTING_SHOUT_H);
            DisruptingShout_Timer = 15000;
        }
        else
            DisruptingShout_Timer -= diff;

        if (JaggedKnife_Timer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_JAGGED_KNIFE, true);
            JaggedKnife_Timer = 10000;
        }
        else
            JaggedKnife_Timer -= diff;

        if (CommandSound_Timer < diff)
        {
            switch(urand(0, 4))
            {
                case 0: DoScriptText(SAY_COMMAND1, m_creature); break;
                case 1: DoScriptText(SAY_COMMAND2, m_creature); break;
                case 2: DoScriptText(SAY_COMMAND3, m_creature); break;
                case 3: DoScriptText(SAY_COMMAND4, m_creature); break;
                case 4: DoScriptText(SAY_COMMAND5, m_creature); break;
            }

            CommandSound_Timer = 30000;
        }
        else
            CommandSound_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_boss_razuvious(Creature* pCreature)
{
    return new boss_razuviousAI(pCreature);
}

void AddSC_boss_razuvious()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "npc_obedience_crystal";
    NewScript->pGossipHello = &GossipHello_npc_obedience_crystal;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_razuvious";
    NewScript->GetAI = &GetAI_boss_razuvious;
    NewScript->RegisterSelf();
}
