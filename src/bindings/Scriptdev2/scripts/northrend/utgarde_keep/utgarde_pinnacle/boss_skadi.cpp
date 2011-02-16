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
SDName: Boss_Skadi
SD%Complete: %
SDComment:
SDAuthor: /// dev FallenAngelX ///
SDCategory: Utgarde Pinnacle
TODO::
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    SAY_AGGRO                           = -1575019,
    SAY_DRAKEBREATH_1                   = -1575020,
    SAY_DRAKEBREATH_2                   = -1575021,
    SAY_DRAKEBREATH_3                   = -1575022,
    SAY_DRAKE_HARPOON_1                 = -1575023,
    SAY_DRAKE_HARPOON_2                 = -1575024,
    SAY_KILL_1                          = -1575025,
    SAY_KILL_2                          = -1575026,
    SAY_KILL_3                          = -1575027,
    SAY_DEATH                           = -1575028,
    SAY_DRAKE_DEATH                     = -1575029,
    EMOTE_HARPOON_RANGE                 = -1575030,

    SPELL_CRUSH                         = 50234,
    SPELL_CRUSH_H                       = 59330,

    SPELL_WHIRLWIND                     = 50228,
    SPELL_WHIRLWIND_H                   = 59322,

    SPELL_POISONED_SPEAR                = 50255,
    SPELL_POISONED_SPEAR_H              = 59331,
    SPELL_POISONED                      = 50258,
    SPELL_POISONED_H                    = 59334,

    // casted with base of creature 22515 (World Trigger), so we must make sure
    // to use the close one by the door leading further in to instance.
    SPELL_SUMMON_GAUNTLET_MOBS          = 48630,                // tick every 30 sec
    SPELL_SUMMON_GAUNTLET_MOBS_H        = 59275,                // tick every 25 sec

    SPELL_GAUNTLET_PERIODIC             = 47546,                // what is this? Unknown use/effect, but probably related

    SPELL_LAUNCH_HARPOON                = 48642,                // this spell hit drake to reduce HP (force triggered from 48641)
    ITEM_HARPOON                        = 37372,

    SPELL_SUMMON_HARPOONER_E            = 48633,
    SPELL_SUMMON_HARPOONER_W            = 48634,
    SPELL_SUMMON_WITCH_DOCTOR_E         = 48636,
    SPELL_SUMMON_WITCH_DOCTOR_W         = 48635

    // ToDo: Find spell summoning warrior
};

uint64 goHarpoons[3] = {GO_HARPOON1 ,GO_HARPOON2, GO_HARPOON3};

// has to be replaced by propper spells (summon ymirjar harpooners, 
uint32 m_uiSkadiAdds[3] = {26692, 26690, 26691};

/*######
## boss_skadi
######*/

struct MANGOS_DLL_DECL boss_skadiAI : public ScriptedAI
{
    boss_skadiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsLandPhase;

    //Land Phase
    uint32 m_uiCrushTimer;
    uint32 m_uiPoisonedSpearTimer;
    uint32 m_uiWirlwhindTimer;

    //Event Phase
    uint8  m_uiNextWaveCount;
    uint32 m_uiIsInHarpoonRangeTimer;
    uint32 m_uiNextWaveTimer;
    uint32 m_uiGraufBrathTimer;

    void Reset()
    {
        //Land Phase
        m_uiCrushTimer              = urand(5000, 10000);
        m_uiPoisonedSpearTimer      = urand(5000, 10000);
        m_uiWirlwhindTimer          = urand(5000, 10000);
        m_bIsLandPhase              = false;

        //Event Phase
        m_uiGraufBrathTimer         = 30000;
        m_uiNextWaveCount           = 0;
        m_uiIsInHarpoonRangeTimer   = urand(5000, 10000);
        m_uiNextWaveTimer           = urand(5000, 10000);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SKADI, NOT_STARTED);
            m_pInstance->SetData(TYPE_HARPOONLUNCHER, NOT_STARTED);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetVisibility(VISIBILITY_OFF);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SKADI, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
        {
            if (pSummoned->AI())
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void SendNextWeave()
    {
        uint8 waveType = urand(0, 1);
        for(uint8 i=0; i<(urand(5, 6)); ++i)
        {
            switch(waveType)
            {
                case 0: DoCast(m_creature, urand(0, 1) ? SPELL_SUMMON_HARPOONER_E: SPELL_SUMMON_HARPOONER_W, false);
                case 1: DoCast(m_creature, urand(0, 1) ? SPELL_SUMMON_WITCH_DOCTOR_E : SPELL_SUMMON_WITCH_DOCTOR_W, false);
            }
        }
       ++m_uiNextWaveCount;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(!m_bIsLandPhase)
        {
            if(m_uiNextWaveCount > 4)
            {
                if(m_pInstance && m_pInstance->GetData(TYPE_HARPOONLUNCHER) != (m_bIsRegularMode ? DONE : SPECIAL))
                {
                    if(m_uiIsInHarpoonRangeTimer < uiDiff)
                    {
                        DoScriptText(EMOTE_HARPOON_RANGE, m_creature);

                        //only 1 from 3 harpoons  is  aloowe to use at one time
                        if (GameObject* pGo = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(goHarpoons[urand(0,2)])))
                            pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);

                        m_uiIsInHarpoonRangeTimer = urand(20000, 30000);
                    }else m_uiIsInHarpoonRangeTimer -= uiDiff;
                }
                else
                {
                    DoScriptText(SAY_DRAKE_DEATH, m_creature);
                    m_bIsLandPhase = true;
                    m_creature->SetVisibility(VISIBILITY_ON);
                    if(m_pInstance)
                    {
                        m_pInstance->SetData(TYPE_SKADI, IN_PROGRESS);
                        m_pInstance->SetData(TYPE_HARPOONLUNCHER, 0);
                    }
                }
            }

            if(m_uiNextWaveTimer < uiDiff)
            {
                SendNextWeave();
                m_uiNextWaveTimer = urand(20000, 30000);
            }else m_uiNextWaveTimer -= uiDiff;

            if(m_uiGraufBrathTimer < uiDiff)
            {
                switch(urand(0, 2))
                {
                    case 0: DoScriptText(SAY_DRAKEBREATH_1, m_creature); break;
                    case 1: DoScriptText(SAY_DRAKEBREATH_2, m_creature); break;
                    case 2: DoScriptText(SAY_DRAKEBREATH_3, m_creature); break;
                }
                //breath ID missing
                if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                {
                    DoCastSpellIfCan(pPlayer, m_bIsRegularMode ? SPELL_POISONED_SPEAR : SPELL_POISONED_SPEAR_H);
                    pPlayer->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_POISONED : SPELL_POISONED_H, true);
                }
                //Spell brath id ?
                m_uiGraufBrathTimer = urand(10000, 20000);
            }
            else
                m_uiGraufBrathTimer -= uiDiff;

        }
        else
        {
            if(m_uiPoisonedSpearTimer < uiDiff)
            {
                if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                {
                    DoCastSpellIfCan(pPlayer, m_bIsRegularMode ? SPELL_POISONED_SPEAR : SPELL_POISONED_SPEAR_H);
                    pPlayer->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_POISONED : SPELL_POISONED_H, true);
                }
                m_uiPoisonedSpearTimer = urand(5000, 10000);
            }else m_uiPoisonedSpearTimer -= uiDiff;
            
            if(m_uiCrushTimer < uiDiff)
            {
                if(m_creature->getVictim())
                    DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CRUSH : SPELL_CRUSH_H);
                m_uiCrushTimer = urand(10000, 15000);
            }else m_uiCrushTimer -= uiDiff;

            if(m_uiWirlwhindTimer < uiDiff)
            {
                if(m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WHIRLWIND : SPELL_WHIRLWIND_H, false);
                m_uiWirlwhindTimer = urand(10000, 20000);
            }else m_uiWirlwhindTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_skadi(Creature* pCreature)
{
    return new boss_skadiAI(pCreature);
}

bool AreaTrigger_at_skadi(Player* pPlayer, const AreaTriggerEntry* pAt)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_SKADI) == NOT_STARTED)
            pInstance->SetData(TYPE_SKADI, SPECIAL);
    }

    return false;
}

bool GOUse_go_skaldi_harpoonluncher(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    if(pPlayer->HasItemCount(ITEM_HARPOON,1))
    {
        pInstance->SetData(TYPE_HARPOONLUNCHER, IN_PROGRESS);
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    }
    return false;
}

void AddSC_boss_skadi()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_skadi";
    newscript->GetAI = &GetAI_boss_skadi;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_skadi";
    newscript->pAreaTrigger = &AreaTrigger_at_skadi;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_skaldi_harpoonluncher";
    newscript->pGOUse = &GOUse_go_skaldi_harpoonluncher;
    newscript->RegisterSelf();
}
