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
SDName: boss_hodir
SD%Complete: 60%
SDComment:Auras needs core fix
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

enum
{
    SPELL_ENRAGE                = 26662,

    SPELL_FROZEN_BLOWS          = 62478,
    SPELL_FROZEN_BLOWS_H        = 63512,
    SPELL_FREEZE                = 62469,
    SPELL_BITTER_COLD           = 62038,	// SPELL BROKEN!!!!
    SPELL_ICICLE                = 62460,    // full spell -> needs core fix
    SPELL_ICE_SHARDS            = 65370,    // icicle damage -> 14k
    SPELL_ICICLE_DUMMY          = 62453,
    SPELL_SNOWDRIFT             = 62463,
    SPELL_FLASH_FREEZE          = 61968,
    SPELL_FLASH_FREEZE_VIS      = 62148,
    SPELL_FLASH_FREEZE_STUN     = 64175,
    SPELL_FLASH_FREEZE_KILL     = 62226,
    SPELL_FLASH_FREEZE_NPC_STUN = 61990,    // used to freeze npcs

    NPC_SNOWDRIFT_TARGET    = 33174,
    NPC_ICICLE              = 33169,
    NPC_SNOW_ICICLE         = 33173,
    NPC_FLASH_FREEZE        = 32926,
    NPC_FLASH_FREEZE_NPC    = 32938, 

    SAY_AGGRO               = -1603085,
    SAY_DEATH               = -1603084,
    SAY_SLAY01              = -1603083,
    SAY_SLAY02              = -1603082,
    SAY_FLASH_FREEZE        = -1603081,
    SAY_FROZEN_BLOWS        = -1603080,
    SAY_BERSERK             = -1603087,
    EMOTE_FLASH_FREEZE      = -1603360,
    EMOTE_FROZEN_BLOWS      = -1603361,

    ACHIEV_RARE_CACHE       = 3182,
    ACHIEV_RARE_CACHE_H     = 3184,
    ACHIEV_COOLEST_FRIEND   = 2963,
    ACHIEV_COOLEST_FRIEND_H = 2965,
    ACHIEV_GETTING_COLD     = 2967,
    ACHIEV_GETTING_COLD_H   = 2968,
    ACHIEV_CHEESE_FREEZE    = 2961,
    ACHIEV_CHEESE_FREEZE_H  = 2962,

    // helper npcs
    // druid
    SPELL_WRATH         = 62793,
    SPELL_STARLIGHT     = 62807, // friendly

    // shaman
    SPELL_LAVA_BURST    = 61924,
    SPELL_STORM_CLOUD   = 65123, // friendly
    SPELL_STORM_CLOUD_H = 65133,
    SPELL_STORM_POWER   = 65134, // friendly

    // mage
    SPELL_FIREBALL      = 61909,
    SPELL_TOASTY_FIRE   = 62823, // friendly -> summon
    NPC_TOASTY_FIRE     = 33342,
    SPELL_TOASTY_FIRE_A = 62821,
    SPELL_MELT_ICE      = 64528,
    SPELL_SIGNED        = 65280,

    // priest
    SPELL_SMITE         = 61923,
    SPELL_GREAT_HEAL    = 62809,    //friendly
    SPELL_DISPEL_MAGIC  = 63499,    //friendly

};

#define CENTER_X    2000.0f
#define CENTER_Y    -234.21f

bool m_bCoolestFriend;

// script for Flash freeze
struct MANGOS_DLL_DECL mob_flashFreezeAI : public ScriptedAI
{
    mob_flashFreezeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
        if(m_bIsRegularMode)
            m_creature->SetMaxHealth(35000);
        m_creature->SetRespawnDelay(DAY);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);
        DoCast(pWho, SPELL_FLASH_FREEZE_STUN);
        pWho->CastSpell(pWho, SPELL_FLASH_FREEZE_STUN, false);
        m_uiVictimGUID = pWho->GetGUID();
        // kill targets that are frozen
        if(pWho->HasAura(SPELL_FREEZE, EFFECT_INDEX_0))
        {
            pWho->CastSpell(pWho, SPELL_FLASH_FREEZE_KILL, false);
            m_creature->ForcedDespawn();
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) 
            pVictim->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_STUN);
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = m_creature->GetMap()->GetUnit( m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_STUN);

        if (Killer)
            Killer->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_STUN);
    }

    void UpdateAI(const uint32 diff) {}
};

// script for Icicles
struct MANGOS_DLL_DECL mob_icicleAI : public ScriptedAI
{
    mob_icicleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetCombatMovement(false);
		pCreature->setFaction(14);
        Reset();
    }

    uint32 m_uiSpellDelayTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_ICICLE);
        m_uiSpellDelayTimer = 500;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 diff)
    { 
        if(m_uiSpellDelayTimer < diff)
        {
            DoCast(m_creature, SPELL_ICICLE_DUMMY);
            m_uiSpellDelayTimer = 30000;
        }
        else m_uiSpellDelayTimer -= diff;
    }
};

// Toasty fire. Used by mage
struct MANGOS_DLL_DECL mob_toasty_fireAI : public ScriptedAI
{
    mob_toasty_fireAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    void Reset()
    {
        DoCast(m_creature, SPELL_TOASTY_FIRE_A);
    }

    void UpdateAI(const uint32 diff) {}
};

// Script for the Flash freeze which is enchasing the npcs in ice at the begginign of the fight
// this needs some fixing on spells
struct MANGOS_DLL_DECL mob_npc_flashFreezeAI : public ScriptedAI
{
    mob_npc_flashFreezeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetDisplayId(25865);     // invisible
		pCreature->GetMotionMaster()->MoveIdle();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    std::list<Creature*> lVictims;

    void Reset()
    {
        lVictims.clear();
        SetVictim();
        DoCast(m_creature, SPELL_FLASH_FREEZE_NPC_STUN);
    }

    void Aggro(Unit *who) 
    {
        if (Creature* pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
        {
            pHodir->AI()->AttackStart(who);
            pHodir->AddThreat(who, 0.0f);
        }
    }

    void SetVictim()
    {
        // druids
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33325, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32901, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32941, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33333, 2.0f);
        // shamys
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33328, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32900, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33332, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32950, 2.0f);
        // mages
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32893, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33327, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33331, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32946, 2.0f);
        // priests
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32897, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33326, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 32948, 2.0f);
        GetCreatureListWithEntryInGrid(lVictims, m_creature, 33330, 2.0f);
        if (!lVictims.empty())
        {
            for(std::list<Creature*>::iterator iter = lVictims.begin(); iter != lVictims.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive() && !(*iter)->HasAura(SPELL_FLASH_FREEZE_NPC_STUN, EFFECT_INDEX_0))
                    (*iter)->CastSpell((*iter), SPELL_FLASH_FREEZE_NPC_STUN, false);
            }
        }
    }

    void JustDied(Unit* Killer)
    {
        if (!lVictims.empty())
        {
            for(std::list<Creature*>::iterator iter = lVictims.begin(); iter != lVictims.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive() && (*iter)->HasAura(SPELL_FLASH_FREEZE_NPC_STUN, EFFECT_INDEX_0))
                {
                    (*iter)->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_NPC_STUN);
                    if (Creature* pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
                        (*iter)->AddThreat(pHodir, 100.0f);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff) 
    { }
};

// Hodir
struct MANGOS_DLL_DECL boss_hodirAI : public ScriptedAI
{
    boss_hodirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    // hard mode timer
    uint32 m_uiSpeedKillTimer;

    uint32 m_uiEnrageTimer;
    uint32 m_uiFlashFreezeTimer;
    uint32 m_uiFlashFreezeCastTimer;
    uint32 m_uiFrozenBlowsTimer;
    uint32 m_uiFreezeTimer;
    uint32 m_uiIcicleTimer;
    uint8 m_uiIcicleCount;
    bool m_bIsCheese;

    bool m_bIsOutro;
    uint32 m_uiOutroTimer;
    uint32 m_uiStep;

    std::list<Creature*> lFriends;

    void Reset()
    {
        m_uiSpeedKillTimer      = 0;
        m_bCoolestFriend        = true;
        m_uiEnrageTimer         = 480000;
        m_uiFlashFreezeTimer    = 50000;
        m_uiFlashFreezeCastTimer= 90000;
        m_uiFrozenBlowsTimer    = 60000;
        m_uiFreezeTimer         = urand(15000, 20000);
        m_uiIcicleTimer         = 10000;
        m_uiIcicleCount         = 0;
        m_uiOutroTimer          = 10000;
        m_uiStep                = 1;
        m_bIsOutro              = false;
        m_bIsCheese             = true;

        // respawn friendly npcs
        // druids
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33325, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32901, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32941, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33333, DEFAULT_VISIBILITY_INSTANCE);
        // shamys
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33328, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32900, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33332, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32950, DEFAULT_VISIBILITY_INSTANCE);
        // mages
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32893, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33327, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33331, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32946, DEFAULT_VISIBILITY_INSTANCE);
        // priests
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32897, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33326, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32948, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33330, DEFAULT_VISIBILITY_INSTANCE);
        // flash freeze for them: 
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32938, DEFAULT_VISIBILITY_INSTANCE);
        if (!lFriends.empty())
        {
            for(std::list<Creature*>::iterator iter = lFriends.begin(); iter != lFriends.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
    }

    void JustReachedHome()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_HODIR, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_HODIR, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        DoCast(m_creature, SPELL_BITTER_COLD);
    }

    void DoOutro()
    {
        if(m_pInstance) 
        {
            if(m_uiSpeedKillTimer < 180000)
            {
                m_pInstance->SetData(TYPE_HODIR_HARD, DONE);
                m_pInstance->SetData(TYPE_HODIR, DONE);
                // hacky way to complete achievements; use only if you have this function
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_RARE_CACHE : ACHIEV_RARE_CACHE_H);
            }
            else
                m_pInstance->SetData(TYPE_HODIR, DONE);

            // hacky way to complete achievements; use only if you have this function
            if (m_bCoolestFriend)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_COOLEST_FRIEND : ACHIEV_COOLEST_FRIEND_H);

            if (m_bIsCheese)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_CHEESE_FREEZE : ACHIEV_CHEESE_FREEZE_H);
        }
        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_HODIR, DONE);
            if(m_uiSpeedKillTimer > 0)
                m_pInstance->SetData(TYPE_HODIR_HARD, DONE);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealthPercent() < 1.0f)
        {
            uiDamage = 0;
            m_bIsOutro = true;
        }
    }

    void KilledUnit(Unit *who)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY01, m_creature);
        else
            DoScriptText(SAY_SLAY02, m_creature);
    }

	// Flash freeze. Hacky way, needs core support
	// PLEASE REMOVE FOR REVISION!
    void DoFlashFreeze()
    {
        std::list<Creature*> lSnowdrift;
        GetCreatureListWithEntryInGrid(lSnowdrift, m_creature, NPC_SNOWDRIFT_TARGET, DEFAULT_VISIBILITY_INSTANCE);
        
        Map* pMap = m_creature->GetMap();
        if(pMap)
        {
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            if (!lPlayers.isEmpty())
            {
                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    if (Player* pPlayer = itr->getSource())
                    {
                        if(pPlayer && pPlayer->isAlive())
                        {
                            if (!lSnowdrift.empty())
                            {
                                for(std::list<Creature*>::iterator iter = lSnowdrift.begin(); iter != lSnowdrift.end(); ++iter)
                                {
                                    if ((*iter) && pPlayer->GetDistance2d((*iter)) > 5.0f)
                                    {
                                        if(Creature* pTemp = m_creature->SummonCreature(NPC_FLASH_FREEZE, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 9000))
                                            pTemp->AddThreat(pPlayer, 100.0f);
                                        // don't complete achievement
                                        m_bIsCheese = false;
                                    }
                                }
                            }
                            else
                            {
                                if(Creature* pTemp = m_creature->SummonCreature(NPC_FLASH_FREEZE, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 9000))
                                    pTemp->AddThreat(pPlayer, 100.0f);
                            }
                        }
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_bIsOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            // reset if fighting only the npcs
            // this gets bugged if some of the npcs get top aggro
            if(m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER)
                EnterEvadeMode();

            // hard mode check
            m_uiSpeedKillTimer += uiDiff;

            // Flash freeze visual
            if(m_uiFlashFreezeTimer < uiDiff)
            {
                DoScriptText(EMOTE_FLASH_FREEZE, m_creature);
                DoScriptText(SAY_FLASH_FREEZE, m_creature);
                DoCast(m_creature, SPELL_FLASH_FREEZE);
                m_uiFlashFreezeTimer = 50000;
                m_uiFlashFreezeCastTimer = 9000;
            }
            else m_uiFlashFreezeTimer -= uiDiff;

            // hacky way for flash freeze
            if (m_uiFlashFreezeCastTimer < uiDiff)
            {
                DoFlashFreeze();
                DoCast(m_creature, SPELL_FLASH_FREEZE_VIS);
                m_uiFlashFreezeCastTimer = 90000;
            }
            else m_uiFlashFreezeCastTimer -= uiDiff;

            // icicles
            // should be done be spell
            if(m_uiIcicleTimer < uiDiff)
            {
                float angle = (float) rand()*360/RAND_MAX + 1;
                float homeX = CENTER_X + urand(0, 30)*cos(angle*(M_PI/180));
                float homeY = CENTER_Y + urand(0, 30)*sin(angle*(M_PI/180));
                m_creature->SummonCreature(NPC_ICICLE, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 3000);
                m_uiIcicleTimer = urand(2000, 5000);
            }
            else m_uiIcicleTimer -= uiDiff;

            // frozen blows
            if(m_uiFrozenBlowsTimer < uiDiff)
            {
                DoScriptText(SAY_FROZEN_BLOWS, m_creature);
                DoScriptText(EMOTE_FROZEN_BLOWS, m_creature);
                DoCast(m_creature, m_bIsRegularMode ? SPELL_FROZEN_BLOWS : SPELL_FROZEN_BLOWS_H);
                m_uiFrozenBlowsTimer = urand(50000, 60000);
            }
            else m_uiFrozenBlowsTimer -= uiDiff;

            // freeze
            if(m_uiFreezeTimer < uiDiff)
            {
                if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_FREEZE);
                m_uiFreezeTimer = urand(5000, 10000);
            }
            else m_uiFreezeTimer -= uiDiff;

            // enrage
            if(m_uiEnrageTimer < uiDiff)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                DoCast(m_creature, SPELL_ENRAGE);
                m_uiEnrageTimer = 30000;
            }
            else m_uiEnrageTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        // outro
        if(m_bIsOutro)
        {
            switch(m_uiStep)
            {
            case 1:
                m_creature->setFaction(35);
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovePoint(0, 1984.64f, -206.37f, 432.68f);
                ++m_uiStep;
                m_uiOutroTimer = 10000;
                break;
            case 3:
                DoScriptText(SAY_DEATH, m_creature);
                ++m_uiStep;
                m_uiOutroTimer = 5000;
                break;
            case 5:
                DoOutro();
                ++m_uiStep;
                m_uiOutroTimer = 10000;
                break;
            }
        }
        else return;

        if (m_uiOutroTimer <= uiDiff)
        {
            ++m_uiStep;
            m_uiOutroTimer = 330000;
        } m_uiOutroTimer -= uiDiff;
    }
};

// Helper npcs
struct MANGOS_DLL_DECL npc_hodir_druidAI : public ScriptedAI
{
    npc_hodir_druidAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20.0f);
        }
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return m_creature->GetGUID();
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
                    DoCast(pHodir, SPELL_WRATH);
                break;
            case 1:
                Unit *pTemp = m_creature->GetMap()->GetUnit((SelectRandomPlayer()));
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40)
                    DoCast(pTemp, SPELL_STARLIGHT);
                break;
            }
            spellTimer = urand(2000, 5000);
        }else spellTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_hodir_druid(Creature* pCreature)
{
    return new npc_hodir_druidAI(pCreature);
}

struct MANGOS_DLL_DECL npc_hodir_shamanAI : public ScriptedAI
{
    npc_hodir_shamanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20.0f);
        }
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return m_creature->GetGUID();
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
                    DoCast(pHodir, SPELL_LAVA_BURST);
                break;
            case 1:
                Unit *pTemp = m_creature->GetMap()->GetUnit((SelectRandomPlayer()));
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40)
                    DoCast(pTemp, m_bIsRegularMode ? SPELL_STORM_CLOUD : SPELL_STORM_CLOUD_H);
                break;
            }
            spellTimer = urand(2000,5000);
        }else spellTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_hodir_shaman(Creature* pCreature)
{
    return new npc_hodir_shamanAI(pCreature);
}

struct MANGOS_DLL_DECL npc_hodir_mageAI : public ScriptedAI
{
    npc_hodir_mageAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20.0f);
        }
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return m_creature->GetGUID();
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 4))
            {
            case 0:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
                    DoCast(pHodir, SPELL_FIREBALL);
                break;
            case 1:
            case 2:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
                    DoCast(pHodir, SPELL_SIGNED);
                break;
            case 3:
                if(Creature *pTemp = GetClosestCreatureWithEntry(m_creature, NPC_FLASH_FREEZE, 50.0f))
                    DoCast(pTemp, SPELL_MELT_ICE);
                break;
            case 4:
                Unit *pTemp = m_creature->GetMap()->GetUnit((SelectRandomPlayer()));
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40)
                    DoCast(pTemp, SPELL_TOASTY_FIRE);
                break;
            }
            spellTimer = urand(2000,5000);
        }else spellTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_hodir_mage(Creature* pCreature)
{
    return new npc_hodir_mageAI(pCreature);
}

struct MANGOS_DLL_DECL npc_hodir_priestAI : public ScriptedAI
{
    npc_hodir_priestAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20.0f);
        }
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return m_creature->GetGUID();
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 4))
            {
            case 0:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
                    DoCast(pHodir, SPELL_SMITE);
                break;
            case 1:
            case 2:
            case 3:
                if (Unit* pHealTarget = DoSelectLowestHpFriendly(40.0f))
                    DoCast(pHealTarget, SPELL_GREAT_HEAL);
                break;
            case 4:
                if (Unit* pTemp = m_creature->GetMap()->GetUnit((SelectRandomPlayer())))
                    DoCast(m_creature, SPELL_DISPEL_MAGIC);
                break;
            }
            spellTimer = urand(2000,5000);
        }else spellTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_hodir_priest(Creature* pCreature)
{
    return new npc_hodir_priestAI(pCreature);
}

CreatureAI* GetAI_boss_hodir(Creature* pCreature)
{
    return new boss_hodirAI(pCreature);
}

CreatureAI* GetAI_mob_flashFreeze(Creature* pCreature)
{
    return new mob_flashFreezeAI(pCreature);
}

CreatureAI* GetAI_mob_icicle(Creature* pCreature)
{
    return new mob_icicleAI(pCreature);
}

CreatureAI* GetAI_mob_toasty_fire(Creature* pCreature)
{
    return new mob_toasty_fireAI(pCreature);
}

CreatureAI* GetAI_mob_npc_flashFreeze(Creature* pCreature)
{
    return new mob_npc_flashFreezeAI(pCreature);
}

void AddSC_boss_hodir()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hodir";
    newscript->GetAI = &GetAI_boss_hodir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flashFreeze";
    newscript->GetAI = &GetAI_mob_flashFreeze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_npc_flashFreeze";
    newscript->GetAI = &GetAI_mob_npc_flashFreeze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icicle";
    newscript->GetAI = &GetAI_mob_icicle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toasty_fire";
    newscript->GetAI = &GetAI_mob_toasty_fire;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_priest";
    newscript->GetAI = &GetAI_npc_hodir_priest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_mage";
    newscript->GetAI = &GetAI_npc_hodir_mage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_druid";
    newscript->GetAI = &GetAI_npc_hodir_druid;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_shaman";
    newscript->GetAI = &GetAI_npc_hodir_shaman;
    newscript->RegisterSelf();
}