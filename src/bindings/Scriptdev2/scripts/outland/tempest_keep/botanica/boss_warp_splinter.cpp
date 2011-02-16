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
SDName: Boss_Warp_Splinter
SD%Complete: 80
SDComment: Includes Sapling (need some better control with these). Spells for boss possibly need some rework.
SDCategory: Tempest Keep, The Botanica
EndScriptData */

#include "precompiled.h"

/*#####
# mob_treant (Sapling)
#####*/

struct MANGOS_DLL_DECL mob_treantAI  : public ScriptedAI
{
    mob_treantAI (Creature* pCreature) : ScriptedAI(pCreature)
    {
        WarpGuid = 0;
        Reset();
    }

    uint64 WarpGuid;

    void Reset()
    {
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
    }

    void MoveInLineOfSight(Unit *who) { }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->getVictim()->GetGUID() != WarpGuid)
            DoMeleeAttackIfReady();
    }
};

/*#####
# boss_warp_splinter
#####*/

#define SAY_AGGRO           -1553007
#define SAY_SLAY_1          -1553008
#define SAY_SLAY_2          -1553009
#define SAY_SUMMON_1        -1553010
#define SAY_SUMMON_2        -1553011
#define SAY_DEATH           -1553012

#define WAR_STOMP           34716
#define SUMMON_TREANTS      34727                           // DBC: 34727, 34731, 34733, 34734, 34736, 34739, 34741 (with Ancestral Life spell 34742)   // won't work (guardian summon)
#define ARCANE_VOLLEY       36705                           //37078, 34785 //must additional script them (because Splinter eats them after 20 sec ^)
#define SPELL_HEAL_FATHER   6262

#define CREATURE_TREANT     19949
#define TREANT_SPAWN_DIST   50                              //50 yards from Warp Splinter's spawn point

float treant_pos[6][3] =
{
    {24.301233f, 427.221100f, -27.060635f},
    {16.795492f, 359.678802f, -27.355425f},
    {53.493484f, 345.381470f, -26.196192f},
    {61.867096f, 439.362732f, -25.921030f},
    {109.86187f, 423.201630f, -27.356019f},
    {106.78015f, 355.582580f, -27.593357f}
};

struct MANGOS_DLL_DECL boss_warp_splinterAI : public ScriptedAI
{
    boss_warp_splinterAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Treant_Spawn_Pos_X = pCreature->GetPositionX();
        Treant_Spawn_Pos_Y = pCreature->GetPositionY();
        Reset();
    }

    uint32 War_Stomp_Timer;
    uint32 Summon_Treants_Timer;
    uint32 Arcane_Volley_Timer;
    uint32 CheckTreantLOS_Timer;
    uint32 TreantLife_Timer;
    uint64 Treant_GUIDs[6];

    float Treant_Spawn_Pos_X;
    float Treant_Spawn_Pos_Y;

    void Reset()
    {
        War_Stomp_Timer = urand(25000, 40000);
        Summon_Treants_Timer = 45000;
        Arcane_Volley_Timer = urand(8000, 20000);
        CheckTreantLOS_Timer = 1000;
        TreantLife_Timer = 999999;

        for(int i = 0; i < 6; ++i)
            Treant_GUIDs[i] = 0;

        m_creature->SetSpeedRate(MOVE_RUN, 0.7f);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void SummonTreants()
    {
        for(int i = 0; i < 6; ++i)
        {
            float angle = (M_PI / 3) * i;

            float X = Treant_Spawn_Pos_X + TREANT_SPAWN_DIST * cos(angle);
            float Y = Treant_Spawn_Pos_Y + TREANT_SPAWN_DIST * sin(angle);
            //float Z = m_creature->GetMap()->GetHeight(X,Y, m_creature->GetPositionZ());
            //float Z = m_creature->GetPositionZ();
            float O = - m_creature->GetAngle(X,Y);

            if (Creature* pTreant = m_creature->SummonCreature(CREATURE_TREANT,treant_pos[i][0],treant_pos[i][1],treant_pos[i][2],O,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,40000))
            {
                //pTreant->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature));
                pTreant->AddThreat(m_creature);
                Treant_GUIDs[i] = pTreant->GetGUID();

                if (mob_treantAI* pTreantAI = dynamic_cast<mob_treantAI*>(pTreant->AI()))
                    pTreantAI->WarpGuid = m_creature->GetGUID();
            }
        }

        DoScriptText(urand(0, 1) ? SAY_SUMMON_1 : SAY_SUMMON_2, m_creature);
    }

    // Warp Splinter eat treants if they are near him
    void EatTreant()
    {
        for(int i=0; i<6; ++i)
        {
            Creature* pTreant = m_creature->GetMap()->GetCreature(Treant_GUIDs[i]);

            if (pTreant)
            {
                if (m_creature->IsWithinDistInMap(pTreant, 5))
                {
                    // 2) Heal Warp Splinter
                    int32 CurrentHP_Treant = (int32)pTreant->GetHealth();
                    m_creature->CastCustomSpell(m_creature,SPELL_HEAL_FATHER,&CurrentHP_Treant, 0, 0, true,0 ,0, m_creature->GetGUID());

                    // 3) Kill Treant
                    pTreant->DealDamage(pTreant, pTreant->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Check for War Stomp
        if (War_Stomp_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),WAR_STOMP);
            War_Stomp_Timer = urand(25000, 40000);
        } else War_Stomp_Timer -= diff;

        //Check for Arcane Volley
        if (Arcane_Volley_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),ARCANE_VOLLEY);
            Arcane_Volley_Timer = urand(20000, 35000);
        } else Arcane_Volley_Timer -= diff;

        //Check for Summon Treants
        if (Summon_Treants_Timer < diff)
        {
            SummonTreants();
            Summon_Treants_Timer = 45000;
        } else Summon_Treants_Timer -= diff;

        // I check if there is a Treant in Warp Splinter's LOS, so he can eat them
        if (CheckTreantLOS_Timer < diff)
        {
            EatTreant();
            CheckTreantLOS_Timer = 1000;
        } else CheckTreantLOS_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_warp_splinter(Creature* pCreature)
{
    return new boss_warp_splinterAI(pCreature);
}

CreatureAI* GetAI_mob_treant(Creature* pCreature)
{
    return new mob_treantAI(pCreature);
}

void AddSC_boss_warp_splinter()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_warp_splinter";
    newscript->GetAI = &GetAI_boss_warp_splinter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_warp_splinter_treant";
    newscript->GetAI = &GetAI_mob_treant;
    newscript->RegisterSelf();
}
