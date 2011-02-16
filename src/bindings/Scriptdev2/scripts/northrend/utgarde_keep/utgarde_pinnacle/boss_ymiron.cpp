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
SDName: Boss_Ymiron
SD%Complete: %
SDComment:
SDAuthot: /// dev /// FallenAngelX
SDCategory: Utgarde Pinnacle
TODO::
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum Sounds
{
    SAY_AGGRO                               = -1575031,
    SAY_SUMMON_BJORN                        = -1575032,
    SAY_SUMMON_HALDOR                       = -1575033,
    SAY_SUMMON_RANULF                       = -1575034,
    SAY_SUMMON_TORGYN                       = -1575035,
    SAY_SLAY_1                              = -1575036,
    SAY_SLAY_2                              = -1575037,
    SAY_SLAY_3                              = -1575038,
    SAY_SLAY_4                              = -1575039,
    SAY_DEATH                               = -1575040
};

enum Spells
{
    SPELL_BANE                              = 48294,
    H_SPELL_BANE                            = 59301,
    SPELL_DARK_SLASH                        = 48292,
    SPELL_FETID_ROT                         = 48291,
    H_SPELL_FETID_ROT                       = 59300,
    SPELL_SCREAMS_OF_THE_DEAD               = 51750,
    SPELL_SPIRIT_BURST                      = 48529,
    H_SPELL_SPIRIT_BURST                    = 59305,
    SPELL_SPIRIT_STRIKE                     = 48423,
    H_SPELL_SPIRIT_STRIKE                   = 59304,
    SPELL_ANCESTORS_VENGEANCE               = 16939,

    SPELL_CHANNEL_SPIRIT_TO_YMIRON          = 48316,
    SPELL_CHANNEL_YMIRON_TO_SPIRIT          = 48307,

    SPELL_SPIRIT_FOUNT                      = 48380,
    H_SPELL_SPIRIT_FOUNT                    = 59320,
    SPELL_SPIRIT_FOUNT_BEAM                 = 48385,

    SPELL_SPIRIT_EMERGE                     = 56864,
    SPELL_SPIRIT_DIES                       = 48596,

    SPELL_SUMMON_SPIRIT_FOUNT               = 48386,
    SPELL_SPIRIT_SUMMONED_VISUAL            = 52096,
    SPELL_AVENGING_SPIRITS                  = 48590,

    SPELL_TORGYN_VISUAL                     = 48313,
    SPELL_BJORN_VISUAL                      = 48308,    
    SPELL_HALDOR_VISUAL                     = 48311,    
    SPELL_RANULF_VISUAL                     = 48312,

    // currently not used
    SPELL_CHOOSE_SPIRIT                     = 48306,
    SPELL_DESTROY_ALL_SPIRITS               = 44659
};

enum Creatures
{   
    CREATURE_SPIRIT_FOUNT                   = 27339,
    CREATURE_AVENGING_SPIRIT                = 27386,
};

uint32 Kings[4][2] = 
{
    {DATA_TORGYN, SPELL_TORGYN_VISUAL},
    {DATA_BJORN,  SPELL_BJORN_VISUAL },
    {DATA_HALDOR, SPELL_HALDOR_VISUAL},
    {DATA_RANULF, SPELL_RANULF_VISUAL}    
};

int32 KingsGossip[4] = {SAY_SUMMON_TORGYN, SAY_SUMMON_BJORN, SAY_SUMMON_HALDOR, SAY_SUMMON_RANULF};

/*######
## boss_ymiron
######*/

struct MANGOS_DLL_DECL boss_ymironAI : public ScriptedAI
{
    boss_ymironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    std::list<uint64> Summons;
    bool m_bIsRegularMode;
    bool bKingSequence;
    bool bKingSpiritPresent;
    bool bIsInRun;

    uint32 m_uiPauseTimer;
    uint32 m_uiBaneTimer;
    uint32 m_uiFetidRotTimer; 
    uint32 m_uiDarkSlashTimer;
    uint32 m_uiAncestorsVengeanceTimer;
    uint32 m_uiSpecialAbilityTimer;
    uint8  m_uiCurrentKing;
    uint32 m_uiThreshhold;
    uint8 subphase;

    void Reset()
    {
        bKingSequence               = false;
        bIsInRun                    = false;
        bKingSpiritPresent          = false;
        m_uiFetidRotTimer           = urand(8000, 13000);
        m_uiBaneTimer               = urand(18000, 23000);
        m_uiDarkSlashTimer          = urand(28000, 33000);
        m_uiAncestorsVengeanceTimer = 50000;
        m_uiSpecialAbilityTimer     = 8000;
        m_uiCurrentKing             = 0;
        m_uiPauseTimer              = 0;
        m_uiThreshhold              = 80;
        subphase                    = 0;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

     void JustReachedHome()
     {
        // Hide triggers
        for (uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pKingSpirit = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(Kings[i][0])))
            {
                pKingSpirit->InterruptNonMeleeSpells(false);
                pKingSpirit->RemoveAllAuras();
            }
        }

        DespawnAdds();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, NOT_STARTED);
     }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE)
           return;
    
        if (!bKingSequence)
        {
            bIsInRun      = false;
            bKingSequence = true;
        }
    }


    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
            case 3: DoScriptText(SAY_SLAY_4, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, DONE);

        DoScriptText(SAY_DEATH, m_creature);
        DespawnAdds();
        BurnKing();
    }

    void JustSummoned(Creature* pSummoned)
    {
        // stor GUIDs in list for easy management
        Summons.push_back(pSummoned->GetGUID());

        if (Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
        {
            if (pSummoned->IsHostileTo(pUnit) && pUnit->isInAccessablePlaceFor(pSummoned))
            {
                pSummoned->AddThreat(pUnit);
                pSummoned->GetMotionMaster()->MoveChase(pUnit);
                pUnit->AddThreat(pSummoned);
            }
        }

        if (pSummoned->GetEntry() == CREATURE_SPIRIT_FOUNT)
        {
            DoCast(pSummoned, SPELL_SPIRIT_FOUNT_BEAM, true);
            pSummoned->CastSpell(pSummoned, m_bIsRegularMode ? SPELL_SPIRIT_FOUNT : H_SPELL_SPIRIT_FOUNT, true);
        }
        
    }

    void DespawnAdds()
    {
        if (!Summons.empty())
        {
            for (std::list<uint64>::iterator itr = Summons.begin(); itr != Summons.end(); ++itr)
            {
                if (Creature* pSummoned = m_creature->GetMap()->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            }
            Summons.clear();
        }
    }

    void BurnKing()
    {
        if (m_uiCurrentKing <= 0)
            return;

        if (Unit* pKingSpirit = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(Kings[m_uiCurrentKing - 1][0])))
        {
            pKingSpirit->InterruptSpell(CURRENT_CHANNELED_SPELL);
            pKingSpirit->CastSpell(pKingSpirit, SPELL_SPIRIT_DIES, false);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(bKingSequence)
        {
            if(m_uiPauseTimer <= uiDiff)
            {
                if (Creature* pKingSpirit = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(Kings[m_uiCurrentKing][0])))
                {
                    switch(subphase)
                    {
                        case 0:
                            DoScriptText(KingsGossip[m_uiCurrentKing], m_creature, pKingSpirit);
                            m_creature->SetFacingToObject(pKingSpirit);
                            m_uiThreshhold = m_uiThreshhold - 20;
                            m_creature->CastSpell(pKingSpirit, SPELL_CHANNEL_YMIRON_TO_SPIRIT, true);
                            ++subphase;
                            m_uiPauseTimer = 3000;
                            break;
                        case 1:
                            pKingSpirit->CastSpell(pKingSpirit, Kings[m_uiCurrentKing][1], true);
                            pKingSpirit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_EMERGE);
                            ++subphase;
                            m_uiPauseTimer = 3000;
                            break;
                        case 2:
                            pKingSpirit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            pKingSpirit->CastSpell(m_creature, SPELL_CHANNEL_SPIRIT_TO_YMIRON, true);
                            ++m_uiCurrentKing;
                            bKingSpiritPresent = true;
                            bKingSequence = false;
                            bIsInRun = false;
                            subphase = 0;
                            m_uiPauseTimer = 0;
                            break;
                        default: break;
                    }
                }
            }else m_uiPauseTimer -= uiDiff;
        }
        else if (!bIsInRun)
        {
            if(m_uiBaneTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_BANE : H_SPELL_BANE);
                m_uiBaneTimer = urand(15000, 20000);
            }else m_uiBaneTimer -= uiDiff;

            if(m_uiFetidRotTimer <= uiDiff)
            {
                if(Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    DoCastSpellIfCan(pUnit, m_bIsRegularMode ? SPELL_FETID_ROT : H_SPELL_FETID_ROT);
                m_uiFetidRotTimer = urand(10000, 15000);
            }else m_uiFetidRotTimer -= uiDiff;

            if(m_uiDarkSlashTimer <= uiDiff)
            {
                if(Unit* pUnit = m_creature->getVictim())
                {
                    int32 damage = int32(pUnit->GetHealth() * 0.5);
                    m_creature->CastCustomSpell(pUnit, SPELL_DARK_SLASH, &damage, 0, 0, false); 
                }                                                               
                m_uiDarkSlashTimer = urand(30000, 35000);
            }else m_uiDarkSlashTimer -= uiDiff;

            if(m_uiAncestorsVengeanceTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_ANCESTORS_VENGEANCE);
                m_uiAncestorsVengeanceTimer =  (m_bIsRegularMode ? urand(60000, 65000) : urand(45000, 50000));
            }else m_uiAncestorsVengeanceTimer -= uiDiff;

            if (bKingSpiritPresent)
            {
                if (m_uiSpecialAbilityTimer <= uiDiff)
                {
                    switch(m_uiCurrentKing)
                    {
                        case 1:
                            DoCastSpellIfCan(m_creature, SPELL_AVENGING_SPIRITS, false);
                            // used only once
                            bKingSpiritPresent = false;
                            m_uiSpecialAbilityTimer = 5000;
                            break;
                        case 2:
                            DoCast(m_creature, SPELL_SUMMON_SPIRIT_FOUNT, false);
                            // used only once
                            bKingSpiritPresent = false;
                            m_uiSpecialAbilityTimer = 5000;
                            break;
                        case 3:
                            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SPIRIT_STRIKE : H_SPELL_SPIRIT_STRIKE, false);
                            m_uiSpecialAbilityTimer = 5000;
                            break;
                        case 4:
                            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SPIRIT_BURST : H_SPELL_SPIRIT_BURST, false);
                            m_uiSpecialAbilityTimer = 10000;
                            break;
                    }
                }else m_uiSpecialAbilityTimer -= uiDiff;
            }

            if((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth()) < m_uiThreshhold)
            {
                DoCast(m_creature->getVictim(), SPELL_SCREAMS_OF_THE_DEAD, false);
                
                BurnKing();
                DespawnAdds();

                if (Creature* pKingSpirit = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(Kings[m_uiCurrentKing][0])))
                {
                    float x, y, z;
                    pKingSpirit->GetClosePoint(x, y, z, pKingSpirit->GetObjectBoundingRadius(), 2*INTERACTION_DISTANCE, pKingSpirit->GetAngle(m_creature));
                    // z hardcoded
                    m_creature->GetMotionMaster()->MovePoint(0, x, y, 104.76f);
                    bIsInRun = true;
                }     
            }
            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_ymiron(Creature* pCreature)
{
    return new boss_ymironAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ymiron_addAI : public ScriptedAI
{
    mob_ymiron_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        if (m_creature->GetEntry() == CREATURE_AVENGING_SPIRIT)
            DoCast(m_creature, SPELL_SPIRIT_SUMMONED_VISUAL, false);

        if (!m_pInstance)
            return;
        if (Creature* pYmiron = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_YMIRON)))
            pYmiron->AI()->JustSummoned(m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_creature->GetEntry() == CREATURE_AVENGING_SPIRIT)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ymiron_add(Creature* pCreature)
{
    return new mob_ymiron_addAI(pCreature);
}

void AddSC_boss_ymiron()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ymiron";
    newscript->GetAI = &GetAI_boss_ymiron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ymiron_add";
    newscript->GetAI = &GetAI_mob_ymiron_add;
    newscript->RegisterSelf();
}
