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
SDName: Stratholme
SD%Complete: 100
SDComment: Misc mobs for instance. GO-script to apply aura and start event for quest 8945
SDCategory: Stratholme
EndScriptData */

/* ContentData
go_gauntlet_gate
mob_freed_soul
mob_restless_soul
mobs_spectral_ghostly_citizen
EndContentData */

#include "precompiled.h"
#include "stratholme.h"

/*######
## go_gauntlet_gate (this is the _first_ of the gauntlet gates, two exist)
######*/

bool GOUse_go_gauntlet_gate(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    if (pInstance->GetData(TYPE_BARON_RUN) != NOT_STARTED)
        return false;

    if (Group *pGroup = pPlayer->GetGroup())
    {
        for(GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* pGroupie = itr->getSource();
            if (!pGroupie)
                continue;

            if (pGroupie->GetQuestStatus(QUEST_DEAD_MAN_PLEA) == QUEST_STATUS_INCOMPLETE &&
                !pGroupie->HasAura(SPELL_BARON_ULTIMATUM, EFFECT_INDEX_0) &&
                pGroupie->GetMap() == pGo->GetMap())
                pGroupie->CastSpell(pGroupie,SPELL_BARON_ULTIMATUM,true);
        }
    }
    else
    {
        if (pPlayer->GetQuestStatus(QUEST_DEAD_MAN_PLEA) == QUEST_STATUS_INCOMPLETE &&
            !pPlayer->HasAura(SPELL_BARON_ULTIMATUM, EFFECT_INDEX_0) &&
            pPlayer->GetMap() == pGo->GetMap())
            pPlayer->CastSpell(pPlayer, SPELL_BARON_ULTIMATUM, true);
    }

    pInstance->SetData(TYPE_BARON_RUN,IN_PROGRESS);
    return false;
}

/*######
## mob_freed_soul
######*/

//Possibly more of these quotes around.
#define SAY_ZAPPED0 -1329000
#define SAY_ZAPPED1 -1329001
#define SAY_ZAPPED2 -1329002
#define SAY_ZAPPED3 -1329003

struct MANGOS_DLL_DECL mob_freed_soulAI : public ScriptedAI
{
    mob_freed_soulAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_ZAPPED0, m_creature); break;
            case 1: DoScriptText(SAY_ZAPPED1, m_creature); break;
            case 2: DoScriptText(SAY_ZAPPED2, m_creature); break;
            case 3: DoScriptText(SAY_ZAPPED3, m_creature); break;
        }
    }
};

CreatureAI* GetAI_mob_freed_soul(Creature* pCreature)
{
    return new mob_freed_soulAI(pCreature);
}

/*######
## mob_restless_soul
######*/

#define SPELL_EGAN_BLASTER  17368
#define SPELL_SOUL_FREED    17370
#define QUEST_RESTLESS_SOUL 5282
#define ENTRY_RESTLESS      11122
#define ENTRY_FREED         11136

struct MANGOS_DLL_DECL mob_restless_soulAI : public ScriptedAI
{
    mob_restless_soulAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 Tagger;
    uint32 Die_Timer;
    bool Tagged;

    void Reset()
    {
        Tagger = 0;
        Die_Timer = 5000;
        Tagged = false;
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if (!Tagged && spell->Id == SPELL_EGAN_BLASTER && ((Player*)caster)->GetQuestStatus(QUEST_RESTLESS_SOUL) == QUEST_STATUS_INCOMPLETE)
            {
                Tagged = true;
                Tagger = caster->GetGUID();
            }
        }
    }

    void JustSummoned(Creature *summoned)
    {
        summoned->CastSpell(summoned,SPELL_SOUL_FREED,false);
    }

    void JustDied(Unit* Killer)
    {
        if (Tagged)
            m_creature->SummonCreature(ENTRY_FREED, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 300000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Tagged)
        {
            if (Die_Timer < diff)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(Tagger))
                    pPlayer->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            else
                Die_Timer -= diff;
        }
    }
};

CreatureAI* GetAI_mob_restless_soul(Creature* pCreature)
{
    return new mob_restless_soulAI(pCreature);
}

/*######
## mobs_spectral_ghostly_citizen
######*/

enum
{
    SPELL_HAUNTING_PHANTOM  = 16336,
    SPELL_SLAP              = 6754
};

struct MANGOS_DLL_DECL mobs_spectral_ghostly_citizenAI : public ScriptedAI
{
    mobs_spectral_ghostly_citizenAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Die_Timer;
    bool Tagged;

    void Reset()
    {
        Die_Timer = 5000;
        Tagged = false;
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (!Tagged && spell->Id == SPELL_EGAN_BLASTER)
            Tagged = true;
    }

    void JustDied(Unit* Killer)
    {
        if (Tagged)
        {
            for(uint32 i = 1; i <= 4; ++i)
            {
                float x,y,z;
                m_creature->GetRandomPoint(m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),20.0f,x,y,z);

                //100%, 50%, 33%, 25% chance to spawn
                uint32 j = urand(1,i);
                if (j==1)
                    m_creature->SummonCreature(ENTRY_RESTLESS,x,y,z,0,TEMPSUMMON_CORPSE_DESPAWN,600000);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (Tagged)
        {
            if (Die_Timer < diff)
            {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            else
                Die_Timer -= diff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void ReceiveEmote(Player* pPlayer, uint32 emote)
    {
        switch(emote)
        {
            case TEXTEMOTE_DANCE:
                EnterEvadeMode();
                break;
            case TEXTEMOTE_RUDE:
                if (m_creature->IsWithinDistInMap(pPlayer, INTERACTION_DISTANCE))
                    m_creature->CastSpell(pPlayer,SPELL_SLAP,false);
                else
                    m_creature->HandleEmote(EMOTE_ONESHOT_RUDE);
                break;
            case TEXTEMOTE_WAVE:
                m_creature->HandleEmote(EMOTE_ONESHOT_WAVE);
                break;
            case TEXTEMOTE_BOW:
                m_creature->HandleEmote(EMOTE_ONESHOT_BOW);
                break;
            case TEXTEMOTE_KISS:
                m_creature->HandleEmote(EMOTE_ONESHOT_FLEX);
                break;
        }
    }
};

CreatureAI* GetAI_mobs_spectral_ghostly_citizen(Creature* pCreature)
{
    return new mobs_spectral_ghostly_citizenAI(pCreature);
}

void AddSC_stratholme()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_gauntlet_gate";
    pNewScript->pGOUse = &GOUse_go_gauntlet_gate;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_freed_soul";
    pNewScript->GetAI = &GetAI_mob_freed_soul;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_restless_soul";
    pNewScript->GetAI = &GetAI_mob_restless_soul;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mobs_spectral_ghostly_citizen";
    pNewScript->GetAI = &GetAI_mobs_spectral_ghostly_citizen;
    pNewScript->RegisterSelf();
}
