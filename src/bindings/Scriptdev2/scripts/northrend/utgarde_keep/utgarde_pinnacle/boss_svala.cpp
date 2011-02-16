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
SDName: Boss_Svala
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
    SAY_INTRO_1                 = -1575000,
    SAY_INTRO_2_ARTHAS          = -1575001,
    SAY_INTRO_3                 = -1575002,
    SAY_INTRO_4_ARTHAS          = -1575003,
    SAY_INTRO_5                 = -1575004,

    SAY_AGGRO                   = -1575005,
    SAY_SLAY_1                  = -1575006,
    SAY_SLAY_2                  = -1575007,
    SAY_SLAY_3                  = -1575008,
    SAY_SACRIFICE_1             = -1575009,
    SAY_SACRIFICE_2             = -1575010,
    SAY_SACRIFICE_3             = -1575011,
    SAY_SACRIFICE_4             = -1575012,
    SAY_SACRIFICE_5             = -1575013,
    SAY_DEATH                   = -1575014,

    NPC_SVALA_SORROW            = 26668,
    NPC_ARTHAS_IMAGE            = 29280,
    NPC_CHANNELER               = 27281,

    SPELL_ARTHAS_VISUAL         = 54134,

    // don't know how these should work in relation to each other
    SPELL_TRANSFORMING          = 54205,
    SPELL_TRANSFORMING_FLOATING = 54140,
    SPELL_TRANSFORMING_CHANNEL  = 54142,

    SPELL_RITUAL_OF_SWORD       = 48276,
    SPELL_CALL_FLAMES           = 48258,
    SPELL_BOLT                  = 39252,
    SPELL_SINISTER_STRIKE       = 15667,
    SPELL_SINISTER_STRIKE_H     = 59409,
    SPELL_KILL                  = 5,

    //channelers spells
    SPELL_PARALYZE              = 48278,
    SPELL_SHADOWS               = 59407,
};

float fCoord[4][4] =
{
    {296.498169f, -346.462433f, 90.547546f, 0.0f}, 
    {299.563782f, -343.736572f, 90.559288f, 3.93f}, 
    {293.811676f, -343.331238f, 90.529503f, 5.340091f}, 
    {296.490417f, -349.221039f, 90.5550446f, 1.578029f} 
};

/*######
## boss_svala
######*/

struct MANGOS_DLL_DECL boss_svalaAI : public ScriptedAI
{
    boss_svalaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bIsIntroDone = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    Creature* pArthas;

    bool m_bIsIntroDone;
    uint32 m_uiIntroTimer;
    uint32 m_uiIntroCount;

    bool   m_bIsSacrifice;
    uint32 m_uiSinisterStrikeTimer;
    uint32 m_uiCallFlamesTimer;
    uint32 m_uiSacrificeTimer;
    uint32 m_uiSacrificeEndTimer;

    uint64 m_uiPlayerGUID;
    uint64 m_uiAddsGUID[3];

    void Reset()
    {
        m_uiPlayerGUID;
        for(uint8 i=0; i<3; ++i)
            m_uiAddsGUID[i] = 0;

        m_bIsSacrifice = false;
        m_uiSinisterStrikeTimer = urand(10000,20000);
        m_uiCallFlamesTimer = urand(15000,25000);
        m_uiSacrificeTimer = 20000;
        m_uiSacrificeEndTimer = 120000;

        pArthas = NULL;

        m_uiIntroTimer = 2500;
        m_uiIntroCount = 0;

        if (m_creature->isAlive() && m_pInstance && m_pInstance->GetData(TYPE_SVALA) > IN_PROGRESS)
        {
            if (m_creature->GetEntry() != NPC_SVALA_SORROW)
                m_creature->UpdateEntry(NPC_SVALA_SORROW);

            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            m_bIsIntroDone = true;
        }
    }

    void JustReachedHome()
    {
        DoMoveToPosition();
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bIsIntroDone)
        {
            if (m_pInstance && m_pInstance->GetData(TYPE_SVALA) == IN_PROGRESS)
            {
                m_pInstance->SetData(TYPE_SVALA, SPECIAL);

                float fX, fY, fZ;
                m_creature->GetClosePoint(fX, fY, fZ, m_creature->GetObjectBoundingRadius(), 16.0f, 0.0f);

                // we assume m_creature is spawned in proper location
                m_creature->SummonCreature(NPC_ARTHAS_IMAGE, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 60000);
            }

            return;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit* pWho)
    {
        if (m_creature->HasSplineFlag(SPLINEFLAG_FLYING))
            m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ARTHAS_IMAGE)
        {
            pSummoned->CastSpell(pSummoned, SPELL_ARTHAS_VISUAL, true);
            pArthas = pSummoned;
            pSummoned->SetFacingToObject(m_creature);
        }
    }

    void SummonedCreatureDespawn(Creature* pDespawned)
    {
        if (pDespawned->GetEntry() == NPC_ARTHAS_IMAGE)
            pArthas = NULL;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_TRANSFORMING)
        {
            if (pArthas)
                pArthas->InterruptNonMeleeSpells(true);

            m_creature->UpdateEntry(NPC_SVALA_SORROW);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SVALA, DONE);
    }

    void DoMoveToPosition()
    {
        float fX, fZ, fY;
        m_creature->GetRespawnCoord(fX, fY, fZ);

        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);

        m_creature->SendMonsterMoveWithSpeed(fX, fY, fZ + 5.0f, m_uiIntroTimer);
        m_creature->GetMap()->CreatureRelocation(m_creature, fX, fY, fZ + 5.0f, m_creature->GetOrientation());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (m_bIsIntroDone)
                return;

            if (pArthas && pArthas->isAlive())
            {
                if (m_uiIntroTimer < uiDiff)
                {
                    m_uiIntroTimer = 10000;

                    switch(m_uiIntroCount)
                    {
                        case 0:
                            DoScriptText(SAY_INTRO_1, m_creature);
                            break;
                        case 1:
                            DoScriptText(SAY_INTRO_2_ARTHAS, pArthas);
                            break;
                        case 2:
                            pArthas->CastSpell(m_creature, SPELL_TRANSFORMING_CHANNEL, false);
                            m_creature->CastSpell(m_creature, SPELL_TRANSFORMING_FLOATING, false);
                            DoMoveToPosition();
                            break;
                        case 3:
                            m_creature->CastSpell(m_creature, SPELL_TRANSFORMING, false);
                            DoScriptText(SAY_INTRO_3, m_creature);
                            break;
                        case 4:
                            DoScriptText(SAY_INTRO_4_ARTHAS, pArthas);
                            break;
                        case 5:
                            DoScriptText(SAY_INTRO_5, m_creature);
                            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                             m_creature->GetMap()->CreatureRelocation(m_creature, fCoord[0][0], fCoord[0][1], fCoord[0][2], m_creature->GetOrientation());
                            m_bIsIntroDone = true;
                            break;
                    }

                    ++m_uiIntroCount;
                }
                else
                    m_uiIntroTimer -= uiDiff;
            }

            return;
        }

        if(m_uiSacrificeEndTimer < uiDiff && m_bIsSacrifice)
        {
            for(uint8 i=0; i<3; ++i)
            {
                Unit* pAdd = m_creature->GetMap()->GetCreature(m_uiAddsGUID[i]);
                if(pAdd && pAdd->isAlive())
                {
                    Unit* pPlayer = m_creature->GetMap()->GetCreature(m_uiPlayerGUID);
                    if(pPlayer)
                        m_creature->CastSpell(pPlayer, SPELL_KILL, false);

                    for(uint8 k=0; k<3; ++k)
                    {
                        Unit* pAdd = m_creature->GetMap()->GetCreature(m_uiAddsGUID[i]);
                        if(pAdd && pAdd->isAlive())
                        {
                            pAdd->SetVisibility(VISIBILITY_OFF);
                            pAdd->setFaction(35);
                        }
                        m_uiAddsGUID[k] = 0;
                    }
                    m_bIsSacrifice = false;
                    return;
                }
            }
            m_uiPlayerGUID = 0;
            m_bIsSacrifice = false;
        }else m_uiSacrificeEndTimer -= uiDiff;

        if(m_uiSinisterStrikeTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SINISTER_STRIKE : SPELL_SINISTER_STRIKE_H, false);
            m_uiSinisterStrikeTimer = urand(10000,20000);
        }else m_uiSinisterStrikeTimer -= uiDiff;

        if(m_uiCallFlamesTimer < uiDiff)
        {
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                Unit *TargetedPlayer = m_creature->GetMap()->GetCreature((*itr)->getUnitGuid());  
                if(TargetedPlayer && TargetedPlayer->isAlive())
                    m_creature->CastSpell(TargetedPlayer, SPELL_BOLT, true);
            }
            m_uiCallFlamesTimer = urand(15000,25000);
        }else m_uiCallFlamesTimer -= uiDiff;

        if(m_uiSacrificeTimer < uiDiff)
        {
            m_uiPlayerGUID = 0;
            if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                m_uiPlayerGUID = pPlayer->GetGUID();
                DoTeleportPlayer(pPlayer, fCoord[0][0], fCoord[0][1], fCoord[0][2], pPlayer->GetOrientation());
                m_uiSacrificeEndTimer = 8000;

                for(uint8 i=0; i<3; ++i)
                    if(Creature* pAdd = m_creature->SummonCreature(NPC_CHANNELER, fCoord[i][0], fCoord[i][1], fCoord[i][2], fCoord[i][3], TEMPSUMMON_TIMED_DESPAWN, 9000))
                    {
                        m_uiAddsGUID[i] = pAdd->GetGUID();
                        pAdd->AI()->AttackStart(pPlayer);
                        pAdd->CastSpell(pPlayer, SPELL_PARALYZE, false);
                    }
            }
            m_bIsSacrifice = true;
            m_uiSacrificeTimer = 20000;
        }else m_uiSacrificeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## npc_annhylde
######*/
struct MANGOS_DLL_DECL npc_svala_channelerAI : public Scripted_NoMovementAI
{
    npc_svala_channelerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() {}

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage) 
    {
        if(!pDoneBy)
            return;

        m_creature->CastSpell(pDoneBy, SPELL_SHADOWS, false);
    }

    void UpdateAI(const uint32 uiDiff) 
    {
       if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_npc_svala_channeler(Creature* pCreature)
{
    return new npc_svala_channelerAI(pCreature);
}

CreatureAI* GetAI_boss_svala(Creature* pCreature)
{
    return new boss_svalaAI(pCreature);
}

bool AreaTrigger_at_svala_intro(Player* pPlayer, const AreaTriggerEntry* pAt)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_SVALA) == NOT_STARTED)
            pInstance->SetData(TYPE_SVALA, IN_PROGRESS);
    }

    return false;
}

void AddSC_boss_svala()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_svala";
    newscript->GetAI = &GetAI_boss_svala;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_svala_channeler";
    newscript->GetAI = &GetAI_npc_svala_channeler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_svala_intro";
    newscript->pAreaTrigger = &AreaTrigger_at_svala_intro;
    newscript->RegisterSelf();
}
