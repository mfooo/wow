/* Copyright (C) 2010 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_general
SD%Complete: ?%
SDComment: by /dev/rsa
SDCategory: General boss instance script template
EndScriptData */

#include "precompiled.h"
#include "def_instance.h"

enum BossSpells
{
    SPELL_SUPERSPELL         = 99999,
};

struct MANGOS_DLL_DECL boss_generalAI : public BSWScriptedAI
{
    boss_generalAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_GENERAL, NOT_STARTED);
        resetTimers();
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE) return;
    }

    void KilledUnit(Unit* pVictim)
    {
/*    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };*/
    }

    void JustReachedHome()
    {
        if (!pInstance) return;
        pInstance->SetData(TYPE_GENERAL, FAIL);
    }

    void JustSummoned(Creature* summoned)
    {
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_GENERAL, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return; 
        pInstance->SetData(TYPE_GENERAL, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_SUPERSPELL, diff);

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_general(Creature* pCreature)
{
    return new boss_generalAI(pCreature);
}

void AddSC_boss_general()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_general";
    newscript->GetAI = &GetAI_boss_general;
    newscript->RegisterSelf();
}
