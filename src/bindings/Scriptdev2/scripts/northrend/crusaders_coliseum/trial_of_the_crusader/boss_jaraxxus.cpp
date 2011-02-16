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
// Jaraxxus - Magic aura (from start?) not fully offlike implemented.
// Legion flame visual effect not imlemented

/* ScriptData
SDName: trial_of_the_crusader
SD%Complete: 80%
SDComment: by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"
enum Equipment
{
    EQUIP_MAIN           = 47266,
    EQUIP_OFFHAND        = 46996,
    EQUIP_RANGED         = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_LEGION_FLAME     = 34784,
    NPC_INFERNAL_VOLCANO = 34813,
    NPC_FEL_INFERNAL     = 34815,
    NPC_NETHER_PORTAL    = 34825,
    NPC_MISTRESS         = 34826,
};

enum BossSpells
{
SPELL_NETHER_POWER      = 67108,
SPELL_INFERNAL          = 66258,
SPELL_INFERNAL_ERUPTION = 66255,
SPELL_FEL_FIREBALL      = 66532,
SPELL_FEL_LIGHTING      = 66528,
SPELL_INCINERATE_FLESH  = 66237,
SPELL_BURNING_INFERNO   = 66242,
SPELL_NETHER_PORTAL     = 66264,
SPELL_LEGION_FLAME_0    = 66199,
SPELL_LEGION_FLAME_1    = 66197,
SPELL_SHIVAN_SLASH      = 67098,
SPELL_SPINNING_STRIKE   = 66316,
SPELL_FEL_INFERNO       = 67047,
SPELL_FEL_STREAK        = 66494,
SPELL_BERSERK           = 26662,
};

/*######
## boss_jaraxxus
######*/

struct MANGOS_DLL_DECL boss_jaraxxusAI : public BSWScriptedAI
{
    boss_jaraxxusAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;
    uint8 substage;
    uint8 m_portalsCount;
    uint8 m_volcanoCount;

    void Reset() {
        if(!m_pInstance) return;
        m_pInstance->SetData(TYPE_JARAXXUS, NOT_STARTED);
//        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);
        m_portalsCount = 1;
        if (currentDifficulty == RAID_DIFFICULTY_10MAN_HEROIC || currentDifficulty == RAID_DIFFICULTY_25MAN_HEROIC) 
        {
            m_portalsCount = 2;
            m_volcanoCount = 4;
        } else {
            m_portalsCount = 1;
            m_volcanoCount = 4;
        }
        DoScriptText(-1713517,m_creature);
        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_JARAXXUS, FAIL);
            m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1713525,m_creature);
            m_pInstance->SetData(TYPE_JARAXXUS, DONE);
            m_pInstance->SetData(TYPE_EVENT,2000);
            m_pInstance->SetData(TYPE_STAGE,0);
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance) return;
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_JARAXXUS, IN_PROGRESS);
        DoScriptText(-1713514,m_creature);
        doCast(SPELL_NETHER_POWER);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_FEL_FIREBALL, uiDiff);

        timedCast(SPELL_FEL_LIGHTING, uiDiff);

        if (timedQuery(SPELL_INCINERATE_FLESH, uiDiff)) {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                           {
                           DoScriptText(-1713522,m_creature,pTarget);
                           doCast(SPELL_INCINERATE_FLESH,pTarget);
                           }
                    }

        if (timedQuery(SPELL_LEGION_FLAME_1, uiDiff)) {
                    DoScriptText(-1713518,m_creature);
                    doCast(SPELL_LEGION_FLAME_1);
                    };

        if (timedQuery(SPELL_INFERNAL_ERUPTION, uiDiff)
                             && m_volcanoCount > 0) {
                DoScriptText(-1713520,m_creature);
                if (doCast(NPC_INFERNAL_VOLCANO) == CAST_OK) --m_volcanoCount;
                    };

        if (timedQuery(SPELL_NETHER_PORTAL, uiDiff)
                             && m_portalsCount > 0
                             &&  m_creature->GetHealthPercent() <= 90.0f)
                             {
                DoScriptText(-1713519,m_creature);
                if (doCast(NPC_NETHER_PORTAL) == CAST_OK) --m_portalsCount;
                };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jaraxxus(Creature* pCreature)
{
    return new boss_jaraxxusAI(pCreature);
}

struct MANGOS_DLL_DECL mob_legion_flameAI : public BSWScriptedAI
{
    mob_legion_flameAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiRangeCheck_Timer;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);

        if (Unit* pTarget= m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0) ) {
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->SetSpeedRate(MOVE_RUN, 0.5);
                }
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
        if (m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                    if (m_creature->IsWithinDist(m_creature->getVictim(), 4.0f, false))
                    {
                        DoCast(m_creature,SPELL_LEGION_FLAME_0);
                    }
            }
            m_uiRangeCheck_Timer = 1000;
            if (m_creature->getVictim()) {
                                  m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                                  m_creature->SetSpeedRate(MOVE_RUN, 0.5);
                                  }
        }
        else m_uiRangeCheck_Timer -= uiDiff;

    }
};

CreatureAI* GetAI_mob_legion_flame(Creature* pCreature)
{
    return new mob_legion_flameAI(pCreature);
}

struct MANGOS_DLL_DECL mob_infernal_volcanoAI : public BSWScriptedAI
{
    mob_infernal_volcanoAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 m_Count;
    uint32 m_Timer;

    void Reset()
    {
        m_Timer = 15000;
        m_creature->SetRespawnDelay(DAY);
        if (currentDifficulty != RAID_DIFFICULTY_10MAN_HEROIC && currentDifficulty != RAID_DIFFICULTY_25MAN_HEROIC) 
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_Count = 3;
        } else
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_Count = 6;
        }
    }

    void AttackStart(Unit *who)
    {
        return;
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

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (timedQuery(SPELL_INFERNAL_ERUPTION,diff) && m_Count > 0) {
            doCast(SPELL_INFERNAL_ERUPTION);
            DoScriptText(-1713524,m_creature);
            --m_Count;
            }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

    }
};

CreatureAI* GetAI_mob_infernal_volcano(Creature* pCreature)
{
    return new mob_infernal_volcanoAI(pCreature);
}

struct MANGOS_DLL_DECL mob_fel_infernalAI : public BSWScriptedAI
{
    mob_fel_infernalAI(Creature* pCreature) : BSWScriptedAI(pCreature)
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
        if (m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_FEL_INFERNO, uiDiff);

        timedCast(SPELL_FEL_STREAK, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_fel_infernal(Creature* pCreature)
{
    return new mob_fel_infernalAI(pCreature);
}

struct MANGOS_DLL_DECL mob_nether_portalAI : public BSWScriptedAI
{
    mob_nether_portalAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_Timer;
    uint8  m_Count;

    void Reset()
    {
        m_Timer = 10000;
        m_creature->SetRespawnDelay(DAY);
        if (currentDifficulty != RAID_DIFFICULTY_10MAN_HEROIC && currentDifficulty != RAID_DIFFICULTY_25MAN_HEROIC) 
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_Count = 1;
        } else
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_Count = 2;
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void AttackStart(Unit *who)
    {
        return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_Timer < diff && m_Count > 0) {
            DoCast(m_creature,SPELL_NETHER_PORTAL,false);
            DoScriptText(-1713521,m_creature);
            --m_Count;
            m_Timer = 60000;
            } else m_Timer -= diff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

    }
};

CreatureAI* GetAI_mob_nether_portal(Creature* pCreature)
{
    return new mob_nether_portalAI(pCreature);
}

struct MANGOS_DLL_DECL mob_mistress_of_painAI : public BSWScriptedAI
{
    mob_mistress_of_painAI(Creature* pCreature) : BSWScriptedAI(pCreature)
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
        DoScriptText(-1713523,m_creature, who);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) return;
        if (m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_SHIVAN_SLASH, uiDiff);

        timedCast(SPELL_SPINNING_STRIKE, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_mistress_of_pain(Creature* pCreature)
{
    return new mob_mistress_of_painAI(pCreature);
}

void AddSC_boss_jaraxxus()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_jaraxxus";
    newscript->GetAI = &GetAI_boss_jaraxxus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_legion_flame";
    newscript->GetAI = &GetAI_mob_legion_flame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_infernal_volcano";
    newscript->GetAI = &GetAI_mob_infernal_volcano;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_fel_infernal";
    newscript->GetAI = &GetAI_mob_fel_infernal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nether_portal";
    newscript->GetAI = &GetAI_mob_nether_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_mistress_of_pain";
    newscript->GetAI = &GetAI_mob_mistress_of_pain;
    newscript->RegisterSelf();
}
