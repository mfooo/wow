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
SDName: Boss_Ormorok
SD%Complete: 95%
SDComment: Playable Normal Heroic Support
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum Sounds
{
    SAY_AGGRO                               = -1576020,
    SAY_DEATH                               = -1576021,
    SAY_REFLECT                             = -1576022,
    SAY_CRYSTAL_SPIKES                      = -1576023,
    SAY_KILL                                = -1576024
};

enum
{
    SPELL_CRYSTAL_SPIKES_N                     = 47958, //Don't work, using walkaround
    SPELL_CRYSTAL_SPIKES_H                     = 57082, //Don't work, using walkaround

    SPELL_CRYSTALL_SPIKE_DAMAGE_N              = 47944,
    SPELL_CRYSTALL_SPIKE_DAMAGE_H              = 57067,
    SPELL_CRYSTAL_SPIKE_PREVISUAL              = 50442,

    SPELL_SPELL_REFLECTION                     = 35399, //47981,
    SPELL_TRAMPLE_N                            = 48016,
    SPELL_TRAMPLE_H                            = 57066,
    SPELL_FRENZY_H                             = 48017,
	SPELL_FRENZY_N                             = 57086,
    SPELL_SUMMON_CRYSTALLINE_TANGLER           = 61564, //summons npc 32665
    SPELL_ROOTS                                = 28858, //proper spell id is unknown
};

enum Creatures
{
    MOB_CRYSTAL_SPIKE                          = 27099,
    MOB_CRYSTALLINE_TANGLER                    = 32665,
};

#define SPIKE_DISTANCE                            5.0f

struct MANGOS_DLL_DECL boss_ormorokAI : public ScriptedAI
{
    boss_ormorokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bIsRegularMode;
    bool m_bIsFrenzy;
    bool m_bIsCrystalSpikes;
    
    float m_fBaseX;
    float m_fBaseY;
    float m_fBaseZ;
    float m_fBaseO;
    float m_fSpikeXY[4][2];

    uint32 m_uiSpellCrystalSpikesTimer;
    uint32 m_uiCrystalSpikesTimer;
    uint32 m_uiTrampleTimer;
    uint32 m_uiFrenzyTimer;
    uint32 m_uiReflectionTimer;
    uint32 m_uiSummonTanglerTimer;
    uint8  m_uiCrystalSpikesCount;

    void Reset() 
    {
        m_uiSpellCrystalSpikesTimer = 12000;                         
        m_uiTrampleTimer            = 10000;
        m_uiReflectionTimer         = 30000;
        m_uiSummonTanglerTimer      = 17000;
        m_bIsFrenzy                 = false;
        m_bIsCrystalSpikes          = false;

        if(m_pInstance)
            m_pInstance->SetData(DATA_ORMOROK_EVENT, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if(m_pInstance)
            m_pInstance->SetData(DATA_ORMOROK_EVENT, IN_PROGRESS);
    }

    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_DEATH, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(DATA_ORMOROK_EVENT, DONE);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void UpdateAI(const uint32 diff) 
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsCrystalSpikes)
            if (m_uiCrystalSpikesTimer < diff)
            {
                m_fSpikeXY[0][0] = m_fBaseX+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO));
                m_fSpikeXY[0][1] = m_fBaseY+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO));
                m_fSpikeXY[1][0] = m_fBaseX-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO));
                m_fSpikeXY[1][1] = m_fBaseY-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO));
                m_fSpikeXY[2][0] = m_fBaseX+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO-(M_PI/2)));
                m_fSpikeXY[2][1] = m_fBaseY+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO-(M_PI/2)));
                m_fSpikeXY[3][0] = m_fBaseX-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO-(M_PI/2)));
                m_fSpikeXY[3][1] = m_fBaseY-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO-(M_PI/2)));
                for (uint8 i = 0; i < 4; i++)
                    Creature* Spike = m_creature->SummonCreature(MOB_CRYSTAL_SPIKE, m_fSpikeXY[i][0], m_fSpikeXY[i][1], m_fBaseZ, 0, TEMPSUMMON_TIMED_DESPAWN, 7000);
                if (++m_uiCrystalSpikesCount >= 13)
                    m_bIsCrystalSpikes = false;
                m_uiCrystalSpikesTimer = 200;
            }else m_uiCrystalSpikesTimer -= diff;

        if (!m_bIsFrenzy && (m_creature->GetHealthPercent() < 25.0f))
        {
			DoCast(m_creature, m_bIsRegularMode ? SPELL_FRENZY_N : SPELL_FRENZY_H);
            m_bIsFrenzy = true;
        }

        if (m_uiTrampleTimer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TRAMPLE_N : SPELL_TRAMPLE_H);
            m_uiTrampleTimer = urand(10000, 35000);
        }else m_uiTrampleTimer -= diff;

        if (m_uiReflectionTimer < diff)
        {
            DoScriptText(SAY_REFLECT, m_creature);
            DoCast(m_creature, SPELL_SPELL_REFLECTION);
            m_uiReflectionTimer = 15000;
        }else m_uiReflectionTimer -= diff;

        if (m_uiSpellCrystalSpikesTimer < diff)
        {
            DoScriptText(SAY_CRYSTAL_SPIKES, m_creature);
            m_bIsCrystalSpikes = true;
            m_uiCrystalSpikesCount = 1;
            m_uiCrystalSpikesTimer = 0;
            m_fBaseX = m_creature->GetPositionX();
            m_fBaseY = m_creature->GetPositionY();
            m_fBaseZ = m_creature->GetPositionZ();
            m_fBaseO = m_creature->GetOrientation();
            m_uiSpellCrystalSpikesTimer = 20000;
        }else m_uiSpellCrystalSpikesTimer -=diff;

        if (!m_bIsRegularMode && (m_uiSummonTanglerTimer < diff))
        {
            Creature* CrystallineTangler = m_creature->SummonCreature(MOB_CRYSTALLINE_TANGLER, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            if (CrystallineTangler)
				if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    CrystallineTangler->AI()->AttackStart(target);

            m_uiSummonTanglerTimer = 17000;
        }else m_uiSummonTanglerTimer -=diff;

        DoMeleeAttackIfReady();    
    }
};

struct MANGOS_DLL_DECL mob_crystal_spikeAI : public Scripted_NoMovementAI
{
    mob_crystal_spikeAI(Creature *pCreature) : Scripted_NoMovementAI(pCreature)
    {
        Reset();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;

    uint32 m_uiCrystallSpikeDamageTimer;
    uint32 m_uiCrystalSpikePreVisualTimer;

    void Reset()
    {
        m_uiCrystallSpikeDamageTimer   = 3700;
        m_uiCrystalSpikePreVisualTimer = 1000;
        m_creature->SetLevel(80);                                        //
        m_creature->setFaction(16);                                      //Walkaround to be independent from data in DB
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); //
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); //
    }

    void UpdateAI(const uint32 diff) 
    {
        if (m_uiCrystalSpikePreVisualTimer < diff)
        {
            DoCast(m_creature, SPELL_CRYSTAL_SPIKE_PREVISUAL);
            m_uiCrystalSpikePreVisualTimer = 10000;
        }else m_uiCrystalSpikePreVisualTimer -=diff;

        if (m_uiCrystallSpikeDamageTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_CRYSTALL_SPIKE_DAMAGE_N : SPELL_CRYSTALL_SPIKE_DAMAGE_H);
            m_uiCrystallSpikeDamageTimer = 10000;
        }else m_uiCrystallSpikeDamageTimer -=diff;
    } 
}; 

struct MANGOS_DLL_DECL mob_crystalline_tanglerAI : public ScriptedAI
{
    mob_crystalline_tanglerAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 SPELL_ROOTS_Timer;

    void Reset() {SPELL_ROOTS_Timer = 1000;}

    void UpdateAI(const uint32 diff) 
    {
        if (SPELL_ROOTS_Timer < diff)
        {
			if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(target, SPELL_ROOTS, true);
            SPELL_ROOTS_Timer = 15000;
        }else SPELL_ROOTS_Timer -=diff;
		DoMeleeAttackIfReady();   
    } 
}; 

CreatureAI* GetAI_mob_crystal_spike(Creature *_Creature)
{
    return new mob_crystal_spikeAI (_Creature);
}

CreatureAI* GetAI_mob_crystalline_tangler(Creature *_Creature)
{
    return new mob_crystalline_tanglerAI (_Creature);
}

CreatureAI* GetAI_boss_ormorok(Creature *_Creature)
{
    return new boss_ormorokAI (_Creature);
}

void AddSC_boss_ormorok()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_ormorok";
    newscript->GetAI = GetAI_boss_ormorok;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_crystal_spike";
    newscript->GetAI = &GetAI_mob_crystal_spike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_crystalline_tangler";
    newscript->GetAI = &GetAI_mob_crystalline_tangler;
    newscript->RegisterSelf();
}
