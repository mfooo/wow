/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Alythess
SD%Complete: 90%
SDComment: 
SDAuthor: [DEV]kelthuzad (original script by Aramilpl && ScrappyDoo (c) Andeeria)
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum EredarTwins
{
    YELL_SACROLASH_AGGRO_1          = -1940000,
	YELL_SACROLASH_AGGRO_2			= -1940001,
    YELL_SACROLASH_AGGRO_3		    = -1940002,
    YELL_SACROLASH_AGGRO_4          = -1940003,
    YELL_ALYTHESS_AGGRO_1	        = -1940004,
    YELL_ALYTHESS_AGGRO_2			= -1940005,
	YELL_ALYTHESS_AGGRO_3			= -1940006,
	YELL_ALYTHESS_AGGRO_4			= -1940007,

	YELL_SACROLASH_DEATH			= -1940008,
	YELL_SACROLASH_EMPOWER			= -1940009,
	YELL_SACROLASH_NOVA				= -1940010,
	YELL_SACROLASH_BERSERK			= -1940011,
	YELL_SACROLASH_KILL_1			= -1940012,
	YELL_SACROLASH_KILL_2			= -1940013,

	YELL_ALYTHESS_CONFLAGRATE		= -1940014,
	YELL_ALYTHESS_EMPOWER			= -1940015,
	YELL_ALYTHESS_KILL_1			= -1940016,
	YELL_ALYTHESS_KILL_2			= -1940017,
	YELL_ALYTHESS_BERSERK			= -1940018,
	YELL_ALYTHESS_DEATH				= -1940019,

};

enum Sounds
{
	SOUND_INTRO						= 12484,

	SOUND_SAC_SHADOW_NOVA			= 12485,
	SOUND_SAC_EMPOWER				= 12488,
	SOUND_SAC_KILL_1				= 12486,
	SOUND_SAC_KILL_2				= 12487,

	SOUND_ALY_CANFLAGRATION			= 12489,
	SOUND_ALY_EMPOWER				= 12492,
	SOUND_ALY_KILL_1				= 12490,
	SOUND_ALY_KILL_2				= 12491,
	SOUND_ALY_DEAD					= 12494,
	SOUND_ALY_BERSERK				= 12493,
};

enum Spells
{
    //Lady Sacrolash
    SPELL_DARK_TOUCHED      =   45347,
    SPELL_SHADOW_BLADES     =   45248, //10 secs
    SPELL_DARK_STRIKE       =   45271,
    SPELL_SHADOW_NOVA       =   45329, //30-35 secs
    SPELL_CONFOUNDING_BLOW  =   45256, //25 secs

    //Shadow Image spells
    SPELL_SHADOWFURY        =   45270,
    SPELL_IMAGE_VISUAL      =   45263,

    //Misc spells
    SPELL_TWINS_ENRAGE      =   46587, 
    SPELL_EMPOWER           =   45366,
    SPELL_DARK_FLAME        =   45345,

    //Grand Warlock Alythess spells
    SPELL_PYROGENICS        =   45230, //15secs
    SPELL_FLAME_TOUCHED     =   45348,
    SPELL_CONFLAGRATION     =   45342, //30-35 secs
    SPELL_BLAZE             =   45235, //
	SPELL_BLAZE_FIREPATCH	=	45236,
    SPELL_FLAME_SEAR        =   46771,

    //other spells
    SPELL_BANISH            = 44836,
};

enum Creatures
{
    MOB_SHADOW_IMAGE        =   25214,
};

bool bAlythessDead;
bool bSacrolashDead;

uint32 uiIntroTimer;
uint8 uiIntroCount;

//##############################################
//################## ALYTHESS ##################
//##############################################
struct MANGOS_DLL_DECL boss_alythessAI : public ScriptedAI
{
    boss_alythessAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

	ScriptedInstance* pInstance;  
	uint32 m_uiPyrogenicsTimer;
	uint32 m_uiEnrageTimer;
	bool m_bIsEnraged;
    bool m_bIsBanished;
	//conflagration
	uint32 m_uiConflagrationTimer;
	uint32 m_uiConfTimer;
	uint64 m_uiConfTargetGUID;
	bool m_bIsConf;
	bool m_bIsConfDone;
	//shadow nova
	uint32 m_uiShadowNovaTimer;
	uint32 m_uiNovaTimer;
	uint64 m_uiNovaTargetGUID;
	bool m_bIsNova;
	bool m_bIsNovaDone;
	//blaze
	uint32 m_uiBlazeTimer;
	uint32 m_uiBlaTimer;
	uint64 m_uiBlazeTargetGUID;
	bool m_bIsBlaze;
	bool m_bIsBlazeDone;
	//flame sear
	uint32 m_uiFlameSearTimer;

    void Reset()
	{	
        m_bIsBanished = false;
		m_bIsEnraged = false;
		m_uiPyrogenicsTimer = 100; 
		m_uiEnrageTimer = 360000; //berserk after 6 minutes
		//conflagration
		m_uiConflagrationTimer = 25000 + rand()%15000;
        m_uiConfTimer = 3600;
        m_uiConfTargetGUID = 0;
		m_bIsConf = false;
		m_bIsConfDone = false;
		//shadow nova
		m_uiShadowNovaTimer = 15000;
		m_uiNovaTimer = 3600;
		m_uiNovaTargetGUID = 0;
		m_bIsNova = false;
		m_bIsNovaDone = false;
		//blaze
		m_uiBlazeTimer = 1000; 
		m_uiBlaTimer = 2600;
		m_uiBlazeTargetGUID = 0;
		m_bIsBlaze = false;
		m_bIsBlazeDone = false;
		//flame sear
		m_uiFlameSearTimer = 15000; 

		uiIntroTimer = 0;
		uiIntroCount = 9;

		bAlythessDead = false;
		
        if (pInstance)
            pInstance->SetData(TYPE_EREDAR_TWINS, NOT_STARTED);	
	}

	
	void Aggro(Unit *who)
	{
        if(pInstance)
			if(pInstance->GetData(TYPE_EREDAR_TWINS)==NOT_STARTED)
			{
				if(uiIntroCount==9)
				{
					uiIntroTimer = 0;
					uiIntroCount = 1;
					DoPlaySoundToSet(m_creature,SOUND_INTRO);
				}
            pInstance->SetData(TYPE_EREDAR_TWINS, IN_PROGRESS);
			}
        m_creature->SetInCombatWithZone();
		DoPlaySoundToSet(m_creature, 12484);
	}
	
	void KilledUnit(Unit* victim)
	{
       switch(rand()%2)
       {
			case 0: DoPlaySoundToSet(m_creature, SOUND_ALY_KILL_1); DoScriptText(YELL_ALYTHESS_KILL_1, m_creature); break;
			case 1: DoPlaySoundToSet(m_creature, SOUND_ALY_KILL_2); DoScriptText(YELL_ALYTHESS_KILL_2, m_creature); break;
        }
	}
	
	void JustDied(Unit* Killer)
	{
		DoScriptText(YELL_ALYTHESS_DEATH,m_creature);
		DoPlaySoundToSet(m_creature, SOUND_ALY_DEAD);
		bAlythessDead = true;
		if(bSacrolashDead)
			if(pInstance)
				pInstance->SetData(TYPE_EREDAR_TWINS, DONE);
	}


	
	void UpdateAI(const uint32 diff)
    {       
		
		// return since we have no target
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
           return;

		if (uiIntroCount < 9 && uiIntroTimer < diff)
		{
			switch(uiIntroCount)
			{
				case 2: DoScriptText(YELL_ALYTHESS_AGGRO_1,m_creature); uiIntroCount++; uiIntroTimer=1200; break;
				case 4: DoScriptText(YELL_ALYTHESS_AGGRO_2,m_creature); uiIntroCount++; uiIntroTimer=1200; break;
				case 6: DoScriptText(YELL_ALYTHESS_AGGRO_3,m_creature); uiIntroCount++; uiIntroTimer=2000; break;
				case 8: DoScriptText(YELL_ALYTHESS_AGGRO_4,m_creature); uiIntroCount++; uiIntroTimer=0; break;
			}
		}
		
        /*if (m_creature->HasAura(SPELL_BANISH))
        {
			if (pInstance && pInstance->GetData(TYPE_EREDAR_TWINS) == NOT_STARTED)
            {
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveAurasDueToSpell(SPELL_BANISH);
                ((boss_alythessAI*)m_creature->AI())->Reset();
                m_creature->AI()->EnterEvadeMode();
            }
            return;
        }*/

        // Banish at 1% hp 
        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 1) && !m_bIsBanished)
        {
			if(!bSacrolashDead)
			{
				bAlythessDead = true;
				DoCast(m_creature, SPELL_BANISH, true);
				m_creature->SetVisibility(VISIBILITY_OFF);
				m_bIsBanished = true;

				if (Unit* Sacrolash = m_creature->GetMap()->GetUnit(pInstance->GetData64(DATA_SACROLASH)))
				{
					DoCast(Sacrolash,SPELL_EMPOWER,true);
					Sacrolash->SetHealthPercent(100);
					DoScriptText(YELL_ALYTHESS_EMPOWER,Sacrolash);
					DoPlaySoundToSet(Sacrolash,SOUND_ALY_EMPOWER);
				}
			}
			return;
        }

		// alythess only moves when victim out of spell reach
		if(Unit *who = m_creature->getVictim())
        {
		    if(who && who->IsInRange(m_creature, 0.0f, 15.0f, false))
			    m_creature->StopMoving();
            else 
			    m_creature->CanFreeMove();
        }
		
		// enrage/berserk 100%
        if(m_uiEnrageTimer < diff && !m_bIsEnraged)
        {
            DoScriptText(YELL_ALYTHESS_BERSERK, m_creature);
            DoCast(m_creature, SPELL_TWINS_ENRAGE);
			DoPlaySoundToSet(m_creature,SOUND_ALY_BERSERK);
            m_bIsEnraged = true;
        }else m_uiEnrageTimer -= diff;
		// conflagration while sacrolash alive
		if(!bSacrolashDead)
		{
			if(m_uiConflagrationTimer < diff)
			{
				if (Unit *victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				{			
					DoScriptText(YELL_ALYTHESS_CONFLAGRATE,m_creature);
					DoPlaySoundToSet(m_creature,SOUND_ALY_CANFLAGRATION);

					m_uiConfTargetGUID = victim->GetGUID();

					if(DoCastSpellIfCan(victim,SPELL_CONFLAGRATION)==CAST_OK)
						m_bIsConfDone = true;

					m_bIsConf = true;
				}
				m_uiConflagrationTimer = 20000 + rand()%1000;
			}else m_uiConflagrationTimer -= diff;

			if(m_bIsConf) 
				if(m_uiConfTimer < diff)
				{
					if(Unit *victim = m_creature->GetMap()->GetUnit(m_uiConfTargetGUID))
					{
						if(m_bIsConfDone)
						{
							if(victim->HasAura(SPELL_DARK_TOUCHED,EFFECT_INDEX_0))
								victim->RemoveAurasDueToSpell(SPELL_DARK_TOUCHED,0);

							victim->CastSpell(victim, SPELL_FLAME_TOUCHED, false);
							m_bIsConfDone = false;
						}
					}
					m_bIsConf = false;
					m_uiConfTimer = 3600;
				}else m_uiConfTimer -= diff;
		}
		// shadow nova while sacrolash dead
		else
		{
			if(m_uiShadowNovaTimer < diff)
			{
				if(Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))// todo: victim should be 2nd on threat list
				{	
					m_uiNovaTargetGUID = victim->GetGUID();

					if(DoCastSpellIfCan(victim,SPELL_SHADOW_NOVA)==CAST_OK)
						m_bIsNovaDone = true;

					m_bIsNova = true;
				}
					m_uiShadowNovaTimer = urand(30000,40000);
			}else m_uiShadowNovaTimer -= diff;
		
			if(m_bIsNova) 
				if(m_uiNovaTimer < diff)
				{
					if(Unit *victim = m_creature->GetMap()->GetUnit(m_uiNovaTargetGUID))
					{
						if(m_bIsNovaDone)
						{
							if(victim->HasAura(SPELL_FLAME_TOUCHED,EFFECT_INDEX_0))
								victim->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED,0);

							victim->CastSpell(victim, SPELL_DARK_TOUCHED, false);
							m_bIsNovaDone = false;
						}
					}
					m_bIsNova = false;
					m_uiNovaTimer = 3600;
				}else m_uiNovaTimer -= diff;
		}
		
        		
		// pyrogenics 100% alythess' self buff
        if(m_uiPyrogenicsTimer < diff)
        {
            DoCast(m_creature, SPELL_PYROGENICS);
            m_uiPyrogenicsTimer = 35000;
        }else m_uiPyrogenicsTimer -= diff;

		// blaze TODO:fire spots on the ground that deal extra damage
        if(m_uiBlazeTimer < diff)
        {
			if(Unit *victim = m_creature->getVictim())
			{
				m_uiBlazeTargetGUID = victim->GetGUID();

				if(DoCastSpellIfCan(victim,SPELL_BLAZE)==CAST_OK)
					m_bIsBlazeDone = true;

				m_bIsBlaze = true;
			}
            m_uiBlazeTimer = urand(5000,10000);
        }else m_uiBlazeTimer -= diff;

		if(m_bIsBlaze) 
			if(m_uiBlaTimer < diff)
			{
				if(Unit *victim = m_creature->GetMap()->GetUnit(m_uiBlazeTargetGUID))
				{
					if(m_bIsBlazeDone)
					{
						victim->CastSpell(victim,SPELL_BLAZE_FIREPATCH,true);

						if(victim->HasAura(SPELL_DARK_TOUCHED,EFFECT_INDEX_0))
							victim->RemoveAurasDueToSpell(SPELL_DARK_TOUCHED,0);

						victim->CastSpell(victim, SPELL_FLAME_TOUCHED, false);
						m_bIsBlazeDone = false;
					}
				}
				m_bIsBlaze = false;
				m_uiBlaTimer = 2600;
			}else m_uiBlaTimer -= diff;		

		// flame sear 
        if(m_uiFlameSearTimer < diff)
        {
            uint8 i = urand(3,5);
            for(uint8 k=0; k<i; ++k)
			    if(Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				{
					//if(DoCastSpellIfCan(victim,SPELL_FLAME_SEAR)==CAST_OK)
					DoCast(victim,SPELL_FLAME_SEAR,false);
					{
						if(victim->HasAura(SPELL_DARK_TOUCHED,EFFECT_INDEX_0))
							victim->RemoveAurasDueToSpell(SPELL_DARK_TOUCHED,0);
						victim->CastSpell(victim, SPELL_FLAME_TOUCHED, true);
					}
				}
            m_uiFlameSearTimer = 30000;
        }else m_uiFlameSearTimer -= diff;
        
		
		DoMeleeAttackIfReady();
    }
};

//############################################
//################# SACROLASH ################
//############################################

struct MANGOS_DLL_DECL boss_sacrolashAI : public ScriptedAI
{
    boss_sacrolashAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
			
	ScriptedInstance* pInstance;
	
	uint32 m_uiEnrageTimer;
	uint32 m_uiDarkTouchedTimer;
	uint32 m_uiConfoundingBlowTimer;
	uint32 m_uiSummonShadowImage;
	uint64 m_uiTargetGUID[3];
	bool m_bIsEnraged;
    bool m_bIsBanished;
	//shadow blades
	uint32 m_uiShadowBladesTimer; 
	uint32 m_uiBladesTimer;
	bool m_bIsBladesDone;
	//conflagration
	uint32 m_uiConflagrationTimer;
	uint32 m_uiConfTimer;
	uint64 m_uiConfTargetGUID;
	bool m_bIsConf;
	bool m_bIsConfDone;
	//shadow nova
	uint32 m_uiShadowNovaTimer;
	uint32 m_uiNovaTimer;
	uint64 m_uiNovaTargetGUID;
	bool m_bIsNova;
	bool m_bIsNovaDone;

	void Reset()
	{	
        m_bIsBanished = false;
		m_uiEnrageTimer = 360000; 
		m_uiDarkTouchedTimer = 30000;
		m_uiConfoundingBlowTimer = 3000;
		m_uiSummonShadowImage = 30000;
		m_bIsEnraged = false;
		//shadow blades
		m_uiShadowBladesTimer = 10000; 
		m_uiBladesTimer = 1600;
		m_bIsBladesDone = false;
		//conflagration
		m_uiConflagrationTimer = 25000 + rand()%15000;
        m_uiConfTimer = 3600;
        m_uiConfTargetGUID = 0;
		m_bIsConf = false;
		m_bIsConfDone = false;
		//shadow nova
		m_uiShadowNovaTimer = 15000;
		m_uiNovaTimer = 3600;
		m_uiNovaTargetGUID = 0;
		m_bIsNova = false;
		m_bIsNovaDone = false;

		bSacrolashDead = false;

		uiIntroTimer = 0;
		uiIntroCount = 9;

        if (pInstance)
            pInstance->SetData(TYPE_EREDAR_TWINS, NOT_STARTED);
	}

	void Aggro(Unit *who)
	{
        if(pInstance)
			if(pInstance->GetData(TYPE_EREDAR_TWINS)==NOT_STARTED)
			{
				if(uiIntroCount==9)
				{
					uiIntroTimer = 500;
					uiIntroCount = 1;
					DoPlaySoundToSet(m_creature,SOUND_INTRO);
				}
				pInstance->SetData(TYPE_EREDAR_TWINS, IN_PROGRESS);
			}
        m_creature->SetInCombatWithZone();
	}

	void KilledUnit(Unit* victim)
	{
	   switch(rand()%2)
       {
			case 0: DoPlaySoundToSet(m_creature, SOUND_SAC_KILL_1); DoScriptText(YELL_SACROLASH_KILL_1, m_creature); break;
			case 1: DoPlaySoundToSet(m_creature, SOUND_SAC_KILL_2); DoScriptText(YELL_SACROLASH_KILL_2, m_creature); break;
       }
	}

	void JustDied(Unit* Killer)
	{
		DoScriptText(YELL_SACROLASH_DEATH, m_creature);
		bSacrolashDead = true;
		if(bAlythessDead)
			if(pInstance)
				pInstance->SetData(TYPE_EREDAR_TWINS, DONE);
				
	}

    void UpdateAI(const uint32 diff)
    {
	
        // return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		if (uiIntroCount < 9 && uiIntroTimer < diff)
		{
			switch(uiIntroCount)
			{
				case 1: DoScriptText(YELL_SACROLASH_AGGRO_1,m_creature); uiIntroCount++; uiIntroTimer = 1200; break;
				case 3: DoScriptText(YELL_SACROLASH_AGGRO_2,m_creature); uiIntroCount++; uiIntroTimer = 1200; break;
				case 5: DoScriptText(YELL_SACROLASH_AGGRO_3,m_creature); uiIntroCount++; uiIntroTimer = 1500; break;
				case 7: DoScriptText(YELL_SACROLASH_AGGRO_4,m_creature); uiIntroCount++; uiIntroTimer = 2000; break;
			}
		}
		else uiIntroTimer -= diff;

        /*if (m_creature->HasAura(SPELL_BANISH))
        {
			if (pInstance && pInstance->GetData(TYPE_EREDAR_TWINS) == NOT_STARTED)
            {
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveAurasDueToSpell(SPELL_BANISH);
                ((boss_alythessAI*)m_creature->AI())->Reset();
                m_creature->AI()->EnterEvadeMode();
            }
			return;
        }*/

        /* Banish at 1% hp working */
        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 1) && !m_bIsBanished)
      {
			if(!bAlythessDead)
			{
				bSacrolashDead = true;
				DoCast(m_creature, SPELL_BANISH, true);
				m_creature->SetVisibility(VISIBILITY_OFF);
				m_bIsBanished = true;
				if (Unit* Alythess = m_creature->GetMap()->GetUnit( pInstance->GetData64(DATA_ALYTHESS)))
				{
					DoCast(Alythess,SPELL_EMPOWER,true);
					Alythess->SetHealthPercent(100);
					DoScriptText(YELL_SACROLASH_EMPOWER,Alythess);
					DoPlaySoundToSet(Alythess,SOUND_SAC_EMPOWER);
				}
			}
			return;
        }
		// berserk
		if(m_uiEnrageTimer < diff && !m_bIsEnraged)
        {
			DoScriptText(YELL_SACROLASH_BERSERK, m_creature);
            DoCast(m_creature, SPELL_TWINS_ENRAGE);
            m_bIsEnraged = true;
        }else m_uiEnrageTimer -= diff;
		
		// shadowblades & dark strike
        if(m_uiShadowBladesTimer < diff)
        {
			if(DoCastSpellIfCan(m_creature,SPELL_SHADOW_BLADES)==CAST_OK)
				m_bIsBladesDone = true;

            m_uiShadowBladesTimer = urand(10000,13000);
        }else m_uiShadowBladesTimer -= diff;

		if(m_bIsBladesDone)
			if(m_uiBladesTimer < diff)
			{	
				std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
                for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {		
					Unit *victim = m_creature->GetMap()->GetUnit( (*itr)->getUnitGuid());
					if (victim && victim->GetTypeId() == TYPEID_PLAYER && victim->IsWithinDistInMap(m_creature, 20.0))
                    {
						if(victim->HasAura(SPELL_FLAME_TOUCHED,EFFECT_INDEX_0))
						victim->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED,0);

						victim->CastSpell(victim, SPELL_DARK_TOUCHED, false);

						DoCast(victim,SPELL_DARK_STRIKE,true);	
                    }
                }   
				m_bIsBladesDone = false;
				m_uiBladesTimer = 1600;
			}else m_uiBladesTimer -= diff;


		// shadow nova while alythess alive
		if(!bAlythessDead)
		{
			if(m_uiShadowNovaTimer < diff)
			{
				if(Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))// todo: victim should be 2nd on threat list
				{
					m_uiNovaTargetGUID = victim->GetGUID();

					if(DoCastSpellIfCan(victim,SPELL_SHADOW_NOVA)==CAST_OK)
						m_bIsNovaDone = true;

					m_bIsNova = true;
				}
				m_uiShadowNovaTimer = urand(30000,40000);
			}else m_uiShadowNovaTimer -=diff;
			
			if(m_bIsNova)
				if(m_uiNovaTimer < diff)
				{
					if(Unit *victim = m_creature->GetMap()->GetUnit(m_uiNovaTargetGUID))
					{
						if(m_bIsNovaDone)
						{
							if(victim->HasAura(SPELL_FLAME_TOUCHED,EFFECT_INDEX_0))
								victim->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED,0);

							victim->CastSpell(victim, SPELL_DARK_TOUCHED, false);
							m_bIsNovaDone = false;
						}
					}
					m_bIsNova = false;
					m_uiNovaTimer = 3600;
				}else m_uiNovaTimer -= diff;
		} //conflagration when alythess dead
		else
		{
			if(m_uiConflagrationTimer < diff)
			{
				if (Unit *victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				{			
					DoScriptText(YELL_ALYTHESS_CONFLAGRATE,m_creature);
					DoPlaySoundToSet(m_creature,SOUND_ALY_CANFLAGRATION);

					m_uiConfTargetGUID = victim->GetGUID();

					if(DoCastSpellIfCan(victim,SPELL_CONFLAGRATION)==CAST_OK)
						m_bIsConfDone = true;

					m_bIsConf = true;
				}
				m_uiConflagrationTimer = 20000 + rand()%1000;
			}else m_uiConflagrationTimer -= diff;

			if(m_bIsConf) 
				if(m_uiConfTimer < diff)
				{
					if(Unit *victim = m_creature->GetMap()->GetUnit(m_uiConfTargetGUID))
					{
						if(m_bIsConfDone)
						{
							if(victim->HasAura(SPELL_DARK_TOUCHED,EFFECT_INDEX_0))
								victim->RemoveAurasDueToSpell(SPELL_DARK_TOUCHED,0);

							victim->CastSpell(victim, SPELL_FLAME_TOUCHED, false);
							m_bIsConfDone = false;
						}
					}
					m_bIsConf = false;
					m_uiConfTimer = 3600;
				}else m_uiConfTimer -= diff;
		}
		
		// confounding blow
        if(m_uiConfoundingBlowTimer < diff)
        {
			if(Unit *victim = m_creature->getVictim())
			{
				if(DoCastSpellIfCan(victim,SPELL_CONFOUNDING_BLOW)==CAST_OK)
				{
					if(victim->HasAura(SPELL_FLAME_TOUCHED,EFFECT_INDEX_0))
						victim->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED,0);
					victim->CastSpell(victim, SPELL_DARK_TOUCHED, false);
				}
			}
            m_uiConfoundingBlowTimer = urand(25000,30000);
        }else m_uiConfoundingBlowTimer -= diff;
		

		// summon shadowImage
		if (m_uiSummonShadowImage < diff)
        {
            for(uint8 i=0; i<3; ++i)
            {
			    if(Creature *image = m_creature->SummonCreature(MOB_SHADOW_IMAGE, m_creature->GetPositionX()+urand(4,10), m_creature->GetPositionY()+urand(4,10), m_creature->GetPositionZ(), 0.0,TEMPSUMMON_TIMED_DESPAWN, 10000))
			    {
                    image->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    image->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    image->setFaction(14); 

                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                        image->AI()->AttackStart(target);
			    }
            }
            m_uiSummonShadowImage = urand(30000,40000);
        } else m_uiSummonShadowImage -= diff;
return;
        DoMeleeAttackIfReady();
    }
};

//############################################
//############## SHADOW IMAGE ################
//############################################
struct MANGOS_DLL_DECL npc_shadow_imageAI : public ScriptedAI
{
    npc_shadow_imageAI(Creature *c) : ScriptedAI(c)
    {
        Reset();
    }

	ScriptedInstance* pInstance; 
	
	uint32 m_uiDarkStrikeTimer;
	uint32 m_uiShadowfuryTimer;
	
	void Aggro(Unit* target) 
    {
		m_creature->SetInCombatWithZone();
	}
	
    void Reset()
	{	
        DoCast(m_creature,SPELL_IMAGE_VISUAL,true);

		m_uiDarkStrikeTimer = 1000;
		m_uiShadowfuryTimer = 7000;
	}
	
	void UpdateAI(const uint32 diff)
    {       	
	    if(m_uiDarkStrikeTimer < diff)
        {
			if(Unit *victim = m_creature->getVictim())
				if(DoCastSpellIfCan(victim,SPELL_DARK_STRIKE)==CAST_OK)
				{
					if(victim->HasAura(SPELL_FLAME_TOUCHED,EFFECT_INDEX_0))
						victim->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED,0);

					victim->CastSpell(victim, SPELL_DARK_TOUCHED, true);
				}
            m_uiDarkStrikeTimer = 2000;
        }else m_uiDarkStrikeTimer -= diff;
		
		if(m_uiShadowfuryTimer < diff)
        {
            if(Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			{	
				m_creature->CastSpell(victim, SPELL_SHADOWFURY, true);

				if(victim->HasAura(SPELL_FLAME_TOUCHED,EFFECT_INDEX_0))
					victim->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED,0);

				victim->CastSpell(victim, SPELL_DARK_TOUCHED, true);
			}
			m_uiShadowfuryTimer = 8000;	
        }else m_uiShadowfuryTimer -= diff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_npc_shadow_image(Creature *_Creature)
{
    return new npc_shadow_imageAI (_Creature);
}

CreatureAI* GetAI_boss_alythess(Creature *_Creature)
{
    return new boss_alythessAI (_Creature);
}

CreatureAI* GetAI_boss_sacrolash(Creature *_Creature)
{
    return new boss_sacrolashAI (_Creature);
}

void AddSC_boss_eredar_twins()
{
    Script *newscript;
	
	newscript = new Script;
    newscript->Name = "npc_shadow_image";
    newscript->GetAI = &GetAI_npc_shadow_image;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name = "boss_alythess";
    newscript->GetAI = &GetAI_boss_alythess;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_sacrolash";
    newscript->GetAI = &GetAI_boss_sacrolash;
    newscript->RegisterSelf();
}
