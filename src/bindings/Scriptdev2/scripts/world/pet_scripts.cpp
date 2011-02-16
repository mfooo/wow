/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * Copyright (C) 2011 Infinity_SD2
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
SDName: Pet_Scripts
SD%Complete: 20%
SDComment: Here is the place for scripted AI for guardian and mini pets.
SDCategory: Pets
EndScriptData */

#include "precompiled.h"
#include "PetAI.h"
#include "Pet.h"
#include "Totem.h"
#include "SpellMgr.h"
#include "ObjectGuid.h"


struct MANGOS_DLL_DECL pet_simple_guardianAI : public PetAI
{
    /* This AI is more a kind of additional initialisation,
       that adds spells to the autocast list, defined in
       CreatureInfo::Spells.
       The AI functions are processed by PetAI.
    */
    pet_simple_guardianAI(Pet* pPet): PetAI(pPet)
    {
        // add spells if any
         if (CreatureInfo const* pPetInfo = pPet->GetCreatureInfo())
            for (uint8 i = 0; i<CREATURE_MAX_SPELLS; i++)
            {
                if (!pPetInfo->spells[i])
                  continue;

                if (SpellEntry const *spellInfo = GetSpellStore()->LookupEntry(pPetInfo->spells[i]))
                {
                    // skip spells without any cooldown
                    if (!spellInfo->StartRecoveryTime && !GetSpellRecoveryTime(spellInfo) && !(spellInfo->Attributes & SPELL_ATTR_PASSIVE))
                        continue;
                    // in case applying stat auras, we need to set it modifiable temporary
                    pPet->SetCanModifyStats(true);
                    pPet->addSpell(pPetInfo->spells[i], spellInfo->Attributes & SPELL_ATTR_PASSIVE ? ACT_PASSIVE : ACT_ENABLED);
                    pPet->SetCanModifyStats(false);
                }
            }
    }
};

/*######
## pet_greater_earth_elemental
######*/

enum
{
    SPELL_AOE_TAUNT         = 36213,
    SPELL_IMMUNITY_NATURE   = 7941
};

struct MANGOS_DLL_DECL pet_greater_earth_elementalAI : public PetAI
{
    pet_greater_earth_elementalAI(Pet* pPet) : PetAI(pPet)
    {
        pPet->addSpell(SPELL_IMMUNITY_NATURE);
        Reset();
    }

    uint32 m_timer;

    void Reset()
    {
        m_timer = 500;
    }

    Unit* GetOriginalOwner()
    {
        Unit* owner = ((Pet*)m_creature)->GetOwner();
        if (!owner)
            return NULL;

        if (owner->GetTypeId() != TYPEID_UNIT || !((Creature*)owner)->IsTotem())
            return NULL;

        return ((Totem*)owner)->GetOwner();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        PetAI::UpdateAI(uiDiff);

        // kind of workaround to react also on non-hostile attackers
        if (!m_creature->getVictim())
            if (Unit* origOwner = GetOriginalOwner())
            {
                Unit* pTarget = origOwner->getAttackerForHelper();
                if (pTarget && pTarget->IsWithinDist(m_creature, 35.0f))
                    AttackStart(pTarget);
            }

        if (m_timer < uiDiff)
        {
            if (m_creature->getVictim() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                m_creature->CastSpell(m_creature->getVictim(), SPELL_AOE_TAUNT, false);
                m_timer = urand(2000, 6000);
            }
        }
        else
            m_timer -= uiDiff;
    }
};


/*######
## pet_greater_fire_elemental
######*/

enum
{
    SPELL_FIRE_NOVA = 12470,
    SPELL_FIRE_BLAST = 57984,
    SPELL_IMMUNITY_FIRE = 7942
};

struct MANGOS_DLL_DECL pet_greater_fire_elementalAI : public PetAI
{
    pet_greater_fire_elementalAI(Pet* pPet) : PetAI(pPet)
    {
        // Immunity: Fire
        pPet->addSpell(SPELL_IMMUNITY_FIRE);
        Reset();
    }

    uint32 m_timer_blast, m_timer_nova;
    int32 m_damage_blast, m_damage_nova;

    void Reset()
    {
        m_timer_blast = 500;
        m_timer_nova  = 500;

        // custom damage, as the core calculated damage of this spells is quite high
        // (SPELL_ATTR_LEVEL_DAMAGE_CALCULATION)
        m_damage_blast  = 47;
        m_damage_nova   = 76;
    }

    Unit* GetOriginalOwner()
    {
        Unit* owner = ((Pet*)m_creature)->GetOwner();
        if (!owner)
            return NULL;

        if (owner->GetTypeId() != TYPEID_UNIT || !((Creature*)owner)->IsTotem())
            return NULL;

        return ((Totem*)owner)->GetOwner();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        PetAI::UpdateAI(uiDiff);

        // kind of workaround to react also on non-hostile attackers
        if (!m_creature->getVictim())
            if (Unit* origOwner = GetOriginalOwner())
            {
                Unit* pTarget = origOwner->getAttackerForHelper();
                if (pTarget && pTarget->IsWithinDist(m_creature, 35.0f))
                    AttackStart(pTarget);
            }

        // fire nova
        if (m_timer_nova < uiDiff)
        {
            if (m_creature->getVictim() && m_creature->getVictim()->IsWithinDist(m_creature, 6.0f) && !m_creature->IsNonMeleeSpellCasted(false))
            {
                m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_FIRE_NOVA, &m_damage_nova, 0, 0, false);
                m_timer_nova = urand(5000, 10000);
                m_timer_blast = 3000;
            }
        }
        else
            m_timer_nova -= uiDiff;

        // fire blast
        if (m_timer_blast < uiDiff)
        {
            if (m_creature->getVictim() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                // cast fireblast always if out of range, and only sometimes when in melee range
                bool cast = true;;
                if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
                    cast = !urand(0,2) ? false : true;
                if (cast)
                    m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_FIRE_BLAST, &m_damage_blast, 0, 0, false);
                m_timer_blast = 3000;
                return;
            }
        }
        else
            m_timer_blast -= uiDiff;
    }
};


CreatureAI* GetAI_pet_simple_guardian(Creature* pCreature)
{
    if (pCreature->IsPet())
        return new pet_simple_guardianAI((Pet*)pCreature);
    else
        return NULL;
}

CreatureAI* GetAI_pet_greater_earth_elemental(Creature* pCreature)
{
    if (pCreature->IsPet())
        return new pet_greater_earth_elementalAI((Pet*)pCreature);
    else
        return NULL;
}

CreatureAI* GetAI_pet_greater_fire_elemental(Creature* pCreature)
{
    if (pCreature->IsPet())
        return new pet_greater_fire_elementalAI((Pet*)pCreature);
    else
        return NULL;
}

void AddSC_pets()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "pet_simple_guardian";
    newscript->GetAI = &GetAI_pet_simple_guardian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "pet_greater_earth_elemental";
    newscript->GetAI = &GetAI_pet_greater_earth_elemental;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "pet_greater_fire_elemental";
    newscript->GetAI = &GetAI_pet_greater_fire_elemental;
    newscript->RegisterSelf();

}
