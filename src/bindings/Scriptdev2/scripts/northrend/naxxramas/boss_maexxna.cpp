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
SDName: Boss_Maexxna
SD%Complete: 60
SD author:FallenangelX
SDComment: this needs review, and rewrite of the webwrap ability
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SPELL_WEBWRAP          = 28622,                       //Spell is normally used by the webtrap on the wall NOT by Maexxna
    SPELL_WEBSPRAY         = 29484,
    H_SPELL_WEBSPRAY       = 54125,
    SPELL_POISONSHOCK      = 28741,
    H_SPELL_POISONSHOCK    = 54122,
    SPELL_NECROTICPOISON   = 54121,
    H_SPELL_NECROTICPOISON = 28776,
    SPELL_FRENZY           = 54123,
    H_SPELL_FRENZY         = 54124,

    NPC_MAEXXNA_SPIDERLING = 17055,
	NPC_WEB_WRAP           = 16486
};

#define LOC_X1                  3546.796f
#define LOC_Y1                  -3869.082f
#define LOC_Z1                  296.450f

#define LOC_X2                  3531.271f
#define LOC_Y2                  -3847.424f
#define LOC_Z2                  299.450f

#define LOC_X3                  3497.067f
#define LOC_Y3                  -3843.384f
#define LOC_Z3                  302.384f

struct MANGOS_DLL_DECL mob_webwrapAI : public ScriptedAI
{
    mob_webwrapAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint64 victimGUID;

    void Reset()
    {
        victimGUID = 0;
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void SetVictim(uint64 victim)
    {
        if (victim)
        {
            victimGUID = victim;
            if (Unit* pVictim = m_creature->GetMap()->GetUnit(victimGUID))
                pVictim->CastSpell(pVictim, SPELL_WEBWRAP, true);
        }
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = m_creature->GetMap()->GetUnit(victimGUID))
            pVictim->RemoveAurasDueToSpell(SPELL_WEBWRAP);
    }
};

struct MANGOS_DLL_DECL boss_maexxnaAI : public ScriptedAI
{
    boss_maexxnaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 WebWrap_Timer;
    uint32 WebSpray_Timer;
    uint32 PoisonShock_Timer;
    uint32 NecroticPoison_Timer;
    uint32 SummonSpiderling_Timer;
    bool Enraged;

    void Reset()
    {
        WebWrap_Timer = 20000;                              //20 sec init, 40 sec normal
        WebSpray_Timer = 40000;                             //40 seconds
        PoisonShock_Timer = 10000;                          //10 seconds
        NecroticPoison_Timer = 30000;                       //30 seconds
        SummonSpiderling_Timer = 30000;                     //30 sec init, 40 sec normal
        Enraged = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, FAIL);
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, DONE);
    }

    void Aggro(Unit *who)
    {
        m_creature->SetInCombatWithZone();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, IN_PROGRESS);
    }

    void DoCastSpellIfCanWebWrap()
    {
        float LocX, LocY, LocZ;
        switch(rand()%3)
        {
            case 0:
                LocX = LOC_X1 + rand()%5; LocY = LOC_Y1 + rand()%5; LocZ = LOC_Z1 + 1;
                break;
            case 1:
                LocX = LOC_X2 + rand()%5; LocY = LOC_Y2 + rand()%5; LocZ = LOC_Z2 + 1;
                break;
            case 2:
                LocX = LOC_X3 + rand()%5; LocY = LOC_Y3 + rand()%5; LocZ = LOC_Z3 + 1;
                break;
        }

        ThreatList const& lThreatList = m_creature->getThreatManager().getThreatList();
        std::list<Unit*> lTargets;
        bool bTank = true;
        for (ThreatList::const_iterator i = lThreatList.begin(); i != lThreatList.end(); ++i)
        {
            if (bTank)
            {
                bTank = false;
                continue;
            }
            Unit* pTemp = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());
            if (pTemp && (pTemp->GetTypeId() == TYPEID_PLAYER) && !pTemp->HasAura(SPELL_WEBWRAP))
                lTargets.push_back(pTemp);
        }
        Unit* pTarget = NULL;
        if (!lTargets.empty())
        {
            std::list<Unit*>::iterator pTemp = lTargets.begin();
            advance(pTemp, (rand() % lTargets.size()));
            pTarget = *pTemp;
            if (pTarget)
            {
                DoTeleportPlayer(pTarget, LocX, LocY, LocZ, pTarget->GetOrientation());
                if (Creature* pWrap = m_creature->SummonCreature(NPC_WEB_WRAP, LocX, LocY, LocZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
                    ((mob_webwrapAI*)pWrap->AI())->SetVictim(pTarget->GetGUID());
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //WebTrap_Timer
        if (WebWrap_Timer < diff)
        {
            m_creature->MonsterTextEmote("%s spins her web into a cocoon!", 0, true);
            DoCastSpellIfCanWebWrap();
            if (!m_bIsRegularMode)
                DoCastSpellIfCanWebWrap();
            WebWrap_Timer = 40000;
        }else WebWrap_Timer -= diff;

        //WebSpray_Timer
        if (WebSpray_Timer < diff)
        {
            m_creature->MonsterTextEmote("%s sprays strands of web everywhere!", 0, true);
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WEBSPRAY : H_SPELL_WEBSPRAY);
            WebSpray_Timer = 40000;
        }else WebSpray_Timer -= diff;

        //PoisonShock_Timer
        if (PoisonShock_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POISONSHOCK : H_SPELL_POISONSHOCK);
            PoisonShock_Timer = 10000;
        }else PoisonShock_Timer -= diff;

        //NecroticPoison_Timer
        if (NecroticPoison_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_NECROTICPOISON : H_SPELL_NECROTICPOISON);
            NecroticPoison_Timer = 30000;
        }else NecroticPoison_Timer -= diff;

        //SummonSpiderling_Timer
        if (SummonSpiderling_Timer < diff)
        {
            m_creature->MonsterTextEmote("Spiderlings appear on the web!", 0, true);
            for (uint8 i = 0; i < 8; ++i)
                if (Creature* pSpiderling = DoSpawnCreature(NPC_MAEXXNA_SPIDERLING, urand(0, 5), urand(0, 5), 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000))
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        pSpiderling->AI()->AttackStart(pTarget);

            SummonSpiderling_Timer = 40000;
        }else SummonSpiderling_Timer -= diff;

        //Enrage if not already enraged and below 30%
        if (!Enraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : H_SPELL_FRENZY);
            Enraged = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_webwrap(Creature* pCreature)
{
    return new mob_webwrapAI(pCreature);
}

CreatureAI* GetAI_boss_maexxna(Creature* pCreature)
{
    return new boss_maexxnaAI(pCreature);
}

void AddSC_boss_maexxna()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_maexxna";
    newscript->GetAI = &GetAI_boss_maexxna;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_webwrap";
    newscript->GetAI = &GetAI_mob_webwrap;
    newscript->RegisterSelf();
}
