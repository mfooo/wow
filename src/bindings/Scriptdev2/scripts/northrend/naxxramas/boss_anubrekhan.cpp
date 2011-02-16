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
SDName: Boss_Anubrekhan
SD%Complete: 70
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                   = -1533000,
    SAY_AGGRO1                  = -1533001,
    SAY_AGGRO2                  = -1533002,
    SAY_AGGRO3                  = -1533003,
    SAY_TAUNT1                  = -1533004,
    SAY_TAUNT2                  = -1533005,
    SAY_TAUNT3                  = -1533006,
    SAY_TAUNT4                  = -1533007,
    SAY_SLAY                    = -1533008,

    EMOTE_CRYPT_GUARD           = -1533153,                 // NYI
    EMOTE_INSECT_SWARM          = -1533154,                 // NYI
    EMOTE_CORPSE_SCARABS        = -1533155,                 // NYI

    SPELL_IMPALE                = 28783,                    //May be wrong spell id. Causes more dmg than I expect
    SPELL_IMPALE_H              = 56090,
    SPELL_LOCUSTSWARM           = 28785,                    //This is a self buff that triggers the dmg debuff
    SPELL_LOCUSTSWARM_H         = 54021,
    SPELL_BERSERK               = 26662,

    //spellId invalid
    SPELL_SUMMONGUARD           = 29508,                    //Summons 1 crypt guard at targeted location

    SPELL_SELF_SPAWN_5          = 29105,                    //This spawns 5 corpse scarabs ontop of us (most likely the pPlayer casts this on death)
    SPELL_SELF_SPAWN_10         = 28864,                    //This is used by the crypt guards when they die

    NPC_CRYPT_GUARD             = 16573,
    SPELL_ACID_SPIT             = 28969,
    SPELL_ACID_SPIT_H           = 56098,
    SPELL_CLEAVE                = 40504,
    SPELL_FRENZY                = 8269
};

float m_afCryptGuardPos[4] = {3305.48f, -3476.29f, 287.08f, 3.1416f};

struct MANGOS_DLL_DECL boss_anubrekhanAI : public ScriptedAI
{
    boss_anubrekhanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiImpaleTimer;
    uint32 m_uiLocustSwarmTimer;
    uint32 m_uiSummonTimer;
    bool   m_bHasTaunted;
    uint32 m_uiImpaleDelayTimer;
    Unit*  m_pImpaleVictim;
    bool   m_bSummonGuard;
    uint32 m_uiBerserkTimer;
    std::list<uint64> m_lCryptGuardList;

    void Reset()
    {
        m_uiImpaleTimer = 15000;                            // 15 seconds
        m_uiLocustSwarmTimer = urand(70000, 120000);        // Random time between 70 seconds and 2 minutes for initial cast
        m_uiSummonTimer = urand(18000, 22000);
        m_uiImpaleDelayTimer = 0;
        m_pImpaleVictim = NULL;
        m_bSummonGuard = m_bIsRegularMode;
        m_uiBerserkTimer = 600000;

        DespawnGuards();
        if (!m_bIsRegularMode)
        {
            int iDirection = 1;
            for (uint8 i = 0; i <= 1; i++)
            {
                if (Creature* pTemp = m_creature->SummonCreature(NPC_CRYPT_GUARD, m_afCryptGuardPos[0], m_afCryptGuardPos[1]+20*iDirection, m_afCryptGuardPos[2], m_afCryptGuardPos[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
                    m_lCryptGuardList.push_back(pTemp->GetGUID());

                iDirection = -1;
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        //Force the player to spawn corpse scarabs via spell
        if (urand(0, 1))
            if (pVictim->GetTypeId() == TYPEID_PLAYER)
            {
                pVictim->CastSpell(pVictim, SPELL_SELF_SPAWN_5, true);
                m_creature->MonsterTextEmote("Corpse Scarabs appear from fighter's corpse!", 0, true);
            }

        if (urand(0, 4))
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (!m_lCryptGuardList.empty())
            for (std::list<uint64>::iterator itr = m_lCryptGuardList.begin(); itr != m_lCryptGuardList.end(); ++itr)
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    pTemp->AI()->AttackStart(pWho);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, IN_PROGRESS);

        m_creature->SetInCombatWithZone();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 100.0f))
        {
            switch(urand(0, 4))
            {
                case 0: DoScriptText(SAY_GREET, m_creature); break;
                case 1: DoScriptText(SAY_TAUNT1, m_creature); break;
                case 2: DoScriptText(SAY_TAUNT2, m_creature); break;
                case 3: DoScriptText(SAY_TAUNT3, m_creature); break;
                case 4: DoScriptText(SAY_TAUNT4, m_creature); break;
            }
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void DespawnGuards()
    {
        if (m_lCryptGuardList.empty())
            return;

        for (std::list<uint64>::iterator itr = m_lCryptGuardList.begin(); itr != m_lCryptGuardList.end(); ++itr)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    pTemp->ForcedDespawn();
        }

        m_lCryptGuardList.clear();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BERSERK);
            m_uiBerserkTimer = 300000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        if (m_uiImpaleDelayTimer)
            if (m_uiImpaleDelayTimer <= uiDiff)
            {
                if (!m_bIsRegularMode)
                    DoCastSpellIfCan(m_pImpaleVictim, SPELL_IMPALE_H);
                if (m_pImpaleVictim)
                    m_creature->getThreatManager().addThreat(m_pImpaleVictim, -100000000.0f);
                m_uiImpaleDelayTimer = 0;
            }
            else
            {
                m_uiImpaleDelayTimer -= uiDiff;
                return;
            }

        // Impale
        if (m_uiImpaleTimer < uiDiff)
        {
            //Cast Impale on a random target
            //Do NOT cast it when we are afflicted by locust swarm
            if (!m_creature->HasAura(SPELL_LOCUSTSWARM) && !m_creature->HasAura(SPELL_LOCUSTSWARM_H))
            {
                m_pImpaleVictim = NULL;
                if (m_pImpaleVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->getThreatManager().addThreat(m_pImpaleVictim, 100000000.0f);
                    if (!m_bIsRegularMode)
                        m_uiImpaleDelayTimer = 100;
                    else
                    {
                        DoCastSpellIfCan(m_pImpaleVictim, SPELL_IMPALE);
                        m_uiImpaleDelayTimer = 1100;
                    }
                }
            }

            m_uiImpaleTimer = 15000;
        }
        else
            m_uiImpaleTimer -= uiDiff;

        // Locust Swarm
        if (m_uiLocustSwarmTimer < uiDiff)
        {
            m_creature->MonsterTextEmote("%s begins to unleash an insect swarm!", 0, true);
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LOCUSTSWARM : SPELL_LOCUSTSWARM_H);
            m_uiLocustSwarmTimer = urand(70000, 120000);
            m_bSummonGuard = true;
            m_uiSummonTimer = 3000;
        }
        else
            m_uiLocustSwarmTimer -= uiDiff;

        // Summon
        if (m_bSummonGuard && (m_uiSummonTimer < uiDiff))
        {
            if (Creature* pTemp = m_creature->SummonCreature(NPC_CRYPT_GUARD, m_afCryptGuardPos[0], m_afCryptGuardPos[1], m_afCryptGuardPos[2], m_afCryptGuardPos[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
            {
                pTemp->MonsterTextEmote("A %s joins the fight!", 0, true);
                m_lCryptGuardList.push_back(pTemp->GetGUID());
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pTemp->AI()->AttackStart(pTarget);
                m_bSummonGuard = false;
            }
        }
        else
            m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_crypt_guardAI : public ScriptedAI
{
    mob_crypt_guardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiAcidSpitTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiBerserkTimer;

    void Reset()
    {
        m_uiAcidSpitTimer = 10000 + rand()%1000;
        m_uiCleaveTimer = 5000 + rand()%5000;
        m_uiBerserkTimer = 120000;
    }

    void KilledUnit(Unit* pVictim)
    {
        //Force the player to spawn corpse scarabs via spell
        if (urand(0, 1))
            if (pVictim->GetTypeId() == TYPEID_PLAYER)
            {
                pVictim->CastSpell(pVictim, SPELL_SELF_SPAWN_5, true);
                m_creature->MonsterTextEmote("Corpse Scarabs appear from fighter's corpse!", 0, true);
            }
    }

    void JustDied(Unit* pKiller)
    {
        if (urand(0, 1))
        {
            m_creature->CastSpell(m_creature, SPELL_SELF_SPAWN_10, true);
            m_creature->MonsterTextEmote("Corpse Scarabs appear from %s's corpse!", 0, true);
        }
}

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pAnubRekhan = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ANUB_REKHAN)))
                if (pAnubRekhan->isAlive() && !pAnubRekhan->getVictim())
                    pAnubRekhan->AI()->AttackStart(pWho);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
            if (m_uiBerserkTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_FRENZY);
                m_uiBerserkTimer = 0;
            }
            else
                m_uiBerserkTimer -= uiDiff;

        if (m_uiAcidSpitTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ACID_SPIT : SPELL_ACID_SPIT_H);
            m_uiAcidSpitTimer = 10000 + rand()%1000;
        }
        else
            m_uiAcidSpitTimer -= uiDiff;

        if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 5000 + rand()%5000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubrekhan(Creature* pCreature)
{
    return new boss_anubrekhanAI(pCreature);
}

CreatureAI* GetAI_mob_crypt_guard(Creature* pCreature)
{
    return new mob_crypt_guardAI(pCreature);
}

void AddSC_boss_anubrekhan()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_anubrekhan";
    NewScript->GetAI = &GetAI_boss_anubrekhan;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_crypt_guard";
    NewScript->GetAI = &GetAI_mob_crypt_guard;
    NewScript->RegisterSelf();
}
