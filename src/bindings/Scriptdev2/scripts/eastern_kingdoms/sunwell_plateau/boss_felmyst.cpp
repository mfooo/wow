/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
 
/* ScriptData
SDName: Boss_Felmyst
SD%Complete: 75%
SDComment: PlayAble
SDCategory: Sunwell Plateau
SDAuthot: Scrappy Doo
EndScriptData */
 
/* ToDo
Land Phase          = 100%
Fly Phase           = 50%
DeatchCloud Spell   = 50%
*/

#include "precompiled.h"
#include "sunwell_plateau.h"
#include "../../../game/TemporarySummon.h"
 
enum Sounds
{
    YELL_AGGRO          = -1930000,
    YELL_CLEAVE         = -1930001,
    YELL_CORROSION      = -1930002,
    YELL_NOXIOUSFUMES   = -1930003,
    YELL_SLAY1          = -1930004,
    YELL_SLAY2          = -1930005,
    YELL_SLAY3          = -1930006,
    YELL_DEATH          = -1930007,
	YELL_BERSERK		= -1930008,
};

enum Spells
{
    //Aura
    SPELL_SUNWELLRADIANCE_AURA  = 45769,
    SPELL_NOXIOUSFUMES_AURA     = 47002, // toxic fume aura
 
    //Land Phase
    SPELL_CLEAVE                = 19983, // cleave spell
    SPELL_CORROSION             = 45866, // corrosion
	SPELL_GASNOVA               = 45855, // gasnova 
    SPELL_ENCAPSULATE_CHANNEL   = 45661, // encapsulate
   
    //Flight Phase
    SPELL_DEMONIC_VAPOR         = 45391, // demonic vapor start spell
    SPELL_FELMYST_FORCE_BEAM    = 45388, // demonic vapor, force beam cast spell
    SPELL_SUMMON_DEATH          = 45400, // demonic vapor, unyielding death summon
   
    //Other
    SPELL_ENRAGE                = 26662, // Berserk -> speed 150% dmg 500%
    SPELL_BERSERK               = 45078, // Berserk -> dmg 500%

    //Fog of Mind Control
    SPELL_FOG_TRIGGER           = 45582, // Visual Effect
    SPELL_FOG_CHARM             = 45717, // Damage, Speed Increased
    SPELL_KILL                  = 5,	 // instant kill

    //Image Of Players Spells
    SPELL_PRIEST                = 47077, // 100%
    SPELL_PALADIN               = 37369, // 100%
    SPELL_PALADIN2              = 37369, // 100%
    SPELL_WARLOCK               = 46190, // 100%
    SPELL_WARLOCK2              = 47076, // 100%
    SPELL_MAGE                  = 47074, // 100%
    SPELL_ROGUE                 = 45897, // 100%
    SPELL_WARRIOR               = 17207, // 100%
    SPELL_DRUID                 = 47072, // 100%
    SPELL_SHAMAN                = 47071, // 100%
    SPELL_HUNTER                = 48098, // 100%
};
enum Creatures
{
    MOB_FELMYST         = 25038, //undead felmyst
 
    MOB_MADRIGOSA       = 25160, //madrigosa
    MOB_FELMYST_VISUAL  = 25041, //felmyst visual (friendly)
    MOB_FLIGHT_LEFT     = 25357, 
    MOB_FLIGHT_RIGHT    = 25358, 

    MOB_DEATH_CLOUD     = 25703, 

    CREATURE_IMAGE      = 25708, //sinister reflection 
};

enum FogOfCorruption
{
	NORTH				= 0,
	MIDDLE				= 1,
	SOUTH				= 2,

	EASTPOS				= 3,
	WESTPOS				= 4,
};	

//felmyst flight points X,Y,Z,Orientation
float EASTSTART[]	=	{1467.13f,516.32f,69.50f,1.61f};
float EASTSOUTH[]	=	{1444.82f,521.70f,60.07f,1.63f};
float EASTNORTH[]	=	{1536.18f,507.04f,60.07f,1.87f};
float EASTMID[]		=	{1497.06f,508.96f,60.07f,1.86f};

float WESTSTART[]	=	{1447.15f,699.65f,69.50f,4.97f};
float WESTNORTH[]	=	{1486.18f,693.72f,60.07f,4.87f};
float WESTSOUTH[]	=	{1414.79f,656.45f,60.07f,5.13f};
float WESTMID[]		=	{1453.55f,673.67f,60.07f,4.97f};

/*
Felmyst way in fog of corruption phase

			WESTNORTH - - - EASTNORTH 
		  /                           \
WESTSTART - WESTMID   - - -   EASTMID - EASTSTART
		  \                           /
		    WESTSOUTH - - - EASTSOUTH 

starting from weststart, choosing random track (norh/mid/south) to eaststart (2x) and back (1x)
*/

//Felmyst
struct MANGOS_DLL_DECL boss_felmystAI : public ScriptedAI
{
    boss_felmystAI(Creature *c) : ScriptedAI(c)
    {
        m_pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
 
    //Ground Phase
    uint64 m_uiEncapsulateGUID;
    uint32 m_uiEnrageTimer;
    uint32 m_uiFlyPhaseTimer;
    uint32 m_uiNoxiousFumesTimer;
    uint32 m_uiCorrosionTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiEncapsulateTimer;
    uint32 m_uiGasNovaTimer;
    uint8  m_uiMaxBreathCount;

    bool m_bIsCastedNoxiousFumes;
    bool m_bIsFlyPhase;
	bool m_bDemonicVapor;
	bool m_bIsFogOfCorruption;

    //FlyPhase Timers
    uint64 m_uiVictimGUID;
    uint32 m_uiDemonicVaporTimer;
    uint32 m_uiFogOfCorruptionTimer;
    uint32 m_uiLandPhaseTimer;
    uint32 m_uiDemonicVaporInitTimer;
    uint8  m_uiBreathCount;
    uint8  m_uiFogCount;
	uint32 m_uiBeamTargetTimer;
	//fog of corruption 
	bool m_bToStartPos;
	bool m_bToLineStartPos;
	bool m_bFlyOver;
	bool m_bFog;
	bool m_bNextCycle;
	uint32 m_uiOnStartPosTimer;
	uint32 m_uiFlyOverTimer;
	uint32 m_uiFogTimer;
	uint32 m_uiNextCycleTimer;
	uint8  m_uiCycle;
	uint8  m_uiLine;

	Unit* cBeamTarget;
	Unit* cTarget;

	void Reset()
    {
        //Ground Phase
        m_uiEncapsulateGUID     = 0;
        m_uiEnrageTimer         = 600000;
        m_uiFlyPhaseTimer       = 60000;
        m_uiNoxiousFumesTimer   = 1000;
        m_uiCorrosionTimer      = 72000;
        m_uiCleaveTimer         = 28000;
        m_uiEncapsulateTimer    = 10000;
        m_uiGasNovaTimer        = 30000;

        m_bIsCastedNoxiousFumes = false;
        m_bIsFlyPhase           = false;

        //FlyPhase Timers
        m_uiVictimGUID            = 0;
        m_uiFogOfCorruptionTimer  = 50000;
        m_uiDemonicVaporInitTimer = 120000;
        m_uiBreathCount         = 10;
        m_uiFogCount            = 0;
        m_uiMaxBreathCount      = 2;
		//fog of corruption
        m_bIsFogOfCorruption    = false;

        //Event Resets
		m_creature->SetSplineFlags(SPLINEFLAG_FLYING);
		if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
			m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, NOT_STARTED);

        if(!m_creature->HasAura(SPELL_SUNWELLRADIANCE_AURA))
			DoCast(m_creature, SPELL_SUNWELLRADIANCE_AURA);
    }
 
    void Aggro(Unit *who)
    {
        m_creature->SetInCombatWithZone();
        DoScriptText(YELL_AGGRO, m_creature);
		m_uiFlyPhaseTimer=60000;
 
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, IN_PROGRESS);

        if(Creature* pBrutallus = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_BRUTALLUS)))
            pBrutallus->SetVisibility(VISIBILITY_OFF);
    }
 
    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(YELL_SLAY1, m_creature); break;
            case 1: DoScriptText(YELL_SLAY2, m_creature); break;
            case 2: DoScriptText(YELL_SLAY3, m_creature); break;
        }
        switch(rand()%2)
        {
            case 0: DoPlaySoundToSet(m_creature, 12480); break;
            case 1: DoPlaySoundToSet(m_creature, 12481); break;
        }
    }
 
    void JustDied(Unit* Killer)
    {
        DoScriptText(YELL_DEATH, m_creature);
        DoPlaySoundToSet(m_creature, 12483);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, DONE);
    }
 
    void UpdateAI(const uint32 diff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiEnrageTimer < diff)
        {
			DoCastSpellIfCan(m_creature, SPELL_ENRAGE, true);
            m_uiEnrageTimer = 30000;
        }m_uiEnrageTimer -= diff;
 
        if(!m_bIsFlyPhase) //ground phase 
        {
            if(m_uiFlyPhaseTimer < diff) // fly phase start
            {
				//Go Fly
				m_creature->GetMap()->CreatureRelocation(m_creature, 1469, 596, m_creature->GetPositionZ()+20, m_creature->GetOrientation());     
				m_creature->StopMoving();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();

				//FlyPhase Timers Start
				m_uiLandPhaseTimer			= 80000;
                m_uiDemonicVaporTimer       = 2000;
                m_uiFogOfCorruptionTimer    = 20000;
				m_uiFogTimer				= 500;
                m_uiMaxBreathCount			= 0;
                m_uiFogCount				= 0;
				m_uiCycle					= 0;
                m_bIsFlyPhase				= true;
				m_bToStartPos				= false;
				m_bToLineStartPos			= false;
				m_bFlyOver					= false;
				m_bFog						= false;
				m_bNextCycle				= false;

				

				return;
            }else m_uiFlyPhaseTimer -= diff;
 
            if(m_uiCorrosionTimer < diff)
            {
                DoScriptText(YELL_CORROSION, m_creature);
                if(m_creature->getVictim())
					DoCastSpellIfCan(m_creature->getVictim(), SPELL_CORROSION);
                m_uiCorrosionTimer = 72000;
            }else m_uiCorrosionTimer -= diff;
 
            if(m_uiNoxiousFumesTimer < diff && !m_bIsCastedNoxiousFumes)
            {
                DoScriptText(YELL_NOXIOUSFUMES, m_creature);
                DoPlaySoundToSet(m_creature, 12478);
				DoCastSpellIfCan(m_creature, SPELL_NOXIOUSFUMES_AURA);
                m_bIsCastedNoxiousFumes = true;
            }else m_uiNoxiousFumesTimer -= diff;
 
            if(m_uiGasNovaTimer < diff)
            {
				DoCastSpellIfCan(m_creature, SPELL_GASNOVA);
                m_uiGasNovaTimer = 35000;
            }else m_uiGasNovaTimer -= diff;

            if(m_uiEncapsulateTimer < diff)
            {
                if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_uiEncapsulateGUID = target->GetGUID();
                    DoCastSpellIfCan(target, SPELL_ENCAPSULATE_CHANNEL);
                }
                m_uiEncapsulateTimer = 30000;
            }else m_uiEncapsulateTimer -= diff;
 
            if(m_uiCleaveTimer < diff)
            {
                DoScriptText(YELL_CLEAVE, m_creature);

                if(m_creature->getVictim())
					DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = 28000;
            }else m_uiCleaveTimer -= diff;

            DoMeleeAttackIfReady();
        }
        else  // fly phase
        {
			if(m_uiLandPhaseTimer < diff) //fly phase -> ground phase
            {
                m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()-20, m_creature->GetOrientation());
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiFlyPhaseTimer       = 60000;
                m_bIsFlyPhase           = false;

                m_uiCorrosionTimer      = 20000;
                m_uiCleaveTimer         = 10000;
                m_uiEncapsulateTimer    = 10000;
                m_uiGasNovaTimer        = 15000;

				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

			}else m_uiLandPhaseTimer -= diff;

			//start demonic vapor
			if(m_uiDemonicVaporTimer < diff && m_uiMaxBreathCount <= 2)
			{
                m_bDemonicVapor = true;

				if (m_uiMaxBreathCount < 2)
					DoCast(m_creature, SPELL_DEMONIC_VAPOR);

                if (m_uiMaxBreathCount++ == 2)
                {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_bDemonicVapor      = false;
                    m_bIsFogOfCorruption = true;
					m_bToStartPos		 = true;
                    m_creature->GetMotionMaster()->Clear();
                }
                else
				    m_uiDemonicVaporTimer=12000;
			}
			else
                m_uiDemonicVaporTimer -=diff;

            if(m_bDemonicVapor)
            {
                m_creature->StopMoving();
            }

            // fog of corruption phase
			if(m_bIsFogOfCorruption)
            {
				if(m_bToStartPos) // move felmyst to fog of corruption start position
				{
					++m_uiCycle;
					m_bToStartPos = false;
					// go to Line Start Position
					m_uiOnStartPosTimer = 7000;
					m_bToLineStartPos = true;
					switch(m_uiCycle)
					{
						//move felmyst to west or east start position
						case 1:	m_creature->GetMotionMaster()->MovePoint(0,WESTSTART[0],WESTSTART[1],WESTSTART[2]);break;
						case 2: m_creature->GetMotionMaster()->MovePoint(0,EASTSTART[0],EASTSTART[1],EASTSTART[2]);break;
						case 3: m_creature->GetMotionMaster()->MovePoint(0,WESTSTART[0],WESTSTART[1],WESTSTART[2]);break;
						case 4: m_bIsFogOfCorruption = false; m_bToLineStartPos = false;break;
					}
				}
	
				// felmyst should be on start position now.
				if(m_bToLineStartPos)
					if(m_uiOnStartPosTimer < diff) 
					{
						m_creature->GetMotionMaster()->Clear();
						//go to random Line startposition

						m_uiLine = urand(0,2);
								//west 
						if(m_uiCycle==1||m_uiCycle==3)
							switch(m_uiLine)
							{
								case NORTH:		m_creature->GetMotionMaster()->MovePoint(0,WESTNORTH[0],WESTNORTH[1],WESTNORTH[2]);break;
								case MIDDLE:	m_creature->GetMotionMaster()->MovePoint(0,WESTMID[0],WESTMID[1],WESTMID[2]);break;	
								case SOUTH:		m_creature->GetMotionMaster()->MovePoint(0,WESTSOUTH[0],WESTSOUTH[1],WESTSOUTH[2]);break;	
							}
						else	//east
							switch(m_uiLine)
							{
								case NORTH:		m_creature->GetMotionMaster()->MovePoint(0,EASTNORTH[0],EASTNORTH[1],EASTNORTH[2]);break;
								case MIDDLE:	m_creature->GetMotionMaster()->MovePoint(0,EASTMID[0],EASTMID[1],EASTMID[2]);break;	
								case SOUTH:		m_creature->GetMotionMaster()->MovePoint(0,EASTSOUTH[0],EASTSOUTH[1],EASTSOUTH[2]);break;	
							}
						m_bToLineStartPos = false;
						//go fly over
						m_uiFlyOverTimer = 3500;
						m_bFlyOver = true; 

					}else m_uiOnStartPosTimer -=diff;

				// let felmyst fly over the battlefield
				if(m_bFlyOver)
					if(m_uiFlyOverTimer < diff)
					{
						m_creature->GetMotionMaster()->Clear();
						m_creature->SetSpeedRate(MOVE_FLIGHT,4.0,true);
						m_creature->SetSpeedRate(MOVE_RUN,4.0,true);
						if(m_uiCycle==1||m_uiCycle==3)
							switch(m_uiLine)
							{
								case NORTH:		m_creature->GetMotionMaster()->MovePoint(0,EASTNORTH[0],EASTNORTH[1],EASTNORTH[2]);break;	
								case MIDDLE:	m_creature->GetMotionMaster()->MovePoint(0,EASTMID[0],EASTMID[1],EASTMID[2]);break;		
								case SOUTH:		m_creature->GetMotionMaster()->MovePoint(0,EASTSOUTH[0],EASTSOUTH[1],EASTSOUTH[2]);break;
							}
						else
							switch(m_uiLine)
							{
								case NORTH:		m_creature->GetMotionMaster()->MovePoint(0,WESTNORTH[0],WESTNORTH[1],WESTNORTH[2]);break;	
								case MIDDLE:	m_creature->GetMotionMaster()->MovePoint(0,WESTMID[0],WESTMID[1],WESTMID[2]);break;		
								case SOUTH:		m_creature->GetMotionMaster()->MovePoint(0,WESTSOUTH[0],WESTSOUTH[1],WESTSOUTH[2]);break;
							}
						m_bFlyOver = false;
						//spawn fog of corruption
						m_uiFogCount = 0;
						m_bFog = true;
						//next cycle
						m_uiNextCycleTimer = 4000;
						m_bNextCycle = true;
					
					} else m_uiFlyOverTimer -=diff;

				if(m_bNextCycle)
					if(m_uiNextCycleTimer < diff)
					{
						m_creature->SetSpeedRate(MOVE_FLIGHT,1.0,true);
						m_creature->SetSpeedRate(MOVE_RUN,1.0,true);
						m_bNextCycle = false;
						//next cycle
						m_bToStartPos = true;
					}else m_uiNextCycleTimer -=diff;

				if(m_bFog)// spawn fog of corruption
					if(m_uiFogTimer < diff)
					{
						if(m_uiLine==NORTH)
						{
							// north breath
							if(m_uiCycle==1||m_uiCycle==3)
								switch(m_uiFogCount)	
								{	// fog from west to east
									case 0:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1496.38f,679.56f,21.43f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 1:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1499.99f,659.77f,24.17f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 2:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1506.29f,638.52f,25.90f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 3:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1510.55f,617.70f,28.65f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 4:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1510.78f,598.80f,28.50f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 5:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1521.04f,583.18f,30.23f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 6:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1523.43f,562.92f,32.49f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 7:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1535.54f,542.88f,31.96f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 8: m_creature->SummonCreature(MOB_DEATH_CLOUD,1520.65f,530.94f,28.09f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); m_bFog=false; break;
								}
							else
								switch(m_uiFogCount)
								{	// fog from east to west
									case 0: m_creature->SummonCreature(MOB_DEATH_CLOUD,1520.65f,530.94f,28.09f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 1:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1535.54f,542.88f,31.96f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 2:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1523.43f,562.92f,32.49f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 3:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1521.04f,583.18f,30.23f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 4:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1510.78f,598.80f,28.50f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 5:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1510.55f,617.70f,28.65f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 6:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1506.29f,638.52f,25.90f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 7:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1499.99f,659.77f,24.17f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 8:	m_creature->SummonCreature(MOB_DEATH_CLOUD,1496.38f,679.56f,21.43f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); m_bFog=false; break;
							
								}
						}
						else if(m_uiLine==MIDDLE)
						{
							// middle breath
							if(m_uiCycle==1||m_uiCycle==3)
								switch(m_uiFogCount)
								{	// fog from west to east
									case 0: m_creature->SummonCreature(MOB_DEATH_CLOUD,1458.84f,655.56f,19.10f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 1: m_creature->SummonCreature(MOB_DEATH_CLOUD,1463.73f,637.36f,20.41f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 2: m_creature->SummonCreature(MOB_DEATH_CLOUD,1467.05f,618.97f,21.55f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 3: m_creature->SummonCreature(MOB_DEATH_CLOUD,1471.25f,600.90f,23.22f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 4: m_creature->SummonCreature(MOB_DEATH_CLOUD,1478.99f,581.63f,23.23f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 5: m_creature->SummonCreature(MOB_DEATH_CLOUD,1484.60f,559.09f,23.98f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 6: m_creature->SummonCreature(MOB_DEATH_CLOUD,1491.58f,542.94f,25.15f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); m_bFog=false; break;
								}
							else
								switch(m_uiFogCount)
								{	// fog from east to west
									case 0: m_creature->SummonCreature(MOB_DEATH_CLOUD,1491.58f,542.94f,25.15f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 1: m_creature->SummonCreature(MOB_DEATH_CLOUD,1484.60f,559.09f,23.98f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 2: m_creature->SummonCreature(MOB_DEATH_CLOUD,1478.99f,581.63f,23.23f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 3: m_creature->SummonCreature(MOB_DEATH_CLOUD,1471.25f,600.90f,23.22f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 4: m_creature->SummonCreature(MOB_DEATH_CLOUD,1467.05f,618.97f,21.55f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 5: m_creature->SummonCreature(MOB_DEATH_CLOUD,1463.73f,637.36f,20.41f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 6: m_creature->SummonCreature(MOB_DEATH_CLOUD,1458.84f,655.56f,19.10f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); m_bFog=false; break;
								} 
								
						}
						else if(m_uiLine=SOUTH)
						{
							// south breath
							if(m_uiCycle==1||m_uiCycle==3)
								switch(m_uiFogCount)
								{	// fog from west to east
									case 0: m_creature->SummonCreature(MOB_DEATH_CLOUD,1435.02f,609.98f,18.11f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 1: m_creature->SummonCreature(MOB_DEATH_CLOUD,1438.65f,588.12f,19.21f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 2: m_creature->SummonCreature(MOB_DEATH_CLOUD,1445.03f,572.22f,20.72f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 3: m_creature->SummonCreature(MOB_DEATH_CLOUD,1452.80f,554.16f,21.17f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); m_bFog=false; break;
								}
							else
								switch(m_uiFogCount)
								{	// fog from east to west
									case 0: m_creature->SummonCreature(MOB_DEATH_CLOUD,1452.80f,554.16f,21.17f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 1: m_creature->SummonCreature(MOB_DEATH_CLOUD,1445.03f,572.22f,20.72f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 2: m_creature->SummonCreature(MOB_DEATH_CLOUD,1438.65f,588.12f,19.21f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); break;
									case 3: m_creature->SummonCreature(MOB_DEATH_CLOUD,1435.02f,609.98f,18.11f,0,TEMPSUMMON_TIMED_DESPAWN, 15000); m_bFog=false; break;
								}
						}
						
						++m_uiFogCount;
						m_uiFogTimer = 500;

					} else m_uiFogTimer -=diff;
            }
        }
    }

};

//DeatchCloud 100yard range -> mind control
struct MANGOS_DLL_DECL mob_deathcloudAI : public Scripted_NoMovementAI
{
    mob_deathcloudAI(Creature *c) : Scripted_NoMovementAI(c)
    {
        m_pInstance = (ScriptedInstance*)c->GetInstanceData();
        Reset();
    }
 
    ScriptedInstance* m_pInstance;
 
    uint64 m_uiImageGUID[25][2];
    uint32 m_uiCheckTimer;
    uint32 m_uiImageCount;
    uint32 m_uiImageCastTimer;
 
    void Reset()
    {
        m_creature->setFaction(14);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
 
        m_uiCheckTimer      = 1000;
        m_uiImageCount      = 0;
        m_uiImageCastTimer  = 8000;
 
		DoCastSpellIfCan(m_creature,SPELL_FOG_TRIGGER, true);
 
        for(uint8 i=0; i<25; ++i)
        {
            m_uiImageGUID[i][0] = 0;
            m_uiImageGUID[i][1] = 0;
        }
    }

    void Aggro(Unit* who)
    {
        m_creature->SetInCombatWithZone();
    }

    void SummonImageOf(Unit* victim) // summon image of player when going into fog (mind controll hack)
    {
        uint32 m_uiSpell;
        switch(victim->getClass())
        {
            case CLASS_PRIEST:  m_uiSpell = SPELL_PRIEST; break;
            case CLASS_PALADIN: m_uiSpell = SPELL_PALADIN; break;
            case CLASS_WARLOCK: m_uiSpell = SPELL_WARLOCK; break;
            case CLASS_MAGE:    m_uiSpell = SPELL_MAGE; break;
            case CLASS_ROGUE:   m_uiSpell = SPELL_ROGUE; break;
            case CLASS_WARRIOR: m_uiSpell = SPELL_WARRIOR; break;
            case CLASS_DRUID:   m_uiSpell = SPELL_DRUID; break;
            case CLASS_SHAMAN:  m_uiSpell = SPELL_SHAMAN; break;
            case CLASS_HUNTER:  m_uiSpell = SPELL_HUNTER; break;
        }
        Creature* cImage = m_creature->SummonCreature(CREATURE_IMAGE, victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
        if(cImage)
        {
			cImage->SetName(victim->GetName());
            cImage->setFaction(14);
            cImage->AI()->AttackStart(cImage->getVictim()); 
            cImage->SetDisplayId(9784);	//9784
			cImage->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.30f);
            m_uiImageGUID[m_uiImageCount][0] = cImage->GetGUID();
            m_uiImageGUID[m_uiImageCount][1] = m_uiSpell;
            ++m_uiImageCount;
        }
        m_creature->CastSpell(victim, SPELL_KILL, false);
    }

    void UpdateAI(const uint32 diff)
    {
        //Image Attacks Timer
        if(m_uiImageCastTimer < diff)
        {
            for(uint8 i=0; i<m_uiImageCount; ++i)
			{
                if(Unit* cImage =m_creature->GetMap()->GetUnit(m_uiImageGUID[i][0]))
				{
					if(!cImage->isDead())
					{
						if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
							cImage->CastSpell(target, m_uiImageGUID[i][1], true);
						//DoCastSpellIfCan(cImage->getVictim(),m_uiImageGUID[i][1],
					}
				}
			}
            m_uiImageCastTimer = 8000;
        }else m_uiImageCastTimer -= diff;

        if(m_uiCheckTimer < diff)
        {
            //For Now we need to kill player and replace him by image (sinister from kiljaeden)
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                if(Unit *target = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                    if(target && target->GetTypeId() == TYPEID_PLAYER && target->IsWithinDistInMap(m_creature, 15))
                    {
						//Kill Player & Summon Image
                        SummonImageOf(target);
                    }
            }
            m_uiCheckTimer = 3000;
        }else m_uiCheckTimer -= diff;  
    }
}; 

struct MANGOS_DLL_DECL mob_felmyst_vaporAI : public ScriptedAI
{
    mob_felmyst_vaporAI(Creature *c) : ScriptedAI(c){Reset();}

    void Reset()
    {
        // workaround for better movement
        m_creature->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.0f);
        m_creature->SetFloatValue(UNIT_FIELD_COMBATREACH, 0.0f );

        // workaround, to select a close victim (== summoner in this case)
        if (m_creature->IsTemporarySummon())
            AttackStart(m_creature->GetMap()->GetUnit(((TemporarySummon*)m_creature)->GetSummonerGuid()));

        // force felmyst to cast visual beam
        DoCast(m_creature, SPELL_FELMYST_FORCE_BEAM, true);
    }

    void UpdateAI(const uint32 diff)
    {
        // ignore threat list
        if (!m_creature->getVictim())
        {
            Unit* pTarget =  GetClosestAttackableUnit(m_creature, 100.0f);  // maybe we need to exclude pets?
            // unsummon if no unit is present
            if (!pTarget && m_creature->IsTemporarySummon())
                ((TemporarySummon*)m_creature)->UnSummon();
            AttackStart(pTarget);
        }

        // no melee attack
    }
};

struct MANGOS_DLL_DECL mob_felmyst_vapor_trailAI : public ScriptedAI
{
    mob_felmyst_vapor_trailAI(Creature *c) : ScriptedAI(c){Reset();}

    int32 m_summonTimer;
    int32 m_createSummonTimer;
    uint32 m_liveTimer;

    void Reset()
    {
        // some delay for the obligatoric spawn to give a chance to flee from the sceletons
        m_createSummonTimer = 4000;
        m_summonTimer = 1000;
        m_liveTimer = 25000;
    }

    // CreatureNullAI
    void AttackStart(Unit *) {}
    void AttackedBy( Unit *) {}
    void EnterEvadeMode() {}

    void MoveInLineOfSight(Unit* pWho)
    {
        // summon skelton if unit is close
        if ( !m_summonTimer && m_creature->IsHostileTo(pWho) && m_creature->IsWithinDistInMap(pWho, 3))
        {
            // 50% chance - to make not spawn to much as there are many of this mobs in a trail
            if(!urand(0,2))
                m_creature->SummonCreature(25268,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                //DoCast(pWho, SPELL_SUMMON_DEATH, true);
            m_summonTimer = 1000;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        // on-create summon
        if (m_createSummonTimer)
        {
            m_createSummonTimer -= diff;
            if (m_createSummonTimer <= 0)
            {
                m_creature->SummonCreature(25268,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                //DoCast(m_creature, SPELL_SUMMON_DEATH, true);
                m_createSummonTimer = 0;
            }
        }

        // movement cooldown
        if (m_summonTimer)
        {
            m_summonTimer -= diff;
            if (m_summonTimer <= 0)
                m_summonTimer = 0;
        }

        //despawn after 25 seconds
        if(m_liveTimer < diff)
        {
            m_creature->ForcedDespawn();
        } else m_liveTimer -= diff;
    }
};
 
CreatureAI* GetAI_mob_deathcloud(Creature* pCreature)
{
    return new mob_deathcloudAI(pCreature);
}
 
CreatureAI* GetAI_mob_felmyst_vapor(Creature* pCreature)
{
    return new mob_felmyst_vaporAI(pCreature);
}

CreatureAI* GetAI_mob_felmyst_vapor_trail(Creature* pCreature)
{
    return new mob_felmyst_vapor_trailAI(pCreature);
}
 
CreatureAI* GetAI_boss_felmyst(Creature *pCreature)
{
    return new boss_felmystAI(pCreature);
}
 
void AddSC_boss_felmyst()
{
    Script *newscript;
 
    newscript = new Script;
    newscript->Name = "boss_felmyst";
    newscript->GetAI = &GetAI_boss_felmyst;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name="mob_felmyst_vapor";
    newscript->GetAI = &GetAI_mob_felmyst_vapor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_felmyst_vapor_trail";
    newscript->GetAI = &GetAI_mob_felmyst_vapor_trail;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name = "mob_deathcloud";
    newscript->GetAI = &GetAI_mob_deathcloud;
    newscript->RegisterSelf();
}