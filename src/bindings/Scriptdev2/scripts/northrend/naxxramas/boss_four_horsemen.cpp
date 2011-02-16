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
SDName: Boss_Four_Horsemen
SD%Complete: 75
SDComment: Lady Blaumeux, Thane Korthazz, Sir Zeliek, Baron Rivendare
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    //all horsemen
    SPELL_BERSERK           = 26662,

    //lady blaumeux
    SAY_BLAU_AGGRO          = -1533044,
    SAY_BLAU_TAUNT1         = -1533045,
    SAY_BLAU_TAUNT2         = -1533046,
    SAY_BLAU_TAUNT3         = -1533047,
    SAY_BLAU_SPECIAL        = -1533048,
    SAY_BLAU_SLAY           = -1533049,
    SAY_BLAU_DEATH          = -1533050,

    EMOTE_UNYIELDING_PAIN   = -1533156,                     // NYI

    SPELL_MARK_OF_BLAUMEUX  = 28833,
    SPELL_UNYIELDING_PAIN   = 57381,
    SPELL_VOIDZONE          = 28863,
    H_SPELL_VOIDZONE        = 57463,
    SPELL_SHADOW_BOLT       = 57374,
    H_SPELL_SHADOW_BOLT     = 57464,

    //baron rivendare
    SAY_RIVE_AGGRO1         = -1533065,
    SAY_RIVE_AGGRO2         = -1533066,
    SAY_RIVE_AGGRO3         = -1533067,
    SAY_RIVE_SLAY1          = -1533068,
    SAY_RIVE_SLAY2          = -1533069,
    SAY_RIVE_SPECIAL        = -1533070,
    SAY_RIVE_TAUNT1         = -1533071,
    SAY_RIVE_TAUNT2         = -1533072,
    SAY_RIVE_TAUNT3         = -1533073,
    SAY_RIVE_DEATH          = -1533074,

    SPELL_MARK_OF_RIVENDARE = 28834,
    SPELL_UNHOLY_SHADOW     = 28882,
    H_SPELL_UNHOLY_SHADOW   = 57369,

    //thane korthazz
    SAY_KORT_AGGRO          = -1533051,
    SAY_KORT_TAUNT1         = -1533052,
    SAY_KORT_TAUNT2         = -1533053,
    SAY_KORT_TAUNT3         = -1533054,
    SAY_KORT_SPECIAL        = -1533055,
    SAY_KORT_SLAY           = -1533056,
    SAY_KORT_DEATH          = -1533057,

    SPELL_MARK_OF_KORTHAZZ  = 28832,
    SPELL_METEOR            = 26558,                        // m_creature->getVictim() auto-area spell but with a core problem
    H_SPELL_METEOR          = 57467,

    //sir zeliek
    SAY_ZELI_AGGRO          = -1533058,
    SAY_ZELI_TAUNT1         = -1533059,
    SAY_ZELI_TAUNT2         = -1533060,
    SAY_ZELI_TAUNT3         = -1533061,
    SAY_ZELI_SPECIAL        = -1533062,
    SAY_ZELI_SLAY           = -1533063,
    SAY_ZELI_DEATH          = -1533064,

    EMOTE_CONDEMATION       = -1533157,                     // NYI

    SPELL_MARK_OF_ZELIEK    = 28835,
    SPELL_HOLY_WRATH        = 28883,
    H_SPELL_HOLY_WRATH      = 57466,
    SPELL_HOLY_BOLT         = 57376,
    H_SPELL_HOLY_BOLT       = 57465,
    SPELL_CONDEMNATION      = 57377,
};

/*walk coords*/
#define WALKX_BLAU                2462.112f
#define WALKY_BLAU                -2956.598f
#define WALKZ_BLAU                241.276f

#define WALKX_RIVE                2579.571f
#define WALKY_RIVE                -2960.945f
#define WALKZ_RIVE                241.32f

#define WALKX_KORT                2529.108f
#define WALKY_KORT                -3015.303f
#define WALKZ_KORT                241.32f

#define WALKX_ZELI                2521.039f
#define WALKY_ZELI                -2891.633f
#define WALKZ_ZELI                241.276f

struct MANGOS_DLL_DECL boss_lady_blaumeuxAI : public ScriptedAI
{
    boss_lady_blaumeuxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Mark_Timer;
    uint32 Mark_Count;
    uint32 VoidZone_Timer;
    uint32 ShadowBolt_Timer;
    uint32 Enrage_Timer;

    void Reset()
    {
        Mark_Timer = 20000;
        Mark_Count = 0;
        VoidZone_Timer = 15000;
        ShadowBolt_Timer = 0;
        Enrage_Timer = 0;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pThane = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            if (pZeliek && pZeliek->isDead())
                pZeliek->Respawn();
            if (pThane && pThane->isDead())
                pThane->Respawn();
            if (pRivendare && pRivendare->isDead())
                pRivendare->Respawn();
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FOUR_HORSEMEN, FAIL);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_BLAU_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FOUR_HORSEMEN, IN_PROGRESS);

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_BLAU, WALKY_BLAU, WALKZ_BLAU);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_BLAU_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_BLAU_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            if (pZeliek && pThane && pRivendare && pZeliek->isDead() && pThane->isDead() && pRivendare->isDead())
                 m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() ||
            m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            return;

        if (Mark_Count >= 100)
            if (Enrage_Timer < uiDiff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                Enrage_Timer = 300000;
            }
            else
                Enrage_Timer -= uiDiff;

        if (Mark_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCastSpellIfCan(m_creature, SPELL_MARK_OF_BLAUMEUX);
            Mark_Timer = 12000;
            Mark_Count++;
        }
        else
            Mark_Timer -= uiDiff;

        if (VoidZone_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayersList = pMap->GetPlayers();
            if (!lPlayersList.isEmpty())
            {
                Player* pTarget = lPlayersList.begin()->getSource();
                for(Map::PlayerList::const_iterator itr = lPlayersList.begin(); itr != lPlayersList.end(); ++itr)
                    if (pTarget && itr->getSource() && m_creature->GetDistance2d(itr->getSource()) < m_creature->GetDistance2d(pTarget))
                        pTarget = itr->getSource();

                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_VOIDZONE : H_SPELL_VOIDZONE);
            }
            VoidZone_Timer = 15000;
        }
        else
            VoidZone_Timer -= uiDiff;

        if (ShadowBolt_Timer < uiDiff)
        {
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayersList = pMap->GetPlayers();
            if (!lPlayersList.isEmpty())
            {
                Player* pTarget = lPlayersList.begin()->getSource();
                for(Map::PlayerList::const_iterator itr = lPlayersList.begin(); itr != lPlayersList.end(); ++itr)
                    if (pTarget && itr->getSource() && m_creature->GetDistance2d(itr->getSource()) < m_creature->GetDistance2d(pTarget))
                        pTarget = itr->getSource();

                if (pTarget && m_creature->GetDistance2d(pTarget)< 45.0f)
                    DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT : H_SPELL_SHADOW_BOLT);
                else
                    DoCastSpellIfCan(m_creature, SPELL_UNYIELDING_PAIN);
            }
            ShadowBolt_Timer = 2000;
        }
        else
            ShadowBolt_Timer -= uiDiff;
    }
};


struct MANGOS_DLL_DECL mob_void_zone_naxxAI : public ScriptedAI
{
    mob_void_zone_naxxAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* pInstance;
    bool m_bIsRegularMode;

    uint32 m_despawn_timer;
    uint32 m_uiConsumptionTimer;

    void Reset()
    {
        m_despawn_timer = 75000;
        m_uiConsumptionTimer = 1000;
        SetCombatMovement(false);
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_uiConsumptionTimer <= diff)
        {
            int32 dmg = m_bIsRegularMode ? 3000 : 4500;
            m_creature->CastCustomSpell(m_creature, 30498, &dmg, NULL, NULL, true);
            m_uiConsumptionTimer = 1000;
        }
        else
            m_uiConsumptionTimer -= diff;

        if (m_despawn_timer <= diff)
        {
            m_creature->ForcedDespawn();
        }
        else
            m_despawn_timer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_sir_zeliekAI : public ScriptedAI
{
    boss_sir_zeliekAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Mark_Timer;
    uint32 Mark_Count;
    uint32 HolyWrath_Timer;
    uint32 HolyBolt_Timer;
    uint32 Enrage_Timer;

    void Reset()
    {
        Mark_Timer = 20000;
        Mark_Count = 0;
        HolyWrath_Timer = 15000;
        HolyBolt_Timer = 0;
        Enrage_Timer = 0;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            if (pBlaumeux && pBlaumeux->isDead())
                pBlaumeux->Respawn();
            if (pThane && pThane->isDead())
                pThane->Respawn();
            if (pRivendare && pRivendare->isDead())
                pRivendare->Respawn();
        }
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_ZELI_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_ZELI, WALKY_ZELI, WALKZ_ZELI);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_ZELI_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_ZELI_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            if (pThane && pRivendare && pBlaumeux && pThane->isDead() && pRivendare->isDead() && pBlaumeux->isDead())
                 m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() ||
            m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            return;

        if (Mark_Count >= 100)
            if (Enrage_Timer < uiDiff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                Enrage_Timer = 300000;
            }
            else
                Enrage_Timer -= uiDiff;

        if (Mark_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCastSpellIfCan(m_creature, SPELL_MARK_OF_ZELIEK);
            Mark_Timer = 12000;
            Mark_Count++;
        }
        else
            Mark_Timer -= uiDiff;

        if (HolyWrath_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayersList = pMap->GetPlayers();
            if (!lPlayersList.isEmpty())
            {
                Player* pTarget = lPlayersList.begin()->getSource();
                for(Map::PlayerList::const_iterator itr = lPlayersList.begin(); itr != lPlayersList.end(); ++itr)
                    if (pTarget && itr->getSource() && m_creature->GetDistance2d(itr->getSource()) < m_creature->GetDistance2d(pTarget))
                        pTarget = itr->getSource();

                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_HOLY_WRATH : H_SPELL_HOLY_WRATH);
            }
            HolyWrath_Timer = 15000;
        }
        else
            HolyWrath_Timer -= uiDiff;

        if (HolyBolt_Timer < uiDiff)
        {
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayersList = pMap->GetPlayers();
            if (!lPlayersList.isEmpty())
            {
                Player* pTarget = lPlayersList.begin()->getSource();
                for(Map::PlayerList::const_iterator itr = lPlayersList.begin(); itr != lPlayersList.end(); ++itr)
                    if (pTarget && itr->getSource() && m_creature->GetDistance2d(itr->getSource()) < m_creature->GetDistance2d(pTarget))
                        pTarget = itr->getSource();

                if (pTarget && m_creature->GetDistance2d(pTarget)< 45.0f)
                    DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_HOLY_BOLT : H_SPELL_HOLY_BOLT);
                else
                    DoCastSpellIfCan(m_creature, SPELL_CONDEMNATION);
            }
            HolyBolt_Timer = 2000;
        }
        else
            HolyBolt_Timer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_rivendare_naxxAI : public ScriptedAI
{
    boss_rivendare_naxxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    bool IsMovingToCorner;

    uint32 Mark_Timer;
    uint32 Mark_Count;
    uint32 UnholyShadow_Timer;
    uint32 Enrage_Timer;

    void Reset()
    {
        Mark_Timer = 20000;
        Mark_Count = 0;
        UnholyShadow_Timer = 15000;
        IsMovingToCorner = true;
        Enrage_Timer = 0;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            if (pBlaumeux && pBlaumeux->isDead())
                pBlaumeux->Respawn();
            if (pThane && pThane->isDead())
                pThane->Respawn();
            if (pZeliek && pZeliek->isDead())
                pZeliek->Respawn();
        }
    }

    void Aggro(Unit *who)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_RIVE_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_RIVE_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_RIVE_AGGRO3, m_creature); break;
        }

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_RIVE, WALKY_RIVE, WALKZ_RIVE);
    }

    void KilledUnit(Unit* Victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_RIVE_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_RIVE_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_RIVE_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            if (pZeliek && pThane && pBlaumeux && pZeliek->isDead() && pThane->isDead() && pBlaumeux->isDead())
                 m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (IsMovingToCorner)
        {
            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE)
            {
                IsMovingToCorner = false;
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            return;
        }

        if (Mark_Count >= 100)
            if (Enrage_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                Enrage_Timer = 300000;
            }
            else
                Enrage_Timer -= uiDiff;

        if (Mark_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_MARK_OF_RIVENDARE);
            Mark_Timer = 12000;
            Mark_Count++;
        }
        else
            Mark_Timer -= uiDiff;

        if (UnholyShadow_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_UNHOLY_SHADOW : H_SPELL_UNHOLY_SHADOW);
            UnholyShadow_Timer = 15000;
        }
        else
            UnholyShadow_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_thane_korthazzAI : public ScriptedAI
{
    boss_thane_korthazzAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    bool IsMovingToCorner;

    uint32 Mark_Timer;
    uint32 Mark_Count;
    uint32 Meteor_Timer;
    uint32 Enrage_Timer;

    void Reset()
    {
        Mark_Timer = 20000;
        Mark_Count = 0;
        Meteor_Timer = 15000;
        Enrage_Timer = 0;
        IsMovingToCorner = true;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            if (pBlaumeux && pBlaumeux->isDead())
                pBlaumeux->Respawn();
            if (pRivendare && pRivendare->isDead())
                pRivendare->Respawn();
            if (pZeliek && pZeliek->isDead())
                pZeliek->Respawn();
        }
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_KORT_SLAY, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_KORT_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_KORT, WALKY_KORT, WALKZ_KORT);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_KORT_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            if (pZeliek && pRivendare && pBlaumeux && pZeliek->isDead() && pRivendare->isDead() && pBlaumeux->isDead())
                 m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (IsMovingToCorner)
        {
            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE)
            {
                IsMovingToCorner = false;
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            return;
        }

        if (Mark_Count >= 100)
            if (Enrage_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                Enrage_Timer = 300000;
            }
            else
                Enrage_Timer -= uiDiff;

        if (Mark_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_MARK_OF_KORTHAZZ);
            Mark_Timer = 12000;
            Mark_Count++;
        }
        else
            Mark_Timer -= uiDiff;

        if (Meteor_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_METEOR : H_SPELL_METEOR);
            Meteor_Timer = 15000;
        }
        else
            Meteor_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lady_blaumeux(Creature* pCreature)
{
    return new boss_lady_blaumeuxAI(pCreature);
}

CreatureAI* GetAI_mob_void_zone_naxx(Creature* pCreature)
{
    return new mob_void_zone_naxxAI(pCreature);
}

CreatureAI* GetAI_boss_sir_zeliek(Creature* pCreature)
{
    return new boss_sir_zeliekAI(pCreature);
}

CreatureAI* GetAI_boss_rivendare_naxx(Creature* pCreature)
{
    return new boss_rivendare_naxxAI(pCreature);
}

CreatureAI* GetAI_boss_thane_korthazz(Creature* pCreature)
{
    return new boss_thane_korthazzAI(pCreature);
}

void AddSC_boss_four_horsemen()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_lady_blaumeux";
    NewScript->GetAI = &GetAI_boss_lady_blaumeux;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_void_zone_naxx";
    NewScript->GetAI = &GetAI_mob_void_zone_naxx;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_sir_zeliek";
    NewScript->GetAI = &GetAI_boss_sir_zeliek;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_rivendare_naxx";
    NewScript->GetAI = &GetAI_boss_rivendare_naxx;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_thane_korthazz";
    NewScript->GetAI = &GetAI_boss_thane_korthazz;
    NewScript->RegisterSelf();
}
