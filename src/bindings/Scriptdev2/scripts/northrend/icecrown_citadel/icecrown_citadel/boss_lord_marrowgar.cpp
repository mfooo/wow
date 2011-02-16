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
SDName: boss_lord_marrowgar
SD%Complete: 85%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */
#include "precompiled.h"
#include "def_spire.h"
enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        NPC_BONE_SPIKE                          = 38711,
        NPC_COLD_FLAME                          = 36672,
        //Abilities
        SPELL_SABER_LASH                        = 71021,
        SPELL_CALL_COLD_FLAME                   = 69138,
        SPELL_CALL_COLD_FLAME_1                 = 71580,
        SPELL_COLD_FLAME                        = 69146,
        SPELL_COLD_FLAME_0                      = 69145,
        SPELL_COLD_FLAME_1                      = 69147,
        SPELL_BONE_STRIKE                       = 69057,
        SPELL_BONE_STORM                        = 69076,
        SPELL_BONE_STRIKE_IMPALE                = 69065,
        SPELL_VEHICLE_HARDCODED                 = 46598,
        SPELL_BONE_STORM_STRIKE                 = 69075,
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public BSWScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool intro;

    void Reset()
    {
        if (!pInstance) return;
        if (m_creature->isAlive()) pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);
        resetTimers();
        m_creature->SetSpeedRate(MOVE_RUN, 1);
        m_creature->SetSpeedRate(MOVE_WALK, 1);
//        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        ScriptedAI::MoveInLineOfSight(pWho);
        if (intro) return;
        DoScriptText(-1631000,m_creature);
        intro = true;
    }

    void JustSummoned(Creature* summoned)
    {
        if(!pInstance || !summoned) return;
        summoned->SetCreatorGuid(m_creature->GetObjectGuid());
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_MARROWGAR, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
        DoScriptText(-1631001,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, DONE);
        DoScriptText(-1631009,m_creature);
    }

    void doSummonSpike(Unit* pTarget)
    {
        if (!pTarget || !pTarget->isAlive()) return;
        float fPosX, fPosY, fPosZ;
        pTarget->GetPosition(fPosX, fPosY, fPosZ);
        if (Unit* pSpike = doSummon(NPC_BONE_SPIKE, fPosX, fPosY, fPosZ + 0.5f))
        {
            pSpike->SetOwnerGuid(m_creature->GetObjectGuid());
            pSpike->SetInCombatWith(pTarget);
            pSpike->AddThreat(pTarget, 1000.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(getStage())
        {
            case 0: 
                    if (timedQuery(SPELL_BONE_STRIKE, diff))
                        if (Unit* pTarget = doSelectRandomPlayer(SPELL_BONE_STRIKE_IMPALE, false, 60.0f))
                            if (doCast(SPELL_BONE_STRIKE, pTarget) == CAST_OK)
                            {
                                doSummonSpike(pTarget);
                                switch (urand(0,1)) {
                                                   case 0:
                                                   DoScriptText(-1631003,m_creature,pTarget);
                                                   break;
                                                   case 1:
                                                   DoScriptText(-1631004,m_creature,pTarget);
                                                   break;
                                                   case 2:
                                                   DoScriptText(-1631005,m_creature,pTarget);
                                                   break;
                                                   };

                            };

                    if (timedQuery(SPELL_BONE_STORM, diff)) setStage(1);

                    if (timedQuery(SPELL_CALL_COLD_FLAME, diff))
                    {
                        if (urand(0,1)) doCast(SPELL_CALL_COLD_FLAME);
                            else  doCast(SPELL_CALL_COLD_FLAME_1);

                        if (m_creature->GetHealthPercent() <= 30.0f)
                        {
                            if (urand(0,1)) doCast(SPELL_CALL_COLD_FLAME);
                                else  doCast(SPELL_CALL_COLD_FLAME_1);
                        }
                    }

                    timedCast(SPELL_SABER_LASH, diff);

                    DoMeleeAttackIfReady();

                    break;
            case 1:
                    m_creature->InterruptNonMeleeSpells(true);
                    doCast(SPELL_BONE_STORM);
                    setStage(2);
                    DoScriptText(-1631002,m_creature);
                    DoResetThreat();
                    m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                    m_creature->SetSpeedRate(MOVE_RUN, 3);
                    m_creature->SetSpeedRate(MOVE_WALK, 3);
                    break;
            case 2:
                    if (!m_creature->IsNonMeleeSpellCasted(false)) setStage(3);
                    break;
            case 3:
                    if (isHeroic())
                        if (timedQuery(SPELL_BONE_STRIKE, diff, true))
                            if (Unit* pTarget = doSelectRandomPlayer(SPELL_BONE_STRIKE_IMPALE, false, 60.0f))
                                doSummonSpike(pTarget);

                    if (timedQuery(SPELL_CALL_COLD_FLAME, diff, true)
                        && m_creature->IsWithinDistInMap(m_creature->getVictim(),2.0f))
                    {
                        pInstance->SetData(DATA_DIRECTION, (uint32)(1000*2.0f*M_PI_F*((float)urand(1,16)/16.0f)));
//                        if (urand(0,1)) doCast(SPELL_CALL_COLD_FLAME);
//                            else  doCast(SPELL_CALL_COLD_FLAME_1);
                        float fPosX, fPosY, fPosZ;
                        m_creature->GetPosition(fPosX, fPosY, fPosZ);
                        doSummon(NPC_COLD_FLAME, fPosX, fPosY, fPosZ);
                        DoResetThreat();
                        if (Unit* pTarget = doSelectRandomPlayerAtRange(60.0f))
                            AttackStart(pTarget);
                    }
                    if (!hasAura(SPELL_BONE_STORM_STRIKE, m_creature) && !hasAura(SPELL_BONE_STORM, m_creature)) setStage(4);
                    break;
            case 4:
                    pInstance->SetData(DATA_DIRECTION, 0);
                    m_creature->SetSpeedRate(MOVE_RUN, 1);
                    m_creature->SetSpeedRate(MOVE_WALK, 1);
//                    m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
                    setStage(0);
                    break;
            default:
                    break;
            }

        if (timedQuery(SPELL_BERSERK, diff))
            {
                doCast(SPELL_BERSERK);
                DoScriptText(-1631008,m_creature);
            }


    }
};

struct MANGOS_DLL_DECL mob_coldflameAI : public BSWScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool isFirst;
    bool isXmode;
    float m_direction;
    float x, y, radius;
    bool isCreator;

    void Reset()
    {
        if(!m_pInstance) return;
//        m_creature->SetDisplayId(10045);
        m_creature->SetRespawnDelay(7*DAY);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        setStage(0);
        isCreator = false;

        SetCombatMovement(false);
        doCast(SPELL_COLD_FLAME_0);
    }

    void AttackStart(Unit *who)
    {
    }

    void JustSummoned(Creature* summoned)
    {
        if(!m_pInstance || !summoned) return;
        summoned->SetCreatorGuid(m_creature->GetObjectGuid());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
        {
            m_creature->ForcedDespawn();
        }

        if (m_creature->GetCreatorGuid().IsEmpty()) return;

        if (!isCreator)
        {
            if (m_creature->GetCreatorGuid() == m_pInstance->GetData64(NPC_LORD_MARROWGAR))
            {
                isFirst = true;
                uint32 m_tmpDirection = m_pInstance->GetData(DATA_DIRECTION);
                m_pInstance->SetData(DATA_DIRECTION,0);
                if (m_tmpDirection)
                {
                    m_direction = m_tmpDirection/1000.0f;
                    isXmode = true;
                }
                else
                {
                    m_direction = 2.0f*M_PI_F*((float)urand(1,16)/16.0f);
                    isXmode = false;
                }
            } else isFirst = false;
            isCreator = true;
        }

        if (timedQuery(SPELL_COLD_FLAME_0, uiDiff) && !isFirst)
            m_creature->ForcedDespawn();

        if (isFirst && timedQuery(SPELL_COLD_FLAME_1, uiDiff, true))
        {
            if (getStage() < getSpellData(SPELL_COLD_FLAME_0))
            {
                setStage(getStage()+1);
                radius = getStage()*5;
                m_creature->GetNearPoint2D(x, y, radius, m_direction);
                doSummon(NPC_COLD_FLAME, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, getSpellData(SPELL_COLD_FLAME_1));
                if (isXmode)
                {
                    m_creature->GetNearPoint2D(x, y, radius, m_direction+M_PI_F/2);
                    doSummon(NPC_COLD_FLAME, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, getSpellData(SPELL_COLD_FLAME_1));
                    m_creature->GetNearPoint2D(x, y, radius, m_direction+M_PI_F);
                    doSummon(NPC_COLD_FLAME, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, getSpellData(SPELL_COLD_FLAME_1));
                    m_creature->GetNearPoint2D(x, y, radius, m_direction+M_PI_F*1.5f);
                    doSummon(NPC_COLD_FLAME, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, getSpellData(SPELL_COLD_FLAME_1));
                }
            } else m_creature->ForcedDespawn();
        } else timedCast(SPELL_COLD_FLAME, uiDiff);

    }
};

struct MANGOS_DLL_DECL mob_bone_spikeAI : public BSWScriptedAI
{
    mob_bone_spikeAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    ObjectGuid victimGuid;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetRespawnDelay(7*DAY);
        victimGuid = ObjectGuid();
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        if (victimGuid.IsEmpty() && pWho && pWho->GetTypeId() == TYPEID_PLAYER)
        {
            victimGuid = pWho->GetObjectGuid();
            m_creature->SetInCombatWith(pWho);
            doCast(SPELL_BONE_STRIKE_IMPALE,pWho);
            doCast(SPELL_VEHICLE_HARDCODED,pWho);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
            if (Player* pVictim = m_creature->GetMap()->GetPlayer(victimGuid))
                doRemove(SPELL_BONE_STRIKE_IMPALE,pVictim);
    }

    void AttackStart(Unit *who)
    {
    }

    void KilledUnit(Unit* _Victim)
    {
        if (Player* pVictim = m_creature->GetMap()->GetPlayer(victimGuid))
            if (pVictim->GetObjectGuid() == victimGuid)
                doRemove(SPELL_BONE_STRIKE_IMPALE,pVictim);
    }

    void JustDied(Unit* Killer)
    {
        if (Player* pVictim = m_creature->GetMap()->GetPlayer(victimGuid))
            doRemove(SPELL_BONE_STRIKE_IMPALE,pVictim);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
        {
            if (Player* pVictim = m_creature->GetMap()->GetPlayer(victimGuid))
                doRemove(SPELL_BONE_STRIKE_IMPALE,pVictim);
            m_creature->ForcedDespawn();
        }

        if (victimGuid.IsEmpty())
            return;

        if (Player* pVictim = m_creature->GetMap()->GetPlayer(victimGuid))
        {
            if(!pVictim->isAlive())
                m_creature->ForcedDespawn();
        }
        else
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_bone_spike(Creature* pCreature)
{
    return new mob_bone_spikeAI(pCreature);
}

CreatureAI* GetAI_mob_coldflame(Creature* pCreature)
{
    return new mob_coldflameAI(pCreature);
}

CreatureAI* GetAI_boss_lord_marrowgar(Creature* pCreature)
{
    return new boss_lord_marrowgarAI(pCreature);
}

void AddSC_boss_lord_marrowgar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_marrowgar";
    newscript->GetAI = &GetAI_boss_lord_marrowgar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_coldflame";
    newscript->GetAI = &GetAI_mob_coldflame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bone_spike";
    newscript->GetAI = &GetAI_mob_bone_spike;
    newscript->RegisterSelf();

}
