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
SDName: Boss_Keristrasza
SD%Complete: 95%
SDComment:
SDCategory: The Nexus, The Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum eEnums
{
    CONTAINMENT_SPHERES                             = 3,

	//Achievements
    ACHIEVEMENT_INTENSE_COLD                        = 2036,
	ACHIEVEMENT_NEXUS_N								= 478,
	ACHIEVEMENT_NEXUS_H								= 490,

    //Spells
    SPELL_FROZEN_PRISON                             = 47854,
    SPELL_TAIL_SWEEP                                = 50155,
    SPELL_CRYSTAL_CHAINS                            = 50997,
    SPELL_ENRAGE                                    = 8599,
    SPELL_CRYSTALFIRE_BREATH_N                      = 48096,
    SPELL_CRYSTALFIRE_BREATH_H                      = 57091,
    SPELL_CRYSTALIZE                                = 48179,
    SPELL_INTENSE_COLD                              = 48094,
    SPELL_INTENSE_COLD_TRIGGERED                    = 48095,

    //Yell
    SAY_AGGRO                                       = -1576040,
    SAY_SLAY                                        = -1576041,
    SAY_ENRAGE                                      = -1576042,
    SAY_DEATH                                       = -1576043,
    SAY_CRYSTAL_NOVA                                = -1576044
	
	/*
	(26723,-1576040,'Preserve? Why? There''s no truth in it. No no no... only in the taking! I see that now!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,13450,1,0,0,'keristrasza SAY_AGGRO'),
   (26723,-1576041,'Now we''ve come to the truth!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,13453,1,0,0,'keristrasza SAY_SLAY'),
   (26723,-1576042,'Finish it! FINISH IT! Kill me, or I swear by the Dragonqueen you''ll never see daylight again!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,13452,1,0,0,'keristrasza SAY_ENRAGE'),
   (26723,-1576043,'Dragonqueen... Life-Binder... preserve... me.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,13454,1,0,0,'keristrasza SAY_DEATH'),
   (26723,-1576044,'Stay. Enjoy your final moments.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,13451,1,0,0,'keristrasza SAY_CRYSTAL_NOVA'),*/  ///missing texts need id numbers double checked and impleted
};

struct MANGOS_DLL_DECL boss_keristraszaAI : public ScriptedAI
{
    boss_keristraszaAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

    uint32 CRYSTALFIRE_BREATH_Timer;
    uint32 CRYSTAL_CHAINS_CRYSTALIZE_Timer;
    uint32 TAIL_SWEEP_Timer;
    bool Enrage;

    uint64 ContainmentSphereGUIDs[CONTAINMENT_SPHERES];

    uint32 CheckIntenseColdTimer;
    bool MoreThanTwoIntenseCold; // needed for achievement: Intense Cold(2036)

    void Reset()
    {
        CRYSTALFIRE_BREATH_Timer = 14000;
		CRYSTAL_CHAINS_CRYSTALIZE_Timer = m_bIsRegularMode ? 30000 : 11000;
        TAIL_SWEEP_Timer = 5000;
        Enrage = false;

        CheckIntenseColdTimer = 2000;
        MoreThanTwoIntenseCold = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

        RemovePrison(CheckContainmentSpheres());

        if (m_pInstance)
            m_pInstance->SetData(DATA_KERISTRASZA_EVENT, NOT_STARTED);
    }

    void EnterCombat(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        DoCast(m_creature->getVictim(), SPELL_INTENSE_COLD);

        if (m_pInstance)
            m_pInstance->SetData(DATA_KERISTRASZA_EVENT, IN_PROGRESS);
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_bIsRegularMode && !MoreThanTwoIntenseCold)
        {
            AchievementEntry const *AchievIntenseCold = GetAchievementStore()->LookupEntry(ACHIEVEMENT_INTENSE_COLD);
            if (AchievIntenseCold)
            {
                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->CompletedAchievement(AchievIntenseCold);
                }
            }
        }

		// complete nexus achievements, not really blizzlike (should complete criterias)
		// normal achievement on both difficulties
		AchievementEntry const *AchievNexusNorm = GetAchievementStore()->LookupEntry(ACHIEVEMENT_NEXUS_N);
		if (AchievNexusNorm)
		{
			Map* pMap = m_creature->GetMap();
			if (pMap && pMap->IsDungeon())
			{
				Map::PlayerList const &players = pMap->GetPlayers();
				for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
				itr->getSource()->CompletedAchievement(AchievNexusNorm);
			}
		}
		if(!m_bIsRegularMode)
		{
			AchievementEntry const *AchievNexusHero = GetAchievementStore()->LookupEntry(ACHIEVEMENT_NEXUS_H);
			if (AchievNexusHero)
			{
				Map* pMap = m_creature->GetMap();
				if (pMap && pMap->IsDungeon())
				{
					Map::PlayerList const &players = pMap->GetPlayers();
					for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
					itr->getSource()->CompletedAchievement(AchievNexusHero);
				}
			}
		}

        if (m_pInstance)
            m_pInstance->SetData(DATA_KERISTRASZA_EVENT, DONE);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    bool CheckContainmentSpheres(bool remove_prison = false)
    {
        if(!m_pInstance)
            return false;

        ContainmentSphereGUIDs[0] = m_pInstance->GetData64(ANOMALUS_CONTAINMET_SPHERE);
        ContainmentSphereGUIDs[1] = m_pInstance->GetData64(ORMOROKS_CONTAINMET_SPHERE);
        ContainmentSphereGUIDs[2] = m_pInstance->GetData64(TELESTRAS_CONTAINMET_SPHERE);

        GameObject *ContainmentSpheres[CONTAINMENT_SPHERES];

        for (uint8 i = 0; i < CONTAINMENT_SPHERES; ++i)
        {
            ContainmentSpheres[i] = m_pInstance->instance->GetGameObject(ContainmentSphereGUIDs[i]);
            if (!ContainmentSpheres[i])
                return false;
            if (ContainmentSpheres[i]->GetGoState() != GO_STATE_ACTIVE)
                return false;
        }
        if (remove_prison)
            RemovePrison(true);
        return true;
    }

    void RemovePrison(bool remove)
    {
        if (remove)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (m_creature->HasAura(SPELL_FROZEN_PRISON))
                m_creature->RemoveAurasDueToSpell(SPELL_FROZEN_PRISON);
        }
        else
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoCast(m_creature, SPELL_FROZEN_PRISON, false);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CheckIntenseColdTimer < diff && !MoreThanTwoIntenseCold)
        {
            std::list<HostileReference*> ThreatList = m_creature->getThreatManager().getThreatList();
            for (std::list<HostileReference*>::const_iterator itr = ThreatList.begin(); itr != ThreatList.end(); itr++)
            {
                Unit *pTarget = m_creature->GetMap()->GetCreature((*itr)->getUnitGuid());
                if (!pTarget || pTarget->GetTypeId() != TYPEID_PLAYER)
                    continue;

                Aura *AuraIntenseCold = pTarget->GetAura(SPELL_INTENSE_COLD_TRIGGERED, EFFECT_INDEX_0);
                if (AuraIntenseCold && AuraIntenseCold->GetStackAmount() > 2)
                {
                    MoreThanTwoIntenseCold = true;
                    break;
                }
            }
            CheckIntenseColdTimer = 2000;
        } else CheckIntenseColdTimer -= diff;

        if (!Enrage && (m_creature->GetHealth() < m_creature->GetMaxHealth() * 0.25))
        {
            DoScriptText(SAY_ENRAGE, m_creature);
            DoCast(m_creature, SPELL_ENRAGE);
            Enrage = true;
        }

        if (CRYSTALFIRE_BREATH_Timer <= diff)
        {
			DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CRYSTALFIRE_BREATH_N : SPELL_CRYSTALFIRE_BREATH_H);
            CRYSTALFIRE_BREATH_Timer = 14000;
        } else CRYSTALFIRE_BREATH_Timer -=diff;

        if (TAIL_SWEEP_Timer <= diff)
        {
            DoCast(m_creature, SPELL_TAIL_SWEEP);
            TAIL_SWEEP_Timer = 5000;
        } else TAIL_SWEEP_Timer -=diff;

        if (CRYSTAL_CHAINS_CRYSTALIZE_Timer <= diff)
        {
            DoScriptText(SAY_CRYSTAL_NOVA, m_creature);
            if (!m_bIsRegularMode)
                DoCast(m_creature, SPELL_CRYSTALIZE);
            else if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_CRYSTAL_CHAINS);
			CRYSTAL_CHAINS_CRYSTALIZE_Timer = m_bIsRegularMode ? 30000 : 11000;
        } else CRYSTAL_CHAINS_CRYSTALIZE_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_keristrasza(Creature* pCreature)
{
    return new boss_keristraszaAI (pCreature);
}

bool GOUse_containment_sphere(Player *pPlayer, GameObject *pGO)
{
    ScriptedInstance *pInstance = (ScriptedInstance *)pGO->GetInstanceData();

    Creature *Keristrasza = pGO->GetMap()->GetCreature(pInstance->GetData64(DATA_KERISTRASZA));
    if (Keristrasza && Keristrasza->isAlive())
    {
        // maybe these are hacks :(
        pGO->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
        pGO->SetGoState(GO_STATE_ACTIVE);

        CAST_AI(boss_keristraszaAI, Keristrasza->AI())->CheckContainmentSpheres(true);
    }
    return true;
}

void AddSC_boss_keristrasza()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_keristrasza";
    newscript->GetAI = &GetAI_boss_keristrasza;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_containment_sphere";
    newscript->pGOUse = &GOUse_containment_sphere;
    newscript->RegisterSelf();
}
