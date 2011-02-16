/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Thaddius
SDAuthor: ckegg && FallenangelX
SD%Complete: 0
SDComment: Placeholder. Includes Feugen & Stalagg.
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"
enum
{
    //Stalagg
    SAY_STAL_AGGRO         = -1533023,
    SAY_STAL_SLAY          = -1533024,
    SAY_STAL_DEATH         = -1533025,
    SPELL_POWERSURGE        = 28134,
    H_SPELL_POWERSURGE      = 54529,

    //Feugen
    SAY_FEUG_AGGRO         = -1533026,
    SAY_FEUG_SLAY          = -1533027,
    SAY_FEUG_DEATH         = -1533028,
    SPELL_STATICFIELD      = 28135,
    H_SPELL_STATICFIELD    = 54528,

    //both
    SPELL_MAGNETIC_PULL       = 28337,
    SPELL_WAR_STOMP           = 56427, // walkaround spell when adds are not on platforms

    //Thaddus
    SAY_AGGRO              = -1533029,
    SAY_KILL1              = -1533031,
    SAY_KILL2              = -1533032,
    SAY_KILL3              = -1533033,
    SAY_KILL4              = -1533034,
    SAY_DEATH              = -1533035,
    SAY_SCREAM1            = -1533036,
    SAY_SCREAM2            = -1533037,
    SAY_SCREAM3            = -1533038,
    SAY_SCREAM4            = -1533039,

    SPELL_BALL_LIGHTNING                = 28299,
    SPELL_CHAIN_LIGHTNING              = 28167,
    H_SPELL_CHAIN_LIGHTNING            = 54531,
    SPELL_BERSERK                      = 27680,

    SPELL_POLARITY_SHIFT               = 28089,
    SPELL_CHARGE_POSITIVE_ICON         = 28059,
    SPELL_CHARGE_POSITIVE_DMG          = 28062,
    SPELL_CHARGE_POSITIVE_BUFF         = 29659,
    SPELL_CHARGE_NEGATIVE_ICON         = 28084,
	SPELL_CHARGE_NEGATIVE_DMG          = 28085,
    SPELL_CHARGE_NEGATIVE_BUFF         = 29660
};

struct MANGOS_DLL_DECL boss_thaddiusAI : public ScriptedAI
{
    boss_thaddiusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsPolarityShift;
    bool Activated;
    bool ActivationPrepare;
    bool ActivationBegin;

    uint32 ChainLightning_Timer;
    uint32 BallLightning_Timer;
    uint32 PolarityShift_Timer;
    uint32 PolarityCheck_Timer;
    uint32 PolarityBeginDmg_Timer;
    bool PolarityDmgAllowed;
    uint32 Enrage_Timer;
    uint32 Scream_Timer;
    uint32 Activate_Timer;

    void Reset()
    {
        m_bIsPolarityShift = false;
        Activated = false;

        Activate_Timer = 1000;
        ActivationPrepare = false;
        ActivationBegin = false;
        ChainLightning_Timer = 15000;
        BallLightning_Timer = 1000;
        PolarityShift_Timer = 30000;
        PolarityCheck_Timer = 30000;
        PolarityDmgAllowed = false;
        PolarityBeginDmg_Timer = 35000;
        Enrage_Timer = 360000;
        Scream_Timer = 60000+rand()%30000;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (m_pInstance)
        {
            if (Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
                pStalagg->Respawn();

            if (Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                pFeugen->Respawn();
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, FAIL);
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, IN_PROGRESS);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || !Activated)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if (IsCombatMovement())
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!Activated || !m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (Activate_Timer < uiDiff)
            {
                if (ActivationBegin)
                {
                    Activated = true;
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    Map* map = m_creature->GetMap();
                    if (map->IsDungeon())
                    {
                        Map::PlayerList const &PlayerList = map->GetPlayers();
                        if (!PlayerList.isEmpty())
                            AttackStart(PlayerList.begin()->getSource());
                    }
                    return;
                }

                if (ActivationPrepare)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    ActivationBegin = true;
                    Activate_Timer = 2000;
                    return;
                }

                if (m_pInstance)
                {
                    Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG));
                    Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN));
                    if (pStalagg && pFeugen && pStalagg->isDead() && pFeugen->isDead())
                    {
                        ActivationPrepare = true;
                        Activate_Timer = 13000;
                        return;
                    }
                }

                Activate_Timer = 1000;
            }
            else
                Activate_Timer -= uiDiff;

            return;
        }

        if (Enrage_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BERSERK, true);
            Enrage_Timer = 360000;
        }
        else
            Enrage_Timer -= uiDiff;

        if (ChainLightning_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : H_SPELL_CHAIN_LIGHTNING, true);
            ChainLightning_Timer = 13000+rand()%4000;
        }
        else
            ChainLightning_Timer -= uiDiff;

        if (BallLightning_Timer < uiDiff)
        {
            Unit* pTarget = m_creature->getVictim();
            if (pTarget && !pTarget->IsWithinDistInMap(m_creature, ATTACK_DISTANCE))
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_BALL_LIGHTNING, true);

            BallLightning_Timer = 1000;
        }
        else
            BallLightning_Timer -= uiDiff;

        if (PolarityShift_Timer < uiDiff)
        {
            PolarityCheck_Timer = 0;
            PolarityDmgAllowed = false;
            PolarityBeginDmg_Timer = 5000;

            DoCastSpellIfCan(m_creature, SPELL_POLARITY_SHIFT); // need core support

            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isGameMaster())
                        continue;
                    i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_ICON);
                    i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_ICON);
                    i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_BUFF);
                    i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_BUFF);
                    uint32 uiSpell = roll_chance_i(50) ? SPELL_CHARGE_POSITIVE_ICON : SPELL_CHARGE_NEGATIVE_ICON;
                    if (SpellEntry* TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(uiSpell))
                    {
                        TempSpell->EffectTriggerSpell[1] = 0;
                        i->getSource()->CastCustomSpell(i->getSource(), TempSpell, NULL, NULL, NULL, true);
                    }
                }
            }

            PolarityShift_Timer = 30000;
        }
        else
            PolarityShift_Timer -= uiDiff;

        if (PolarityBeginDmg_Timer < uiDiff)
        {
            PolarityDmgAllowed = true;
            PolarityBeginDmg_Timer = 30000;
        }
        else
            PolarityBeginDmg_Timer -= uiDiff;

        if (PolarityCheck_Timer < uiDiff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_BUFF);
                    i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_BUFF);
                }

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isGameMaster())
                        continue;

                    Map::PlayerList const &PlayerList2 = map->GetPlayers();

                    if (PlayerList2.isEmpty())
                        continue;

                    uint32 IconSpell;
                    uint32 OppositeSpell;
                    uint32 DmgSpell;
                    uint32 BuffSpell;
                    if (i->getSource()->HasAura(SPELL_CHARGE_POSITIVE_ICON))
                    {
                        OppositeSpell = SPELL_CHARGE_NEGATIVE_ICON;
                        IconSpell = SPELL_CHARGE_POSITIVE_ICON;
                        DmgSpell = SPELL_CHARGE_POSITIVE_DMG;
                        BuffSpell = SPELL_CHARGE_POSITIVE_BUFF;
                    }
                    else if (i->getSource()->HasAura(SPELL_CHARGE_NEGATIVE_ICON))
                    {
                        OppositeSpell = SPELL_CHARGE_POSITIVE_ICON;
                        IconSpell = SPELL_CHARGE_NEGATIVE_ICON;
                        DmgSpell = SPELL_CHARGE_NEGATIVE_DMG;
                        BuffSpell = SPELL_CHARGE_NEGATIVE_BUFF;
                    }
                    else
                        continue;

                    for (Map::PlayerList::const_iterator j = PlayerList2.begin(); j != PlayerList2.end(); ++j)
                    {
                        if (j->getSource() == i->getSource() || j->getSource()->isGameMaster())
                            continue;

                        if (i->getSource()->IsWithinDistInMap(j->getSource(), 10.0f))
                        {
                            if (j->getSource()->HasAura(IconSpell))
                                j->getSource()->CastSpell(j->getSource(), BuffSpell, true);
                            if (PolarityDmgAllowed && j->getSource()->HasAura(OppositeSpell))
                                if (SpellEntry* TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(DmgSpell))
                                {
                                    TempSpell->EffectImplicitTargetA[0] = TARGET_SELF;
                                    TempSpell->EffectImplicitTargetB[0] = 0;
                                    int32 dmg = m_bIsRegularMode ? 3500 : 4500;
                                    j->getSource()->CastCustomSpell(j->getSource(), TempSpell, &dmg, NULL, NULL, true);
                                }
                        }
                    }
                }
            }

            PolarityCheck_Timer = 1000;
        }
        else
            PolarityCheck_Timer -= uiDiff;

        if (Scream_Timer < uiDiff)
        {
            switch(rand()%4)
            {
                case 0: DoScriptText(SAY_SCREAM1, m_creature);break;
                case 1: DoScriptText(SAY_SCREAM2, m_creature);break;
                case 2: DoScriptText(SAY_SCREAM3, m_creature);break;
                case 3: DoScriptText(SAY_SCREAM4, m_creature);break;
            }
            Scream_Timer = 60000+rand()%30000;
        }
        else
            Scream_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, DONE);

        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_ICON);
                i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_ICON);
                i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_BUFF);
                i->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_BUFF);
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch (rand()%4)
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
            case 2: DoScriptText(SAY_KILL3, m_creature); break;
            case 3: DoScriptText(SAY_KILL4, m_creature); break;
        }
    }
};

struct MANGOS_DLL_DECL mob_stalaggAI : public ScriptedAI
{
    mob_stalaggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bResurrectionNeeded;

    float HomeX, HomeY;

    uint32 PowerSurge_Timer;
    uint32 DeathCheck_Timer;
    uint32 Punish_Timer;
    uint32 MagneticPull_Timer;


    void Reset()
    {
        m_bResurrectionNeeded = false;

        HomeX = 3450.45f;
        HomeY = -2931.42f;

        PowerSurge_Timer = 10000+rand()%10000;
        DeathCheck_Timer = 1000;
        Punish_Timer = 1000;
        MagneticPull_Timer = 20000;

        if (m_pInstance)
            if (Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
            {
                if (pFeugen->isDead())
                    pFeugen->Respawn();
                if (pFeugen->isInCombat())
                    if (Unit* pTarget = pFeugen->getVictim())
                        m_creature->AI()->AttackStart(pTarget);
            }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_STAL_AGGRO, m_creature);

        if (m_pInstance)
            if (Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                pFeugen->AI()->AttackStart(pWho);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_STAL_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_STAL_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (DeathCheck_Timer < uiDiff)
        {
            if (m_pInstance)
                if (Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                    if (pFeugen->isDead())
                        if (m_bResurrectionNeeded)
                        {
                            pFeugen->Respawn();
                            m_bResurrectionNeeded = false;
                        }
                        else
                            m_bResurrectionNeeded = true;

            if (m_bResurrectionNeeded)
                DeathCheck_Timer = 5000;
            else
                DeathCheck_Timer = 1000;
        }
        else
            DeathCheck_Timer -= uiDiff;

        if (PowerSurge_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_POWERSURGE : H_SPELL_POWERSURGE);
            PowerSurge_Timer = 10000+rand()%10000;
        }
        else
            PowerSurge_Timer -= uiDiff;

        if (MagneticPull_Timer < uiDiff)
        {
            if (m_pInstance)
                if (Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                {
                    Unit* pFeugenTarget = pFeugen->getVictim();
                    Unit* pStalaggTarget = m_creature->getVictim();
                    if (pFeugen->isAlive() && pFeugenTarget && pStalaggTarget)
                    {
                        float pStalaggTargetThreat = m_creature->getThreatManager().getThreat(pStalaggTarget);
                        float pFeugenTargetThreat = pFeugen->getThreatManager().getThreat(pFeugenTarget);
                        pFeugen->CastSpell(pStalaggTarget, SPELL_MAGNETIC_PULL, false);
                        m_creature->CastSpell(pFeugenTarget, SPELL_MAGNETIC_PULL, false);
                        if (HostileReference* ref = pFeugen->getThreatManager().getCurrentVictim())
                            ref->removeReference();
                        if (HostileReference* ref = m_creature->getThreatManager().getCurrentVictim())
                            ref->removeReference();
                        pFeugen->AddThreat(pStalaggTarget, pStalaggTargetThreat);
                        m_creature->AddThreat(pFeugenTarget, pFeugenTargetThreat);
                    }
                }
            MagneticPull_Timer = 20000;
        }
        else
            MagneticPull_Timer -= uiDiff;

        if (Punish_Timer < uiDiff)
        {
            if (m_creature->GetDistance2d(HomeX, HomeY) > 30.0f)
                DoCastSpellIfCan(m_creature, SPELL_WAR_STOMP);
            Punish_Timer = 1000;
        }
        else
            Punish_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_feugenAI : public ScriptedAI
{
    mob_feugenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bResurrectionNeeded;

    float HomeX, HomeY;

    uint32 StaticField_Timer;
    uint32 DeathCheck_Timer;
    uint32 Punish_Timer;

    void Reset()
    {
        m_bResurrectionNeeded = false;

        HomeX = 3508.14f;
        HomeY = -2988.65f;

        StaticField_Timer = 3000;
        DeathCheck_Timer = 1000;
        Punish_Timer = 1000;

        if (m_pInstance)
            if (Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
            {
                if (pStalagg->isDead())
                    pStalagg->Respawn();
                if (pStalagg->isInCombat())
                    if (Unit* pTarget = pStalagg->getVictim())
                        m_creature->AI()->AttackStart(pTarget);
            }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_FEUG_AGGRO, m_creature);

        if (m_pInstance)
            if (Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
                pStalagg->AI()->AttackStart(pWho);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_FEUG_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_FEUG_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (DeathCheck_Timer < uiDiff)
        {
            if (m_pInstance)
                if (Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
                    if (pStalagg->isDead())
                        if (m_bResurrectionNeeded)
                        {
                            pStalagg->Respawn();
                            m_bResurrectionNeeded = false;
                        }
                        else
                            m_bResurrectionNeeded = true;

            if (m_bResurrectionNeeded)
                DeathCheck_Timer = 5000;
            else
                DeathCheck_Timer = 1000;
        }
        else
            DeathCheck_Timer -= uiDiff;

        if (StaticField_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STATICFIELD : H_SPELL_STATICFIELD);
            StaticField_Timer = 3000;
        }else StaticField_Timer -= uiDiff;

        if (Punish_Timer < uiDiff)
        {
            if (m_creature->GetDistance2d(HomeX, HomeY) > 30.0f)
                DoCastSpellIfCan(m_creature, SPELL_WAR_STOMP);
            Punish_Timer = 1000;
        }
        else
            Punish_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thaddius(Creature* pCreature)
{
    return new boss_thaddiusAI(pCreature);
}

CreatureAI* GetAI_mob_stalagg(Creature* pCreature)
{
    return new mob_stalaggAI(pCreature);
}

CreatureAI* GetAI_mob_feugen(Creature* pCreature)
{
    return new mob_feugenAI(pCreature);
}

void AddSC_boss_thaddius()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_thaddius";
    newscript->GetAI = &GetAI_boss_thaddius;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_stalagg";
    newscript->GetAI = &GetAI_mob_stalagg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_feugen";
    newscript->GetAI = &GetAI_mob_feugen;
    newscript->RegisterSelf();
}
