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
// Gormok - Firebomb not implemented, timers need correct
// Snakes - Underground phase not worked, timers need correct
// Icehowl - Trample&Crash event not implemented, timers need correct

/* ScriptData
SDName: northrend_beasts
SD%Complete: 90% 
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

// not implemented:
// snobolds link
// snakes underground cast (not support in core)

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Equipment
{
    EQUIP_MAIN           = 50760,
    EQUIP_OFFHAND        = 48040,
    EQUIP_RANGED         = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_SNOBOLD_VASSAL   = 34800,
    NPC_SLIME_POOL       = 35176,
    NPC_FIRE_BOMB        = 34854,
};

enum BossSpells
{
SPELL_IMPALE           = 66331,
SPELL_STAGGERING_STOMP = 67648,
SPELL_RISING_ANGER     = 66636,
SUMMON_SNOBOLD         = NPC_SNOBOLD_VASSAL,
SPELL_ACID_SPIT        = 66880,
SPELL_PARALYTIC_SPRAY  = 66901,
SPELL_ACID_SPEW        = 66819,
SPELL_PARALYTIC_BITE   = 66824,
SPELL_SWEEP_0          = 66794,
SPELL_SLIME_POOL       = 66883,
SPELL_FIRE_SPIT        = 66796,
SPELL_MOLTEN_SPEW      = 66821,
SPELL_BURNING_BITE     = 66879,
SPELL_BURNING_SPRAY    = 66902,
SPELL_SWEEP_1          = 67646,
SPELL_FEROCIOUS_BUTT   = 66770,
SPELL_MASSIVE_CRASH    = 66683,
SPELL_WHIRL            = 67345,
SPELL_ARCTIC_BREATH    = 66689,
SPELL_TRAMPLE          = 66734,
SPELL_ADRENALINE       = 68667,
SPELL_SNOBOLLED        = 66406,
SPELL_BATTER           = 66408,
SPELL_FIRE_BOMB        = 66313,
SPELL_FIRE_BOMB_1      = 66317,
SPELL_FIRE_BOMB_DOT    = 66318,
SPELL_HEAD_CRACK       = 66407,
SPELL_SUBMERGE_0       = 53421,
SPELL_ENRAGE           = 68335,
SPELL_FROTHING_RAGE    = 66759,
SPELL_STAGGERED_DAZE   = 66758,
SPELL_SLIME_POOL_1     = 66881,
SPELL_SLIME_POOL_2     = 66882,
SPELL_SLIME_POOL_VISUAL  = 63084,
};

struct MANGOS_DLL_DECL boss_gormokAI : public BSWScriptedAI
{
    boss_gormokAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 SnoboldsCount;

    void Reset() {

        if(!m_pInstance) return;
        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);
        m_creature->SetRespawnDelay(7*DAY);
        m_creature->SetInCombatWithZone();
        SnoboldsCount = 4;
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_DONE);
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_IMPALE, uiDiff);

        timedCast(SPELL_STAGGERING_STOMP, uiDiff);

        if (timedQuery(SUMMON_SNOBOLD, uiDiff) && SnoboldsCount > 0 ) {
                        doCast(SUMMON_SNOBOLD);
                        DoScriptText(-1713601,m_creature);
                        --SnoboldsCount;
                        };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

struct MANGOS_DLL_DECL mob_snobold_vassalAI : public BSWScriptedAI
{
    mob_snobold_vassalAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    Unit* pBoss;
    Unit* defaultTarget;

    void Reset()
    {
        pBoss = NULL;
        defaultTarget = NULL;
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
        pBoss = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_GORMOK));
        if (pBoss) doCast(SPELL_RISING_ANGER,pBoss);
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
        defaultTarget = who;
        doCast(SPELL_SNOBOLLED, defaultTarget);
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
    if (defaultTarget && defaultTarget->isAlive()) doRemove(SPELL_SNOBOLLED, defaultTarget);
//      if (pBoss && pBoss->isAlive()) doRemove(SPELL_RISING_ANGER,pBoss);
//      This string - not offlike, in off this buff not removed! especially for small servers.
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_BATTER, uiDiff);

        if (timedCast(SPELL_FIRE_BOMB, uiDiff, m_creature->getVictim()) == CAST_OK) {
        doCast(SPELL_FIRE_BOMB_1, m_creature->getVictim());
        doCast(SPELL_FIRE_BOMB_DOT, m_creature->getVictim());
        }

        timedCast(SPELL_HEAD_CRACK, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_snobold_vassal(Creature* pCreature)
{
    return new mob_snobold_vassalAI(pCreature);
}

struct MANGOS_DLL_DECL boss_acidmawAI : public BSWScriptedAI
{
    boss_acidmawAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;
    bool enraged;

    void Reset()
    {
        stage = 1;
        enraged = false;
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(7*DAY);
        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ACIDMAW_SUBMERGED);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            if (Creature* pSister = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_DREADSCALE)))
               if (!pSister->isAlive())
                         m_pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_DONE);
                else m_pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_SPECIAL);
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
        if (m_pInstance->GetData(TYPE_BEASTS) == IN_PROGRESS
            && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != FAIL)
                        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if ((!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
        && (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != ACIDMAW_SUBMERGED))
            return;

    switch (stage) 
        {
        case 0: {
                timedCast(SPELL_ACID_SPEW, uiDiff);

                timedCast(SPELL_PARALYTIC_BITE, uiDiff);

                timedCast(SPELL_ACID_SPIT, uiDiff);

                timedCast(SPELL_PARALYTIC_SPRAY, uiDiff);

                timedCast(SPELL_SWEEP_0, uiDiff);

                if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == ACIDMAW_SUBMERGED)
                     stage = 1;

                    break;}
        case 1: {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->InterruptNonMeleeSpells(true);
                    doCast(SPELL_SUBMERGE_0);
                    stage = 2;
                    DoScriptText(-1713557,m_creature);
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ACIDMAW_SUBMERGED);
                    break;}
        case 2: {
                if (timedQuery(SPELL_SLIME_POOL, uiDiff))
                    doCast(NPC_SLIME_POOL);

                if ((timedQuery(SPELL_SUBMERGE_0, uiDiff) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == ACIDMAW_SUBMERGED)
                    || m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DREADSCALE_SUBMERGED)
                        stage = 3;
                    break;}
        case 3: {
                    DoScriptText(-1713559,m_creature);
                    doRemove(SPELL_SUBMERGE_0);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    stage = 0;
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DREADSCALE_SUBMERGED);
                    break;}
        }

        if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == SNAKES_SPECIAL && !enraged)
                        {
                        DoScriptText(-1713559,m_creature);
                        doRemove(SPELL_SUBMERGE_0);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        doCast(SPELL_ENRAGE);
                        enraged = true;
                        stage = 0;
                        DoScriptText(-1713504,m_creature);
                        };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_acidmaw(Creature* pCreature)
{
    return new boss_acidmawAI(pCreature);
}

struct MANGOS_DLL_DECL boss_dreadscaleAI : public BSWScriptedAI
{
    boss_dreadscaleAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;
    bool enraged;

    void Reset()
    {
        stage = 0;
        enraged = false;
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(7*DAY);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            if (Creature* pSister = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ACIDMAW)))
               if (!pSister->isAlive())
                         m_pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_DONE);
                else m_pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_SPECIAL);
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
        if (m_pInstance->GetData(TYPE_BEASTS) == IN_PROGRESS
            && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != FAIL)
                        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if ((!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
        && (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != DREADSCALE_SUBMERGED))
            return;

        switch (stage) 
        {
        case 0: {
                timedCast(SPELL_BURNING_BITE, uiDiff);

                timedCast(SPELL_MOLTEN_SPEW, uiDiff);

                timedCast(SPELL_FIRE_SPIT, uiDiff);

                timedCast(SPELL_BURNING_SPRAY, uiDiff);

                timedCast(SPELL_SWEEP_0, uiDiff);

                if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DREADSCALE_SUBMERGED)
                     stage = 1;

                    break;}
        case 1: {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->InterruptNonMeleeSpells(true);
                    doCast(SPELL_SUBMERGE_0);
                    stage = 2;
                    DoScriptText(-1713557,m_creature);
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DREADSCALE_SUBMERGED);
                    break;}
        case 2: {

                if (timedQuery(SPELL_SLIME_POOL, uiDiff))
                    doCast(NPC_SLIME_POOL);

                if ((timedQuery(SPELL_SUBMERGE_0, uiDiff) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DREADSCALE_SUBMERGED)
                    || m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == ACIDMAW_SUBMERGED)
                         stage = 3;
                    break;}
        case 3: {
                    DoScriptText(-1713559,m_creature);
                    doRemove(SPELL_SUBMERGE_0);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    stage = 0;
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ACIDMAW_SUBMERGED);
                    break;}
        }

        if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == SNAKES_SPECIAL && !enraged)
                        {
                        DoScriptText(-1713559,m_creature);
                        doRemove(SPELL_SUBMERGE_0);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        doCast(SPELL_ENRAGE);
                        enraged = true;
                        stage = 0;
                        DoScriptText(-1713504,m_creature);
                        };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dreadscale(Creature* pCreature)
{
    return new boss_dreadscaleAI(pCreature);
}

struct MANGOS_DLL_DECL mob_slime_poolAI : public BSWScriptedAI
{
    mob_slime_poolAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    float m_Size;
    bool cloudcasted;

    void Reset()
    {
        if(!m_pInstance) return;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetInCombatWithZone();
        m_creature->SetSpeedRate(MOVE_RUN, 0.05f);
        SetCombatMovement(false);
        m_creature->GetMotionMaster()->MoveRandom();
        doCast(SPELL_SLIME_POOL_2);
        m_Size = m_creature->GetFloatValue(OBJECT_FIELD_SCALE_X);
        cloudcasted = false;
    }

    void AttackStart(Unit *who)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!cloudcasted) {
                          doCast(SPELL_SLIME_POOL_VISUAL);
                          cloudcasted = true;
                          }

        if (timedQuery(SPELL_SLIME_POOL_2,uiDiff)) {
                m_Size = m_Size*1.035;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_Size);
                }
                // Override especially for clean core
                   if (m_Size >= 6.0f) m_creature->ForcedDespawn();
    }

};

CreatureAI* GetAI_mob_slime_pool(Creature* pCreature)
{
    return new mob_slime_poolAI(pCreature);
}

struct MANGOS_DLL_DECL boss_icehowlAI : public BSWScriptedAI
{
    boss_icehowlAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool MovementStarted;
    bool TrampleCasted;
    uint8 stage;
    float fPosX, fPosY, fPosZ;
    Unit* pTarget;

    void Reset() {
        if(!m_pInstance) return;
        m_creature->SetRespawnDelay(7*DAY);
        MovementStarted = false;
        stage = 0;
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_DONE);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(!m_pInstance) return;
        if(type != POINT_MOTION_TYPE) return;
        if(id != 1 && MovementStarted) 
        {
             m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
        }
        else    {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                }
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (stage) 
        {
        case 0: {
                 timedCast(SPELL_FEROCIOUS_BUTT, uiDiff);

                 timedCast(SPELL_ARCTIC_BREATH, uiDiff);

                 timedCast(SPELL_WHIRL, uiDiff);

                if (timedQuery(SPELL_MASSIVE_CRASH, uiDiff)) stage = 1;

                timedCast(SPELL_FROTHING_RAGE, uiDiff);

                DoMeleeAttackIfReady();

                break;
                }
        case 1: {
                         if (doCast(SPELL_MASSIVE_CRASH) == CAST_OK)
                             stage = 2;
                 break;
                }
        case 2: {
                        if (pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0)) {
                        TrampleCasted = false;
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        stage = 3;
                        resetTimer(SPELL_TRAMPLE);
                        DoScriptText(-1713506,m_creature,pTarget);
                        SetCombatMovement(false);
                        m_creature->GetMotionMaster()->MoveIdle();
                        }
                 break;
                }
        case 3: {
                if (timedQuery(SPELL_TRAMPLE,uiDiff)) {
                        if (pTarget && pTarget->isAlive() && (pTarget->IsWithinDistInMap(m_creature, 200.0f))) {
                                    pTarget->GetPosition(fPosX, fPosY, fPosZ);
                                    TrampleCasted = false;
                                    MovementStarted = true;
                                    m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
                                    DoScriptText(-1713508,m_creature);
                                    doCast(SPELL_ADRENALINE);
                                    stage = 4;
                                    }
                        else        {
                                    TrampleCasted = true;
                                    stage = 5;
                                    }
                        }
                break;
                }
        case 4: {
                if (MovementStarted)
                    {
                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 5.0f)) {
                                doCast(SPELL_TRAMPLE, pPlayer);
                                TrampleCasted = true;
                                MovementStarted = false;
                                m_creature->GetMotionMaster()->MovementExpired();
                                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            }
                    }

                    } else stage = 5;
                    if (TrampleCasted) stage = 5;
                break;
                }
        case 5: {
                if (!TrampleCasted) {
                                    doCast(SPELL_STAGGERED_DAZE);
                                    DoScriptText(-1713507,m_creature);
                                    }
                MovementStarted = false;
                m_creature->GetMotionMaster()->MovementExpired();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                SetCombatMovement(true);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                stage = 0;
                break;
                }
        }

    }
};

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}

void AddSC_northrend_beasts()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_gormok";
    newscript->GetAI = &GetAI_boss_gormok;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_acidmaw";
    newscript->GetAI = &GetAI_boss_acidmaw;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_dreadscale";
    newscript->GetAI = &GetAI_boss_dreadscale;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_icehowl";
    newscript->GetAI = &GetAI_boss_icehowl;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_snobold_vassal";
    newscript->GetAI = &GetAI_mob_snobold_vassal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_slime_pool";
    newscript->GetAI = &GetAI_mob_slime_pool;
    newscript->RegisterSelf();

}
