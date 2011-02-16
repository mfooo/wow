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
SDName: Zulfarrak
SD%Complete: 50
SDComment: Consider it temporary, no instance script made for this instance yet.
SDCategory: Zul'Farrak
EndScriptData */

/* ContentData
npc_sergeant_bly
npc_weegli_blastfuse
go_shallow_grave
boss_antusul
npc_servant
go_gong_of_ghazrilla
boss_ghazrilla
boss_velratha
boss_ruzzlu
boss_ukorz
EndContentData */

#include "precompiled.h"

/*######
## npc_sergeant_bly
######*/

enum
{
    FACTION_HOSTILE   = 14,
    FACTION_FRIENDLY  = 35,

    SPELL_SHIELD_BASH = 11972,
    SPELL_REVENGE     = 12170
};
#define GOSSIP_BLY                  "Jetzt reichts's! Ich habe es satt, Euch zu helfen! Es wird Zeit, dass wir die Dinge auf dem Schlachtfeld regeln."

//find Bly's gossip menus

struct MANGOS_DLL_DECL npc_sergeant_blyAI : public ScriptedAI
{
    npc_sergeant_blyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        //m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    //ScriptedInstance* m_pInstance;

    uint32 m_uiShieldBashTimer;
    uint32 m_uiRevengeTimer;                                //this is wrong, spell should never be used unless m_creature->getVictim() dodge, parry or block attack. Mangos support required.

    void Reset()
    {
        m_uiShieldBashTimer = 5000;
        m_uiRevengeTimer    = 8000;

        m_creature->setFaction(FACTION_FRIENDLY);

        /*if (m_pInstance)
            m_pInstance->SetData(0, NOT_STARTED);*/
    }

    void Aggro(Unit* pWho)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, IN_PROGRESS);*/
    }

    void JustDied(Unit* pVictim)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, DONE);*/
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShieldBashTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SHIELD_BASH);
            m_uiShieldBashTimer = 15000;
        }
        else
            m_uiShieldBashTimer -= uiDiff;

        if (m_uiRevengeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_REVENGE);
            m_uiRevengeTimer = 10000;
        }
        else
            m_uiRevengeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_sergeant_bly(Creature* pCreature)
{
    return new npc_sergeant_blyAI(pCreature);
}

bool GossipHello_npc_sergeant_bly(Player* pPlayer, Creature* pCreature)
{
    /*if (m_pInstance->GetData(0) == DONE)
    {*/
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_BLY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(1517, pCreature->GetGUID());
    /*}
    else if (m_pInstance->GetData(0) == IN_PROGRESS)
        pPlayer->SEND_GOSSIP_MENU(1516, pCreature->GetGUID());
    else
        pPlayer->SEND_GOSSIP_MENU(1515, pCreature->GetGUID());*/

    return true;
}

bool GossipSelect_npc_sergeant_bly(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->setFaction(FACTION_HOSTILE);
        ((npc_sergeant_blyAI*)pCreature->AI())->AttackStart(pPlayer);
    }
    return true;
}

/*######
## npc_weegli_blastfuse
######*/

enum
{
    SPELL_BOMB             = 8858,
    SPELL_GOBLIN_LAND_MINE = 21688,
    SPELL_SHOOT            = 6660,
    SPELL_WEEGLIS_BARREL   = 10772
};

#define GOSSIP_WEEGLI               "Werdet Ihr jetzt die Tür aufsprengen?"

struct MANGOS_DLL_DECL npc_weegli_blastfuseAI : public ScriptedAI
{
    npc_weegli_blastfuseAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        //m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    //ScriptedInstance* m_pInstance;

    void Reset()
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, NOT_STARTED);*/
    }

    void Aggro(Unit* pWho)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, IN_PROGRESS);*/
    }

    void JustDied(Unit* pVictim)
    {
        /*if (m_pInstance)
            m_pInstance->SetData(0, DONE);*/
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_weegli_blastfuse(Creature* pCreature)
{
    return new npc_weegli_blastfuseAI(pCreature);
}

bool GossipHello_npc_weegli_blastfuse(Player* pPlayer, Creature* pCreature)
{
    //event not implemented yet, this is only placeholder for future developement
    /*if (m_pInstance->GetData(0) == DONE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_WEEGLI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(1514, pCreature->GetGUID());//if event can proceed to end
    }
    else if (m_pInstance->GetData(0) == IN_PROGRESS)
        pPlayer->SEND_GOSSIP_MENU(1513, pCreature->GetGUID());//if event are in progress
    else*/
    pPlayer->SEND_GOSSIP_MENU(1511, pCreature->GetGUID());   //if event not started
    return true;
}

bool GossipSelect_npc_weegli_blastfuse(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        //here we make him run to door, set the charge and run away off to nowhere
    }
    return true;
}

/*######
## go_shallow_grave
######*/

enum
{
    ZOMBIE = 7286,
    DEAD_HERO = 7276,
    ZOMBIE_CHANCE = 65,
    DEAD_HERO_CHANCE = 10
};

bool GOUse_go_shallow_grave(Player* pPlayer, GameObject* pGo)
{
    // randomly summon a zombie or dead hero the first time a grave is used
    if (pGo->GetUseCount() == 0)
    {
        uint32 randomchance = urand(0,100);
        if (randomchance < ZOMBIE_CHANCE)
            pGo->SummonCreature(ZOMBIE, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
        else if ((randomchance-ZOMBIE_CHANCE) < DEAD_HERO_CHANCE)
            pGo->SummonCreature(DEAD_HERO, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
    }
    pGo->AddUse();
    return false;
}

/*######
## boss_antusul
######*/

#define SPELL_SUMMON				11894
#define SPELL_CHAIN_LIGHTNING		2860
#define SPELL_EARTH_SHOCK			10413
#define SPELL_FIRE_NOVA				11314

#define SPELL_TOTEM_EARTHGRAB		8376
#define SPELL_TOTEM_HEALING_WARD	11899

#define SPELL_PETRIFY				11020

struct MANGOS_DLL_DECL boss_antusulAI : public ScriptedAI
{
    boss_antusulAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 Summon_Timer;
	uint32 ChainLightning_Timer;
	uint32 EarthShock_Timer;
	uint32 FireNova_Timer;
	uint32 TotemEarthgrab_Timer;
	uint32 TotemHealing_Timer;
	uint32 Rand;
    uint32 RandX;
    uint32 RandY;

	Creature* Summoned;

    void Reset()
    {
		Summon_Timer = 15000;
		ChainLightning_Timer = 11000;
		EarthShock_Timer = 20000;
		FireNova_Timer = 18000;
		TotemEarthgrab_Timer = 30000;
		TotemHealing_Timer = 60000;
	}

    void SummonHounds(Unit* victim)
    {
        Rand = rand()%10;

        switch(urand(0, 1))
        {	
            case 0: RandX = 0 - Rand; break;
            case 1: RandX = 0 + Rand; break;
        }

        Rand = 0;
        Rand = rand()%10;

        switch(urand(0, 1))
        {
            case 0: RandY = 0 - Rand; break;
            case 1: RandY = 0 + Rand; break;
        }

        Rand = 0;
        Summoned = DoSpawnCreature(8156, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000);    // Servant of Antu'sul

        if (Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (ChainLightning_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CHAIN_LIGHTNING);
            ChainLightning_Timer = 11000;
        }else ChainLightning_Timer -= diff;

        if (EarthShock_Timer < diff)
        {
			DoCast(m_creature->getVictim(),SPELL_EARTH_SHOCK);
            EarthShock_Timer = 20000;
        }else EarthShock_Timer -= diff;

        if (FireNova_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_FIRE_NOVA);
            FireNova_Timer = 18000;
        }else FireNova_Timer -= diff;

        if (TotemEarthgrab_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_TOTEM_EARTHGRAB);
            TotemEarthgrab_Timer = 60000;
        }else TotemEarthgrab_Timer -= diff;

        if (TotemHealing_Timer < diff)
        {
			DoCast(m_creature, SPELL_TOTEM_HEALING_WARD);
            TotemHealing_Timer = 60000;
        }else TotemHealing_Timer -= diff;

        if (Summon_Timer < diff)
        {
		    SummonHounds(m_creature->getVictim());
			SummonHounds(m_creature->getVictim());
            Summon_Timer = 15000;
        }else Summon_Timer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_antusul(Creature* pCreature)
{
    return new boss_antusulAI(pCreature);
}

/*######
## npc_servant
######*/

struct MANGOS_DLL_DECL npc_servantAI : public ScriptedAI
{
    npc_servantAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 Petrify_Timer;

    void Reset()
    {
		Petrify_Timer = 15000;
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Petrify_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_PETRIFY);
            Petrify_Timer = 11000;
        }else Petrify_Timer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_servant(Creature* pCreature)
{
    return new npc_servantAI(pCreature);
}

/*######
## go_gong_of_ghazrilla
######*/

// TODO: Gong should be non selectable after using it
bool ZF_Gong = true;

bool GOUse_go_gong_of_ghazrilla(Player* pPlayer, GameObject* pGo)
{
	if (ZF_Gong)
	{
		pGo->SummonCreature(7273, 1663.542358f, 1186.532f, 6.469f, 0.785f, TEMPSUMMON_TIMED_DESPAWN, 180000);
		ZF_Gong = false;
	}
    
	return false;
}

/*######
## boss_ghazrilla
######*/

#define SPELL_ICICLE			11131
#define	SPELL_FREEZE_SOLID		11836
#define	SPELL_GHAZRILLA_SLAM	11902

struct MANGOS_DLL_DECL boss_ghazrillaAI : public ScriptedAI
{
    boss_ghazrillaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 Icicle_Timer;
	uint32 FreezeSolid_Timer;
	uint32 GhazrillaSlam_Timer;

	void Reset()
	{
		Icicle_Timer = 8000;
		FreezeSolid_Timer = 13000;
		GhazrillaSlam_Timer = 18000;
		ZF_Gong = true;
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Icicle_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_ICICLE);
            Icicle_Timer = 8000;
        }else Icicle_Timer -= diff;

		if (GhazrillaSlam_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_GHAZRILLA_SLAM);
            GhazrillaSlam_Timer = 18000;
        }else GhazrillaSlam_Timer -= diff;

		if (FreezeSolid_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				 DoCast(m_creature->getVictim(), SPELL_FREEZE_SOLID);
            FreezeSolid_Timer = 13000;
        }else FreezeSolid_Timer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ghazrilla(Creature* pCreature)
{
    return new boss_ghazrillaAI(pCreature);
}

/*######
## boss_velratha
######*/

#define SPELL_HEALING_WAVE			12491
#define	SPELL_SHADOW_BOLT			12739
#define SPELL_SHADOW_BOLT_VOLLEY	15245
#define	SPELL_WARD					11086

struct MANGOS_DLL_DECL boss_velrathaAI : public ScriptedAI
{
    boss_velrathaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 HealingWave_Timer;
	uint32 ShadowBolt_Timer;
	uint32 ShadowBoltVolley_Timer;
	uint32 Ward_Timer;

	void Reset()
	{
		HealingWave_Timer = 20000;
		ShadowBolt_Timer = 8000;
		ShadowBoltVolley_Timer = 12000;
		Ward_Timer = 35000;
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (HealingWave_Timer < diff)
        {
			DoCast(m_creature, SPELL_HEALING_WAVE);
            HealingWave_Timer = 20000;
        }else HealingWave_Timer -= diff;

		if (ShadowBolt_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT);
            ShadowBolt_Timer = 18000;
        }else ShadowBolt_Timer -= diff;

		if (ShadowBoltVolley_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				 DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
            ShadowBoltVolley_Timer = 12000;
        }else ShadowBoltVolley_Timer -= diff;

        if (Ward_Timer < diff)
        {
			DoCast(m_creature, SPELL_WARD);
            Ward_Timer = 30000;
        }else Ward_Timer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_velratha(Creature* pCreature)
{
    return new boss_velrathaAI(pCreature);
}

/*######
## boss_ruzzlu
######*/

#define SPELL_BESERK_STANCE		7366
#define	SPELL_CLEAVE			15496
#define	SPELL_EXECUTE			7160

struct MANGOS_DLL_DECL boss_ruzzluAI : public ScriptedAI
{
    boss_ruzzluAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 Cleave_Timer;
	uint32 Execute_Timer;

	bool m_bBerserk;

	void Reset()
	{
		Cleave_Timer = 10000;
		Execute_Timer = 35000;
		m_bBerserk = false;
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Cleave_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 10000;
        }else Cleave_Timer -= diff;

		if (Execute_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_EXECUTE);
            Execute_Timer = 35000;
        }else Execute_Timer -= diff;

	    if (!m_bBerserk && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10)
        {
            DoCast(m_creature, SPELL_BESERK_STANCE);
            m_bBerserk = true;
        }else;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ruzzlu(Creature* pCreature)
{
    return new boss_ruzzluAI(pCreature);
}

/*######
## boss_ukorz
######*/

#define SPELL_CLEAVE			15496
#define SPELL_WIDE_SLASH		11837
#define SPELL_FRENCY			8289
#define SPELL_BESERK_STANCE2	7366

struct MANGOS_DLL_DECL boss_ukorzAI : public ScriptedAI
{
    boss_ukorzAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	uint32 Cleave_Timer;
	uint32 WideSlash_Timer;

	bool m_bBerserk2;
	bool m_bFrency;

	void Reset()
	{
		Cleave_Timer = 10000;
		WideSlash_Timer = 18000;

		m_bBerserk2 = false;
		m_bFrency = false;
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Cleave_Timer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 10000;
        }else Cleave_Timer -= diff;

		if (WideSlash_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCast(m_creature->getVictim(), SPELL_WIDE_SLASH);
            WideSlash_Timer = 18000;
        }else WideSlash_Timer -= diff;

	    if (!m_bBerserk2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 60)
        {
            DoCast(m_creature, SPELL_BESERK_STANCE2);
            m_bBerserk2 = true;
        }else;

		if (!m_bFrency && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10)
        {
            DoCast(m_creature, SPELL_FRENCY);
            m_bFrency = true;
        }else;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ukorz(Creature* pCreature)
{
    return new boss_ukorzAI(pCreature);
}


void AddSC_zulfarrak()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_sergeant_bly";
    newscript->GetAI = &GetAI_npc_sergeant_bly;
    newscript->pGossipHello =  &GossipHello_npc_sergeant_bly;
    newscript->pGossipSelect = &GossipSelect_npc_sergeant_bly;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_weegli_blastfuse";
    newscript->GetAI = &GetAI_npc_weegli_blastfuse;
    newscript->pGossipHello =  &GossipHello_npc_weegli_blastfuse;
    newscript->pGossipSelect = &GossipSelect_npc_weegli_blastfuse;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_shallow_grave";
    newscript->pGOUse = &GOUse_go_shallow_grave;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_antusul";
    newscript->GetAI = &GetAI_boss_antusul;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_servant";
    newscript->GetAI = &GetAI_npc_servant;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_gong_of_ghazrilla";
    newscript->pGOUse = &GOUse_go_gong_of_ghazrilla;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_ghazrilla";
    newscript->GetAI = &GetAI_boss_ghazrilla;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "boss_velratha";
    newscript->GetAI = &GetAI_boss_velratha;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "boss_ruzzlu";
    newscript->GetAI = &GetAI_boss_ruzzlu;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "boss_ukorz";
    newscript->GetAI = &GetAI_boss_ukorz;
    newscript->RegisterSelf();
}
