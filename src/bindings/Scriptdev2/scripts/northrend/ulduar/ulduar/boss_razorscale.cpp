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
SDName: boss_razorscale
SD%Complete:
SDComment: harpoons display should change when clicked
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

enum
{
	//yells/emotes
    SAY_INTRO               = -1603020,
    SAY_AGGRO1              = -1603021,
    SAY_AGGRO2              = -1603022,
    SAY_AGGRO3              = -1603023,
    SAY_GROUND              = -1603024,
    EMOTE_DEEP_BREATH       = -1603025,
    SAY_FIRES_EXTINGUISH    = -1603026,
    EMOTE_HARPOON           = -1603353,
    EMOTE_GROUNDED          = -1603354,

	//razorscale air phase
	SPELL_FIREBALL				= 62796,
	SPELL_FIREBALL_H			= 63815,
	SPELL_WING_BUFFET			= 62666,
	SPELL_STUN					= 62794,
    SPELL_SUMMON_DWARF          = 62916,
	//both
	SPELL_BERSERK				= 47008,
	DEVOURING_FLAME_VISUAL		= 63236,
	SPELL_FLAME_BREATH			= 63317,
	SPELL_FLAME_BREATH_H		= 64021,
	//ground
	SPELL_FLAME_BUFFET			= 64016,
	SPELL_FLAME_BUFFET_H		= 64023,
	SPELL_FUSE_ARMOR			= 64771,

	//devouring flame target
	AURA_DEVOURING_FLAME		= 64709,
	AURA_DEVOURING_FLAME_H		= 64734,

    // mole machine
    NPC_MOLE_MACHINE            = 33245,    // used to summon adds in phase 1
    NPC_HARPOONS_DUMMY          = 33282,    // used to cast spells for harpoons
    SPELL_SUMMON_MOLE_MACHINE   = 73071,

    // harpoons
    SPELL_HARPOON_SHOT          = 63659,
    GO_HARPOON                  = 194543, // 41, 42, 194519

	//dark rune watcher
	SPELL_LIGHTNING_BOLT		= 63809,
	SPELL_LIGHTNING_BOLT_H		= 64696,
	SPELL_CHAIN_LIGHTNING		= 64758,
	SPELL_CHAIN_LIGHTNING_H		= 64759,

	//dark rune sentinel
	SPELL_BATTLE_SHOUT			= 46763,
	SPELL_BATTLE_SHOUT_H		= 64062,
	SPELL_WHIRLWIND				= 63808,

	//dark rune guardian
	SPELL_STORMSTRIKE			= 64757,

	//NPC ids
	MOB_DARK_RUNE_WATCHER		= 33453,
	MOB_DARK_RUNE_SENTINEL		= 33846,
	MOB_DARK_RUNE_GUARDIAN		= 33388, 
 
    NPC_EXP_ENGINEER            = 33287,

    ACHIEV_QUICK_SHAVE          = 2919,
    ACHIEV_QUICK_SHAVE_H        = 2921,
    ACHIEV_MEDIUM_RARE          = 2923,
    ACHIEV_MEDIUM_RARE_H        = 2924,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};

static LocationsXY PositionLoc[]=
{
    {621.633301f, -228.671371f, 391.180328f},//right
    {564.140198f, -222.049149f, 391.517212f},//left
    {591.629761f, -209.629761f, 392.629761f},//middle
    {587.629761f, -179.022522f, 391.625061f},//ground
    {587.629761f, -179.022522f, 435.415070f},//air
};

#define HOME_X                      587.546997f
#define HOME_Y                      -174.927002f
 
#define GOSSIP_START     "Bring Razorscale down!"

//expedition commander
// start the event
struct MANGOS_DLL_DECL npc_expedition_commanderAI : public ScriptedAI
{
    npc_expedition_commanderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bHasPlayerNear = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bHasPlayerNear;
    bool m_bIsIntro;
    uint64 m_uiPlayerGUID;
    uint32 m_uiSpeech_Timer;
    uint32 m_uiIntro_Phase;

    void Reset()
    {
        m_uiPlayerGUID      = 0;
        m_uiSpeech_Timer    = 3000;
        m_bIsIntro          = false;
        m_uiIntro_Phase     = 0;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasPlayerNear && m_creature->IsWithinDistInMap(pWho, 40.0f))
        {
            DoScriptText(SAY_INTRO, m_creature);
            m_bHasPlayerNear = true;
        }
    }

    void GetRazorDown()
    {
        if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_RAZORSCALE)))
        {
            pTemp->SetInCombatWithZone();
            if(Unit* pPlayer = m_creature->GetMap()->GetUnit( m_uiPlayerGUID))
            {
                pTemp->AddThreat(pPlayer,0.0f);
                pTemp->AI()->AttackStart(pPlayer);
            }
        }
    }

    void BeginRazorscaleEvent(Player* pPlayer)
    {
        m_uiPlayerGUID      = pPlayer->GetGUID();
        m_bIsIntro          = true;
        m_uiSpeech_Timer    = 3000;
        m_uiIntro_Phase     = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_bIsIntro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiIntro_Phase)
                {
                case 0:
                    if(Creature* pEngineer = GetClosestCreatureWithEntry(m_creature, NPC_EXP_ENGINEER, 50.0f))
                        DoScriptText(SAY_AGGRO1, pEngineer);
                            GetRazorDown();
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 1:
                    DoScriptText(SAY_AGGRO2, m_creature);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 2:
                    if(Creature* pEngineer = GetClosestCreatureWithEntry(m_creature, NPC_EXP_ENGINEER, 50.0f))
                        DoScriptText(SAY_AGGRO3, pEngineer);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 3:
                    m_bIsIntro = false;
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_expedition_commander(Creature* pCreature)
{
    return new npc_expedition_commanderAI(pCreature);
}

bool GossipHello_npc_expedition_commander(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(pInstance->GetData(TYPE_RAZORSCALE) != DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_expedition_commander(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        ((npc_expedition_commanderAI*)pCreature->AI())->BeginRazorscaleEvent(pPlayer);
    }

    return true;
}

// devouring_flame_target
struct MANGOS_DLL_DECL mob_devouring_flame_targetAI : public ScriptedAI
{
    mob_devouring_flame_targetAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiDeath_Timer;

    void Reset()
    {
        m_uiDeath_Timer = 25500;
        m_creature->SetDisplayId(11686);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature,  m_bIsRegularMode ? AURA_DEVOURING_FLAME : AURA_DEVOURING_FLAME_H);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDeath_Timer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDeath_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_devouring_flame_target(Creature* pCreature)
{
    return new mob_devouring_flame_targetAI(pCreature);
}

// dark rune watcher
struct MANGOS_DLL_DECL mob_dark_rune_watcherAI : public ScriptedAI
{
    mob_dark_rune_watcherAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(5000, 10000);
    }

	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSpell_Timer < diff)
        {
			switch(urand(0, 1))
            {
                case 0:
					DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LIGHTNING_BOLT : SPELL_LIGHTNING_BOLT_H);
				break;
				case 1:
					DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H);
				break;
			}
            m_uiSpell_Timer = urand(5000, 10000);
        }else m_uiSpell_Timer -= diff;        
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_watcher(Creature* pCreature)
{
    return new mob_dark_rune_watcherAI(pCreature);
}

// dark rune sentinel
struct MANGOS_DLL_DECL mob_dark_rune_sentinelAI : public ScriptedAI
{
    mob_dark_rune_sentinelAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 m_uiWhirl_Timer;
	uint32 m_uiShout_Timer;

    void Reset()
    {
		m_uiWhirl_Timer = 10000;
		m_uiShout_Timer = 2000;
    }

	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiWhirl_Timer < diff)
        {
			DoCast(m_creature, SPELL_WHIRLWIND);
            m_uiWhirl_Timer = urand(10000, 15000);
        }else m_uiWhirl_Timer -= diff;

		if (m_uiShout_Timer < diff)
        {
			DoCast(m_creature, m_bIsRegularMode ? SPELL_BATTLE_SHOUT : SPELL_BATTLE_SHOUT_H);
            m_uiShout_Timer = 30000;
        }else m_uiShout_Timer -= diff;
		
		DoMeleeAttackIfReady();
	}

};

CreatureAI* GetAI_mob_dark_rune_sentinel(Creature* pCreature)
{
    return new mob_dark_rune_sentinelAI(pCreature);
}

// dark rune guardian
struct MANGOS_DLL_DECL mob_dark_rune_guardianAI : public ScriptedAI
{
    mob_dark_rune_guardianAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 m_uiStormstrike_Timer;

    void Reset()
    {
		m_uiStormstrike_Timer = 10000;
    }

	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiStormstrike_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_STORMSTRIKE);
            m_uiStormstrike_Timer = urand(7000, 13000);
        }else m_uiStormstrike_Timer -= diff;
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_guardian(Creature* pCreature)
{
    return new mob_dark_rune_guardianAI(pCreature);
}

/// mole machine
// used to summon dwarfes
struct MANGOS_DLL_DECL mob_mole_machineAI : public ScriptedAI
{
    mob_mole_machineAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiSummonTimer;
    bool m_bIsSentinel;

    void Reset()
    {
		m_uiSummonTimer     = 8000;
        m_bIsSentinel       = false;
        DoCast(m_creature, SPELL_SUMMON_MOLE_MACHINE);
    }

	void UpdateAI(const uint32 diff)
    {
		if (m_uiSummonTimer < diff)
        {
			// summon 2 dwarfes
            if(!m_bIsSentinel)
            {
                if (Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_WATCHER, m_creature->GetPositionX() + 5, m_creature->GetPositionY() + 5, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                {
                    pTemp->SetInCombatWithZone();
                    pTemp->GetMotionMaster()->MovePoint(0, HOME_X, HOME_Y, m_creature->GetPositionZ());
                }
                if (Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_GUARDIAN, m_creature->GetPositionX() - 5, m_creature->GetPositionY() - 5, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                {
                    pTemp->SetInCombatWithZone();
                    pTemp->GetMotionMaster()->MovePoint(0, HOME_X, HOME_Y, m_creature->GetPositionZ());
                }
            }
			// summon 1 sentinel
            else
            {
                if (Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_SENTINEL, m_creature->GetPositionX() - 5, m_creature->GetPositionY() - 5, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                {
                    pTemp->SetInCombatWithZone();
                    pTemp->GetMotionMaster()->MovePoint(0, HOME_X, HOME_Y, m_creature->GetPositionZ());
                }
            }
            m_uiSummonTimer = 60000;
        }else m_uiSummonTimer -= diff;
	}
};

CreatureAI* GetAI_mob_mole_machine(Creature* pCreature)
{
    return new mob_mole_machineAI(pCreature);
}

//razorscale
struct MANGOS_DLL_DECL boss_razorscaleAI : public ScriptedAI
{
    boss_razorscaleAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 m_uiFireball_Timer;
	uint32 m_uiDevouring_Flame_Timer;
	uint32 m_uiFlame_Buffet_Timer;
	uint32 m_uiFuse_Armor_Timer;
	uint32 m_uiFlame_Breath_Timer;
	uint32 m_uiWave1_spawn;     //right side, 1 of each
	uint32 m_uiWave2_spawn;     //left side, 1 of each
	uint32 m_uiWave3_spawn;     // big guy
	uint32 m_uiBerserk_Timer;
	uint32 m_uiGrounded_Timer;  // 8 secs after ground fase is over, adds come
	uint32 m_uiGround_Cast;
	uint32 m_uiGround_Knockback;
    uint32 m_uiRepairHarpoonTimer;
    uint8 m_uiHarpoonsRepaired;
    uint8 m_uiMaxHarpoons;
    uint64 m_uiHarpoonsGUID[4];
	uint32 m_uiTimetoground;
	uint32 m_uiStun_Timer;
	bool m_bAirphase;
	bool m_bIsGrounded;
	bool m_bHasBerserk;
    bool m_bKnockback;
    uint8 m_uiHarpoonsUsed;
    uint8 m_uiFlyNo;

    std::list<GameObject*> lHarpoons;

    void Reset()
    {
		m_uiFireball_Timer  = 10000;    // 10 secs for the first, fckin spam after that ~2secs
		m_uiDevouring_Flame_Timer = 18000; // 18 secs first, 12 seconds after
		m_uiWave1_spawn     = urand(5000, 10000);    
		m_uiWave2_spawn     = urand(5000, 10000);  
		m_uiWave3_spawn     = urand(5000, 10000);
		m_uiBerserk_Timer   = 600000;   // 10 min
		m_uiTimetoground    = 80000;
        m_uiRepairHarpoonTimer = 53000;
        m_uiHarpoonsRepaired = 0;
        m_uiMaxHarpoons     = m_bIsRegularMode ? 2 : 4;
        for(int i = 0; i < m_uiMaxHarpoons; i++)
            m_uiHarpoonsGUID[i] = 0;
		m_bAirphase         = false;
		m_bIsGrounded       = false;
		m_bHasBerserk       = false;
        m_bKnockback        = false;
        m_uiFlyNo           = 0;
        m_uiHarpoonsUsed    = 0;
        lHarpoons.clear();
        
        if(m_creature->GetPositionZ() < 435.0f)
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, 0.0f);
		    m_creature->SendMonsterMove(PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
        }

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        m_creature->GetMotionMaster()->MoveConfused();

        if (Creature* pCommander = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_COMMANDER)))
            pCommander->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

	void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, DONE);

        if (m_uiFlyNo < 2)
        {
            // hacky way to complete achievements; use only if you have this function
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_QUICK_SHAVE : ACHIEV_QUICK_SHAVE_H);
        }
    }

	void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, IN_PROGRESS);

        GetGameObjectListWithEntryInGrid(lHarpoons, m_creature, GO_BROKEN_HARPOON, 200.0f);
        if (!lHarpoons.empty())
        {
            int i = 0;
            for(std::list<GameObject*>::iterator iter = lHarpoons.begin(); iter != lHarpoons.end(); ++iter)
            {
                if ((*iter))
                {
                    m_uiHarpoonsGUID[i] = (*iter)->GetGUID();
                    i += 1;
                }          
            }
        }

		m_bAirphase = true;
		SetCombatMovement(false);
		m_creature->GetMotionMaster()->MoveIdle();
        m_creature->GetMap()->CreatureRelocation(m_creature, PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, 0.0f);
		m_creature->SendMonsterMove(PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

	void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, NOT_STARTED);

        BreakHarpoons();        
    }

    void BreakHarpoons()
    {
        // reset harpoons
        if (!lHarpoons.empty())
        {
            for(std::list<GameObject*>::iterator iter = lHarpoons.begin(); iter != lHarpoons.end(); ++iter)
            {
                if ((*iter))
                {
                    (*iter)->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                    //(*iter)->SetUInt32Value(GAMEOBJECT_DISPLAYID, 8631);
                }
            }
        }
    }

	void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // AIR PHASE
        // air position check (sometimes it falls to the ground like a rock
        if(m_creature->GetPositionZ() < 430.0f && m_bAirphase && !m_bIsGrounded)
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, 0.0f);
		    m_creature->SendMonsterMove(PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
        }

        // air spells
		if (m_uiFireball_Timer < uiDiff && m_bAirphase && !m_bIsGrounded)
        {
			if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(target, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H);
            m_uiFireball_Timer = 2000;
        }else m_uiFireball_Timer -= uiDiff;   

		if (m_uiDevouring_Flame_Timer < uiDiff && !m_bIsGrounded)
        {
            if (m_bAirphase)
            {
			    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				    DoCast(target, DEVOURING_FLAME_VISUAL);
            }
            else
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
				    DoCast(target, DEVOURING_FLAME_VISUAL);
            }
            m_uiDevouring_Flame_Timer = 12000;
        }else m_uiDevouring_Flame_Timer -= uiDiff;  

        // repair harpoons
        if (m_uiRepairHarpoonTimer < uiDiff && m_bAirphase && !m_bIsGrounded && m_uiHarpoonsRepaired <= m_uiMaxHarpoons)
        {
            if(GameObject* pHarpoon = m_pInstance->instance->GetGameObject(m_uiHarpoonsGUID[m_uiHarpoonsRepaired]))
            {
                pHarpoon->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                //pHarpoon->SetUInt32Value(GAMEOBJECT_DISPLAYID, 8245);
                m_uiHarpoonsRepaired += 1;
            }
            DoScriptText(EMOTE_HARPOON, m_creature);
            m_uiRepairHarpoonTimer = 20000;
        }
        else m_uiRepairHarpoonTimer -= uiDiff;

        // ground adds
		if (m_uiWave1_spawn < uiDiff && m_bAirphase && !m_bIsGrounded)
        {
            m_creature->SummonCreature(NPC_MOLE_MACHINE, PositionLoc[0].x, PositionLoc[0].y, PositionLoc[0].z, 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_uiWave1_spawn = urand(40000, 50000);
        }else m_uiWave1_spawn -= uiDiff;

		if (m_uiWave2_spawn < uiDiff && m_bAirphase && !m_bIsGrounded)
        {
			m_creature->SummonCreature(NPC_MOLE_MACHINE, PositionLoc[1].x, PositionLoc[1].y, PositionLoc[1].z, 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_uiWave2_spawn = urand(40000, 50000);
        }else m_uiWave2_spawn -= uiDiff;

		if (m_uiWave3_spawn < uiDiff && m_bAirphase && !m_bIsGrounded)
        {
			switch(urand(0, 2)) //33% chance of spawning
            {
                case 0:
				break;
				case 1:
					if(Creature* pTemp = m_creature->SummonCreature(NPC_MOLE_MACHINE, PositionLoc[2].x, PositionLoc[2].y, PositionLoc[2].z, 0, TEMPSUMMON_TIMED_DESPAWN, 15000))
                        ((mob_mole_machineAI*)pTemp->AI())->m_bIsSentinel = true;
				break;
				case 2:
				break;
			}
            m_uiWave3_spawn = urand(40000, 50000);
        }else m_uiWave3_spawn -= uiDiff;

        // berserk
		if (m_uiBerserk_Timer < uiDiff && !m_bHasBerserk)
        {
			DoCast(m_creature, SPELL_BERSERK);
            m_bHasBerserk = true;
        }else m_uiBerserk_Timer -= uiDiff;

		if (m_uiHarpoonsUsed == m_uiMaxHarpoons && m_bAirphase)
        {
            if(Creature* pCommander = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_COMMANDER)))
                DoScriptText(SAY_GROUND, pCommander);
			m_creature->GetMap()->CreatureRelocation(m_creature, PositionLoc[3].x, PositionLoc[3].y, PositionLoc[3].z, 1.5);
            m_creature->SendMonsterMove(PositionLoc[3].x, PositionLoc[3].y, PositionLoc[3].z, SPLINETYPE_FACINGSPOT, m_creature->GetSplineFlags(), 1);
			// timers
            m_uiHarpoonsUsed    = 0;
            m_bIsGrounded       = true;
			m_uiStun_Timer      = 2000;
			m_uiGround_Cast     = 35000;
			m_uiGrounded_Timer  = 45000;
            // make boss land
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        }else m_uiTimetoground -= uiDiff;

		if (m_uiStun_Timer < uiDiff && m_bIsGrounded)
		{
			DoCast(m_creature, SPELL_STUN);
			m_uiStun_Timer = 60000;
		}else m_uiStun_Timer -= uiDiff;

		if (m_uiGround_Cast < uiDiff && m_bIsGrounded)
		{
            if (Creature* pCommander = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_COMMANDER)))
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pCommander->GetGUID());
			m_creature->RemoveAurasDueToSpell(SPELL_STUN);
            DoScriptText(EMOTE_DEEP_BREATH, m_creature);
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H);
            m_uiGround_Cast         = 15000;
            m_uiGround_Knockback    = 7000;
		}else m_uiGround_Cast -= uiDiff;

		if (m_uiGround_Knockback < uiDiff && m_bIsGrounded)
		{
            m_creature->CastStop();
            DoCast(m_creature, SPELL_WING_BUFFET);
            m_uiGround_Knockback = 15000;
            m_uiGrounded_Timer  = 3000; 
		}else m_uiGround_Knockback -= uiDiff;

		if (m_uiGrounded_Timer < uiDiff && m_bIsGrounded)
		{
			m_creature->GetMap()->CreatureRelocation(m_creature, PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, 0.0f);
            m_creature->SendMonsterMove(PositionLoc[4].x, PositionLoc[4].y, PositionLoc[4].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
			
            m_bIsGrounded               = false;
			m_uiFireball_Timer          = 10000;
			m_uiDevouring_Flame_Timer   = 18000;
			m_uiWave1_spawn             = urand(5000, 10000);
			m_uiWave2_spawn             = urand(5000, 10000);
			m_uiWave3_spawn             = urand(5000, 10000);
            m_uiRepairHarpoonTimer      = 50000;
            m_uiHarpoonsRepaired        = 0;
            BreakHarpoons();
            // make boss fly
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
            // achiev counter
            m_uiFlyNo += 1;
        }else m_uiGrounded_Timer -= uiDiff;

        // make boss land at 50% hp
        if (m_bAirphase && m_creature->GetHealthPercent() < 50)
        {
            if (m_creature->HasAura(SPELL_STUN))
                m_creature->RemoveAurasDueToSpell(SPELL_STUN);

            DoScriptText(EMOTE_GROUNDED, m_creature);
            m_uiGround_Knockback        = m_bIsGrounded ? 0 : 3000;
            m_bAirphase = false;
            m_bIsGrounded = false;
            m_uiDevouring_Flame_Timer   = 12000;
            m_uiFlame_Buffet_Timer      = 10000; //every 10 secs
            m_uiFuse_Armor_Timer        = 13000; //every ~13
            m_uiFlame_Breath_Timer      = 6000;  //every 14
            SetCombatMovement(true);

            // make boss land
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
         }

        // LAND PHASE
        // knockback at the beginning at the land phase
        if (m_uiGround_Knockback < uiDiff && !m_bKnockback && !m_bAirphase)
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_WING_BUFFET);
            m_bKnockback = true;
        }else m_uiGround_Knockback -= uiDiff;

        if (m_uiFuse_Armor_Timer < uiDiff && !m_bAirphase)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
               DoCast(pTarget, SPELL_FUSE_ARMOR);
            m_uiFuse_Armor_Timer = 13000;
        } else m_uiFuse_Armor_Timer -= uiDiff;

		if (m_uiFlame_Buffet_Timer < uiDiff && !m_bAirphase)
		{
			DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_BUFFET : SPELL_FLAME_BUFFET_H);
			m_uiFlame_Buffet_Timer = 13000;
		}else m_uiFlame_Buffet_Timer -= uiDiff;

		if (m_uiFlame_Breath_Timer < uiDiff && !m_bAirphase)
		{
            DoScriptText(EMOTE_DEEP_BREATH, m_creature);
			DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H);
			m_uiFlame_Breath_Timer = 14000;
		}else m_uiFlame_Breath_Timer -= uiDiff;

		if (!m_bAirphase && !m_bIsGrounded)
			DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 100)
            EnterEvadeMode();
	}
};

CreatureAI* GetAI_boss_razorscale(Creature* pCreature)
{
    return new boss_razorscaleAI(pCreature);
}

bool GOUse_go_broken_harpoon(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    if (Creature* pRazor = pGo->GetMap()->GetCreature(pInstance->GetData64(NPC_RAZORSCALE)))
        ((boss_razorscaleAI*)pRazor->AI())->m_uiHarpoonsUsed += 1;

    return false;
}

void AddSC_boss_razorscale()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_razorscale";
    NewScript->GetAI = GetAI_boss_razorscale;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_devouring_flame_target";
    NewScript->GetAI = &GetAI_mob_devouring_flame_target;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_dark_rune_watcher";
    NewScript->GetAI = &GetAI_mob_dark_rune_watcher;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_dark_rune_sentinel";
    NewScript->GetAI = &GetAI_mob_dark_rune_sentinel;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_dark_rune_guardian";
    NewScript->GetAI = &GetAI_mob_dark_rune_guardian;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_mole_machine";
    NewScript->GetAI = &GetAI_mob_mole_machine;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_expedition_commander";
    NewScript->GetAI = &GetAI_npc_expedition_commander;
    NewScript->pGossipHello = &GossipHello_npc_expedition_commander;
    NewScript->pGossipSelect = &GossipSelect_npc_expedition_commander;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "go_broken_harpoon";
    NewScript->pGOUse = &GOUse_go_broken_harpoon;
    NewScript->RegisterSelf();
}