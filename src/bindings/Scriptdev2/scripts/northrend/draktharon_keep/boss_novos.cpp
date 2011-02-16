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
SDName: Boss_Novos
SD%Complete: 80%
SDComment: Timers
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

enum
{
    SAY_AGGRO                       = -1600005,
    SAY_DEATH                       = -1600006,
    SAY_KILL                        = -1600007,
    SAY_ADDS                        = -1600008,
    SAY_BUBBLE_1                    = -1600009,
    SAY_BUBBLE_2                    = -1600010,

    EMOTE_ASSISTANCE                = -1600011,

    POS                                = 3,

    SPELL_ARCANE_FIELD                 = 47346,
    SPELL_FROSTBOLT                    = 49037,
    H_SPELL_FROSTBOLT                  = 59855,
    SPELL_ARCANE_BLAST                 = 49198,
    H_SPELL_ARCANE_BLAST               = 59909,
    SPELL_BLIZZARD                     = 49034,
    H_SPELL_BLIZZARD                   = 59854,
    SPELL_WRATH_OF_MISERY              = 50089,
    H_SPELL_WRATH_OF_MISERY            = 59856,

    SPELL_RITUAL_CRYSTAL_KEY           = 51404,
    SPELL_EFFECT                       = 52106,
    SPELL_DEAD_EFFECT                  = 47336,

    SPELL_SHADOW_BOLT                  = 51363,
    H_SPELL_SHADOW_BOLT                = 59016,
};

const float PosSummonHandler[POS][3] =
{
    {-337.78f, -720.39f, 28.58f},
    {-379.31f, -818.36f, 59.70f},
    {-412.45f, -726.96f, 28.58f},
};
/*######
## boss_novos
######*/

struct MANGOS_DLL_DECL boss_novosAI : public ScriptedAI
{
    boss_novosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool Phase1;
    bool Phase2;
    uint32 Start_Check;
    uint32 Handler_Spawn;
    uint32 Handler_Count;
    uint32 Cast_Timer;
    uint32 ArcaneBlast_Timer;
    uint32 SpecialCast_Timer;
    uint32 SummonMinion_Timer;

    void Reset()
    {
        Start_Check = 1;
        Handler_Spawn = 5000;
        Handler_Count = 0;
        Phase1 = false;
        Phase2 = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // An Add reached the ground, if its z-pos is near the z pos of Novos
        if (pWho->GetEntry() == NPC_HULKING_CORPSE || pWho->GetEntry() == NPC_FETID_TROLL_CORPSE || pWho->GetEntry() == NPC_RISEN_SHADOWCASTER)
        {
            // Add reached ground, and the failure has not yet been reported
            if (pWho->GetPositionZ() < m_creature->GetPositionZ() + 1.5f && m_pInstance && m_pInstance->GetData(TYPE_NOVOS) == IN_PROGRESS)
                m_pInstance->SetData(TYPE_NOVOS, SPECIAL);
            return;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->SummonCreature(NPC_CRYSTAL_CHANNEL_TARGET, -379.269f, -737.728f, 39.313f, 0 , TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
        m_creature->CallForHelp(50.0f);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOVOS, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOVOS, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOVOS, FAIL);
    }

    void EnterPhase1()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
        Phase1 = true;
    }

    void EnterPhase2()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
        SummonMinion_Timer = urand (15000,20000);
        SpecialCast_Timer = urand(15000, 20000);
        ArcaneBlast_Timer = urand(25000, 30000);
        Cast_Timer = 500;
        Phase1 = false;
        Phase2 = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        if (Phase2 == true)
        {
               //Arcane Blast
            if (ArcaneBlast_Timer < uiDiff)
            {     
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                m_creature->CastSpell(m_creature,  m_bIsRegularMode ? SPELL_ARCANE_BLAST : H_SPELL_ARCANE_BLAST, true);
                ArcaneBlast_Timer = urand(25000, 30000);
            }else ArcaneBlast_Timer -= uiDiff;

            //Wrath Of Misery or Blizzard
            if (SpecialCast_Timer < uiDiff)
            {
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                switch(urand(0, 1))
                {
                    case 0: 
                        DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_WRATH_OF_MISERY : H_SPELL_WRATH_OF_MISERY);
                    case 1: 
                        DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_BLIZZARD : H_SPELL_BLIZZARD);
                }
                SpecialCast_Timer = urand(10000, 15000);
            }else ArcaneBlast_Timer -= uiDiff;
            
            //Regual cast - frostbolt
            if (Cast_Timer < uiDiff && ArcaneBlast_Timer > uiDiff && SpecialCast_Timer > uiDiff)
            {
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_FROSTBOLT : H_SPELL_FROSTBOLT);
                Cast_Timer = 1000;
            }else Cast_Timer -= uiDiff;
            
            //Summon Minions (Heroic) 
            if (SummonMinion_Timer < uiDiff)
            {    
                if(m_bIsRegularMode)
                    return;

                uint8 SummonLoc = rand()%POS;
                if (Creature* pAdd1 = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd1->AI()->AttackStart(m_creature->getVictim());
                if (Creature* pAdd2 = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd2->AI()->AttackStart(m_creature->getVictim());
                if (Creature* pAdd3 = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd3->AI()->AttackStart(m_creature->getVictim());
                if (Creature* pAdd4 = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd4->AI()->AttackStart(m_creature->getVictim());
                if (Creature* pAdd5 = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd5->AI()->AttackStart(m_creature->getVictim());
                SummonMinion_Timer = urand (15000,20000);
            }else SummonMinion_Timer -= uiDiff;

        }

        if (m_creature->getVictim() && Start_Check == 1)
        {
            EnterPhase1();
            DoCast(m_creature, SPELL_ARCANE_FIELD);
            Start_Check = 0;
        }
        
        //Phase 1 Waves spawn
        if (Handler_Spawn < uiDiff && Phase1 == true)
        {    
            Handler_Count ++;
            if(Handler_Count < 5)
            {
                uint8 SummonLoc = rand()%POS;
                m_creature->SummonCreature(NPC_CRYSTAL_HANDLER, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000);
                if (Creature* pAdd1 = m_creature->SummonCreature(NPC_HULKING_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd1->AI()->AttackStart(m_creature->getVictim());
                if (Creature* pAdd2 = m_creature->SummonCreature(NPC_RISEN_SHADOWCASTER, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd2->AI()->AttackStart(m_creature->getVictim());
                if (Creature* pAdd3 = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd3->AI()->AttackStart(m_creature->getVictim());
                if (Creature* pAdd4 = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, PosSummonHandler[SummonLoc][0],PosSummonHandler[SummonLoc][1],PosSummonHandler[SummonLoc][2],0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                    pAdd4->AI()->AttackStart(m_creature->getVictim());
                Handler_Spawn = 17500;
            }
            if(Handler_Count == 5)
            {            
                EnterPhase2();
                if (Creature* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL, 85.0f))
                    pTrigger->ForcedDespawn();
            }
        }else Handler_Spawn -= uiDiff;
    }
};



CreatureAI* GetAI_boss_novos(Creature* pCreature)
{
    return new boss_novosAI(pCreature);
}

struct MANGOS_DLL_DECL crystal_handlerAI : public ScriptedAI
{
    crystal_handlerAI(Creature* pCreature) : ScriptedAI(pCreature){Reset();}
    void Reset(){}
    void MoveInLineOfSight(Unit* who)
    {
        if (Unit* pNovos = GetClosestCreatureWithEntry(m_creature, NPC_NOVOS, 85.0f))
            m_creature->AI()->AttackStart(pNovos->getVictim());
    }

    void JustDied()
    {
        if (Creature* pDeadTrigger = m_creature->SummonCreature(NPC_TRIGGER_TARGET,0,0,0,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,10000))
        {
            if (Unit* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL, 85.0f))
            {
                pDeadTrigger->CastSpell(pTarget, SPELL_DEAD_EFFECT, true);
            }
        }
        if (Creature* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL, 85.0f))
            pTrigger->ForcedDespawn();
    }
};

CreatureAI* GetAI_crystal_handler(Creature* pCreature)
{
    return new crystal_handlerAI(pCreature);
}

struct MANGOS_DLL_DECL crystal_channelAI : public ScriptedAI
{
    crystal_channelAI(Creature* pCreature) : ScriptedAI(pCreature){Reset();}
    uint32 Check_Timer;
    void Reset()
    {
        Check_Timer = 1000;
    }
    void AttackStart(Unit *pWho){}
    void UpdateAI(const uint32 uiDiff)
    {
        if (Check_Timer < uiDiff)
        {
            Creature* pNovos =  GetClosestCreatureWithEntry(m_creature, NPC_NOVOS, 85.0f);
            if (Creature* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL_TARGET , 85.0f))
                if (pNovos && ((boss_novosAI*)pNovos->AI())->Phase1 == true)
                    DoCast(pTarget, SPELL_EFFECT, true);
                else
                    pTarget->ForcedDespawn();
            Check_Timer = 1000;
        }else Check_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_crystal_channel(Creature* pCreature)
{
    return new crystal_channelAI(pCreature);
}

struct MANGOS_DLL_DECL risen_shadowcasterAI : public ScriptedAI
{
    risen_shadowcasterAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 Check_Timer;
    void Reset()
    {
        Check_Timer = 1000;
    }
    
    void UpdateAI(const uint32 uiDiff)
    {
        if (Check_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BOLT : H_SPELL_SHADOW_BOLT);
            Check_Timer = 1000;
        }else Check_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_risen_shadowcaster(Creature* pCreature)
{
    return new risen_shadowcasterAI(pCreature);
}
void AddSC_boss_novos()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_novos";
    newscript->GetAI = &GetAI_boss_novos;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "crystal_handler";
    newscript->GetAI = &GetAI_crystal_handler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "crystal_channel";
    newscript->GetAI = &GetAI_crystal_channel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "risen_shadowcaster";
    newscript->GetAI = &GetAI_risen_shadowcaster;
    newscript->RegisterSelf();

}
