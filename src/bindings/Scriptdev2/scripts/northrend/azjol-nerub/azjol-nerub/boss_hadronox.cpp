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
SDName: boss_hadronox
SD%Complete: ?%
SDComment: by MaxXx2021
SDCategory: Azjol-Nerub instance
EndScriptData */
/*
* Comment: No Waves atm and the doors spells are crazy...
*
* When your group enters the main room (the one after the bridge), you will notice a group of 3 Nerubians.
* When you engage them, 2 more groups like this one spawn behind the first one - it is important to pull the first group back,
* so you don't aggro all 3. Hadronox will be under you, fighting Nerubians.
*
* This is the timed gauntlet - waves of non-elite spiders
* will spawn from the 3 doors located a little above the main room, and will then head down to fight Hadronox. After clearing the
* main room, it is recommended to just stay in it, kill the occasional non-elites that will attack you instead of the boss, and wait for
* Hadronox to make his way to you. When Hadronox enters the main room, she will web the doors, and no more non-elites will spawn.
*/

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SPELL_ACID_CLOUD                              = 53400, // Victim
    SPELL_LEECH_POISON                            = 53030, // Victim
    SPELL_PIERCE_ARMOR                            = 53418, // Victim
    SPELL_WEB_GRAB                                = 57731, // Victim
    SPELL_WEB_FRONT_DOORS                         = 53177, // Self
    SPELL_WEB_SIDE_DOORS                          = 53185, // Self
};

struct MANGOS_DLL_DECL boss_hadronoxAI : public ScriptedAI
{
    boss_hadronoxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
        bFirstTime = true;
    }

    ScriptedInstance* pInstance;

    uint32 uiAcidTimer;
    uint32 uiLeechTimer;
    uint32 uiPierceTimer;
    uint32 uiGrabTimer;
    uint32 uiDoorsTimer;
    uint32 uiCheckDistanceTimer;

    bool bFirstTime;

    float fMaxDistance;

    void Reset()
    {
        m_creature->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 9.0f);
        m_creature->SetFloatValue(UNIT_FIELD_COMBATREACH, 9.0f);

        uiAcidTimer = urand(10000,14000);
        uiLeechTimer = urand(3000,9000);
        uiPierceTimer = urand(1000,3000);
        uiGrabTimer = urand(15000,19000);
        uiDoorsTimer = urand(20000,30000);
        uiCheckDistanceTimer = 2000;

        fMaxDistance = 50.0f;

        if (pInstance && (pInstance->GetData(TYPE_HADRONOX) != DONE && !bFirstTime))
            pInstance->SetData(TYPE_HADRONOX, FAIL);

        bFirstTime = false;
    }

    //when Hadronox kills any enemy (that includes a party member) she will regain 10% of her HP if the target had Leech Poison on
    void KilledUnit(Unit* Victim)
    {
        // not sure if this aura check is correct, I think it is though
        if (!Victim || !Victim->HasAura(SPELL_LEECH_POISON) || !m_creature->isAlive())
            return;

        uint32 health = m_creature->GetMaxHealth()/10;

        if ((m_creature->GetHealth()+health) >= m_creature->GetMaxHealth())
            m_creature->SetHealth(m_creature->GetMaxHealth());
        else
            m_creature->SetHealth(m_creature->GetHealth()+health);
    }

    void JustDied(Unit* Killer)
    {
        if (pInstance)
            pInstance->SetData(TYPE_HADRONOX, DONE);
    }

    void EnterCombat(Unit* who)
    {
        if (pInstance)
            pInstance->SetData(TYPE_HADRONOX, IN_PROGRESS);
        m_creature->SetInCombatWithZone();
    }

    void CheckDistance(float dist, const uint32 uiDiff)
    {
        if (!m_creature->isInCombat())
            return;

        float x=0.0f, y=0.0f, z=0.0f;
        m_creature->GetRespawnCoord(x,y,z);

        if (uiCheckDistanceTimer <= uiDiff)
            uiCheckDistanceTimer = 5000;
        else
        {
            uiCheckDistanceTimer -= uiDiff;
            return;
        }
        if (m_creature->IsInEvadeMode() || !m_creature->getVictim())
            return;
        if (m_creature->GetDistance(x,y,z) > dist)
            EnterEvadeMode();
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Without he comes up through the air to players on the bridge after krikthir if players crossing this bridge!
        CheckDistance(fMaxDistance, diff);

        if (m_creature->HasAura(SPELL_WEB_FRONT_DOORS) || m_creature->HasAura(SPELL_WEB_SIDE_DOORS))
        {
            if (IsCombatMovement())
                SetCombatMovement(false);
        }
        else if (!IsCombatMovement())
            SetCombatMovement(true);

        if (uiPierceTimer <= diff)
        {
            DoCast(m_creature->getVictim(), SPELL_PIERCE_ARMOR);
            uiPierceTimer = 8000;
        } else uiPierceTimer -= diff;

        if (uiAcidTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_ACID_CLOUD);

            uiAcidTimer = urand(20000,30000);
        } else uiAcidTimer -= diff;

        if (uiLeechTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_LEECH_POISON);

            uiLeechTimer = urand(11000,14000);
        } else uiLeechTimer -= diff;

        if (uiGrabTimer <= diff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)) // Draws all players (and attacking Mobs) to itself.
                DoCast(pTarget, SPELL_WEB_GRAB);

            uiGrabTimer = urand(15000,30000);
        } else uiGrabTimer -= diff;

        if (uiDoorsTimer <= diff)
        {
            //DoCast(me, urand(SPELL_WEB_FRONT_DOORS, SPELL_WEB_SIDE_DOORS));
            uiDoorsTimer = urand(30000,60000);
        } else uiDoorsTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hadronox(Creature* pCreature)
{
    return new boss_hadronoxAI (pCreature);
}

void AddSC_boss_hadronox()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_hadronox";
    newscript->GetAI = &GetAI_boss_hadronox;
    newscript->RegisterSelf();
}
