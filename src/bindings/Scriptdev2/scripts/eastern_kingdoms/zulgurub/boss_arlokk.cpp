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
SDName: Boss_Arlokk
SD%Complete: 95
SDComment: Wrong cleave and red aura is missing.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

bool GOUse_go_gong_of_bethekk(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_ARLOKK) == DONE || pInstance->GetData(TYPE_ARLOKK) == IN_PROGRESS)
            return true;

        pInstance->SetData(TYPE_ARLOKK, IN_PROGRESS);
    }

    return false;
}

enum
{
    SAY_AGGRO                   = -1309011,
    SAY_FEAST_PANTHER           = -1309012,
    SAY_DEATH                   = -1309013,

    SPELL_SHADOWWORDPAIN        = 23952,
    SPELL_GOUGE                 = 24698,
    SPELL_MARK                  = 24210,
    SPELL_CLEAVE                = 26350,                    //Perhaps not right. Not a red aura...
    SPELL_PANTHER_TRANSFORM     = 24190,

    MODEL_ID_NORMAL             = 15218,
    MODEL_ID_PANTHER            = 15215,
    MODEL_ID_BLANK              = 11686,

    NPC_ZULIAN_PROWLER          = 15101
};

struct MANGOS_DLL_DECL boss_arlokkAI : public ScriptedAI
{
    boss_arlokkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowWordPain_Timer;
    uint32 m_uiGouge_Timer;
    uint32 m_uiMark_Timer;
    uint32 m_uiCleave_Timer;
    uint32 m_uiVanish_Timer;
    uint32 m_uiVisible_Timer;

    uint32 m_uiSummon_Timer;
    uint32 m_uiSummonCount;

    uint64 m_uiMarkedGUID;

    bool m_bIsPhaseTwo;
    bool m_bIsVanished;

    void Reset()
    {
        m_uiShadowWordPain_Timer = 8000;
        m_uiGouge_Timer = 14000;
        m_uiMark_Timer = 35000;
        m_uiCleave_Timer = 4000;
        m_uiVanish_Timer = 60000;
        m_uiVisible_Timer = 6000;

        m_uiSummon_Timer = 5000;
        m_uiSummonCount = 0;

        m_bIsPhaseTwo = false;
        m_bIsVanished = false;

        m_uiMarkedGUID = 0;

        m_creature->SetDisplayId(MODEL_ID_NORMAL);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARLOKK, NOT_STARTED);

        //we should be summoned, so despawn
        m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        m_creature->SetDisplayId(MODEL_ID_NORMAL);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARLOKK, DONE);
    }

    void DoSummonPhanters()
    {
        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiMarkedGUID))
        {
            if (pPlayer->isAlive())
                DoScriptText(SAY_FEAST_PANTHER, m_creature, pPlayer);
        }

        m_creature->SummonCreature(NPC_ZULIAN_PROWLER, -11532.7998f, -1649.6734f, 41.4800f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
        m_creature->SummonCreature(NPC_ZULIAN_PROWLER, -11532.9970f, -1606.4840f, 41.2979f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiMarkedGUID))
        {
            if (pPlayer->isAlive())
                pSummoned->AI()->AttackStart(pPlayer);
        }

        ++m_uiSummonCount;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsPhaseTwo)
        {
            if (m_uiShadowWordPain_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_SHADOWWORDPAIN);
                m_uiShadowWordPain_Timer = 15000;
            }
            else
                m_uiShadowWordPain_Timer -= uiDiff;

            if (m_uiMark_Timer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (Player* pMark = pTarget->GetCharmerOrOwnerPlayerOrPlayerItself())
                    {
                        DoCastSpellIfCan(pMark, SPELL_MARK);
                        m_uiMarkedGUID = pMark->GetGUID();
                    }
                    else
                    {
                        if (m_uiMarkedGUID)
                            m_uiMarkedGUID = 0;

                        error_log("SD2: boss_arlokk could not accuire a new target to mark.");
                    }
                }

                m_uiMark_Timer = 15000;
            }
            else
                m_uiMark_Timer -= uiDiff;
        }
        else
        {
            //Cleave_Timer
            if (m_uiCleave_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleave_Timer = 16000;
            }
            else
                m_uiCleave_Timer -= uiDiff;

            //Gouge_Timer
            if (m_uiGouge_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_GOUGE);

                if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-80);

                m_uiGouge_Timer = urand(17000, 27000);
            }
            else
                m_uiGouge_Timer -= uiDiff;
        }

        if (m_uiSummonCount <= 30)
        {
            if (m_uiSummon_Timer < uiDiff)
            {
                DoSummonPhanters();
                m_uiSummon_Timer = 5000;
            }
            else
                m_uiSummon_Timer -= uiDiff;
        }

        if (m_uiVanish_Timer < uiDiff)
        {
            //Invisble Model
            m_creature->SetDisplayId(MODEL_ID_BLANK);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            m_creature->AttackStop();
            DoResetThreat();

            m_bIsVanished = true;

            m_uiVanish_Timer = 45000;
            m_uiVisible_Timer = 6000;
        }
        else
            m_uiVanish_Timer -= uiDiff;

        if (m_bIsVanished)
        {
            if (m_uiVisible_Timer < uiDiff)
            {
                //The Panther Model
                m_creature->SetDisplayId(MODEL_ID_PANTHER);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 35)));
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 35)));
                m_creature->UpdateDamagePhysical(BASE_ATTACK);

                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    AttackStart(pTarget);

                m_bIsPhaseTwo = true;
                m_bIsVanished = false;
            }
            else
                m_uiVisible_Timer -= uiDiff;
        }
        else
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_arlokk(Creature* pCreature)
{
    return new boss_arlokkAI(pCreature);
}

void AddSC_boss_arlokk()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_gong_of_bethekk";
    newscript->pGOUse = &GOUse_go_gong_of_bethekk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_arlokk";
    newscript->GetAI = &GetAI_boss_arlokk;
    newscript->RegisterSelf();
}
