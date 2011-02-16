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
SDName: boss_krikthir
SD%Complete: ?%
SDComment: by MaxXx2021
SDCategory: Azjol-Nerub instance
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum Spells
{
    SPELL_MIND_FLAY                               = 52586,
    H_SPELL_MIND_FLAY                             = 59367,
    SPELL_CURSE_OF_FATIGUE                        = 52592,
    H_SPELL_CURSE_OF_FATIGUE                      = 59368,
    SPELL_FRENZY                                  = 28747, //maybe 53361
    SPELL_SUMMON_SKITTERING_SWARMER               = 52438, //AOE Effect 140, maybe 52439
    SPELL_SUMMON_SKITTERING_SWARMER_1             = 52439, //Summon 3x 28735
    H_SPELL_ACID_SPLASH                           = 59363,
    SPELL_ACID_SPLASH                             = 52446,
    SPELL_CHARGE                                  = 16979,//maybe is another spell
    SPELL_BACKSTAB                                = 52540,
    SPELL_SHADOW_BOLT                             = 52534,
    H_SPELL_SHADOW_BOLT                           = 59357,
    SPELL_SHADOW_NOVA                             = 52535,
    H_SPELL_SHADOW_NOVA                           = 59358,
    SPELL_STRIKE                                  = 52532,
    SPELL_CLEAVE                                  = 49806,
    SPELL_ENRAGE                                  = 52470,
    SPELL_INFECTED_BITE                           = 52469,
    H_SPELL_INFECTED_BITE                         = 59364,
    SPELL_WEB_WRAP                                = 52086,//the spell is not working properly
    SPELL_BLINDING_WEBS                           = 52524,
    H_SPELL_BLINDING_WEBS                         = 59365,
    SPELL_POSION_SPRAY                            = 52493,
    H_SPELL_POSION_SPRAY                          = 59366,

    MOB_SKITTERING_SWARMER                        = 28735,
    MOB_SKITTERING_SWARMER_CONTROLLER             = 32593,
    MOB_SKITTERING_INFECTIOR                      = 28736,

    SAY_AGGRO                                     = -1601011,
    SAY_SLAY_1                                    = -1601012,
    SAY_SLAY_2                                    = -1601013,
    SAY_DEATH                                     = -1601014,
    //Not in db
    SAY_SEND_GROUP_1                              = -1601020,
    SAY_SEND_GROUP_2                              = -1601021,
    SAY_SEND_GROUP_3                              = -1601022,
    SAY_SWARM_1                                   = -1601015,
    SAY_SWARM_2                                   = -1601016,
    SAY_PREFIGHT_1                                = -1601017,
    SAY_PREFIGHT_2                                = -1601018,
    SAY_PREFIGHT_3                                = -1601019,

    ACHIEV_WATH_HIM_DIE                           = 1296
};

struct Locations
{
    float x, y, z, o;
    uint32 id;
};

static Locations SpawnPoint[8] =
{
    {566.164f, 682.087f, 769.079f, 2.21f},
    {529.042f, 706.941f, 777.298f, 1.08f},
    {489.975f, 671.239f, 772.131f, 0.26f},
    {488.556f, 692.950f, 771.764f, 4.88f},
    {553.340f, 640.387f, 777.419f, 1.20f},
    {517.486f, 706.398f, 777.335f, 5.35f},
    {504.010f, 637.693f, 777.479f, 0.50f},
    {552.625f, 706.408f, 777.177f, 3.43f}
};
struct MANGOS_DLL_DECL boss_krikthirAI : public ScriptedAI
{
    boss_krikthirAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 uiMindFlayTimer;
    uint32 uiCurseFatigueTimer;
    uint32 uiSummonTimer;

    void Reset()
    {
        uiMindFlayTimer = 15000;
        uiCurseFatigueTimer = 12000;

        if (pInstance)
            pInstance->SetData(TYPE_KRIKTHIR, NOT_STARTED);
    }

    void EnterCombat(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        Summon();
        uiSummonTimer = 15000;

        if (pInstance)
            pInstance->SetData(TYPE_KRIKTHIR, IN_PROGRESS);
    }

    void Summon()
    {
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[0].x,SpawnPoint[0].y,SpawnPoint[0].z,SpawnPoint[0].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[0].x,SpawnPoint[0].y,SpawnPoint[0].z,SpawnPoint[0].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[1].x,SpawnPoint[1].y,SpawnPoint[1].z,SpawnPoint[1].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[1].x,SpawnPoint[1].y,SpawnPoint[1].z,SpawnPoint[1].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[2].x,SpawnPoint[2].y,SpawnPoint[2].z,SpawnPoint[2].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[2].x,SpawnPoint[2].y,SpawnPoint[2].z,SpawnPoint[2].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[3].x,SpawnPoint[3].y,SpawnPoint[3].z,SpawnPoint[3].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[3].x,SpawnPoint[3].y,SpawnPoint[3].z,SpawnPoint[3].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_INFECTIOR,SpawnPoint[4].x,SpawnPoint[4].y,SpawnPoint[4].z,SpawnPoint[4].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[4].x,SpawnPoint[4].y,SpawnPoint[4].z,SpawnPoint[4].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_INFECTIOR,SpawnPoint[5].x,SpawnPoint[5].y,SpawnPoint[5].z,SpawnPoint[5].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[5].x,SpawnPoint[5].y,SpawnPoint[5].z,SpawnPoint[5].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_INFECTIOR,SpawnPoint[6].x,SpawnPoint[6].y,SpawnPoint[6].z,SpawnPoint[6].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[6].x,SpawnPoint[6].y,SpawnPoint[6].z,SpawnPoint[6].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[7].x,SpawnPoint[7].y,SpawnPoint[7].z,SpawnPoint[7].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,SpawnPoint[7].x,SpawnPoint[7].y,SpawnPoint[7].z,SpawnPoint[7].o,TEMPSUMMON_CORPSE_TIMED_DESPAWN,25000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiSummonTimer <= diff)
        {
            Summon();
            uiSummonTimer = 15000;
        } else uiSummonTimer -= diff;

        if (uiMindFlayTimer <= diff)
        {
                DoCast(m_creature->getVictim(), SPELL_MIND_FLAY);
                uiMindFlayTimer = 15000;
            } else uiMindFlayTimer -= diff;

        if (uiCurseFatigueTimer <= diff)
        {
            //WowWiki say "Curse of Fatigue-Kirk'thir will cast Curse of Fatigue on 2-3 targets periodically."
            if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
               DoCast(pTarget, SPELL_CURSE_OF_FATIGUE);
            if(Unit *pTarget_1 = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
               DoCast(pTarget_1, SPELL_CURSE_OF_FATIGUE);

            uiCurseFatigueTimer = 10000;
        } else uiCurseFatigueTimer -= diff;

        if (!m_creature->HasAura(SPELL_FRENZY) && m_creature->GetHealthPercent() < 10.0f)
            DoCast(m_creature, SPELL_FRENZY);

        DoMeleeAttackIfReady();
    }
    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
        {
            pInstance->SetData(TYPE_KRIKTHIR, DONE);
            //Achievement: Watch him die
            //Creature *pAdd1, *pAdd2, *pAdd3;
            //if ((pAdd1 = pInstance->instance->GetCreature(pInstance->GetData64(DATA_WATCHER_GASHRA))) && pAdd1->isAlive() &&
                //(pAdd2 = pInstance->instance->GetCreature(pInstance->GetData64(DATA_WATCHER_SILTHIK))) && pAdd2->isAlive() &&
                //(pAdd3 = pInstance->instance->GetCreature(pInstance->GetData64(DATA_WATCHER_NARJIL))) && pAdd3->isAlive() &&
                //m_bIsRegularMode)
                //pInstance->DoCompleteAchievement(ACHIEV_WATH_HIM_DIE);
        }
    }
    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        DoScriptText(urand(SAY_SLAY_1,SAY_SLAY_2), m_creature);
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->GetMotionMaster()->MovePoint(0,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ());
    }
};

struct MANGOS_DLL_DECL npc_skittering_infectorAI : public ScriptedAI
{
    npc_skittering_infectorAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

    void Reset()
    {
    }

    void JustDied(Unit* killer)
    {
        DoCast(m_creature->getVictim(), SPELL_ACID_SPLASH);
    }

    void UpdateAI(const uint32 uiDiff)
    {
         DoMeleeAttackIfReady();
         return;
    }
};

CreatureAI* GetAI_npc_skittering_infector(Creature* pCreature)
{
    return new npc_skittering_infectorAI(pCreature);
}

struct MANGOS_DLL_DECL npc_anub_ar_skirmisherAI : public ScriptedAI
{
    npc_anub_ar_skirmisherAI(Creature *c) : ScriptedAI(c) 
    {
       Reset();
    }

    uint32 uiChargeTimer;
    uint32 uiBackstabTimer;

    void Reset()
    {
        uiChargeTimer   = 11000;
        uiBackstabTimer = 7000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiChargeTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoResetThreat();
                m_creature->AddThreat(pTarget,1.0f);
                DoCast(pTarget, SPELL_CHARGE, true);
            }
            uiChargeTimer = 15000;
        } else uiChargeTimer -= diff;

        if (uiBackstabTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_BACKSTAB);
            uiBackstabTimer = 12000;
        } else uiBackstabTimer -= diff;

        DoMeleeAttackIfReady();

    }
};

struct MANGOS_DLL_DECL npc_anub_ar_shadowcasterAI : public ScriptedAI
{
    npc_anub_ar_shadowcasterAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
       Reset();
    }

    uint32 uiShadowBoltTimer;
    uint32 uiShadowNovaTimer;

    void Reset()
    {
        uiShadowBoltTimer = 6000;
        uiShadowNovaTimer = 15000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiShadowBoltTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                 DoCast(pTarget, SPELL_SHADOW_BOLT);
            uiShadowBoltTimer = 15000;
        } else uiShadowBoltTimer -= diff;

        if (uiShadowNovaTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHADOW_NOVA);
            uiShadowNovaTimer = 17000;
        } else uiShadowNovaTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_anub_ar_warriorAI : public ScriptedAI
{
    npc_anub_ar_warriorAI(Creature *c) : ScriptedAI(c){ Reset(); }

    uint32 uiCleaveTimer;
    uint32 uiStrikeTimer;

    void Reset()
    {
        uiCleaveTimer = 11000;
        uiStrikeTimer = 6000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiStrikeTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_STRIKE, true);
            uiStrikeTimer = 15000;
        } else uiStrikeTimer -= diff;

        if (uiCleaveTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE, true);
            uiCleaveTimer = 17000;
        } else uiCleaveTimer -= diff;

        DoMeleeAttackIfReady();

    }

};

struct MANGOS_DLL_DECL npc_watcher_gashraAI : public ScriptedAI
{
    npc_watcher_gashraAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
       Reset();
    }

    uint32 uiWebWrapTimer;
    uint32 uiInfectedBiteTimer;

    void Reset()
    {
        uiWebWrapTimer      = 11000;
        uiInfectedBiteTimer = 4000;
    }

    void EnterCombat(Unit* who)
    {
        DoCast(m_creature, SPELL_ENRAGE, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiWebWrapTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_WEB_WRAP);
            uiWebWrapTimer = 17000;
        } else uiWebWrapTimer -= diff;

        if (uiInfectedBiteTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_INFECTED_BITE);
            uiInfectedBiteTimer = 15000;
        } else uiInfectedBiteTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_watcher_narjilAI : public ScriptedAI
{
    npc_watcher_narjilAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
       Reset();
    }

    uint32 uiWebWrapTimer;
    uint32 uiInfectedBiteTimer;
    uint32 uiBindingWebsTimer;

    void Reset()
    {
        uiWebWrapTimer      = 11000;
        uiInfectedBiteTimer = 4000;
        uiBindingWebsTimer = 17000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiWebWrapTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_WEB_WRAP);
            uiWebWrapTimer = 15000;
        } else uiWebWrapTimer -= diff;

        if (uiInfectedBiteTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_INFECTED_BITE);
            uiInfectedBiteTimer = 11000;
        } else uiInfectedBiteTimer -= diff;

        if (uiBindingWebsTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_BLINDING_WEBS);
            uiBindingWebsTimer = 17000;
        } else uiBindingWebsTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_watcher_silthikAI : public ScriptedAI
{
    npc_watcher_silthikAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
       Reset();
    }

    uint32 uiWebWrapTimer;
    uint32 uiInfectedBiteTimer;
    uint32 uiPoisonSprayTimer;

    void Reset()
    {
        uiWebWrapTimer      = 11000;
        uiInfectedBiteTimer = 4000;
        uiPoisonSprayTimer  = 15000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiWebWrapTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_WEB_WRAP);

            uiWebWrapTimer = 15000;
        } else uiWebWrapTimer -= diff;

        if (uiInfectedBiteTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_INFECTED_BITE);
            uiInfectedBiteTimer = 15000;
        } else uiInfectedBiteTimer -= diff;

        if (uiPoisonSprayTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_POSION_SPRAY);
            uiPoisonSprayTimer = 17000;
        } else uiPoisonSprayTimer -= diff;

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_krikthir(Creature* pCreature)
{
    return new boss_krikthirAI(pCreature);
}

CreatureAI* GetAI_npc_anub_ar_skirmisher(Creature* pCreature)
{
    return new npc_anub_ar_skirmisherAI(pCreature);
}

CreatureAI* GetAI_npc_anub_ar_shadowcaster(Creature* pCreature)
{
    return new npc_anub_ar_shadowcasterAI(pCreature);
}

CreatureAI* GetAI_npc_anub_ar_warrior(Creature* pCreature)
{
    return new npc_anub_ar_warriorAI(pCreature);
}

CreatureAI* GetAI_npc_watcher_gashra(Creature* pCreature)
{
    return new npc_watcher_gashraAI(pCreature);
}

CreatureAI* GetAI_npc_watcher_narjil(Creature* pCreature)
{
    return new npc_watcher_narjilAI(pCreature);
}

CreatureAI* GetAI_npc_watcher_silthik(Creature* pCreature)
{
    return new npc_watcher_silthikAI(pCreature);
}

void AddSC_boss_krikthir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_krikthir";
    newscript->GetAI = &GetAI_boss_krikthir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_skittering_infector";
    newscript->GetAI = &GetAI_npc_skittering_infector;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_anub_ar_skirmisher";
    newscript->GetAI = &GetAI_npc_anub_ar_skirmisher;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_anub_ar_shadowcaster";
    newscript->GetAI = &GetAI_npc_anub_ar_shadowcaster;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_watcher_gashra";
    newscript->GetAI = &GetAI_npc_watcher_gashra;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_anub_ar_warrior";
    newscript->GetAI = &GetAI_npc_anub_ar_warrior;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_watcher_silthik";
    newscript->GetAI = &GetAI_npc_watcher_silthik;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_watcher_narjil";
    newscript->GetAI = &GetAI_npc_watcher_narjil;
    newscript->RegisterSelf();
}

