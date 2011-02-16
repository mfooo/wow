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
SDName: Boss_Kalecgos
SD%Complete: 40
SDComment: Script must be considered not complete.
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum KalecgosEncounter
{
    //kalecgos dragon form
    SAY_EVIL_AGGRO                  = -1580000,
    SAY_EVIL_SPELL1                 = -1580001,
    SAY_EVIL_SPELL2                 = -1580002,
    SAY_EVIL_SLAY1                  = -1580003,
    SAY_EVIL_SLAY2                  = -1580004,
    SAY_EVIL_ENRAGE                 = -1580005,

    //kalecgos humanoid form
    SAY_GOOD_AGGRO                  = -1580006,
    SAY_GOOD_NEAR_DEATH             = -1580007,
    SAY_GOOD_NEAR_DEATH2            = -1580008,
    SAY_GOOD_PLRWIN                 = -1580009,

	//Sathrovarr
    SAY_SATH_AGGRO                  = -1580010,
    SAY_SATH_DEATH                  = -1580011,
    SAY_SATH_SPELL1                 = -1580012,
    SAY_SATH_SPELL2                 = -1580013,
    SAY_SATH_SLAY1                  = -1580014,
    SAY_SATH_SLAY2                  = -1580015,
    SAY_SATH_ENRAGE                 = -1580016,

    //Kalecgos
    SPELL_SPECTRAL_BLAST_DUMMY      = 44869,
    SPELL_SPECTRAL_BLAST            = 44866,	

    SPELL_ARCANE_BUFFET             = 45018,	
    SPELL_FROST_BREATH              = 44799,	
    SPELL_HEROIC_STRIKE             = 45026,	
    SPELL_REVITALIZE                = 45027,	
    SPELL_TAIL_LASH                 = 45122,	
    SPELL_TRANSFORM_KALEC           = 45027,				//this should be 70993 ?
    SPELL_CRAZED_RAGE               = 44806,                // this should be 44807 instead

     //Sathrovarr
    SPELL_SPECTRAL_INVIS            = 44801,
    SPELL_CORRUPTING_STRIKE         = 45029,
    SPELL_CURSE_OF_BOUNDLESS_AGONY  = 45032,
    SPELL_SHADOW_BOLT_VOLLEY        = 45031,

    //Misc
    SPELL_BANISH                    = 44836,		
};

bool bSathalive;		
bool bKalecalive;	

//#define  DEMON_REALM_Z				(float)-74.558


uint32 WildMagic[]= { 44978, 45001, 45002, 45004, 45006, 45010 };

const float KALECGOS_ARENA[3] = { 1704.34f, 928.17f, 53.08f };

#define NOTIFY_SPECTRALLY_EXHAUSTED      "Your body is too exhausted to travel to the Spectral Realm."

//kalecgos dragon 
struct MANGOS_DLL_DECL boss_kalecgosAI : public ScriptedAI
{
    boss_kalecgosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiArcaneBuffetTimer;
    uint32 m_uiFrostBreathTimer;
    uint32 m_uiWildMagicTimer;
	uint32 m_uiTailLashTimer;
    uint32 m_uiSpectralBlastTimer;
    uint32 m_uiExitTimer;

    bool m_bUncorrupted;
    bool m_bBanished;
    bool m_bChecked;
    bool m_bEnraged;
    bool m_bHasSpectralTarget;

    Unit* pKiller;

    void Reset()
    {
        m_uiArcaneBuffetTimer       = 8000;
        m_uiFrostBreathTimer        = 24000;
        m_uiWildMagicTimer          = 18000;
        m_uiSpectralBlastTimer      = urand(20000, 25000);
		m_uiTailLashTimer           = urand(25000, 40000);

        m_uiExitTimer = 0;

        m_bUncorrupted = false;
        m_bBanished    = false;
        m_bChecked     = false;
        m_bEnraged     = false;
        m_bHasSpectralTarget = false;

        pKiller = 0;
    }

    void JustReachedHome()	
    {
        if (m_pInstance)
        {
            // Reset Sathrovarr too
            if (Creature* pSath = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SATHROVARR)))
            {
                if (pSath->isAlive() && pSath->getVictim())
                    pSath->AI()->EnterEvadeMode();
            }

            m_pInstance->SetData(TYPE_KALECGOS, NOT_STARTED);
        }
    }

    void Aggro(Unit* who)	
    {
        DoScriptText(SAY_EVIL_AGGRO, m_creature);

		bSathalive=true;		
		bKalecalive=true;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KALECGOS, IN_PROGRESS);		
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage >= m_creature->GetHealth() && done_by != m_creature)
        {
            if (!m_bUncorrupted)
            {
                damage = 0;
                m_bBanished = true;
                DoCast(m_creature, SPELL_BANISH, true);
                m_creature->GetMotionMaster()->MoveIdle();
            }
            else
            {
                pKiller = done_by;
                damage = 0;
                BeginOutro();
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_EVIL_SLAY1 : SAY_EVIL_SLAY2, m_creature);
    }

    void SendToInnerVeil(Unit* pTarget)
    {
        if (m_pInstance)
        {      
			pTarget->CastSpell(pTarget, SPELL_SPECTRAL_EXHAUSTION, true);
			m_pInstance->SetData64(DATA_PLAYER_SPECTRAL_REALM, pTarget->GetGUID());
			((Player*)pTarget)->TeleportTo(pTarget->GetMapId(), pTarget->GetPositionX(), pTarget->GetPositionY(), DEMON_REALM_Z, pTarget->GetOrientation());
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_SPECTRAL_BLAST_DUMMY && !m_bHasSpectralTarget)
        {
            if (pTarget->GetTypeId() != TYPEID_PLAYER)
                return;

            if (pTarget->HasAura(SPELL_SPECTRAL_EXHAUSTION, EFFECT_INDEX_0))
                return;

            if (pTarget == m_creature->getVictim())					
                return;

            m_bHasSpectralTarget = true;
            pTarget->CastSpell(pTarget, SPELL_SPECTRAL_BLAST, true);
            SendToInnerVeil(pTarget);
        }
    }

    void BeginOutro()
    {
        debug_log("SD2: KALEC: Beginning Outro");

        if (!m_pInstance)
            return;

        if (Creature* pSathrovarr = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SATHROVARR)))
        {
            if (pSathrovarr->isAlive())
            {
                pSathrovarr->NearTeleportTo(KALECGOS_ARENA[0], KALECGOS_ARENA[1], KALECGOS_ARENA[2], 0.0f);
                if(pKiller)
                    pKiller->DealDamage(pSathrovarr, pSathrovarr->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else
		    pSathrovarr->DealDamage(pSathrovarr, pSathrovarr->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            
	    }
        }

        if (Creature* pKalec = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_KALECGOS_HUMAN)))
        {
            pKalec->DeleteThreatList();
            pKalec->SetVisibility(VISIBILITY_OFF);
        }

        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->setFaction(35);
        DoScriptText(SAY_GOOD_PLRWIN, m_creature);
        m_uiExitTimer = 1000;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim() || !m_creature->SelectHostileTarget() || m_bBanished)
            return;

		//enrage kalecgos & sathrovarr @ kalecgos low health
        if (!m_bEnraged && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10))
        {
            if (Creature* pSathrovarr = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SATHROVARR)))
            {
                if (pSathrovarr->isAlive())
                    pSathrovarr->CastSpell(pSathrovarr, SPELL_CRAZED_RAGE, true);
            }

            m_creature->CastSpell(m_creature, SPELL_CRAZED_RAGE, true);
            m_bEnraged = true;
        }
		//banish when corrupted or begin outro when not corrupted(=sathrovarr dead)
        if (!m_bChecked && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 1))
        {
            m_bChecked = true;

            if (!m_bUncorrupted)
            {
                m_bBanished = true;
                DoCastSpellIfCan(m_creature, SPELL_BANISH, true);
                m_creature->GetMotionMaster()->MoveIdle();
            }
            else
            {
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_KALECGOS, DONE);
                BeginOutro();
            }
        }
		//let kalecgos exit the arena on outro
        if (m_uiExitTimer)
        {
            if (m_uiExitTimer <= diff)
            {
                debug_log("SD2: KALEC: Exiting the arena");

                float x, y, z;
                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 30, x, y, z);

                z = 70.0f;

                m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
                m_uiExitTimer = 0;
            }else m_uiExitTimer -= diff;
        }

        if (m_uiArcaneBuffetTimer < diff)
        {
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_BUFFET) == CAST_OK)
            {
                if (!urand(0, 2))
                    DoScriptText(SAY_EVIL_SPELL1, m_creature);

                //m_uiArcaneBuffetTimer = 20000;
                m_uiArcaneBuffetTimer = urand(15000, 20000);
            }
        }
        else m_uiArcaneBuffetTimer -= diff;

        if (m_uiFrostBreathTimer < diff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_BREATH) == CAST_OK)
            {
                if (!urand(0, 1))
                    DoScriptText(SAY_EVIL_SPELL2, m_creature);

                //m_uiFrostBreathTimer = 25000;
                m_uiFrostBreathTimer = urand(15000, 25000);
            }
        }
        else m_uiFrostBreathTimer -= diff;

        if (m_uiWildMagicTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, WildMagic[rand()%6]);

            m_uiWildMagicTimer = 19000;
        }
        else m_uiWildMagicTimer -= diff;

		if (m_uiTailLashTimer < diff)
        {
            //if (m_creature->getVictim())
			DoCastSpellIfCan(m_creature, SPELL_TAIL_LASH);
            m_uiTailLashTimer = urand(25000, 40000);
        }else m_uiTailLashTimer -= diff;

        if (m_uiSpectralBlastTimer < diff)
        {
			
			if (bKalecalive && bSathalive)		//no spectral blast if kalecgos_humanoid or Sathrovarr dead
			{
				m_bHasSpectralTarget = false;
				m_creature->CastSpell(m_creature, SPELL_SPECTRAL_BLAST_DUMMY, false);
				m_uiSpectralBlastTimer = 30000;
			}
        }
        else m_uiSpectralBlastTimer -= diff;

        if (!m_bBanished)
            DoMeleeAttackIfReady();
    }
};

//Sathrovarr
struct MANGOS_DLL_DECL boss_sathrovarrAI : public ScriptedAI
{
    boss_sathrovarrAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 CorruptingStrikeTimer;
    uint32 CurseOfBoundlessAgonyTimer;
    uint32 ShadowBoltVolleyTimer;
    bool m_bBanished;
    bool m_bEnraged;

    void Reset()
    {
        CorruptingStrikeTimer = 5000;
        CurseOfBoundlessAgonyTimer = 15000;
        ShadowBoltVolleyTimer = 10000;

        m_bBanished = false;
        m_bEnraged  = false;
        
        //summon kalecgos_human
        m_creature->SummonCreature(24891,1702.3f,938.f,-74.55f,4.93f,TEMPSUMMON_CORPSE_DESPAWN,10000);

        //m_creature->CastSpell(m_creature, SPELL_SPECTRAL_INVIS, true);	//buggy		
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_SATH_AGGRO, m_creature);
        if (!m_pInstance)
            return;

        if (Creature* pKalec = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KALECGOS_HUMAN)))
        {
            m_creature->AddThreat(pKalec, 500000.0f);
            pKalec->AddThreat(m_creature, 500000.0f);
        }
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if(!m_pInstance)
            return;

            if (damage >= m_creature->GetHealth() && m_pInstance->GetData(TYPE_KALECGOS) != DONE)
            {
            damage = 0;
            DoCastSpellIfCan(m_creature, SPELL_BANISH, true);
            m_bBanished = true;
            DoScriptText(SAY_SATH_DEATH, m_creature);

            if (!m_pInstance)
                return;

            m_pInstance->SetData(DATA_SET_SPECTRAL_CHECK, 5000);

            if (Creature* pKalecgos = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_KALECGOS_DRAGON)))
            {
                if (boss_kalecgosAI* pKalecgosAI = dynamic_cast<boss_kalecgosAI*>(pKalecgos->AI()))
                {
                    pKalecgosAI->m_bChecked = false;
                    pKalecgosAI->m_bUncorrupted = true;
                }
            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SATH_SLAY1 : SAY_SATH_SLAY2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim() || !m_creature->SelectHostileTarget() || m_bBanished)
            return;

		if(m_creature->GetPositionZ() > DEMON_REALM_Z+5)
			m_creature->NearTeleportTo(m_creature->GetPositionX(),m_creature->GetPositionY(),DEMON_REALM_Z,m_creature->GetOrientation());
			

        if (!m_bEnraged && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 10))
        {
            if (Creature* pKalecgos = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KALECGOS_DRAGON)))
            {
                if (pKalecgos->isAlive())
                    pKalecgos->CastSpell(pKalecgos, SPELL_CRAZED_RAGE, true);
            }

            m_creature->CastSpell(m_creature, SPELL_CRAZED_RAGE, true);
            m_bEnraged = true;
        }

        if (CorruptingStrikeTimer < diff)
        {
            if (urand(0, 1))
                DoScriptText(SAY_SATH_SPELL2, m_creature);

            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CORRUPTING_STRIKE);
            //CorruptingStrikeTimer = 15000;
            CorruptingStrikeTimer = urand(10000, 15000);		
        }else CorruptingStrikeTimer -= diff;

        if (CurseOfBoundlessAgonyTimer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_CURSE_OF_BOUNDLESS_AGONY);

            CurseOfBoundlessAgonyTimer = 27000;		
        }else CurseOfBoundlessAgonyTimer -= diff;

        if (ShadowBoltVolleyTimer < diff)
        {
            if (urand(0, 1))
                DoScriptText(SAY_SATH_SPELL1, m_creature);

            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
            ShadowBoltVolleyTimer = 15000;			
        }else ShadowBoltVolleyTimer -= diff;

		if(!m_bBanished)
        DoMeleeAttackIfReady();
    }
	void JustDied(Unit* killer)			
    {
		bSathalive=false;			
    }
};



//kalecgos human
struct MANGOS_DLL_DECL boss_kalecgos_humanoidAI : public ScriptedAI
{
    boss_kalecgos_humanoidAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 RevitalizeTimer;
    uint32 HeroicStrikeTimer;

    bool HasYelled10Percent;
    bool HasYelled20Percent;

    void Reset()
    {
        //TODO: Times!
        RevitalizeTimer = 20000;
        HeroicStrikeTimer = 8000;

        HasYelled10Percent = false;
        HasYelled20Percent = false;

		if(m_pInstance)
            m_pInstance->SetData(TYPE_KALECGOS, NOT_STARTED);

        //m_creature->CastSpell(m_creature, SPELL_SPECTRAL_INVIS, true);	//buggy
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_GOOD_AGGRO, m_creature);
    }

    void JustDied(Unit* killer)
    {
		m_creature->SetVisibility(VISIBILITY_OFF);	
		bKalecalive=false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim() || !m_creature->SelectHostileTarget())
            return;

        if (RevitalizeTimer < diff)				
        {
            if (m_pInstance)
            {
               DoCastSpellIfCan(m_creature, SPELL_REVITALIZE, true);
               RevitalizeTimer = 30000;
            }
        }else RevitalizeTimer -= diff;
		
        if (HeroicStrikeTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HEROIC_STRIKE);
            HeroicStrikeTimer = 30000;
        }else HeroicStrikeTimer -= diff;
		
        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20) && !HasYelled20Percent)
        {
            DoScriptText(SAY_GOOD_NEAR_DEATH, m_creature);
            HasYelled20Percent = true;
        }

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10) && !HasYelled10Percent)
        {
            DoScriptText(SAY_GOOD_NEAR_DEATH2, m_creature);
            HasYelled10Percent = true;
        }

		DoMeleeAttackIfReady();
    }
};





//spectral rift

bool GOUse_go_spectral_rift(Player* pPlayer, GameObject* pGo)
{
    //if (pGo->GetGoType() != GAMEOBJECT_TYPE_GOOBER)
    //    return true;

    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        if (pPlayer->HasAura(SPELL_SPECTRAL_EXHAUSTION, EFFECT_INDEX_0))
		{
			pPlayer->GetSession()->SendNotification(NOTIFY_SPECTRALLY_EXHAUSTED);
            return true;
		}
        // Make them able to see Sathrovarr (he's invisible for some reason). Also, when this buff wears off, they get teleported back to Normal Realm (this is handled by Instance Script)
        //pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_TO_SPECTRAL_REALM, true);
		pPlayer->CastSpell(pPlayer, SPELL_SPECTRAL_EXHAUSTION, true);
		//pPlayer->CastSpell(pPlayer, SPELL_SPECTRAL_REALM_FORCE_FACTION, true);
        pInstance->SetData64(DATA_PLAYER_SPECTRAL_REALM, pPlayer->GetGUID());
		((Player*)pPlayer)->TeleportTo(pPlayer->GetMapId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), DEMON_REALM_Z, pPlayer->GetOrientation());
    }
    return true;
}

CreatureAI* GetAI_boss_kalecgos(Creature* pCreature)
{
    return new boss_kalecgosAI(pCreature);
}

CreatureAI* GetAI_boss_sathrovarr(Creature* pCreature)
{
    return new boss_sathrovarrAI(pCreature);
}

CreatureAI* GetAI_boss_kalecgos_humanoid(Creature* pCreature)
{
    return new boss_kalecgos_humanoidAI(pCreature);
}

void AddSC_boss_kalecgos()
{
    Script* newscript;

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_kalecgos;
    newscript->Name = "boss_kalecgos";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_sathrovarr;
    newscript->Name = "boss_sathrovarr";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_kalecgos_humanoid;
    newscript->Name = "boss_kalecgos_humanoid";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->pGOUse = &GOUse_go_spectral_rift;
    newscript->Name = "go_spectral_rift";
    newscript->RegisterSelf();
}  