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
SDName: boss_anubarak_trial
SD%Complete: 70%
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

// Anubarak - underground phase partially not worked, timers need correct
// Burrower - underground phase not implemented, buff not worked.
// Leecheng Swarm spell not worked - awaiting core support
// Anubarak spike aura worked only after 9750

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Summons
{
    NPC_FROST_SPHERE     = 34606,
    NPC_BURROWER         = 34607,
    NPC_SCARAB           = 34605,
    NPC_SPIKE            = 34660,
};

enum BossSpells
{
SPELL_COLD              = 66013,
SPELL_MARK              = 67574,
SPELL_LEECHING_SWARM    = 66118,
SPELL_LEECHING_HEAL     = 66125,
SPELL_LEECHING_DAMAGE   = 66240,
SPELL_IMPALE            = 65920,
SPELL_SPIKE_CALL        = 66169,
SPELL_POUND             = 66012,
SPELL_SHOUT             = 67730,
SPELL_SUBMERGE_0        = 53421,
SPELL_SUBMERGE_1        = 67322,
SPELL_SUMMON_BEATLES    = 66339,
SPELL_DETERMINATION     = 66092,
SPELL_ACID_MANDIBLE     = 67861,
SPELL_SPIDER_FRENZY     = 66129,
SPELL_EXPOSE_WEAKNESS   = 67847,
SUMMON_SCARAB           = NPC_SCARAB,
SUMMON_BORROWER         = NPC_BURROWER,
SUMMON_FROSTSPHERE      = NPC_FROST_SPHERE,
SPELL_BERSERK           = 26662,
SPELL_PERMAFROST        = 66193,
};

struct MANGOS_DLL_DECL boss_anubarak_trialAI : public BSWScriptedAI
{
    boss_anubarak_trialAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;
    bool intro;
    Unit* pTarget;

    void Reset() {
        if(!m_pInstance) return;
        stage = 0;
        intro = true;
        m_creature->SetRespawnDelay(DAY);
        pTarget = NULL;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }


    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(-1713563,m_creature);
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (!intro) return;
        DoScriptText(-1713554,m_creature);
        intro = false;
        m_creature->SetInCombatWithZone();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, FAIL);
//            m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1713564,m_creature);
            m_pInstance->SetData(TYPE_ANUBARAK, DONE);
    }

    void Aggro(Unit* pWho)
    {
        if (!intro) DoScriptText(-1713555,m_creature);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                timedCast(SPELL_POUND, uiDiff);
                timedCast(SPELL_COLD, uiDiff);
                if (timedQuery(SUMMON_BORROWER, uiDiff)) {
                        doCast(SUMMON_BORROWER);
                        DoScriptText(-1713556,m_creature);
                        };
                if (timedQuery(SPELL_SUBMERGE_0, uiDiff)) stage = 1;

                    break;}
            case 1: {
                    doCast(SPELL_SUBMERGE_0);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    stage = 2;
                    DoScriptText(-1713557,m_creature);
                    break;}
            case 2: {
                    if (timedQuery(SPELL_SPIKE_CALL, uiDiff)) {
                         pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
//                         doCast(SPELL_SPIKE_CALL);
//                         This summon not supported in database. Temporary override.
                         Unit* spike = doSummon(NPC_SPIKE,TEMPSUMMON_TIMED_DESPAWN,60000);
                         if (spike) { spike->AddThreat(pTarget, 1000.0f);
                                      DoScriptText(-1713558,m_creature,pTarget);
                                      doCast(SPELL_MARK,pTarget);
                                      spike->GetMotionMaster()->MoveChase(pTarget);
                                     }
                         };
                    if (timedQuery(SPELL_SUMMON_BEATLES, uiDiff)) {
                            doCast(SPELL_SUMMON_BEATLES);
                            doCast(SUMMON_SCARAB);
                            DoScriptText(-1713560,m_creature);
                         };
                    if (timedQuery(SPELL_SUBMERGE_0, uiDiff)) stage = 3;
                    break;}
            case 3: {
                    stage = 0;
                    DoScriptText(-1713559,m_creature);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    doRemove(SPELL_SUBMERGE_0,m_creature);
                    break;}
            case 4: {
                            doCast(SPELL_LEECHING_SWARM);
                            DoScriptText(-1713561,m_creature);
                    stage = 5;
                    break;}
            case 5: {
                        timedCast(SPELL_POUND, uiDiff);
                        timedCast(SPELL_COLD, uiDiff);
                        break;}

        }
        timedCast(SUMMON_FROSTSPHERE, uiDiff);

        timedCast(SPELL_BERSERK, uiDiff);

        if (m_creature->GetHealthPercent() < 30.0f && stage == 0) stage = 4;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubarak_trial(Creature* pCreature)
{
    return new boss_anubarak_trialAI(pCreature);
}

struct MANGOS_DLL_DECL mob_swarm_scarabAI : public BSWScriptedAI
{
    mob_swarm_scarabAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_DETERMINATION, uiDiff);

        timedCast(SPELL_ACID_MANDIBLE, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_swarm_scarab(Creature* pCreature)
{
    return new mob_swarm_scarabAI(pCreature);
};

struct MANGOS_DLL_DECL mob_nerubian_borrowerAI : public BSWScriptedAI
{
    mob_nerubian_borrowerAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool submerged;
    Unit* currentTarget;

    void Reset()
    {
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
        submerged = false;
        currentTarget = NULL;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_EXPOSE_WEAKNESS, uiDiff);

        if (timedQuery(SPELL_SPIDER_FRENZY, uiDiff))
            if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_BURROWER, 50.0f))
            {
            currentTarget = pTemp;
            doCast(SPELL_SPIDER_FRENZY);
            };

        if (m_creature->GetHealthPercent() < 20.0f && timedQuery(SPELL_SUBMERGE_1, uiDiff) && !submerged)
           {
            doCast(SPELL_SUBMERGE_1);
            submerged = true;
            DoScriptText(-1713557,m_creature);
            };

        if (m_creature->GetHealthPercent() > 50.0f && submerged)
            {
             doRemove(SPELL_SUBMERGE_1,m_creature);
             submerged = false;
             DoScriptText(-1713559,m_creature);
             };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_nerubian_borrower(Creature* pCreature)
{
    return new mob_nerubian_borrowerAI(pCreature);
};

struct MANGOS_DLL_DECL mob_frost_sphereAI : public BSWScriptedAI
{
    mob_frost_sphereAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_creature->SetSpeedRate(MOVE_RUN, 0.1f);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MoveRandom();
    }

    void EnterCombat(Unit* attacker)
    {
        doCast(SPELL_PERMAFROST);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
           m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_frost_sphere(Creature* pCreature)
{
    return new mob_frost_sphereAI(pCreature);
};

struct MANGOS_DLL_DECL mob_anubarak_spikeAI : public BSWScriptedAI
{
    mob_anubarak_spikeAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    Unit* defaultTarget;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        defaultTarget = NULL;
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
        doCast(SPELL_IMPALE);
        defaultTarget = who;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
        if (defaultTarget)
            if (!defaultTarget->isAlive() || !hasAura(SPELL_MARK,defaultTarget))
                 m_creature->ForcedDespawn();

/*        if (timedQuery(SPELL_IMPALE,uiDiff)) {
        if (m_creature->IsWithinDist(m_creature->getVictim(), 4.0f)
            && !hasAura(SPELL_PERMAFROST,m_creature->getVictim()))
           {
              doCast(SPELL_IMPALE);
           }  else doRemove(SPELL_IMPALE);
        }*/
    }
};

CreatureAI* GetAI_mob_anubarak_spike(Creature* pCreature)
{
    return new mob_anubarak_spikeAI(pCreature);
};

void AddSC_boss_anubarak_trial()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_anubarak_trial";
    newscript->GetAI = &GetAI_boss_anubarak_trial;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_swarm_scarab";
    newscript->GetAI = &GetAI_mob_swarm_scarab;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nerubian_borrower";
    newscript->GetAI = &GetAI_mob_nerubian_borrower;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_anubarak_spike";
    newscript->GetAI = &GetAI_mob_anubarak_spike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frost_sphere";
    newscript->GetAI = &GetAI_mob_frost_sphere;
    newscript->RegisterSelf();

}