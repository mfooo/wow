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
SDName: Silvermoon_City
SD%Complete: 100
SDComment: Quest support: 9685
SDCategory: Silvermoon City
EndScriptData */

/* ContentData
npc_blood_knight_stillblade
EndContentData */

#include "precompiled.h"

/*#######
# npc_blood_knight_stillblade
#######*/

#define SAY_HEAL                        -1000193

#define QUEST_REDEEMING_THE_DEAD        9685
#define SPELL_SHIMMERING_VESSEL         31225
#define SPELL_REVIVE_SELF               32343
#define NPC_BLOOD_KNIGHT_STILLBLADE     17768

struct MANGOS_DLL_DECL npc_blood_knight_stillbladeAI : public ScriptedAI
{
    npc_blood_knight_stillbladeAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 lifeTimer;
    bool spellHit;

    void Reset()
    {
        lifeTimer = 120000;
        m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
        spellHit = false;
    }

    void MoveInLineOfSight(Unit *who) { }

    void UpdateAI(const uint32 diff)
    {
        if (m_creature->IsStandState())
        {
            if (lifeTimer < diff)
                m_creature->AI()->EnterEvadeMode();
            else
                lifeTimer -= diff;
        }
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if ((Spellkind->Id == SPELL_SHIMMERING_VESSEL) && !spellHit &&
            (Hitter->GetTypeId() == TYPEID_PLAYER) && (((Player*)Hitter)->IsActiveQuest(QUEST_REDEEMING_THE_DEAD)))
        {
            //((Player*)Hitter)->AreaExploredOrEventHappens(QUEST_REDEEMING_THE_DEAD);
            ((Player*)Hitter)->KilledMonsterCredit(NPC_BLOOD_KNIGHT_STILLBLADE);
            DoCastSpellIfCan(m_creature,SPELL_REVIVE_SELF);
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
            m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
            //m_creature->RemoveAllAuras();
            DoScriptText(SAY_HEAL, m_creature, Hitter);
            spellHit = true;
        }
    }
};

CreatureAI* GetAI_npc_blood_knight_stillblade(Creature* pCreature)
{
    return new npc_blood_knight_stillbladeAI(pCreature);
}

void AddSC_silvermoon_city()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "npc_blood_knight_stillblade";
    newscript->GetAI = &GetAI_npc_blood_knight_stillblade;
    newscript->RegisterSelf();
}
