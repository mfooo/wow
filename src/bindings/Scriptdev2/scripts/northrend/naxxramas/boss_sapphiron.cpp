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
SDName: Boss_Sapphiron
SD author: i believe insider42
SD%Complete: 0
SDComment: Place Holder
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_BREATH       = -1533082,
    EMOTE_ENRAGE       = -1533083,

    SPELL_BERSERK           = 26662,
    SPELL_CLEAVE            = 19983,
    SPELL_FROST_AURA        = 28531,
    SPELL_FROST_AURA_H      = 55799,
    SPELL_TAIL_SWEEP        = 55697,
    SPELL_TAIL_SWEEP_H      = 55696,
    SPELL_ICEBOLT           = 28522,
    SPELL_ICE_BLOCK_VISUAL  = 62766, //hack
    SPELL_FROST_BREATH      = 28524,
    SPELL_FROST_BREATH_BALL = 30101,
    SPELL_LIFE_DRAIN        = 28542,
    SPELL_LIFE_DRAIN_H      = 55665,
    SPELL_BLIZZARD          = 28547,
    SPELL_BLIZZARD_H        = 55699,
    SPELL_WING_BUFFET       = 29328,
    SPELL_DIES              = 29357,
};

#define CENTER_X            3522.39f
#define CENTER_Y            -5236.78f
#define CENTER_Z            137.71f


struct MANGOS_DLL_DECL boss_sapphironAI : public ScriptedAI
{
    boss_sapphironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Icebolt_Count;
    uint32 Icebolt_Timer;
    uint32 FrostBreath_Timer;
    uint32 FrostAura_Timer;
    uint32 LifeDrain_Timer;
    uint32 Blizzard_Timer;
    uint32 Phase_Timer;
    uint32 Berserk_Timer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiTailSweepTimer;
    uint32 LandingDelay_Timer;
    uint32 WingBuffet_Timer;
    uint32 FrostBreath_Phase;

    bool LandPhase;
    bool LandingDelay;
    std::list<Unit*> IceBlockTargets;


    void Reset()
    {
        LifeDrain_Timer = 10000;
        Blizzard_Timer = 16000;
        Phase_Timer = 45000;
        Berserk_Timer = 900000;
        m_uiCleaveTimer =  urand(10000, 15000);
        m_uiTailSweepTimer =  urand(10000, 15000);
        WingBuffet_Timer = 2000;
        LandingDelay_Timer = 0;
        LandingDelay = false;
        LandPhase = true;
        FrostBreath_Phase = 1;
        IceBlockTargets.clear();
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetVisibility(VISIBILITY_ON);

        if (m_pInstance && !m_bIsRegularMode)
        {
            for (uint8 i = 1; i <= 13; ++i)
                if (m_pInstance->GetData(i) != DONE)
                {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    break;
                }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, FAIL);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
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

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FROST_AURA : SPELL_FROST_AURA_H, true);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, DONE);

        m_creature->CastSpell(m_creature, SPELL_DIES, true);

        Map *map = m_creature->GetMap();
        if (map && map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                {
                    itr->getSource()->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                    itr->getSource()->RemoveAurasDueToSpell(SPELL_ICE_BLOCK_VISUAL);
                }
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type == POINT_MOTION_TYPE && id == 1)
        {
            m_creature->MonsterTextEmote("Sapphiron lifts off into the air!", NULL, true);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            m_creature->SetHover(true);
            Icebolt_Timer = 6000;
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            Icebolt_Count = 0;
        }
    }

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        switch (spell->Id)
        {
            case SPELL_ICEBOLT:
            {
                if (target && target->isAlive() && target->HasAura(SPELL_ICEBOLT))
                {
                    target->CastSpell(target, SPELL_ICE_BLOCK_VISUAL, true);
                    IceBlockTargets.push_back(target);
                }
                if (Icebolt_Count == (m_bIsRegularMode ? 2 : 3))
                {
                    //DoScriptText(EMOTE_BREATH, m_creature);
                    m_creature->MonsterTextEmote("Sapphiron takes a deep breath.", NULL, true);
                    DoCastSpellIfCan(m_creature, SPELL_FROST_BREATH_BALL);
                    m_creature->SetHover(true);
                    FrostBreath_Timer = 6900;
                }
                break;
            }
            case SPELL_FROST_BREATH:
            {
                if (target && target->GetTypeId() == TYPEID_PLAYER && target->HasAura(SPELL_ICEBOLT))
                {
                    target->RemoveAurasDueToSpell(SPELL_ICE_BLOCK_VISUAL);
                    target->RemoveAurasDueToSpell(SPELL_ICEBOLT);
                }
                break;
            }
        }
    }

    bool IsInBetween(WorldObject* obj1, WorldObject* obj2, WorldObject* obj3, float size)
    {
	    if (obj1->GetPositionX() > std::max(obj2->GetPositionX(), obj3->GetPositionX())
            || obj1->GetPositionX() < std::min(obj2->GetPositionX(), obj3->GetPositionX())
    		|| obj1->GetPositionY() > std::max(obj2->GetPositionY(), obj3->GetPositionY())
	    	|| obj1->GetPositionY() < std::min(obj2->GetPositionY(), obj3->GetPositionY()))
		    return false;

        if (!size)
            size = obj1->GetObjectBoundingRadius() / 2;

	    float angle = obj2->GetAngle(obj1) - obj2->GetAngle(obj3);
    	return abs(sin(angle)) * obj1->GetDistance2d(obj2->GetPositionX(), obj2->GetPositionY()) < size;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            if (m_pInstance)
            {
                for (uint8 i = 1; i <= 13; ++i)
                    if (m_pInstance->GetData(i) != DONE)
                        return;

                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->SetVisibility(VISIBILITY_ON);
            }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Berserk_Timer < diff)
        {
            DoScriptText(EMOTE_ENRAGE, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_BERSERK);
            Berserk_Timer = 300000;
        }
        else
            Berserk_Timer -= diff;

        if (LandingDelay)
        {
            if (LandingDelay_Timer < diff)
            {
                LandPhase = true;
                m_creature->MonsterTextEmote("Sapphiron resumes his attacks!", NULL, true);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                Phase_Timer = 45000;
                LandingDelay = false;
            }
            else
                LandingDelay_Timer -= diff;

            return;
        }

        if (LandPhase)
        {
            if (LifeDrain_Timer < diff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LIFE_DRAIN : SPELL_LIFE_DRAIN_H);
                LifeDrain_Timer = 24000;
            }
            else
                LifeDrain_Timer -= diff;

            if (m_uiCleaveTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = urand(10000, 15000);
            }
            else
                m_uiCleaveTimer -= diff;

            if (m_uiTailSweepTimer < diff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TAIL_SWEEP : SPELL_TAIL_SWEEP_H);
                m_uiTailSweepTimer = urand(10000, 15000);
            }
            else
                m_uiTailSweepTimer -= diff;

            if (Blizzard_Timer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_BLIZZARD : SPELL_BLIZZARD_H);

                Blizzard_Timer = 3500;
            }
            else
                Blizzard_Timer -= diff;

            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 10)
                if (Phase_Timer < diff)
                {
                    LandPhase = false;
                    Icebolt_Timer = 60000;
                    FrostBreath_Timer = 60000;
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MovePoint(1, CENTER_X, CENTER_Y, CENTER_Z);
                }
                else
                    Phase_Timer -= diff;
        }
        else
        {
            if (Icebolt_Count < uint8(m_bIsRegularMode ? 2 : 3) && Icebolt_Timer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(target, SPELL_ICEBOLT);

                m_creature->SetHover(true);
                ++Icebolt_Count;
                Icebolt_Timer = 4000;
            }
            else
                Icebolt_Timer -= diff;

            /*if (WingBuffet_Timer < diff)
            {
                DoCastSpellIfCan(m_creature, SPELL_WING_BUFFET);
                m_creature->SetHover(true);
                WingBuffet_Timer = 2000;
            }
            else
                WingBuffet_Timer -= diff;*/

            if (FrostBreath_Timer < diff)
            {
                switch (FrostBreath_Phase)
                {
                    case 1:
                    {
                        if (!IceBlockTargets.empty())
                        {
                            Map *map = m_creature->GetMap();
                            if (map && map->IsDungeon())
                            {
                                Map::PlayerList const &PlayerList = map->GetPlayers();
                                if (!PlayerList.isEmpty())
                                    for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                                    {
                                        if (itr->getSource()->isGameMaster())
                                            continue;

                                        for (std::list<Unit*>::const_iterator i = IceBlockTargets.begin(); i != IceBlockTargets.end(); ++i)
                                        {
                                            if (IsInBetween(*i, m_creature, itr->getSource(), 3.0f) &&
                                                m_creature->GetDistance2d(itr->getSource()->GetPositionX(), itr->getSource()->GetPositionY()) - m_creature->GetDistance2d((*i)->GetPositionX(), (*i)->GetPositionY()) < 5.0f)
                                                itr->getSource()->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                                                (*i)->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                                        }
                                    }
                            }
                        }
                        FrostBreath_Phase = 2;
                        FrostBreath_Timer = 100;
                        break;
                    }
                    case 2:
                    {
                        DoCastSpellIfCan(m_creature, SPELL_FROST_BREATH);
                        FrostBreath_Phase = 3;
                        FrostBreath_Timer = 100;
                        break;
                    }
                    case 3:
                    {
                        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                        LandingDelay = true;
                        LandingDelay_Timer = 3000;
                        m_creature->SetHover(false);
                        IceBlockTargets.clear();
                        Map *map = m_creature->GetMap();
                        if (map && map->IsDungeon())
                        {
                            Map::PlayerList const &PlayerList = map->GetPlayers();
                            if (!PlayerList.isEmpty())
                                for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                                    itr->getSource()->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                        }
                        FrostBreath_Phase = 1;
                        break;
                    }
                }
            }
            else
                FrostBreath_Timer -= diff;
        }

        if (LandPhase)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sapphiron(Creature* pCreature)
{
    return new boss_sapphironAI(pCreature);
}

void AddSC_boss_sapphiron()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_sapphiron";
    NewScript->GetAI = &GetAI_boss_sapphiron;
    NewScript->RegisterSelf();
}
