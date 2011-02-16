/* Copyright (C) 2010 - 2011 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_baltharus
SD%Complete: 90%
SDComment: by notagain and /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */
// Not fully offlike clone work, but Blizz idea is intact.
// Need correct timers

#include "precompiled.h"
#include "ruby_sanctum.h"

static Locations SpawnLoc[]=
{
    {3152.329834f, 359.41757f, 85.301605f},    // Baltharus target point
    {3153.06f, 389.486f, 86.2596f},            // Baltharus initial point
};

enum Equipment
{
    EQUIP_MAIN           = 49888,
    EQUIP_OFFHAND        = EQUIP_NO_CHANGE,
    EQUIP_RANGED         = EQUIP_NO_CHANGE,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum BossSpells
{
    SPELL_BLADE_TEMPEST              = 75125, // every 22 secs
    SPELL_ENERVATING_BRAND           = 74502, // friendlies in 12yards = 74505
    SPELL_REPELLING_WAVE             = 74509, // once if call clone
    SPELL_SABER_LASH                 = 40504, // every 10-15 secs
    SPELL_SUMMON_CLONE               = 74511, // summons npc 39899 (Clone)
    SPELL_CHANNEL_SPELL              = 76221, // Channeling dummy spell
};

/*######
## boss_baltharus
######*/

struct MANGOS_DLL_DECL boss_baltharusAI : public BSWScriptedAI
{
    boss_baltharusAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    Creature* pDummyTarget;
    Creature* pClone;
    bool inCombat;
    bool intro;

    void Reset()
    {
        if(!pInstance)
            return;

        if (m_creature->isAlive()) pInstance->SetData(TYPE_BALTHARUS, NOT_STARTED);
        m_creature->SetRespawnDelay(7*DAY);
        resetTimers();
        setStage(0);
        pClone = NULL;
        inCombat = false;
        intro = false;
        if (pDummyTarget = m_creature->GetMap()->GetCreature( pInstance->GetData64(NPC_BALTHARUS_TARGET)))
        {
            if (!pDummyTarget->isAlive()) pDummyTarget->Respawn();

            pDummyTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            pDummyTarget->GetMotionMaster()->MoveIdle();
        }
        else if (pDummyTarget = m_creature->SummonCreature(NPC_BALTHARUS_TARGET, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 1000))
        {
            pDummyTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            pDummyTarget->GetMotionMaster()->MoveIdle();
        }

        if(Creature* pTarget = m_creature->GetMap()->GetCreature( pInstance->GetData64(NPC_XERESTRASZA)))
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTarget->GetGUID());
    }

    void JustReachedHome()
    {
        if (!pInstance) return;

        pInstance->SetData(TYPE_BALTHARUS, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        ScriptedAI::MoveInLineOfSight(pWho);
        if(!pInstance || intro ||
            pWho->GetTypeId() != TYPEID_PLAYER ||
            !pWho->IsWithinDistInMap(m_creature, 60.0f)) return;

        pInstance->SetData(TYPE_EVENT, 10);
        DoScriptText(-1666305,m_creature);
        intro = true;
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance) return;

        if (pDummyTarget) pDummyTarget->ForcedDespawn();
        DoScriptText(-1666303,m_creature);
        pInstance->SetData(TYPE_BALTHARUS, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666301,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666302,m_creature,pVictim);
               break;
        };
    }

    void JustSummoned(Creature* summoned)
    {
        if(!pInstance || !summoned) return;

        if ( summoned->GetEntry() != NPC_BALTHARUS_TARGET )
        {
             if (!pClone) pClone = summoned;
             else if (!pClone->isAlive()) pClone = summoned;
             pClone->SetInCombatWithZone();
        }
    }

    void SummonedCreatureJustDied(Creature* summoned)
    {
         if (!pInstance || !summoned) return;

         if (summoned == pClone) pClone = NULL;
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance) return;
        if (pWho->GetTypeId() != TYPEID_PLAYER) return;

        if (pDummyTarget) pDummyTarget->ForcedDespawn();

        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);

        inCombat = true;
        m_creature->InterruptNonMeleeSpells(true);
        SetCombatMovement(true);
        pInstance->SetData(TYPE_BALTHARUS, IN_PROGRESS);
        DoScriptText(-1666300,m_creature);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance) return;

        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) 
          return;

        if (pClone && pClone->isAlive())
        {
            pDoneBy->DealDamage(pClone, uiDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            uiDamage = 0;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance) return;

        if (!inCombat && !m_creature->IsNonMeleeSpellCasted(false))
            timedCast(SPELL_CHANNEL_SPELL, uiDiff, pDummyTarget);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (getStage())
        {
            case 0:
                 if ( m_creature->GetHealthPercent() <= 66.0f) setStage(1);
                 break;

            case 1:
                 m_creature->InterruptNonMeleeSpells(true);
                 if (is25())
                     doCast(SPELL_SUMMON_CLONE);
                 setStage(2);
                 break;

            case 2:
                 if (m_creature->IsNonMeleeSpellCasted(false)) return;
                 doCast(SPELL_REPELLING_WAVE);
                 setStage(3);

            case 3:
                 if ( m_creature->GetHealthPercent() <= 50.0f) setStage(4);
                 break;

            case 4:
                 m_creature->InterruptNonMeleeSpells(true);
                 if (!is25())
                        doCast(SPELL_SUMMON_CLONE);
                 setStage(5);
                 break;

            case 5:
                 if (m_creature->IsNonMeleeSpellCasted(false)) return;
                 doCast(SPELL_REPELLING_WAVE);
                 setStage(6);

            case 6:
                 if ( m_creature->GetHealthPercent() <= 33.0f) setStage(7);
                 break;

            case 7:
                 m_creature->InterruptNonMeleeSpells(true);
                 if (is25())
                     doCast(SPELL_SUMMON_CLONE);
                 setStage(8);
                 break;

            case 8:
                 if (m_creature->IsNonMeleeSpellCasted(false)) return;
                 doCast(SPELL_REPELLING_WAVE);
                 setStage(9);

            case 9:
            default:
                 break;
        }

        timedCast(SPELL_BLADE_TEMPEST, uiDiff);
        timedCast(SPELL_ENERVATING_BRAND, uiDiff);
        timedCast(SPELL_SABER_LASH, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_baltharus(Creature* pCreature)
{
    return new boss_baltharusAI(pCreature);
}

/*######
## mob_baltharus_clone
######*/

struct MANGOS_DLL_DECL mob_baltharus_cloneAI : public BSWScriptedAI
{
    mob_baltharus_cloneAI(Creature* pCreature) : BSWScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        if(!pInstance) return;
        resetTimers();
        m_creature->SetRespawnDelay(7*DAY);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666301,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666302,m_creature,pVictim);
               break;
        };
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance) return;
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance) return;

        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);

        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!pInstance) return;

        if (pInstance->GetData(TYPE_BALTHARUS) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        doCastAll(uiDiff);

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_baltharus_clone(Creature* pCreature)
{
    return new mob_baltharus_cloneAI(pCreature);
}


void AddSC_boss_baltharus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_baltharus";
    newscript->GetAI = &GetAI_boss_baltharus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_baltharus_clone";
    newscript->GetAI = &GetAI_mob_baltharus_clone;
    newscript->RegisterSelf();
}
