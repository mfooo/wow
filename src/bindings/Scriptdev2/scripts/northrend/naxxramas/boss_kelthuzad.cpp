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
SDName: Boss_KelThuzud
SD%Complete: 75
SDComment: Timers will need adjustments, along with tweaking positions and amounts
SDCategory: Naxxramas
EndScriptData */

// some not answered questions:
// - will intro mobs, not sent to center, despawn when phase 2 start?
// - what happens if raid fail, can they start the event as soon after as they want?

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    //when shappiron dies. dialog between kel and lich king (in this order)
    SAY_SAPP_DIALOG1                    = -1533084,
    SAY_SAPP_DIALOG2_LICH               = -1533085,
    SAY_SAPP_DIALOG3                    = -1533086,
    SAY_SAPP_DIALOG4_LICH               = -1533087,
    SAY_SAPP_DIALOG5                    = -1533088,

    //when cat dies
    SAY_CAT_DIED                        = -1533089,

    //when each of the 4 wing bosses dies
    SAY_TAUNT1                          = -1533090,
    SAY_TAUNT2                          = -1533091,
    SAY_TAUNT3                          = -1533092,
    SAY_TAUNT4                          = -1533093,

    SAY_SUMMON_MINIONS                  = -1533105,         //start of phase 1

    EMOTE_PHASE2                        = -1533135,         //start of phase 2
    SAY_AGGRO1                          = -1533094,
    SAY_AGGRO2                          = -1533095,
    SAY_AGGRO3                          = -1533096,

    SAY_SLAY1                           = -1533097,
    SAY_SLAY2                           = -1533098,

    SAY_DEATH                           = -1533099,

    SAY_CHAIN1                          = -1533100,
    SAY_CHAIN2                          = -1533101,
    SAY_FROST_BLAST                     = -1533102,

    SAY_REQUEST_AID                     = -1533103,         //start of phase 3
    SAY_ANSWER_REQUEST                  = -1533104,         //lich king answer

    SAY_SPECIAL1_MANA_DET               = -1533106,
    SAY_SPECIAL3_MANA_DET               = -1533107,
    SAY_SPECIAL2_DISPELL                = -1533108,

    EMOTE_GUARDIAN                      = -1533134,         // at each guardian summon

    //spells to be casted
    SPELL_FROST_BOLT                    = 28478,
    SPELL_FROST_BOLT_H                  = 55802,
    SPELL_FROST_BOLT_NOVA               = 28479,
    SPELL_FROST_BOLT_NOVA_H             = 55807,

    SPELL_CHAINS_OF_KELTHUZAD           = 28408,            // 3.x, heroic only
    SPELL_CHAINS_OF_KELTHUZAD_TARGET    = 28410,

    SPELL_MANA_DETONATION               = 27819,
    SPELL_SHADOW_FISSURE                = 27810,
    SPELL_FROST_BLAST                   = 27808
};

static float M_F_ANGLE = 0.2f;                              // to adjust for map rotation
static float M_F_HEIGHT = 2.0f;                             // adjust for height difference
static float M_F_RANGE = 55.0f;                             // ~ range from center of chamber to center of alcove

#define M_PI_F        float(M_PI)

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_auiGuardiansGUID[5];
    uint32 m_uiGuardiansCount;
    uint32 m_uiGuardiansCountMax;
    uint32 m_uiGuardiansTimer;
    uint32 m_uiFrostBoltTimer;
    uint32 m_uiFrostBoltNovaTimer;
    uint32 m_uiChainsTimer;
    uint32 m_uiManaDetonationTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiFrostBlastTimer;

    uint32 m_uiPhase1Timer;
    uint32 m_uiSoldierTimer;
    uint32 m_uiUndeadTimer;
    uint32 m_uiChainsEndTimer;
    uint32 m_uiChainsTargetsCastTimer;
    bool m_bSummonedIntro;
    bool m_bIsPhase3;

    std::set<uint64> m_lSoldierSet;                         // keeps explicit guids of intro soldiers
    std::set<uint64> m_lUndeadSet;                          // the rest of the intro mobs
    std::set<Unit*> m_lChainsTargets;

    void GetChamberCenterCoords(float &fX, float &fY, float &fZ)
    {
        fX = 3716.63f;
        fY = -5106.91f;
        fZ = 141.29f;
    }

    void Reset()
    {
        m_uiFrostBoltTimer = urand(1000, 5000);
        m_uiFrostBoltNovaTimer = 15000;
        m_uiChainsTimer = urand(60000, 90000);
        m_uiManaDetonationTimer = urand(30000, 60000);
        m_uiShadowFissureTimer = urand (15000, 40000);
        m_uiFrostBlastTimer = urand(45000, 50000);
        m_uiGuardiansTimer = 5000;
        memset(&m_auiGuardiansGUID, 0, sizeof(m_auiGuardiansGUID));
        m_uiGuardiansCount = 0;
        m_uiGuardiansCountMax = m_bIsRegularMode ? 2 : 4;
        m_uiPhase1Timer = 228000;                           //Phase 1 lasts "3 minutes and 48 seconds"
        m_uiSoldierTimer = 5000;
        m_uiUndeadTimer = 5000;
        DespawnAllIntroCreatures();
        m_bSummonedIntro = false;
        m_bIsPhase3 = false;
        m_lChainsTargets.clear();

        // it may be some spell should be used instead, to control the intro phase
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        for(int i=0; i<5; ++i)
        {
            if (m_auiGuardiansGUID[i])
            {
                Creature* pGuardian = m_creature->GetMap()->GetCreature(m_auiGuardiansGUID[i]);

                if (!pGuardian || !pGuardian->isAlive())
                    continue;

                pGuardian->AI()->EnterEvadeMode();
            }
        }

        if (!m_lChainsTargets.empty())
        {
            for(std::set<Unit*>::iterator itr = m_lChainsTargets.begin(); itr != m_lChainsTargets.end(); ++itr)
            {
                ((Player*)(*itr))->SetClientControl(*itr, 1);
            }
        }

        m_lSoldierSet.clear();
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, DONE);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, FAIL);
    }

    void DespawnAllIntroCreatures()
    {
        if (!m_lSoldierSet.empty())
        {
            for(std::set<uint64>::iterator itr = m_lSoldierSet.begin(); itr != m_lSoldierSet.end(); ++itr)
            {
                if (Creature* pSoldier = m_pInstance->instance->GetCreature(*itr))
                    pSoldier->ForcedDespawn();
            }
        }

        if (!m_lUndeadSet.empty())
        {
            for(std::set<uint64>::iterator itr = m_lUndeadSet.begin(); itr != m_lUndeadSet.end(); ++itr)
            {
                if (Creature* pSoldier = m_pInstance->instance->GetCreature(*itr))
                    pSoldier->ForcedDespawn();
            }
        }

        m_lSoldierSet.clear();
        m_lUndeadSet.clear();
    }

    float GetLocationAngle(uint32 uiId)
    {
        switch(uiId)
        {
            case 1: return M_PI_F - M_F_ANGLE;              // south
            case 2: return (M_PI_F / 2) * 3 - M_F_ANGLE;    // east
            case 3: return M_PI_F / 2 - M_F_ANGLE;          // west
            case 4: return M_PI_F / 4 - M_F_ANGLE;          // north-west
            case 5: return (M_PI_F / 4) * 7 - M_F_ANGLE;    // north-east
            case 6: return (M_PI_F / 4) * 5 - M_F_ANGLE;    // south-east
            case 7: return 3*M_PI_F / 4 - M_F_ANGLE;        // south-west
        }

        return M_F_ANGLE;
    }

    void SummonIntroStart()
    {
        if (!m_pInstance)
            return;

        for(int i = 0; i < 7; ++i)
        {
            float fAngle = GetLocationAngle(i+1);

            float fx, fy, fz;
            GetChamberCenterCoords(fx, fy, fz);

            fx += M_F_RANGE * cos(fAngle);
            fy += M_F_RANGE * sin(fAngle);
            fz += M_F_HEIGHT;

            MaNGOS::NormalizeMapCoord(fx);
            MaNGOS::NormalizeMapCoord(fy);

            for(int i = 0; i < 14; ++i)
            {
                uint32 uiNpcEntry = NPC_SOUL_WEAVER;

                if (i > 0)
                {
                    if (i < 4)
                        uiNpcEntry = NPC_UNSTOPPABLE_ABOM;
                    else
                        uiNpcEntry = NPC_SOLDIER_FROZEN;
                }

                float ffx, ffy, ffz;
                m_creature->GetRandomPoint(fx, fy, fz, 15.0f, ffx, ffy, ffz);

                m_creature->SummonCreature(uiNpcEntry, ffx, ffy, ffz, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 5000);
            }
        }
    }

    void SummonGuardian()
    {
        if (!m_pInstance)
            return;

        float fAngle = GetLocationAngle(urand(1,7));

        float fx, fy, fz;
       GetChamberCenterCoords(fx, fy, fz);

        fx += M_F_RANGE * cos(fAngle);
        fy += M_F_RANGE * sin(fAngle);
        fz += M_F_HEIGHT;

        MaNGOS::NormalizeMapCoord(fx);
        MaNGOS::NormalizeMapCoord(fy);

        m_creature->SummonCreature(NPC_GUARDIAN, fx, fy, fz, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 4000);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_GUARDIAN:
            {
                DoScriptText(EMOTE_GUARDIAN, m_creature);

                pSummoned->SetInCombatWithZone();

                //Safe storing of creatures
                m_auiGuardiansGUID[m_uiGuardiansCount] = pSummoned->GetGUID();

                //Update guardian count
                ++m_uiGuardiansCount;
                break;
            }
            case NPC_SOLDIER_FROZEN:
                m_lSoldierSet.insert(pSummoned->GetGUID());
                break;
            case NPC_UNSTOPPABLE_ABOM:
            case NPC_SOUL_WEAVER:
                m_lUndeadSet.insert(pSummoned->GetGUID());
                break;
        }
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType == POINT_MOTION_TYPE && uiPointId == 0)
            m_creature->SetInCombatWithZone();
    }

    bool SendRandomSoldierToCenter()
    {
        if (!m_lSoldierSet.empty())
            for(std::set<uint64>::iterator itr = m_lSoldierSet.begin(); itr != m_lSoldierSet.end(); ++itr)
                if (Creature* pSoldier = m_pInstance->instance->GetCreature(*itr))
                    if (pSoldier->isDead())
                        m_lSoldierSet.erase(pSoldier->GetGUID());

        std::set<uint64>::iterator itr = m_lSoldierSet.begin();

        uint32 uiPosition = urand(0, m_lSoldierSet.size()-1);
        advance(itr, uiPosition);

        if (*itr)
        {
            if (Creature* pSoldier = m_pInstance->instance->GetCreature(*itr))
            {
                if (pSoldier->getVictim())
                    return false;

                float fx, fy, fz;
                GetChamberCenterCoords(fx, fy, fz);
                pSoldier->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pSoldier->GetMotionMaster()->MovePoint(0, fx, fy, fz);
                return true;
            }
        }

        return false;
    }

    bool SendRandomUndeadToCenter()
    {
        if (!m_lUndeadSet.empty())
            for(std::set<uint64>::iterator itr = m_lUndeadSet.begin(); itr != m_lUndeadSet.end(); ++itr)
                if (Creature* pSoldier = m_pInstance->instance->GetCreature(*itr))
                    if (pSoldier->isDead())
                        m_lUndeadSet.erase(pSoldier->GetGUID());

        std::set<uint64>::iterator itr = m_lUndeadSet.begin();

        uint32 uiPosition = urand(0, m_lUndeadSet.size()-1);

        advance(itr, uiPosition);

        if (*itr)
        {
            if (Creature* pUndead = m_pInstance->instance->GetCreature(*itr))
            {
                if (pUndead->getVictim())
                    return false;

                float fx, fy, fz;
                GetChamberCenterCoords(fx, fy, fz);
                pUndead->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pUndead->GetMotionMaster()->MovePoint(0, fx, fy, fz);
                return true;
            }
        }

        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
        {
            if (!m_pInstance)
                return;

            if (m_pInstance->GetData(TYPE_KELTHUZAD) != IN_PROGRESS)
                return;

            if (!m_bSummonedIntro)
            {
                m_bSummonedIntro = true;
                SummonIntroStart();
            }

            if (m_uiPhase1Timer < uiDiff)
            {
                m_creature->SetInCombatWithZone();
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                {
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_KELTHUZAD, FAIL);

                    Reset();
                    return;
                }

                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                DoScriptText(EMOTE_PHASE2, m_creature);
                return;
            }
            else
            {
                if (m_uiSoldierTimer < uiDiff)
                {
                    if (SendRandomSoldierToCenter())
                        m_uiSoldierTimer = 3000;
                }
                else
                    m_uiSoldierTimer -= uiDiff;

                if (m_uiUndeadTimer < uiDiff)
                {
                    if (SendRandomUndeadToCenter())
                        m_uiUndeadTimer = 13000;
                }
                else
                    m_uiUndeadTimer -= uiDiff;

                m_uiPhase1Timer -= uiDiff;
            }

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostBoltTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BOLT : SPELL_FROST_BOLT_H);
            m_uiFrostBoltTimer = urand(2000, 30000);
        }
        else
            m_uiFrostBoltTimer -= uiDiff;

        if (m_uiFrostBoltNovaTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FROST_BOLT_NOVA : SPELL_FROST_BOLT_NOVA_H);
            m_uiFrostBoltNovaTimer = 15000;
        }
        else
            m_uiFrostBoltNovaTimer -= uiDiff;

        if (!m_bIsRegularMode)
            if (m_uiChainsTimer < uiDiff)
            {
                m_lChainsTargets.clear();
                if (SpellEntry* TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_CHAINS_OF_KELTHUZAD_TARGET))
                {
                    TempSpell->EffectApplyAuraName[0]=SPELL_AURA_MOD_CHARM;
                    uint8 uiChainsTargetsCount = 0;
                    for (uint8 i=0; i<25; ++i)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                        {
                            if (pTarget->GetTypeId() == TYPEID_PLAYER && !pTarget->HasAura(SPELL_CHAINS_OF_KELTHUZAD_TARGET, EFFECT_INDEX_0))
                            {
                                m_creature->InterruptNonMeleeSpells(false);
                                m_creature->CastCustomSpell(pTarget, TempSpell, NULL, NULL, NULL, true);
                                ((Player*)pTarget)->SetClientControl(pTarget, 0);
                                m_lChainsTargets.insert(pTarget);
                                ++uiChainsTargetsCount;
                            }
                            if (uiChainsTargetsCount>=3)
                                break;
                        }
                    }
                    m_uiChainsEndTimer = 20000;
                    m_uiChainsTargetsCastTimer = 3500;
                    DoResetThreat();
                }

                //DoCastSpellIfCan(pTarget, SPELL_CHAINS_OF_KELTHUZAD);

                DoScriptText(urand(0, 1) ? SAY_CHAIN1 : SAY_CHAIN2, m_creature);

                m_uiChainsTimer = urand(60000, 90000);
            }
            else
                m_uiChainsTimer -= uiDiff;

        if (!m_lChainsTargets.empty())
        {
            if (m_uiChainsTargetsCastTimer < uiDiff)
            {
                for (std::set<Unit*>::iterator itr = m_lChainsTargets.begin(); itr != m_lChainsTargets.end(); ++itr)
                {
                    if (*itr)
                    {
                        int32 amount = 150000;
                        (*itr)->CastCustomSpell(m_creature, 36983, &amount, NULL, NULL, false);
                    }
                }

                m_uiChainsTargetsCastTimer = 4500;
            }
            else
                m_uiChainsTargetsCastTimer -= uiDiff;

            if (m_uiChainsEndTimer < uiDiff)
            {
                for(std::set<Unit*>::iterator itr = m_lChainsTargets.begin(); itr != m_lChainsTargets.end(); ++itr)
                {
                    if (*itr)
                    {
                        ((Player*)(*itr))->SetClientControl(*itr, 1);
                    }
                }
                m_lChainsTargets.clear();
            }
            else
                m_uiChainsEndTimer -= uiDiff;
        }

        if (m_uiManaDetonationTimer < uiDiff)
        {
            Unit* pTarget = NULL;
            Map *map = m_creature->GetMap();
            if (map && map->IsDungeon())
            {
                std::set<Unit*> lManaDetonationTargets;
                Map::PlayerList const &PlayerList = map->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                    {
                        uint8 uiTargetClass = ((Unit*)itr->getSource())->getClass();
                        if (!itr->getSource()->isGameMaster() && itr->getSource()->isAlive() && uiTargetClass!=CLASS_WARRIOR
                            && uiTargetClass!=CLASS_ROGUE && uiTargetClass!=CLASS_DEATH_KNIGHT)
                        {
                            lManaDetonationTargets.insert(itr->getSource());
                        }
                    }

                std::set<Unit*>::iterator itr = lManaDetonationTargets.begin();
                advance(itr, urand(0, lManaDetonationTargets.size()-1));
                pTarget = *itr;
            }

            if (pTarget)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(pTarget, SPELL_MANA_DETONATION);
            }

            if (urand(0, 1))
                DoScriptText(SAY_SPECIAL1_MANA_DET, m_creature);

            m_uiManaDetonationTimer = urand(30000, 60000);
        }
        else
            m_uiManaDetonationTimer -= uiDiff;

        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(pTarget, SPELL_SHADOW_FISSURE);
            }

            if (urand(0, 1))
                DoScriptText(SAY_SPECIAL3_MANA_DET, m_creature);

            m_uiShadowFissureTimer = urand (15000, 40000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        if (m_uiFrostBlastTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, m_bIsRegularMode ? 1 : 0))
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(pTarget, SPELL_FROST_BLAST);
            }

            if (urand(0, 1))
                DoScriptText(SAY_FROST_BLAST, m_creature);

            m_uiFrostBlastTimer = urand(45000, 50000);
        }
        else
            m_uiFrostBlastTimer -= uiDiff;

        //start phase 3 when we are 45% health
        if (!m_bIsPhase3 && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 45.0f)
        {
            m_bIsPhase3 = true;
            DoScriptText(SAY_REQUEST_AID, m_creature);

            //here Lich King should respond to KelThuzad but I don't know which creature to make talk
            //so for now just make Kelthuzad says it.
            DoScriptText(SAY_ANSWER_REQUEST, m_creature);
        }

        if (m_bIsPhase3 && m_uiGuardiansCount < m_uiGuardiansCountMax)
        {
            if (m_uiGuardiansTimer < uiDiff)
            {
                //Summon a Guardian of Icecrown in a random alcove
                SummonGuardian();

                //5 seconds until summoning next guardian
                m_uiGuardiansTimer = 5000;
            }
            else
                m_uiGuardiansTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kelthuzad(Creature* pCreature)
{
    return new boss_kelthuzadAI(pCreature);
}

void AddSC_boss_kelthuzad()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_kelthuzad";
    NewScript->GetAI = &GetAI_boss_kelthuzad;
    NewScript->RegisterSelf();
}
