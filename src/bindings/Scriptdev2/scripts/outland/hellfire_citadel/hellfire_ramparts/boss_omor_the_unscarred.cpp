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
SDName: Boss_Omar_The_Unscarred
SD%Complete: 90
SDComment: Temporary solution for orbital/shadow whip-ability. Needs more core support before making it more proper.
SDCategory: Hellfire Citadel, Hellfire Ramparts
EndScriptData */

#include "precompiled.h"

#define SAY_AGGRO_1                 -1543009
#define SAY_AGGRO_2                 -1543010
#define SAY_AGGRO_3                 -1543011
#define SAY_SUMMON                  -1543012
#define SAY_CURSE                   -1543013
#define SAY_KILL_1                  -1543014
#define SAY_DIE                     -1543015
#define SAY_WIPE                    -1543016

#define SPELL_ORBITAL_STRIKE        30637
#define SPELL_SHADOW_WHIP           30638
#define SPELL_TREACHEROUS_AURA      30695
#define H_SPELL_BANE_OF_TREACHERY   37566
#define SPELL_DEMONIC_SHIELD        31901
#define SPELL_SHADOW_BOLT           30686
#define H_SPELL_SHADOW_BOLT         39297
#define SPELL_SUMMON_FIENDISH_HOUND 30707

struct MANGOS_DLL_DECL boss_omor_the_unscarredAI : public ScriptedAI
{
    boss_omor_the_unscarredAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    uint32 OrbitalStrike_Timer;
    uint32 ShadowWhip_Timer;
    uint32 Aura_Timer;
    uint32 DemonicShield_Timer;
    uint32 Shadowbolt_Timer;
    uint32 Summon_Timer;
    uint32 SummonedCount;
    uint64 playerGUID;
    bool CanPullBack;

    void Reset()
    {
        DoScriptText(SAY_WIPE, m_creature);

        OrbitalStrike_Timer = 25000;
        ShadowWhip_Timer = 2000;
        Aura_Timer = 10000;
        DemonicShield_Timer = 1000;
        Shadowbolt_Timer = 2000;
        Summon_Timer = 10000;
        SummonedCount = 0;
        playerGUID = 0;
        CanPullBack = false;
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

    void KilledUnit(Unit* victim)
    {
        if (urand(0, 1))
            return;

        DoScriptText(SAY_KILL_1, m_creature);
    }

    void JustSummoned(Creature* summoned)
    {
        DoScriptText(SAY_SUMMON, m_creature);

        if (Unit* random = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            summoned->AI()->AttackStart(random);

        ++SummonedCount;
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DIE, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //only two may be wrong, perhaps increase timer and spawn periodically instead.
        if (SummonedCount < 2)
        {
            if (Summon_Timer < diff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(m_creature,SPELL_SUMMON_FIENDISH_HOUND);
                Summon_Timer = urand(15000, 30000);
            }else Summon_Timer -= diff;
        }

        if (CanPullBack)
        {
            if (ShadowWhip_Timer < diff)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(playerGUID))
                {
                    //if unit dosen't have this flag, then no pulling back (script will attempt cast, even if orbital strike was resisted)
                    if (pPlayer->HasMovementFlag(MOVEFLAG_FALLING))
                    {
                        m_creature->InterruptNonMeleeSpells(false);
                        DoCastSpellIfCan(pPlayer,SPELL_SHADOW_WHIP);
                    }
                }
                playerGUID = 0;
                ShadowWhip_Timer = 2000;
                CanPullBack = false;
            }else ShadowWhip_Timer -= diff;
        }
        else if (OrbitalStrike_Timer < diff)
        {
            Unit* temp = NULL;
            if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
                temp = m_creature->getVictim();
            else temp = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);

            if (temp && temp->GetTypeId() == TYPEID_PLAYER)
            {
                DoCastSpellIfCan(temp,SPELL_ORBITAL_STRIKE);
                OrbitalStrike_Timer = urand(14000, 16000);
                playerGUID = temp->GetGUID();

                if (playerGUID)
                    CanPullBack = true;
            }
        }else OrbitalStrike_Timer -= diff;

        if (m_creature->GetHealthPercent() < 20.0f)
        {
            if (DemonicShield_Timer < diff)
            {
                DoCastSpellIfCan(m_creature,SPELL_DEMONIC_SHIELD);
                DemonicShield_Timer = 15000;
            }else DemonicShield_Timer -= diff;
        }

        if (Aura_Timer < diff)
        {
            DoScriptText(SAY_CURSE, m_creature);

            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_TREACHEROUS_AURA : H_SPELL_BANE_OF_TREACHERY);
                Aura_Timer = urand(8000, 16000);
            }
        }else Aura_Timer -= diff;

        if (Shadowbolt_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                if (target)
                    target = m_creature->getVictim();

                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_SHADOW_BOLT : H_SPELL_SHADOW_BOLT);
                Shadowbolt_Timer = urand(4000, 6500);
            }
        }else Shadowbolt_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_omor_the_unscarredAI(Creature* pCreature)
{
    return new boss_omor_the_unscarredAI(pCreature);
}

void AddSC_boss_omor_the_unscarred()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_omor_the_unscarred";
    newscript->GetAI = &GetAI_boss_omor_the_unscarredAI;
    newscript->RegisterSelf();
}
