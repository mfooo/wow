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
SDName: Boss_Tharonja
SD%Complete: 80%
SDComment: Timers
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

enum
{
    SAY_KING_DRED_TALON                = -1600020,
    SAY_CALL_FOR_RAPTOR                = -1600021,
    
    SPELL_BELLOWING_ROAR            = 22686,
    SPELL_FEARSOME_ROAR                = 48849,
    H_SPELL_FEARSOME_ROAR            = 59422,
    SPELL_GRIEVOUS_BITE                = 48920,
    SPELL_MANGLING_SLASH            = 48873,
    SPELL_PIERCING_SLASH            = 48878,
    SPELL_RAPTOR_CALL                = 59416,            //not yet implemented

};

const float PosSummon1[3] = {-528.8f, -690.58f, 30.25f};
/*######
## boss_dred
######*/

struct MANGOS_DLL_DECL boss_dredAI : public ScriptedAI
{
    boss_dredAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 FearsomeRoar_Timer;
    uint32 ManglingSlash_Timer;
    uint32 PiercingSlash_Timer;
    uint32 GrievousBite_Timer;
    uint32 BellowingRoar_Timer;
    uint32 Check_Timer;
    uint32 CallForRaptor_Timer;
    uint32 CallForRaptorSpawn_Timer;
    uint32 CallForRaptorSpawn_Check;
    
    void Reset()
    {
        FearsomeRoar_Timer = 15000;
        ManglingSlash_Timer = urand(5000, 10000);
        PiercingSlash_Timer = urand(10000, 15000);
        GrievousBite_Timer = urand (15000, 20000);
        BellowingRoar_Timer = 60000;
        Check_Timer = 15000;
        CallForRaptor_Timer = 25000;
        CallForRaptorSpawn_Check = 0;
    }

    void Aggro(Unit* pWho)
    {
       
    }

    void KilledUnit(Unit* pVictim)
    {
        
    }

    void JustDied(Unit* pKiller)
    {
        
    }
    
    void CallForRaptorSpawnCheck()
    {
        CallForRaptorSpawn_Timer = 1000;
        CallForRaptorSpawn_Check = 1;
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Fearsome Roar
        if (FearsomeRoar_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FEARSOME_ROAR : H_SPELL_FEARSOME_ROAR, true);
            FearsomeRoar_Timer = 15000;
        }else FearsomeRoar_Timer -= uiDiff;

        //Piercing Slash
        if (PiercingSlash_Timer < uiDiff)
        {    
            DoCast(m_creature->getVictim(), SPELL_PIERCING_SLASH, true);
            PiercingSlash_Timer = urand(20000, 25000);
        }else PiercingSlash_Timer -= uiDiff;

        //Mangling Slash
        if (ManglingSlash_Timer < uiDiff)
        {    
            DoCast(m_creature->getVictim(), SPELL_MANGLING_SLASH, true);
            ManglingSlash_Timer = urand(20000, 25000);
        }else ManglingSlash_Timer -= uiDiff;

        //Mangling Slash
        if (GrievousBite_Timer < uiDiff)
        {    
            DoCast(m_creature->getVictim(), SPELL_GRIEVOUS_BITE, true);
            GrievousBite_Timer = urand(20000, 25000);
        }else GrievousBite_Timer -= uiDiff;

        //Grievous Bite remove
        if (Check_Timer < uiDiff)
        {
            Unit* pPlayer = m_creature->getVictim();
            if (pPlayer->GetHealth() == pPlayer->GetMaxHealth())
                if (pPlayer->HasAura(SPELL_GRIEVOUS_BITE))
                        pPlayer->RemoveAura(SPELL_GRIEVOUS_BITE, EFFECT_INDEX_0);
            Check_Timer = 1000;
        }else Check_Timer -= uiDiff;
        
        //Bellowing Roar
        if (BellowingRoar_Timer < uiDiff)
        {    
            DoCast(m_creature, SPELL_BELLOWING_ROAR);
            BellowingRoar_Timer = 60000;
        }else BellowingRoar_Timer -= uiDiff;

        //Call For Raptor - spell
        if (CallForRaptor_Timer < uiDiff)
        {    
            DoScriptText(SAY_CALL_FOR_RAPTOR, m_creature);
            m_creature->CastSpell(m_creature, SAY_CALL_FOR_RAPTOR, true);
            CallForRaptor_Timer = 25000;
            CallForRaptorSpawnCheck();
        }else CallForRaptor_Timer -= uiDiff;

        //Call For Raptor - spawn
        if (CallForRaptorSpawn_Timer < uiDiff && CallForRaptorSpawn_Check == 1)
        {    
            switch(urand(0, 1))
            {
                case 0:
                {
                    if (Creature* pRaptor1 = m_creature->SummonCreature(NPC_DRAKKARI_GUTRIPPER, PosSummon1[0], PosSummon1[1], PosSummon1[2], 0 , TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000))
                        pRaptor1->AI()->AttackStart(m_creature->getVictim());
                }
                case 1:
                {
                    if (Creature* pRaptor2 = m_creature->SummonCreature(NPC_DRAKKARI_SCYTHECLAW, PosSummon1[0], PosSummon1[1], PosSummon1[2], 0 , TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000))
                        pRaptor2->AI()->AttackStart(m_creature->getVictim());
                }
            }
            CallForRaptorSpawn_Check = 0;
        }else CallForRaptorSpawn_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dred(Creature* pCreature)
{
    return new boss_dredAI(pCreature);
}

void AddSC_boss_dred()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_dred";
    newscript->GetAI = &GetAI_boss_dred;
    newscript->RegisterSelf();
}