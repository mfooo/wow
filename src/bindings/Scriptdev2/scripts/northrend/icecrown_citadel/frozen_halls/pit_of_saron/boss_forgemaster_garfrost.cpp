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
SDName: boss_forgemaster_gafrost
SD%Complete: 60%
SDComment: by Tacx
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"
enum 
{
        //common
    SPELL_BERSERK                           = 47008,
        //yells
    SAY_AGGRO                               = -1658001,
    SAY_SLAY_1                              = -1658002,
    SAY_SLAY_2                              = -1658003,
    SAY_DEATH                               = -1658004,
    SAY_PHASE2                              = -1658005,
    SAY_PHASE3                              = -1658006,
    SAY_DEEPFREZE                           = -1658006,
    SAY_TYRANNUS_DEATH                      = -1659007,
        //summons
        //Abilities
    SPELL_PERMAFROST                            = 70326,
    SPELL_PERMAFROST_TRIGGER                    = 68786,
    SPELL_THROW_SARONITE                        = 68788,
    SPELL_THUNDERING_STOMP                      = 68771,
    SPELL_CHILLING_WAVE                         = 68778,
    SPELL_CHILLING_WAVE_H                       = 70333,
    SPELL_DEEP_FREEZE                           = 70381,
    SPELL_DEEP_FREEZE_H                         = 72930,
    SPELL_FORGE_MACE                            = 68785,
    SPELL_FORGE_MACE_H                          = 70335,
    SPELL_FORGE_BLADE                           = 68774,
    SPELL_FORGE_BLADE_H                         = 70334,
    EQUIP_ID_SWORD                              = 49345,
    EQUIP_ID_MACE                               = 49344,
    ACHIEV_DOESNT_GO_TO_ELEVEN                  = 4524

};

/*
enum saysSD2
{
    SAY_AGGRO                           = -1658014,
    SAY_SLAY_1                          = -1658015,
    SAY_BOULDER_HIT                     = -1658016,
    SAY_DEATH                           = -1658017,
    SAY_FORGE_1                         = -1658018,
    SAY_FORGE_2                         = -1658019,
    SAY_TYRANNUS_GARFROST               = -1658020,
    SAY_GENERAL_GARFROST                = -1658021,

    EMOTE_THROW_SARONITE                = -1658022,
    EMOTE_DEEP_FREEZE                   = -1658023,
};
*/

struct MANGOS_DLL_DECL boss_forgemaster_garfrostAI : public ScriptedAI
{
    boss_forgemaster_garfrostAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *m_pInstance;


    bool m_bIsRegularMode;
    bool m_bIsPhase2;
    bool m_bIsPhase3;
    bool m_bIsAchievement;

    uint32 m_uiThrowSaronite_Timer;
    uint32 m_uiChillingWave_Timer;
    uint32 m_uiDeepFreeze_Timer;
    uint32 m_uiBladeReturn_Timer;
    uint32 m_uiMaceReturn_Timer;

    void Reset()
    {
        m_bIsPhase2 = false;
        m_bIsPhase3 = false;
        m_bIsAchievement = true;
        m_uiThrowSaronite_Timer = 20000;
        m_uiChillingWave_Timer = 9990000;
        m_uiDeepFreeze_Timer = 9990000;
        m_uiBladeReturn_Timer = 4500;
        m_uiMaceReturn_Timer = 6000;

        if(!m_pInstance) return;
            m_pInstance->SetData(TYPE_GARFROST, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        DoCast(m_creature, SPELL_PERMAFROST);
        if(!m_pInstance) return;
            m_pInstance->SetData(TYPE_GARFROST, IN_PROGRESS);
    }

    void KilledUnit(Unit* victim)
    {
        switch (urand(0,1))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
        }
    }

    void JustDied(Unit* pkiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    if(!m_pInstance) return;
            m_pInstance->SetData(TYPE_GARFROST, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 66) && !m_bIsPhase2)
        {
            m_bIsPhase2 = true;
            DoScriptText(SAY_PHASE2, m_creature);
            DoCast(m_creature, SPELL_THUNDERING_STOMP);	
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MovePoint(0, 654.021f, -201.438f, 526.699f); 
        }

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 33) && !m_bIsPhase3)
        {
            m_bIsPhase3 = true;
            DoScriptText(SAY_PHASE3, m_creature);
            DoCast(m_creature, SPELL_THUNDERING_STOMP);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MovePoint(0, 718.009f, -229.447f, 526.847f);
        }

        if (m_bIsPhase2)
        {
            if (m_uiBladeReturn_Timer < diff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_FORGE_BLADE : SPELL_FORGE_BLADE_H);
                SetEquipmentSlots(false, EQUIP_ID_SWORD, -1, -1);
                m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiBladeReturn_Timer = 9900000;
            }
            else
                m_uiBladeReturn_Timer -= diff;

            m_uiChillingWave_Timer = 10000;
        }

        if (m_bIsPhase3)
        {
            if (m_uiMaceReturn_Timer < diff)
            {
                m_creature->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_FORGE_BLADE : SPELL_FORGE_BLADE_H);
                DoCast(m_creature, m_bIsRegularMode ? SPELL_FORGE_MACE : SPELL_FORGE_MACE_H);
                SetEquipmentSlots(false, EQUIP_ID_MACE, -1, -1);
                m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiMaceReturn_Timer = 9900000;
            }
            else
                m_uiMaceReturn_Timer -= diff;
            m_uiChillingWave_Timer = 999000;
            m_uiDeepFreeze_Timer = 10000;
        }

        if (m_uiThrowSaronite_Timer < diff)
        {
            if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(Target, SPELL_THROW_SARONITE);
            m_uiThrowSaronite_Timer = (m_bIsRegularMode ? 20000 : 25000);
        }
        else 
            m_uiThrowSaronite_Timer -= diff;

        if (m_uiChillingWave_Timer < diff)
        {
            DoCast(m_creature, SPELL_CHILLING_WAVE);
            m_uiChillingWave_Timer = (m_bIsRegularMode ? 40000 : 30000);
        }
        else 
            m_uiChillingWave_Timer -= diff;

        if (m_uiDeepFreeze_Timer < diff)
        {
            if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(Target, m_bIsRegularMode ? SPELL_DEEP_FREEZE : SPELL_DEEP_FREEZE_H);
            m_uiDeepFreeze_Timer = (m_bIsRegularMode ? 27500 : 25000);
        }
        else 
            m_uiDeepFreeze_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_forgemaster_garfrost(Creature* pCreature)
{
    return new boss_forgemaster_garfrostAI(pCreature);
}


void AddSC_boss_garfrost()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_forgemaster_garfrost";
    newscript->GetAI = &GetAI_boss_forgemaster_garfrost;
    newscript->RegisterSelf();
}
