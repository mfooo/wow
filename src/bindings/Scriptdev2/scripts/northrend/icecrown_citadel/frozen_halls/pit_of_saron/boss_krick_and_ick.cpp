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
SDName: boss_ick && boss_krick
SD%Complete: 1%
SDComment: by ..., modified by /dev/rsa
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"
enum
{
    //common
    SPELL_BERSERK                         = 47008,
    //yells
    //summons
    NPC_EXPLODING_ORB                     = 36610,
    //Abilities
    SPELL_FEAR                            = 68950,
    SPELL_EXPLOSIVE_ORB                   = 69019,
    SPELL_EXPLOSIVE                       = 69012,
    SPELL_SHADOWBOLT                      = 69028,
    SPELL_STRANGULATE                     = 69413,
    SPELL_TOXIC                           = 69024,
    SPELL_KICK                            = 69021,
    SPELL_POISON                          = 68989,
    SPELL_POISON_H                        = 70434,
    SPELL_PURSUIT                         = 68987,
    SPELL_PUSTULANT                       = 69581,
    SPELL_CONFUSION                       = 69029,
};

struct MANGOS_DLL_DECL boss_krickAI : public ScriptedAI
{
    boss_krickAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_KRICK, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_KRICK, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_KRICK, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_ickAI : public ScriptedAI
{
    boss_ickAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    void Reset()
    {
    }

    void Aggro(Unit *who) 
    {
    }

    void JustDied(Unit *killer)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_krick(Creature* pCreature)
{
    return new boss_krickAI(pCreature);
}

CreatureAI* GetAI_boss_ick(Creature* pCreature)
{
    return new boss_ickAI(pCreature);
}

struct MANGOS_DLL_DECL mob_exploding_orbAI : public ScriptedAI
{
   mob_exploding_orbAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
   }

   ScriptedInstance* pInstance;
   uint32 ExplodeTimer;

    void Reset()
    {
      ExplodeTimer = 18000;
    }

    void AttackStart(Unit* who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!pInstance) return;

        if (ExplodeTimer < diff)
        {
           DoCast(m_creature, SPELL_EXPLOSIVE_ORB);
           m_creature->ForcedDespawn();
        } else ExplodeTimer -= diff;
        return;
    }

};

CreatureAI* GetAI_mob_exploding_orb(Creature* pCreature)
{
    return new mob_exploding_orbAI(pCreature);
}


enum
{
    SAY_AGGRO                           = -1658024,
    SAY_SLAY_1                          = -1658025,
    SAY_SLAY_2                          = -1658026,
    SAY_ORDER_STOP                      = -1658027,
    SAY_ORDER_BLOW                      = -1658028,
    SAY_TARGET_1                        = -1658029,
    SAY_TARGET_2                        = -1658030,
    SAY_TARGET_3                        = -1658031,

    EMOTE_KRICK_MINES                   = -1658032,
    EMOTE_ICK_POISON                    = -1658033,
    EMOTE_ICK_CHASING                   = -1658034,

    SAY_OUTRO_1                         = -1658035,
    SAY_JAINA_KRICK_1                   = -1658036,
    SAY_SYLVANAS_KRICK_1                = -1658037,
    SAY_OUTRO_2                         = -1658038,
    SAY_JAINA_KRICK_2                   = -1658039,
    SAY_SYLVANAS_KRICK_2                = -1658040,
    SAY_OUTRO_3                         = -1658041,
    SAY_TYRANNUS_KRICK_1                = -1658042,
    SAY_OUTRO_4                         = -1658043,
    SAY_TYRANNUS_KRICK_2                = -1658044,
    SAY_JAINA_KRICK_3                   = -1658045,
    SAY_SYLVANAS_KRICK_3                = -1658046,
};

void AddSC_boss_krick_and_ick()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_krick";
    newscript->GetAI = &GetAI_boss_krick;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_ick";
    newscript->GetAI = &GetAI_boss_ick;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_exploding_orb";
    newscript->GetAI = &GetAI_mob_exploding_orb;
    newscript->RegisterSelf();

}
