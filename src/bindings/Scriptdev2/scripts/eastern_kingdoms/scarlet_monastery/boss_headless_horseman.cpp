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
SDName: boss_headless_horseman
SD%Complete: 0
SDComment: Place Holder
SDCategory: Scarlet Monastery
EndScriptData

SQLs
UPDATE creature_template SET ScriptName = 'boss_headless_horseman' WHERE entry = 23682;
UPDATE creature_template SET ScriptName = 'npc_horsemans_head' WHERE entry = 23775;
-- Head of the Horseman
UPDATE `creature_template` SET
`minlevel`='70',
`maxlevel`='70',
`minhealth`='63000',
`maxhealth`='63000',
`armor`='1000',
`mindmg`='1200',
`maxdmg`='1600',
`faction_A`='14',
`faction_H`='14',
`mechanic_immune_mask`='2048'
WHERE `entry`='23775';

UPDATE creature_template SET
`minlevel` = 70,
`maxlevel` = 70,
`minhealth` = 4541,
`maxhealth` = 4541,
`armor` = 6792,
`mindmg` = 651,
`maxdmg` = 834,
`baseattacktime` = 2000,
`mechanic_immune_mask` = '8388624',
`faction_A` = 14,
`faction_H` = 14
WHERE entry IN (23545,23694);

UPDATE creature_template SET ScriptName = 'mob_pulsing_pumpkin' WHERE entry = 23694;

*support for Pulsing Pumpkins/Pumpkin Fiends
*Pulsing Pumpkin template
*improved visual effect of thossing head
*stored Headless Horseman guid in instance script to easier access
*despawn Head immediately after death
*/

#include "precompiled.h"
#include "scarlet_monastery.h"

enum
{
    SAY_ENTRANCE                = -1189022,
    SAY_REJOINED                = -1189023,
    SAY_BODY_DEFEAT             = -1189024,
    SAY_LOST_HEAD               = -1189025,
    SAY_CONFLAGRATION           = -1189026,
    SAY_SPROUTING_PUMPKINS      = -1189027,
    SAY_SLAY                    = -1189028,
    SAY_DEATH                   = -1189029,

    EMOTE_LAUGH                 = -1189030,

    SAY_PLAYER1                 = -1189031,
    SAY_PLAYER2                 = -1189032,
    SAY_PLAYER3                 = -1189033,
    SAY_PLAYER4                 = -1189034,

    //spells
    SPELL_CLEAVE                = 42587,
    SPELL_CONFLAGRATION         = 42380,        //Phase 2, can't find real spell(Dim Fire?)
    SPELL_SUMMON_PUMPKIN        = 42394,        //summon 4 pumpkin or 52236, Phase 3(requres summon_spells.patch - TARGET_EFFECT_SELECT not implemened in core)

    //spell handling laugh
    SPELL_LAUGH0                = 43881,
    SPELL_LAUGH1                = 43885,
    SPELL_LAUGH2                = 43894,

    SPELL_WHIRLWIND             = 43116,        //or  ?41194
    SPELL_CLEAVE_PROC           = 42587,        //procs after "killing" body(at each end of phase or when he really dies?)
    SPELL_IMMUNE                = 42556,
    SPELL_BODY_REGEN            = 42403,        //regen 4% of total hp per 1sec;not used, because this, body regen and whirlwind non stackable with each other
    SPELL_BODY_CONFUSE          = 43105,        //confused movement

    SPELL_SEND_HEAD             = 42399,        //visual flying head
    SPELL_HEAD                  = 42413,        //visual buff, "head"
    SPELL_HEAD_IS_DEAD          = 42428,        //at killing head, Phase 3
    SPELL_HEAD_INVIS            = 44312,        //not used
    SPELL_SUMMON_WISP_HEAD      = 42823,        //summonig whispering head

    SPELL_PUMPKIN_AURA          = 42280,
    SPELL_PUMPKIN_AURA_GREEN    = 42294,
    SPELL_PUMPKIN_DEATH         = 42291,
    SPELL_SQUASH_SOUL           = 42514,        //pumpkin debuff
    SPELL_SPROUTING             = 42281,
    SPELL_SPROUT_BODY           = 42285,

    //Effects
    SPELL_RHYME_BIG             = 42909,        //summoning rhyme
    SPELL_RHYME_SMALL           = 42910,
    SPELL_HEAD_SPEAKS           = 43129,
    SPELL_HEAD_LANDS            = 42400,
    SPELL_BODY_FLAME            = 42074,
    SPELL_ON_KILL_PROC          = 43877,        //procs after killing players?
    SPELL_ENRAGE_VISUAL         = 42438,        // he uses this spell?
    SPELL_WISP                  = 43034,

    NPC_HEAD                    = 23775,
    NPC_PUMPKIN_FIEND           = 23545
};

float FlightPoint[21][3] =
{
    {1754.00f, 1346.00f, 17.50f},
    {1765.00f, 1347.00f, 19.00f},
    {1784.00f, 1346.80f, 25.40f},
    {1803.30f, 1347.60f, 33.00f},
    {1824.00f, 1350.00f, 42.60f},
    {1838.80f, 1353.20f, 49.80f},
    {1852.00f, 1357.60f, 55.70f},
    {1861.30f, 1364.00f, 59.40f},
    {1866.30f, 1374.80f, 61.70f},
    {1864.00f, 1387.30f, 63.20f},
    {1854.80f, 1399.40f, 64.10f},
    {1844.00f, 1406.90f, 64.10f},
    {1824.30f, 1411.40f, 63.30f},
    {1801.00f, 1412.30f, 60.40f},
    {1782.00f, 1410.10f, 55.50f},
    {1770.50f, 1405.20f, 50.30f},
    {1765.20f, 1400.70f, 46.60f},
    {1761.40f, 1393.40f, 41.70f},
    {1759.10f, 1386.70f, 36.60f},
    {1757.80f, 1378.20f, 29.00f},
    {1758.00f, 1367.00f, 19.51f}
};
/*######
## npc_horsemans_head
######*/

struct MANGOS_DLL_DECL npc_horsemans_headAI : public ScriptedAI
{
    npc_horsemans_headAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance* m_pInstance;

    void Reset()
    {
        if (!m_creature->HasAura(SPELL_HEAD_INVIS))
            m_creature->CastSpell(m_creature,SPELL_HEAD_INVIS,false);
        if (m_creature->HasAura(SPELL_HEAD))
            m_creature->RemoveAurasDueToSpell(SPELL_HEAD);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,6);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (!pSpell)
            return;

        if (pSpell->Id == SPELL_SEND_HEAD)
        {
            m_creature->CastSpell(m_creature,SPELL_HEAD,false);
            DoScriptText(SAY_LOST_HEAD,m_creature);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        Creature* pHorseman = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_HEADLESS_HORSEMAN));
        if (!pHorseman)
            return;

        switch(m_pInstance->GetData(TYPE_HALLOWSEND_EVENT))
        {
            case 0:
            case 2:
            case 4:
                if (m_creature->HasAura(SPELL_HEAD))
                {
                    DoCast(pHorseman,SPELL_SEND_HEAD,true);
                    m_creature->RemoveAurasDueToSpell(SPELL_HEAD);
                }
                if (!m_creature->HasAura(SPELL_HEAD_INVIS))
                    m_creature->CastSpell(m_creature,SPELL_HEAD_INVIS,false);
                break;
            case 1:
            case 3:
            case 5:
                if (m_creature->HasAura(SPELL_HEAD_INVIS))
                    m_creature->RemoveAurasDueToSpell(SPELL_HEAD_INVIS);

                DoMeleeAttackIfReady();
                break;
            // Head or body died
            case 6:
                if (m_creature->isAlive())
                    m_creature->ForcedDespawn();
                break;
            default: break;
        }
    }

};
CreatureAI* GetAI_npc_horsemans_head(Creature* pCreature)
{
    return new npc_horsemans_headAI(pCreature);
}

/*######
## boss_headless_horseman
######*/
struct MANGOS_DLL_DECL boss_headless_horsemanAI : public ScriptedAI
{
    boss_headless_horsemanAI(Creature* pCreature) : ScriptedAI(pCreature)

    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
        pEntered = false;
    }

    ScriptedInstance* m_pInstance;

    bool pEntered;

    uint8 point_id;

    uint32 m_uiFlyPoint_Timer;
    uint32 m_uiCleave_Timer;
    uint32 m_uiConflageration_Timer;
    uint32 m_uiPumpkinSprout_Timer;

    uint64 HeadGUID;

    void Reset()
    {
        point_id = 0;
        m_uiFlyPoint_Timer = 0;
        m_uiCleave_Timer = 5000;
        m_uiConflageration_Timer = 5000;
        m_uiPumpkinSprout_Timer = 5000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,0);
        if (m_pInstance)
            m_pInstance->SetData64(DATA_HEADLESS_HORSEMAN,m_creature->GetGUID());
    }

    void Laugh()
    {
        switch(urand(0,2))
        {
            case 0: DoCast(m_creature,SPELL_LAUGH0,true); break;
            case 1: DoCast(m_creature,SPELL_LAUGH1,true); break;
            case 2: DoCast(m_creature,SPELL_LAUGH2,true); break;
        }
    }

    void Aggro(Unit* pWho)
    {
        if (!pWho)
            return;

        if (pEntered)
            m_creature->SetInCombatWithZone();
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,6);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (!pSpell)
            return;

        if (pSpell->Id == SPELL_SEND_HEAD)
        {
            DoScriptText(SAY_REJOINED,m_creature);
            m_creature->CastSpell(m_creature,SPELL_HEAD,false);
        }
    }
    void RemoveRegenAuras()
    {
        if (m_creature->HasAura(SPELL_BODY_REGEN))
            m_creature->RemoveAurasDueToSpell(SPELL_BODY_REGEN);
        if (m_creature->HasAura(SPELL_IMMUNE))
            m_creature->RemoveAurasDueToSpell(SPELL_IMMUNE);
        if (m_creature->HasAura(SPELL_BODY_CONFUSE))
            m_creature->RemoveAurasDueToSpell(SPELL_BODY_CONFUSE);
    }

    void ApplyRegenAuras()
    {
        if (!m_creature->HasAura(SPELL_BODY_REGEN))
            DoCast(m_creature,SPELL_BODY_REGEN,true);
        if (!m_creature->HasAura(SPELL_IMMUNE))
            DoCast(m_creature,SPELL_IMMUNE,true);
        if (!m_creature->HasAura(SPELL_BODY_CONFUSE))
            DoCast(m_creature,SPELL_BODY_CONFUSE,true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pEntered)
        {
            if (m_uiFlyPoint_Timer <= uiDiff)
            {
                switch (point_id)
                {
                    case 21:
                        float x,y,z;
                        m_creature->GetClosePoint(x, y, z, m_creature->GetObjectBoundingRadius(), 15.0f, 0.0f);
                        if (Creature* pTemp = m_creature->SummonCreature(NPC_HEAD, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
                            HeadGUID = pTemp->GetGUID();
                        else
                        {
                            error_log("Headless Horsemann could't summon his head so abording the event");
                            m_creature->ForcedDespawn();
                        }
                        pEntered = true;
                        break;
                    default:
                        m_creature->SendMonsterMove(FlightPoint[point_id][0], FlightPoint[point_id][1], FlightPoint[point_id][2], SPLINETYPE_NORMAL, SPLINEFLAG_WALKMODE,999);
                        ++point_id;
                        break;
                }
                m_uiFlyPoint_Timer = 1000;
            }else m_uiFlyPoint_Timer -= uiDiff;
        }

        if (!m_pInstance)
            return;

        Creature* pHead = m_creature->GetMap()->GetCreature(HeadGUID);
        if (pEntered && !pHead)
        {
            error_log("Debug: Headless Horseman aggroed but no Head of Horseman found so abording!");
            m_creature->ForcedDespawn();
        }

        switch (m_pInstance->GetData(TYPE_HALLOWSEND_EVENT))
        {
            //Phase 1a
            case 0:
                if (!m_creature->HasAura(SPELL_HEAD))
                    DoCast(m_creature,SPELL_HEAD,false);

                if (((m_creature->GetHealth()*100) / (m_creature->GetMaxHealth())) <= 1)
                    m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,1);

                if (m_uiCleave_Timer <= uiDiff)
                {
                    m_creature->CastSpell(m_creature->getVictim(),SPELL_CLEAVE_PROC,false);
                    m_uiCleave_Timer = 5000;
                }else m_uiCleave_Timer -= uiDiff;
                DoMeleeAttackIfReady();
                break;

            //Phase 1b
            case 1:
                if (m_creature->HasAura(SPELL_HEAD))
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_HEAD);
                    m_creature->CastSpell(pHead,SPELL_SEND_HEAD,false);
                }

                ApplyRegenAuras();

                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    if (pHead->AI())
                        pHead->AI()->AttackStart(pTarget);

                if ((pHead->GetHealth()*100) / (pHead->GetMaxHealth()) <= 66)
                {
                    RemoveRegenAuras();

                    if ((m_creature->GetHealth()*100) / (m_creature->GetMaxHealth()) < 100)
                        m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,2);
                    else
                        m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,0);
                    pHead->SetHealth(pHead->GetMaxHealth()*0.7);
                }
                break;

            //Phase 2a
            case 2:
                if (((m_creature->GetHealth()*100) / (m_creature->GetMaxHealth())) <= 1)
                    m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,3);

                if (m_uiConflageration_Timer <= uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        DoScriptText(SAY_CONFLAGRATION,m_creature);
                        pTarget->CastSpell(pTarget,SPELL_CONFLAGRATION,false);
                    }
                    m_uiConflageration_Timer = 15000;
                }else m_uiConflageration_Timer -= uiDiff;

                DoMeleeAttackIfReady();
                break;

            //Phase 2b
            case 3:
                if (m_creature->HasAura(SPELL_HEAD))
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_HEAD);
                    m_creature->CastSpell(pHead,SPELL_SEND_HEAD,false);
                }

                ApplyRegenAuras();

                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    if (pHead->AI())
                        pHead->AI()->AttackStart(pTarget);

                if ((pHead->GetHealth()*100) / (pHead->GetMaxHealth()) <= 33)
                {
                    RemoveRegenAuras();

                    if ((m_creature->GetHealth()*100) / (m_creature->GetMaxHealth()) < 100)
                        m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,4);
                    else
                        m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,2);
                    pHead->SetHealth(pHead->GetMaxHealth()*0.4);
                }
                break;
            //Phase 3a
            case 4:
                if ((m_creature->GetHealth()*100) / (m_creature->GetMaxHealth()) <= 1)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        if (pHead->AI())
                            pHead->AI()->AttackStart(pTarget);

                    m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,5);
                }
                if (m_uiPumpkinSprout_Timer <= uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        DoScriptText(SAY_SPROUTING_PUMPKINS,m_creature);
                        m_creature->CastSpell(pTarget,SPELL_SUMMON_PUMPKIN,false);
                    }
                    m_uiPumpkinSprout_Timer = 30000;
                }else m_uiPumpkinSprout_Timer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
            //Phase 3b
            case 5:
                if (m_creature->HasAura(SPELL_HEAD))
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_HEAD);
                    m_creature->CastSpell(pHead,SPELL_SEND_HEAD,false);
                }

                ApplyRegenAuras();

                if (((m_creature->GetHealth()*100) / (m_creature->GetMaxHealth())) == 100)
                {
                    RemoveRegenAuras();
                    m_pInstance->SetData(TYPE_HALLOWSEND_EVENT,4);
                }
                break;
            // Head or body died
            case 6:
                if (m_creature->isAlive())
                    m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                break;

            default: break;
        }
    }
};

CreatureAI* GetAI_boss_headless_horseman(Creature* pCreature)
{
    return new boss_headless_horsemanAI(pCreature);
}

/*######
## mob_pulsing_pumpkin
######*/

struct MANGOS_DLL_DECL mob_pulsing_pumpkinAI : public ScriptedAI
{
    mob_pulsing_pumpkinAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        // on creature create
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
        DoCast(m_creature,SPELL_PUMPKIN_AURA_GREEN,true);
        DoCast(m_creature,SPELL_PUMPKIN_AURA,true);
        DoCast(m_creature,SPELL_SPROUTING,false);

        bSquashed = false;
        bTransformed = false;
    }

    ScriptedInstance* m_pInstance;

    bool bSquashed;
    bool bTransformed;

    void Reset() {}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        Creature* pHorseman = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_HEADLESS_HORSEMAN));
        if (!pHorseman)
            return;

        // Case horseman or his head died
        if (m_pInstance->GetData(TYPE_HALLOWSEND_EVENT) == 6)
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        // Case event reset
        if (m_pInstance->GetData(TYPE_HALLOWSEND_EVENT) == 0)
            m_creature->ForcedDespawn();

        // Apply squash after landing
        if (!bSquashed && m_creature->HasAura(SPELL_PUMPKIN_AURA))
        {
            Map::PlayerList const& lPlayers = m_pInstance->instance->GetPlayers();
            if (!lPlayers.isEmpty())
            {
                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    if (Player* pPlayer = itr->getSource())
                        if (m_creature->GetDistance(pPlayer) <= INTERACTION_DISTANCE)
                            DoCast(pPlayer,SPELL_SQUASH_SOUL,true);
                }
            }
            bSquashed = true;
        }

        if (!bTransformed && m_creature->HasAura(SPELL_SPROUT_BODY))
        {
            m_creature->UpdateEntry(NPC_PUMPKIN_FIEND);
            if (Unit* pTarget = pHorseman->getVictim())
                AttackStart(pTarget);
            bTransformed = true;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_pulsing_pumpkin(Creature* pCreature)
{
    return new mob_pulsing_pumpkinAI(pCreature);
}


void AddSC_boss_headless_horseman()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "npc_horsemans_head";
    NewScript->GetAI = GetAI_npc_horsemans_head;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_headless_horseman";
    NewScript->GetAI = GetAI_boss_headless_horseman;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_pulsing_pumpkin";
    NewScript->GetAI = GetAI_mob_pulsing_pumpkin;
    NewScript->RegisterSelf();
}
