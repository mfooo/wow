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
SDName: Boss_Noth
SD%Complete: 90%
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1                          = -1533075,
    SAY_AGGRO2                          = -1533076,
    SAY_AGGRO3                          = -1533077,
    SAY_SUMMON                          = -1533078,
    SAY_SLAY1                           = -1533079,
    SAY_SLAY2                           = -1533080,
    SAY_DEATH                           = -1533081,

    SPELL_BLINK                         = 29211,            //29208, 29209 and 29210 too
    SPELL_CRIPPLE                       = 29212,
    SPELL_CURSE_PLAGUEBRINGER           = 29213,
    SPELL_CURSE_PLAGUEBRINGER_H         = 54835,

    SPELL_BERSERK                       = 26662,            //probably incorrect spell

    SPELL_SUMMON_CHAMPION_AND_CONSTRUCT = 29240,
    SPELL_SUMMON_GUARDIAN_AND_CONSTRUCT = 29269,

    NPC_PLAGUED_WARRIOR                 = 16984,
    NPC_PLAGUED_CHAMPION                = 16983,
    NPC_PLAGUED_GUARDIAN                = 16981,
};

#define CENTER_X    2684.77f
#define CENTER_Y    -3502.44f
#define CENTER_Z    261.31f

#define BALCONY_X 2631.370f
#define BALCONY_Y -3529.680f
#define BALCONY_Z 274.040f
#define BALCONY_O 6.277f

float adds_coords[5][4] =
{
    {2726.49f, -3515.57f, 263.59f, 2.89f},
    {2727.15f, -3464.06f, 263.94f, 3.93f},
    {2704.39f, -3458.15f, 264.59f, 4.29f},
    {2664.04f, -3460.16f, 265.03f, 5.25f},
    {2644.45f, -3464.11f, 265.20f, 5.60f}
};

struct MANGOS_DLL_DECL boss_nothAI : public ScriptedAI
{
    boss_nothAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsEnraged;

    bool isTeleported;
    bool m_bDelay;
    
    uint8 SecondPhaseCounter;

    uint32 Blink_Timer;
    uint32 Curse_Timer;
    uint32 Summon_Timer;
    uint32 SecondPhase_Timer;
    uint32 Teleport_Timer;

    void Reset()
    {
        isTeleported = false;
        m_bDelay = false;
        SecondPhaseCounter = 0;
        m_bIsEnraged = false;
        Blink_Timer = urand(20000, 30000);
        Curse_Timer = 6000;
        Summon_Timer = 30000;
        Teleport_Timer = 120000;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, FAIL);
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        m_creature->SetInCombatWithZone();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, IN_PROGRESS);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || isTeleported)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if (IsCombatMovement())
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bDelay)
        {
            DoStartMovement(m_creature->getVictim());
            m_bDelay = false;
        }

        if (isTeleported)
        {
            if (Teleport_Timer < diff)
            {
                m_creature->MonsterTextEmote("%s teleports back into the battle!", 0, true);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMap()->CreatureRelocation(m_creature, CENTER_X, CENTER_Y, CENTER_Z, 0);
                m_creature->SendMonsterMove(CENTER_X, CENTER_Y, CENTER_Z, SPLINETYPE_NORMAL, SPLINEFLAG_DONE, 0);
                isTeleported = false;
                m_bDelay = true;
                Teleport_Timer = 120000;
            }
            else
                Teleport_Timer -= diff;

            if (SecondPhase_Timer < diff)
            {
                m_creature->MonsterTextEmote("%s raises more skeletons!", 0, true);
                for (uint8 i = 1; i <= (m_bIsRegularMode ? 2 : 4); ++i)
                {
                    uint32 uiAdd = NPC_PLAGUED_CHAMPION;
                    if (SecondPhaseCounter != 1 && i%2 == 0)
                        uiAdd = NPC_PLAGUED_GUARDIAN;
                    uint8 uiI = urand(0, 4);
                    m_creature->SummonCreature(uiAdd, adds_coords[uiI][0], adds_coords[uiI][1], adds_coords[uiI][2], adds_coords[uiI][4], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                }
                SecondPhase_Timer = 35000;
            }
            else
                SecondPhase_Timer -= diff;

            return;
        }

        //Blink_Timer
        if (!m_bIsRegularMode)
            if (Blink_Timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CRIPPLE);
                uint8 uiRX = urand(0, 60);
                uint8 uiRY = urand(0, 60);
                m_creature->GetMap()->CreatureRelocation(m_creature, CENTER_X-30+uiRX, CENTER_Y-30+uiRY, CENTER_Z, 0);
                m_creature->SendMonsterMove(CENTER_X-30+uiRX, CENTER_Y-30+uiRY, CENTER_Z, SPLINETYPE_NORMAL, SPLINEFLAG_DONE, 0);
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoResetThreat();
                    AttackStart(pTarget);
                }
                Blink_Timer = urand(20000, 30000);
                m_bDelay = true;
            }
            else
                Blink_Timer -= diff;

        //Curse_Timer
        if (Curse_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CURSE_PLAGUEBRINGER : SPELL_CURSE_PLAGUEBRINGER_H);
            Curse_Timer = urand (25000, 30000);
        }
        else
            Curse_Timer -= diff;

        //Summon_Timer
        if (Summon_Timer < diff)
        {
            DoScriptText(SAY_SUMMON, m_creature);
            m_creature->MonsterTextEmote("%s summons the Skeletal Warriors!", 0, true);

            for (uint8 i = 0; i < (m_bIsRegularMode ? 2 : 3); ++i)
            {
                uint8 uiI = urand(0, 4);
                m_creature->SummonCreature(NPC_PLAGUED_WARRIOR, adds_coords[uiI][0], adds_coords[uiI][1], adds_coords[uiI][2], adds_coords[uiI][4], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            }

            Summon_Timer = 30000;
        }
        else
            Summon_Timer -= diff;

        if (!m_bIsEnraged)
            if (Teleport_Timer < diff)
                if (SecondPhaseCounter < 3)
                {
                    m_creature->MonsterTextEmote("%s teleports to the balcony above!", 0, true);
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->StopMoving();
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->GetMap()->CreatureRelocation(m_creature, BALCONY_X, BALCONY_Y, BALCONY_Z, BALCONY_O);
                    m_creature->SendMonsterMove(BALCONY_X, BALCONY_Y, BALCONY_Z, SPLINETYPE_NORMAL, SPLINEFLAG_DONE, 0);
                    isTeleported = true;
                    SecondPhase_Timer = 0;
                    Teleport_Timer = 70000;
                    SecondPhaseCounter++;
                }
                else
                {
                    DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                    m_bIsEnraged = true;
                }
            else
                Teleport_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_noth(Creature* pCreature)
{
    return new boss_nothAI(pCreature);
}

void AddSC_boss_noth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_noth";
    newscript->GetAI = &GetAI_boss_noth;
    newscript->RegisterSelf();
}
