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
SDName: Boss_Grobbulus
SDAuthor: ckegg && FallenangelX
SD%Complete: 90
SDComment: main poison attacks need a little fixing // need to fix achievement 
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SPELL_BOMBARD_SLIME        = 28280,
    SPELL_POISON_CLOUD         = 28240,
    SPELL_MUTATING_INJECTION   = 28169,
    SPELL_SLIME_SPRAY          = 28157,
    H_SPELL_SLIME_SPRAY        = 54364,
    SPELL_BERSERK              = 26662,

    MOB_FALLOUT_SLIME          = 16290,
    MOB_GROBBULUS_POISON_CLOUD = 16363,

    SPELL_POISON_CLOUD_HACK    = 30914,
    SPELL_AOE_NATURE_DMG       = 30915,

	//achievements
    GROBBULUS_DEAD             = 1371,
    GROBBULUS_DEAD_H           = 1381
};

struct MANGOS_DLL_DECL boss_grobbulusAI : public ScriptedAI
{
    boss_grobbulusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 PoisonCloud_Timer;
    uint32 MutatingInjection_Timer;
    uint32 SlimeSpary_Timer;
    uint32 Enrage_Timer;
	uint32 uiEncounterTimer;

    void Reset()
    {
        PoisonCloud_Timer = 15000;
        MutatingInjection_Timer = 20000;
        SlimeSpary_Timer = 15000+rand()%10000;
		uiEncounterTimer        = 0;
        Enrage_Timer = (m_bIsRegularMode ? 720000 : 540000);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, FAIL);
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, DONE);


        if (uiEncounterTimer < 9999999)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? GROBBULUS_DEAD : GROBBULUS_DEAD_H);
        }
    }

    void Aggro(Unit *who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, IN_PROGRESS);
    }

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_SLIME_SPRAY || spell->Id == H_SPELL_SLIME_SPRAY)
        {
            if (Creature* pTemp = m_creature->SummonCreature(MOB_FALLOUT_SLIME, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    pTemp->AddThreat(pTarget);
                    pTemp->AI()->AttackStart(pTarget);
                }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
			
        // Achiev timer
        uiEncounterTimer += diff;

        if (PoisonCloud_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_POISON_CLOUD);
            DoSpawnCreature(MOB_GROBBULUS_POISON_CLOUD, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 120000);
            PoisonCloud_Timer = 15000;
        }else PoisonCloud_Timer -= diff;

        if (MutatingInjection_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, SPELL_MUTATING_INJECTION);

            MutatingInjection_Timer = 20000;
            float fPercentHealth = m_creature->GetHealth() / m_creature->GetMaxHealth() * 100;
            if (fPercentHealth < 75)
                MutatingInjection_Timer = 18000;
            if (fPercentHealth < 50)
                MutatingInjection_Timer = 16000;
            if (fPercentHealth < 25)
                MutatingInjection_Timer = 14000;
            if (fPercentHealth < 10)
                MutatingInjection_Timer = 10000;
        }
        else
            MutatingInjection_Timer -= diff;

        if (SlimeSpary_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),m_bIsRegularMode ? SPELL_SLIME_SPRAY : H_SPELL_SLIME_SPRAY);
            SlimeSpary_Timer = 15000+rand()%10000;
        }
        else
            SlimeSpary_Timer -= diff;

        if (Enrage_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BERSERK);
            Enrage_Timer = 300000;
        }
        else
            Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_grobbulus_poison_cloudAI : public ScriptedAI
{
    npc_grobbulus_poison_cloudAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    uint32 Dmg_Timer;
    uint32 Disappear_Timer;

    void Reset()
    {
        Dmg_Timer = 1000;
        Disappear_Timer = 65000;
        SetCombatMovement(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.85f);
        if (SpellEntry* TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_POISON_CLOUD_HACK))
        {
            TempSpell->EffectTriggerSpell[0] = 204;
            m_creature->CastCustomSpell(m_creature, TempSpell, NULL, NULL, NULL, true);
        }
    }

    void MoveInLineOfSight(Unit *pWho) { }

    void AttackStart(Unit *pWho) { }

    void UpdateAI(const uint32 diff)
    {
        if (Dmg_Timer < diff)
        {
            uint8 RadiusIndex;
            switch (uint32(Disappear_Timer / 4300))
            {
                case 0:
                case 1:  RadiusIndex = 43; break;
                case 2:  RadiusIndex = 18; break;
                case 3:  RadiusIndex = 61; break;
                case 4:  RadiusIndex = 17; break;
                case 5:  RadiusIndex = 32; break;
                case 6:  RadiusIndex = 42; break;
                case 7:  RadiusIndex = 13; break;
                case 8:  RadiusIndex = 40; break;
                case 9:  RadiusIndex = 14; break;
                case 10: RadiusIndex = 37; break;
                case 11: RadiusIndex = 29;  break;
                case 12: RadiusIndex = 8; break;
                case 13: RadiusIndex = 26; break;
                case 14:
                case 15: RadiusIndex = 15;  break;
            }
            if (SpellEntry* TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_AOE_NATURE_DMG))
            {
                TempSpell->EffectRadiusIndex[0] = RadiusIndex;
                TempSpell->EffectBasePoints[0] = (m_bIsRegularMode ? 2000 : 4250);
                m_creature->CastCustomSpell(m_creature, TempSpell, NULL, NULL, NULL, true);
            }
            Dmg_Timer = 1000;
        }else Dmg_Timer -= diff;

        if (Disappear_Timer < diff)
        {
            m_creature->ForcedDespawn();
        }else Disappear_Timer -= diff;

    }
};

struct MANGOS_DLL_DECL npc_fallout_slimeAI : public ScriptedAI
{
    npc_fallout_slimeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    uint32 Dmg_Timer;

    void Reset()
    {
        Dmg_Timer = 2000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (Dmg_Timer < diff)
        {
            if (SpellEntry* TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_AOE_NATURE_DMG))
            {
                TempSpell->EffectRadiusIndex[0] = 13;
                TempSpell->EffectBasePoints[0] = (m_bIsRegularMode ? 1000 : 2000);
                m_creature->CastCustomSpell(m_creature, TempSpell, NULL, NULL, NULL, true);
            }
            Dmg_Timer = 2000;
        }else Dmg_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_grobbulus(Creature* pCreature)
{
    return new boss_grobbulusAI(pCreature);
}

CreatureAI* GetAI_npc_grobbulus_poison_cloud(Creature* pCreature)
{
    return new npc_grobbulus_poison_cloudAI(pCreature);
}

CreatureAI* GetAI_npc_fallout_slime(Creature* pCreature)
{
    return new npc_fallout_slimeAI(pCreature);
}

void AddSC_boss_grobbulus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_grobbulus";
    newscript->GetAI = &GetAI_boss_grobbulus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_grobbulus_poison_cloud";
    newscript->GetAI = &GetAI_npc_grobbulus_poison_cloud;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_fallout_slime";
    newscript->GetAI = &GetAI_npc_fallout_slime;
    newscript->RegisterSelf();
}
