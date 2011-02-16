/* Copyright  2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_muru_entropius
SD%Complete: 99
SDComment:
SDAuthor: ScrappyDoo (c) Andeeria
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"
 
enum spells // Boss spells
{
    ENRAGE                      = 26662,
    SPELL_NEGATIVE              = 46285, //negative energy -> deals damage
    SPELL_NEGATIVEENERGY        = 46008, //negative energy -> black beams
    SPELL_NEGATIVEENERGY_CHAIN  = 46289, //negative energy -> 

    AURA_SINGULARITY            = 46228, //black hole passive  visual effect
    AURA_SUNWELL_RADIANCE       = 45769, 

    DARKNESS                    = 45996, // darkness 
    ENTROPIUS_EFFECT            = 46223, // entropius cosmetic spawn 

	SUMMON_VOID_SENTINEL		= 45988, // visual effect & summon void sentinel
	SUMMON_VOID_SENTINEL_IMAGE	= 45989, // summon void sentine - summoning visual effect
	SUMMON_VOID_SENTINEL_SUMMON	= 45978, // 
	SUMMON_VOID_SENTINEL_PORTAL	= 45977, // portal visual effect 

    SPELL_SINGULARITY           = 46282,
	
	//45976


    //Dark Fiend
    DARK_FIEND_AURA             = 45934, // summon dark fiend
    DARK_FIEND_DEBUFF           = 45944, // dark fiend debuff
    SPELL_DISPELL               = 32375, // mass dispell
    SPELL_PURGE                 = 8012,  // purge (reinigung)

    SPELL_ARCANEFORM            = 48019, // arcaneform

    VOID_AURA                   = 46087, // shadow pulse
    VOID_BLAST                  = 46161, // void blast
};

enum summons
{
    ID_SWFuryMage	            = 25799, // shadowsword fury mage
    ID_SWBerserker              = 25798, // shadowsword berserker
    ID_DARK_FIEND               = 25744, // dark fiend
    ID_VOID_SENTINEL            = 25772, // void sentinel
    ID_SINGULARITY              = 25855, // visual effect
    ID_SPAWN                    = 25824, // void spawn
};

//Boss sounds
#define SOUND_CHANGE_PHASE 12560
#define SAY_ENTROPIUS_SUMMON	-1950000
 
// Sumoned trash coordinates
float Trash[6][2] =
{
        {1853.300f,588.653f},
        {1781.502f,659.254f},
        {1853.300f,588.653f},
        {1853.300f,588.653f},     
        {1781.502f,659.254f},
        {1781.502f,659.254f},
};
// Dark Fiend spawn coordinates
float DarkFiendSpawn[8][2] =
{
	{1825.39f,620.36f},
	{1827.11f,628.24f},
	{1821.82f,636.50f},
	{1812.42f,637.12f},
	{1805.03f,632.02f},
	{1804.04f,623.09f},
	{1809.85f,615.46f},
	{1818.69f,614.17f},
};
float DarkFiendSpawnZ = 69.7f;
 
// m'uru 
struct MANGOS_DLL_DECL boss_muruAI : public ScriptedAI
{
    boss_muruAI(Creature *c) : ScriptedAI(c) 
    {
	    pInstance = ((ScriptedInstance*)c->GetInstanceData());
	    Reset(); 
    }

    ScriptedInstance* pInstance; 
    bool Phase1;

    float m_fDarkPosX;
    float m_fDarkPosY;

    uint32 TargetsCount;
    uint32 TargetsCountTimer;
    uint32 SingularityTimer;
    uint32 NegativeEnergyTimer;
    uint32 SummonTrashTimer;
    uint32 SummonVoidTimer;
    uint32 DarknessTimer;
    uint32 EnrageTimer;
    uint32 DarkFiendTimer;
	uint32 VoidSentinelTimer;
    uint32 BlackHoleTimer;

    bool Darkness;
	bool VoidSentinel;

	Unit* pPortalTarget;
    Unit* pBlackHole;
    
    void Reset()
    {
        m_fDarkPosX = 0;
        m_fDarkPosY = 0;

        m_creature->SetDisplayId(23404);
        m_creature->SetName("M'uru");

        NegativeEnergyTimer = 1000;
        SummonTrashTimer = 10000;
        SummonVoidTimer = 30000; 
        DarknessTimer = 45000;
        BlackHoleTimer = 10000;
        EnrageTimer = 600000;
        Phase1 = true;
        Darkness = false;
		VoidSentinel = false;

        m_creature->SetVisibility(VISIBILITY_ON);

		if(pInstance)
			pInstance->SetData(TYPE_MURU,NOT_STARTED);
   
        if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

		if(!m_creature->HasAura(AURA_SUNWELL_RADIANCE, EFFECT_INDEX_0))
            m_creature->CastSpell(m_creature, AURA_SUNWELL_RADIANCE, true);
    }
    
    void Aggro(Unit *who) 
    {
        m_creature->StopMoving();
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveIdle();
		if(pInstance)
			pInstance->SetData(TYPE_MURU,IN_PROGRESS);
    }  
    void KilledUnit(Unit *Victim) {}

	void JustDied(Unit* Killer)
    {
        if(pInstance)
			pInstance->SetData(TYPE_MURU, DONE);
    }
    
    void UpdateAI(const uint32 diff)
    {       
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
        
        if(EnrageTimer < diff)
        {
            m_creature->CastSpell(m_creature, ENRAGE, true);
            EnrageTimer = 60000;
        }else EnrageTimer -= diff;
        
        if(Phase1)
        {
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();

			//change phase if muru dead
            if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 42)
            {
                m_creature->SetDisplayId(23428);
                m_creature->SetName("Entropius");
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                DoPlaySoundToSet(m_creature, SOUND_CHANGE_PHASE);
                DoCast(m_creature, ENTROPIUS_EFFECT, true);

                TargetsCount = 1;
                TargetsCountTimer = 10000;
                SingularityTimer = 50000;
                Phase1 = false;
            }

			
           //Spawns Shadow portal and Void Sentinel -> working
            if(SummonVoidTimer < diff)
            {
				//m_creature->MonsterYell("try open portal",LANG_UNIVERSAL,0);
				if(pPortalTarget = m_creature->GetMap()->GetUnit(pInstance->GetData64(DATA_SHADOW_PORTAL)))
				{
					//pPortalTarget->MonsterYell("portal opened",LANG_UNIVERSAL,0);
					pPortalTarget->CastSpell(pPortalTarget,SUMMON_VOID_SENTINEL_PORTAL,false);
					VoidSentinel = true;
					VoidSentinelTimer = 3000;
				} 
				//else m_creature->MonsterYell("failed",LANG_UNIVERSAL,0);
                SummonVoidTimer = 30000; 
            }else SummonVoidTimer -= diff;

			if(VoidSentinel)
				if(VoidSentinelTimer < diff)
				{
					if(pPortalTarget)
						pPortalTarget->CastSpell(pPortalTarget,SUMMON_VOID_SENTINEL,true);
					VoidSentinel = false;
				} else VoidSentinelTimer -= diff;
            
             //Summon 6 humanoids every 1min (1mage & 2berserkers) -> working
            if(SummonTrashTimer < diff)
            {
                    for(uint8 i = 0; i < 6; i++)
                    {
                        uint32 ID;

                        if((i == 1) | (i == 2))
							ID = ID_SWFuryMage;
                        else
							ID = ID_SWBerserker;

                        Creature* sTrash = m_creature->SummonCreature(ID, Trash[i][0], Trash[i][1], m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        
                        if(Unit* sTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            if(sTrash)
                                sTrash->AI()->AttackStart(sTarget);
                    }
                    SummonTrashTimer = 60000;
            }else SummonTrashTimer -= diff;

			//negative energy on 4-5 targets every second -> working
            if(NegativeEnergyTimer < diff)
            {	//choose 4-5 targets
				for(uint8 i=rand()%2; i<5; ++i)
                {
                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_creature->CastSpell(target, SPELL_NEGATIVE, false);
                }
                NegativeEnergyTimer = 1000;
            }else NegativeEnergyTimer -= diff;

		    //Cast Darkness
            if(DarknessTimer < diff)
            {// use spell 45999 / 46268 ?
                m_creature->CastSpell(m_creature, DARKNESS, true);

                Darkness = true;
                DarkFiendTimer = 5000;
                DarknessTimer = 45000;
            }else DarknessTimer -= diff;

        }
        else // else Entropius phase
        {
            
            // +1 target every 10 seconds
            if(TargetsCountTimer < diff)
            {
                ++TargetsCount;
                TargetsCountTimer = 10000;
            }else TargetsCountTimer -= diff;

            //Utrudnienie -> chain negative energy
            if(NegativeEnergyTimer < diff)
            {
                for(uint8 i=0; i<TargetsCount; ++i)
                {
                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_creature->CastSpell(target, SPELL_NEGATIVE, false);
                }

                NegativeEnergyTimer = 1000;
            }else NegativeEnergyTimer -= diff;

            //Summon Singularity
            if(SingularityTimer < diff)
            {
                if(Unit* sTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    //Creature* Singularity = m_creature->SummonCreature(ID_SINGULARITY, sTarget->GetPositionX(), sTarget->GetPositionY(), sTarget->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                    m_creature->CastSpell(sTarget,SPELL_SINGULARITY,true);
                    //if(Singularity)
                    //    Singularity->AI()->AttackStart(sTarget);
                }
                SingularityTimer = 10000;
            }else SingularityTimer -= diff; 

            if(BlackHoleTimer < diff)
            {   //summon black hole visual
                pBlackHole = m_creature->SummonCreature(32953,1790+rand()%50,599+rand()%50,m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,2000);
                Darkness = true;
                DarkFiendTimer = 500;
                BlackHoleTimer = 10000;
            }else BlackHoleTimer -= diff;

            DoMeleeAttackIfReady();
        }

        if(Darkness && DarkFiendTimer < diff)
			{
                if(Phase1)// summon dark fiends
				    for(int i=0;i<8;i++)
                    {
					    Unit* fiend = m_creature->SummonCreature(ID_DARK_FIEND,DarkFiendSpawn[i][0],DarkFiendSpawn[i][1],DarkFiendSpawnZ,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                    }
                else
                    if(pBlackHole)
                    {
                        Unit* fiend = m_creature->SummonCreature(ID_DARK_FIEND,pBlackHole->GetPositionX(),pBlackHole->GetPositionY(),pBlackHole->GetPositionZ(),0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                    }
            Darkness = false;
			} else DarkFiendTimer -= diff;
    }
};
 
// dark fiend
struct MANGOS_DLL_DECL dark_fiendAI : public ScriptedAI
{
    dark_fiendAI(Creature *c) : ScriptedAI(c) 
    {
	    pInstance = ((ScriptedInstance*)c->GetInstanceData());
	    Reset(); 
    }

    ScriptedInstance* pInstance; 

    bool Reached;

    void Reset() 
    {
        m_creature->SetDisplayId(25206);
		if(!m_creature->HasAura(DARK_FIEND_AURA,EFFECT_INDEX_0))
            m_creature->CastSpell(m_creature, DARK_FIEND_AURA, true);
        Reached = false;

        if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            AttackStart(target);
    }
    
    void Aggro(Unit *who) {}
    void JustDied(Unit* Killer) {}
    void KilledUnit(Unit *Victim) {}
        
    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (Reached == false)
        {
            if (spell->Id == SPELL_DISPELL||spell->Id == SPELL_PURGE)
			{
				Reached  = true;
				m_creature->DealDamage(m_creature,m_creature->GetHealth(),NULL,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,NULL,false);
			}
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
        
        if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
        {
            //If we are within range melee the target
            if(!Reached)
                if(m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
                {
                        m_creature->CastSpell(m_creature->getVictim(), DARK_FIEND_DEBUFF, true);
						Reached = true;
                        m_creature->DealDamage(m_creature,m_creature->GetHealth(),NULL,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,NULL,false);   
                }
        }
    }
}; 
struct MANGOS_DLL_DECL mob_voidsentinelAI : public ScriptedAI
{
    mob_voidsentinelAI(Creature *c) : ScriptedAI(c) { Reset(); }
 
    uint32 AuraTimer;
    uint32 BlastTimer;

    void Reset() 
    {   
        AuraTimer   = 3000;
        BlastTimer  = 15000;
        if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            AttackStart(target);
    }
    
    void JustDied(Unit* Killer) 
    {
        m_creature->ForcedDespawn();
        for(uint8 i=0; i<8; ++i)
        {
			// void spawn AI handled by EventAI
            if(Creature* voidSpawn = m_creature->SummonCreature(ID_SPAWN, m_creature->GetPositionX()+rand()%2, m_creature->GetPositionY()+rand()%2, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
				voidSpawn->getVictim();
        }
    }
    void KilledUnit(Unit *Victim) {}
      
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
        
		//shadow pulse
        if(AuraTimer < diff)
        {
            DoCast(m_creature, VOID_AURA);
            AuraTimer = 3000;
        }else AuraTimer -=diff;


        if(BlastTimer < diff)
        {
            DoCast(m_creature->getVictim(), VOID_BLAST, true);
            BlastTimer = 20000;
        }else BlastTimer -= diff;

        DoMeleeAttackIfReady();

    }
};


struct MANGOS_DLL_DECL mob_singularityAI : public ScriptedAI
{
    mob_singularityAI(Creature *c) : ScriptedAI(c) { Reset(); }
 
    uint32 ChangeTargetTimer;
    uint32 LifeTime;

    void Reset()                    
    {
        m_creature->SetDisplayId(25206);
        m_creature->CastSpell(m_creature, AURA_SINGULARITY, true);  //Sigularity aura
        m_creature->CastSpell(m_creature, SPELL_ARCANEFORM, true);
        ChangeTargetTimer = 5000;
        LifeTime = 22000;
        if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            AttackStart(target);
    }
    void Aggro(Unit *who)           {} 
    void JustDied(Unit* Killer)     {}
    void KilledUnit(Unit *Victim)   {}
      
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

        if(ChangeTargetTimer < diff)
        {
            if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                AttackStart(target);
            ChangeTargetTimer = 5000;
        }else ChangeTargetTimer -= diff;

        if(LifeTime < diff)
        {
            m_creature->DealDamage(m_creature,m_creature->GetHealth(),NULL,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,NULL,false);
            m_creature->ForcedDespawn();
        }else LifeTime -= diff;
    }
};

 
CreatureAI* GetAI_boss_muru(Creature *_Creature)
{
    return new boss_muruAI(_Creature);
}
 
CreatureAI* GetAI_dark_fiend(Creature *_Creature)
{
    return new dark_fiendAI(_Creature);
}
 
CreatureAI* GetAI_mob_voidsentinel(Creature *_Creature)
{
    return new mob_voidsentinelAI(_Creature);
}

CreatureAI* GetAI_mob_singularity(Creature *_Creature)
{
    return new mob_singularityAI(_Creature);
}

void AddSC_boss_muru_entropius()
{
    Script *newscript;
        
    newscript = new Script;
    newscript->Name="boss_muru";
    newscript->GetAI = &GetAI_boss_muru;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="dark_fiend";
    newscript->GetAI = &GetAI_dark_fiend;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_voidsentinel";
    newscript->GetAI = &GetAI_mob_voidsentinel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_singularity";
    newscript->GetAI = &GetAI_mob_singularity;
    newscript->RegisterSelf();
}

