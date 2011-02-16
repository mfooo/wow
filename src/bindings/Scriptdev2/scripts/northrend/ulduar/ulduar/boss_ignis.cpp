/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_ignis
SD%Complete:
SDComment: slag pot damage missing
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

enum
{
    //yells
    SAY_AGGRO       = -1603010,
    SAY_SCORCH1     = -1603011,
    SAY_SCORCH2     = -1603012,
    SAY_SLAGPOT     = -1603013,
    EMOTE_FLAMEJETS = -1603014,
    SAY_SUMMON      = -1603015,
    SAY_SLAY1       = -1603016,
    SAY_SLAY2       = -1603017,
    SAY_BERSERK     = -1603018,
    SAY_DEATH       = -1603019,

    //ignis the furnace master
    SPELL_FLAME_JETS			= 62680,
    SPELL_FLAME_JETS_H			= 63472,
    SPELL_SLAG_POT				= 62717,
    SPELL_SLAG_POT_H			= 63477,
    SPELL_SLAG_POT_DMG			= 65722,
    SPELL_SLAG_POT_DMG_H		= 65723,
    SPELL_SCORCH				= 62546,
    SPELL_SCORCH_H				= 63474,
    BUFF_STRENGHT_OF_CREATOR	= 64473,
    SPELL_STRENGHT_OF_CREATOR2	= 64474,
    SPELL_STRENGHT_OF_CREATOR3	= 64475,
    SPELL_HASTE					= 66045,
    SPELL_ENRAGE                = 26662,
    //iron construct
    SPELL_HEAT					= 65667,
    SPELL_MOLTEN				= 62373,
    SPELL_BRITTLE				= 62382,
    SPELL_SHATTER				= 62383,
    //scorch target
    AURA_SCORCH					= 62548,
    AURA_SCORCH_H				= 63476,
    AURA_HEAT                   = 65667,
    SPELL_FREEZE_ANIM           = 16245,
    //NPC ids
    MOB_IRON_CONSTRUCT			= 33121,
    MOB_SCORCH_TARGET			= 33221,

    ACHIEV_STOKIN_THE_FURNACE   = 2930,
    ACHIEV_STOKIN_THE_FURNACE_H = 2929,
    ACHIEV_SHATTERED            = 2925,
    ACHIEV_SHATTERED_H          = 2926,
};

#define HOME_X                      586.747009f
#define HOME_Y                      381.997986f

// scorch target
struct MANGOS_DLL_DECL mob_scorch_targetAI : public ScriptedAI
{
    mob_scorch_targetAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiDeath_Timer;

    void Reset()
    {
        m_uiDeath_Timer = 55000;
        m_creature->SetDisplayId(11686);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature,  m_bIsRegularMode ? AURA_SCORCH : AURA_SCORCH_H);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if (m_uiDeath_Timer < diff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDeath_Timer -= diff;
    }
};

CreatureAI* GetAI_mob_scorch_target(Creature* pCreature)
{
    return new mob_scorch_targetAI(pCreature);
}

// iron construct
struct MANGOS_DLL_DECL mob_iron_constructAI : public ScriptedAI
{
    mob_iron_constructAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiDeath_Timer;
    uint32 m_uiAura_Check_Timer;
    uint32 m_uiScorchTimer;
    uint32 m_uiMoltenTimer;
    uint32 m_uiBrittleTimer;
    bool m_bIsBrittle;
    bool m_bIsShatter;
    bool m_bIsMolten;
    bool m_bIsInCombat;

    uint32 m_uiWaterCheckTimer;

    void Reset()
    {
        m_bIsShatter        = false;
        m_bIsBrittle        = false;
        m_bIsMolten         = false;
        m_bIsInCombat       = false;
        m_uiWaterCheckTimer = 1000;
        m_uiScorchTimer     = 5000;
        m_uiAura_Check_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoCast(m_creature, SPELL_FREEZE_ANIM);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

    // remove 1 stack of the buff from Ignis, hacky way, should be done by spell
        if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_IGNIS)))
        {
            if (pTemp->isAlive())
            {
                if (pTemp->HasAura(BUFF_STRENGHT_OF_CREATOR))
                {
                     if(SpellAuraHolder* strenght = pTemp->GetSpellAuraHolder(BUFF_STRENGHT_OF_CREATOR))
                     {
                         if(strenght->ModStackAmount(-1))
                              pTemp->RemoveAurasDueToSpell(BUFF_STRENGHT_OF_CREATOR);
                     }
                }
            }
        }
    }

    // shatter if is brittle
    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if (m_bIsBrittle)
        {
            if (uiDamage > 5000)
            {
                DoCast(m_creature, SPELL_SHATTER);
                m_bIsShatter = true;
                m_bIsBrittle = false;
                m_uiDeath_Timer = 500;
            }
        }
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bIsInCombat)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    // set in combat
    void GetInCombat()
    { 
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if (m_creature->HasAura(SPELL_FREEZE_ANIM, EFFECT_INDEX_0))
            m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);
        m_bIsInCombat = true;

        if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_IGNIS)))
        {
            if (pTemp->isAlive())
            {
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MovePoint(0, pTemp->GetPositionX(), pTemp->GetPositionY(), pTemp->GetPositionZ());

                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->AddThreat(pTarget,100.0f);
                    m_creature->AI()->AttackStart(pTarget);
                    m_creature->SetInCombatWithZone();
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // death after casted shatter
        if (m_uiDeath_Timer < uiDiff && m_bIsShatter)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDeath_Timer -= uiDiff;   

        // check for aura
        if (m_uiAura_Check_Timer < uiDiff && !m_bIsMolten)
        {
            if(Aura* aura = m_creature->GetAura(SPELL_HEAT,EFFECT_INDEX_0))
            {
                if(aura->GetStackAmount() > 9)
                {
                    DoCast(m_creature, SPELL_MOLTEN);
                    m_creature->RemoveAurasDueToSpell(SPELL_HEAT);
                    m_uiMoltenTimer = 30000;
                    m_bIsMolten = true;
                }
            }
            m_uiAura_Check_Timer = 1000;
        }else m_uiAura_Check_Timer -= uiDiff;

        //Water checks
        if(m_bIsMolten)
        {
        // should work with Vmaps3
            if (m_uiWaterCheckTimer <= uiDiff)
            {
                if(m_creature->IsInWater())
                {
                    DoCast(m_creature, SPELL_BRITTLE);
                    m_bIsBrittle = true;
                    m_bIsMolten = false;
                }
                // workaround
                /* else use workaround
                if( m_creature->GetDistance2d(524.15f, 277.0f) < 18 || m_creature->GetDistance2d(648.5f, 277.0f) < 18)
                {
                    DoCast(m_creature, SPELL_BRITTLE);
                    m_creature->RemoveAurasDueToSpell(SPELL_MOLTEN);
                    m_bIsBrittle = true;
                    m_bIsMolten = false;
                }*/
                m_uiWaterCheckTimer = 500;
            }else m_uiWaterCheckTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_iron_construct(Creature* pCreature)
{
    return new mob_iron_constructAI(pCreature);
}

//ignis the furnace master
struct MANGOS_DLL_DECL boss_ignisAI : public ScriptedAI
{
    boss_ignisAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> m_lIronConstructGUIDList;

    uint32 m_uiFlame_Jets_Timer;
    uint32 m_uiSlag_Pot_Timer;
    uint32 m_uiSlag_Pot_Dmg_Timer;
    uint32 m_uiScorch_Timer;
    uint32 m_uiSummon_Timer;
    uint32 m_uiPotDmgCount;
    uint32 m_uiEnrageTimer;

    uint64 m_uiPotTargetGUID;
    std::list<Creature*> lConstructs;

    uint32 m_uiEncounterTimer;
    bool m_bHasSlagPotCasted;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

        m_uiFlame_Jets_Timer    = 20000;
        m_uiSlag_Pot_Timer      = 25000;
        m_uiSlag_Pot_Dmg_Timer  = 26000;
        m_uiScorch_Timer        = 13000;
        m_uiSummon_Timer        = 10000;
        m_uiEnrageTimer         = 600000;   // 10 MIN
        m_uiPotDmgCount         = 0;
        m_uiPotTargetGUID       = 0;
        m_lIronConstructGUIDList.clear();

        m_uiEncounterTimer      = 0;
        m_bHasSlagPotCasted     = false;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, DONE);

        DoScriptText(SAY_DEATH, m_creature);

        if (m_uiEncounterTimer < 240000)
        {
        // hacky way to complete achievements; use only if you have this function
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_STOKIN_THE_FURNACE : ACHIEV_STOKIN_THE_FURNACE_H);
        }
    }

    Creature* SelectRandomConstruct(float fRange)
    {
        std::list<Creature* > lConstructList;
        GetCreatureListWithEntryInGrid(lConstructList, m_creature, MOB_IRON_CONSTRUCT, fRange);

        if (lConstructList.empty()){
            m_uiSummon_Timer = 5000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lConstructList.begin();
        advance(iter, urand(0, lConstructList.size()-1));

        if((*iter)->isAlive())
            return *iter;
        else
        {
            m_uiSummon_Timer = 500;
            return NULL;
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, FAIL);

        // respawn constructs
        GetCreatureListWithEntryInGrid(lConstructs, m_creature, MOB_IRON_CONSTRUCT, DEFAULT_VISIBILITY_INSTANCE);
        if (!lConstructs.empty())
        {
            for(std::list<Creature*>::iterator iter = lConstructs.begin(); iter != lConstructs.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        m_uiEncounterTimer += uiDiff;

        // enrage
        if(m_uiEnrageTimer < uiDiff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_ENRAGE);
            m_uiEnrageTimer = 30000;
        }
        else m_uiEnrageTimer -= uiDiff;

        if (m_uiFlame_Jets_Timer < uiDiff)
        {
            DoScriptText(EMOTE_FLAMEJETS, m_creature);
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_JETS : SPELL_FLAME_JETS_H);
            m_uiFlame_Jets_Timer = 35000;
        }else m_uiFlame_Jets_Timer -= uiDiff;   

        // need vehicle support!!!
        if (m_uiSlag_Pot_Timer < uiDiff)
        {
            DoScriptText(SAY_SLAGPOT, m_creature);
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                DoCast(target, m_bIsRegularMode ? SPELL_SLAG_POT : SPELL_SLAG_POT_H);
                m_uiPotTargetGUID = target->GetGUID();
            }
            m_uiSlag_Pot_Timer      = 30000;
            m_uiSlag_Pot_Dmg_Timer  = 1000;
            m_bHasSlagPotCasted     = true;
            m_uiPotDmgCount         = 0;
        }else m_uiSlag_Pot_Timer -= uiDiff;  

        // hacky way of doing damage
        if (m_uiSlag_Pot_Dmg_Timer < uiDiff && m_bHasSlagPotCasted)
        {
            if (Unit* pPotTarget = m_creature->GetMap()->GetUnit( m_uiPotTargetGUID))
            {
                if (m_uiPotDmgCount < 10)
                    DoCast(pPotTarget, m_bIsRegularMode ? SPELL_SLAG_POT_DMG : SPELL_SLAG_POT_DMG_H);
                else if (m_uiPotDmgCount == 10)
                {
                    if(pPotTarget->isAlive())
                        pPotTarget->CastSpell(pPotTarget, SPELL_HASTE, false);
                    m_bHasSlagPotCasted = false;
                }
            }
            ++m_uiPotDmgCount;
            m_uiSlag_Pot_Dmg_Timer = 1000;
        }else m_uiSlag_Pot_Dmg_Timer -= uiDiff;

        // call the golems
        if (m_uiSummon_Timer < uiDiff)
        {
            DoScriptText(SAY_SUMMON, m_creature);

            if(Creature* pConstruct = SelectRandomConstruct(200.0f))
            {
                ((mob_iron_constructAI*)pConstruct->AI())->GetInCombat();
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pConstruct->AddThreat(target, 100.0f);
            }

            m_uiSummon_Timer = 40000;

            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, BUFF_STRENGHT_OF_CREATOR);
        }else m_uiSummon_Timer -= uiDiff;

        if (m_uiScorch_Timer < uiDiff)
        {
            if(irand(0,1))
                DoScriptText(SAY_SCORCH1, m_creature);
            else
                DoScriptText(SAY_SCORCH2, m_creature);

            DoCast(m_creature, m_bIsRegularMode ? SPELL_SCORCH : SPELL_SCORCH_H);
            if (Creature* pTemp = m_creature->SummonCreature(MOB_SCORCH_TARGET, m_creature->getVictim()->GetPositionX(), m_creature->getVictim()->GetPositionY(), m_creature->getVictim()->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
            {
                pTemp->AddThreat(m_creature->getVictim(),0.0f);
                pTemp->AI()->AttackStart(m_creature->getVictim());
            }
            m_uiScorch_Timer = 28000;
        }else m_uiScorch_Timer -= uiDiff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 200)
            EnterEvadeMode();
    }
};

CreatureAI* GetAI_boss_ignis(Creature* pCreature)
{
    return new boss_ignisAI(pCreature);
}

void AddSC_boss_ignis()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_ignis";
    NewScript->GetAI = GetAI_boss_ignis;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_scorch_target";
    NewScript->GetAI = &GetAI_mob_scorch_target;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_iron_construct";
    NewScript->GetAI = &GetAI_mob_iron_construct;
    NewScript->RegisterSelf();
}