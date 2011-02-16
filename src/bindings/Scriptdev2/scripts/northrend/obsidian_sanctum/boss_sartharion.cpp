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
SDName: Boss Sartharion
SD%Complete: 100%
SDComment: It's alive! ;) Now this is script is alive realy! (c) MaxXx2021 and PSZ
SDCategory: Obsidian Sanctum
EndScriptData */

#include "precompiled.h"
#include "obsidian_sanctum.h"

enum
{
    //Sartharion Yell
    SAY_SARTHARION_AGGRO                        = -1615018,
    SAY_SARTHARION_BERSERK                      = -1615019,
    SAY_SARTHARION_BREATH                       = -1615020,
    SAY_SARTHARION_CALL_SHADRON                 = -1615021,
    SAY_SARTHARION_CALL_TENEBRON                = -1615022,
    SAY_SARTHARION_CALL_VESPERON                = -1615023,
    SAY_SARTHARION_DEATH                        = -1615024,
    SAY_SARTHARION_SPECIAL_1                    = -1615025,
    SAY_SARTHARION_SPECIAL_2                    = -1615026,
    SAY_SARTHARION_SPECIAL_3                    = -1615027,
    SAY_SARTHARION_SPECIAL_4                    = -1615028,
    SAY_SARTHARION_SLAY_1                       = -1615029,
    SAY_SARTHARION_SLAY_2                       = -1615030,
    SAY_SARTHARION_SLAY_3                       = -1615031,

    WHISPER_LAVA_CHURN                          = -1615032,

    WHISPER_SHADRON_DICIPLE                     = -1615008,
    WHISPER_VESPERON_DICIPLE                    = -1615041,
    WHISPER_HATCH_EGGS                          = -1615017,
    WHISPER_OPEN_PORTAL                         = -1615042, // whisper, shared by two dragons

    //Sartharion Spells
    SPELL_BERSERK                               = 61632,    // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
    SPELL_CLEAVE                                = 56909,    // Inflicts 35% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
    SPELL_FLAME_BREATH                          = 56908,    // Inflicts 8750 to 11250 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_H                        = 58956,    // Inflicts 10938 to 14062 Fire damage to enemies in a cone in front of the caster.
    SPELL_TAIL_LASH                             = 56910,    // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TAIL_LASH_H                           = 58957,    // A sweeping tail strike hits all enemies behind the caster, inflicting 4375 to 5625 damage and stunning them for 2 sec.
    SPELL_WILL_OF_SARTHARION                    = 61254,    // Sartharion's presence bolsters the resolve of the Twilight Drakes, increasing their total health by 25%. This effect also increases Sartharion's health by 25%.
    SPELL_LAVA_STRIKE                           = 57571,    // (Real spell casted should be 57578) 57571 then trigger visual missile, then summon Lava Blaze on impact(spell 57572)
    SPELL_CYCLONE_AURA_2                        = 57598,
    SPELL_TWILIGHT_REVENGE                      = 60639,

    SPELL_PYROBUFFET                            = 56916,    // currently used for hard enrage after 15 minutes
    SPELL_PYROBUFFET_RANGE                      = 58907,    // possibly used when player get too far away from dummy creatures (2x creature entry 30494)

    SPELL_TWILIGHT_SHIFT_ENTER                  = 57620,    // enter phase. Player get this when click GO
    SPELL_TWILIGHT_SHIFT_DMG                    = 57874,    // damage during being in twilight realm
    SPELL_TWILIGHT_SHIFT_REMOVAL                = 61187,    // leave phase
    SPELL_TWILIGHT_SHIFT_REMOVAL_ALL            = 61190,    // leave phase (probably version to make all leave)

    //Mini bosses common spells
    SPELL_TWILIGHT_RESIDUE                      = 61885,    // makes immune to shadow damage, applied when leave phase

    //Miniboses (Vesperon, Shadron, Tenebron)
    SPELL_SHADOW_BREATH_H                       = 59126,    // Inflicts 8788 to 10212 Fire damage to enemies in a cone in front of the caster.
    SPELL_SHADOW_BREATH                         = 57570,    // Inflicts 6938 to 8062 Fire damage to enemies in a cone in front of the caster.

    SPELL_SHADOW_FISSURE_H                      = 59127,    // Deals 9488 to 13512 Shadow damage to any enemy within the Shadow fissure after 5 sec.
    SPELL_SHADOW_FISSURE                        = 57579,    // Deals 6188 to 8812 Shadow damage to any enemy within the Shadow fissure after 5 sec.

    //Vesperon
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    NPC_ACOLYTE_OF_VESPERON                     = 31219,    // Acolyte of Vesperon
    SPELL_POWER_OF_VESPERON                     = 61251,    // Vesperon's presence decreases the maximum health of all enemies by 25%.
    SPELL_TWILIGHT_TORMENT_VESP                 = 57948,    // (Shadow only) trigger 57935 then 57988
    SPELL_TWILIGHT_TORMENT_VESP_ACO             = 58853,    // (Fire and Shadow) trigger 58835 then 57988

    //Shadron
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    NPC_ACOLYTE_OF_SHADRON                      = 31218,    // Acolyte of Shadron
    SPELL_POWER_OF_SHADRON                      = 58105,    // Shadron's presence increases Fire damage taken by all enemies by 100%.
    SPELL_GIFT_OF_TWILIGTH_SHA                  = 57835,    // TARGET_SCRIPT shadron
    SPELL_GIFT_OF_TWILIGTH_SAR                  = 58766,    // TARGET_SCRIPT sartharion

    //Tenebron
    //in the portal spawns 6 eggs, if not killed in time (approx. 20s)  they will hatch,  whelps can cast 60708
    SPELL_POWER_OF_TENEBRON                     = 61248,    // Tenebron's presence increases Shadow damage taken by all enemies by 100%.
    //Tenebron, dummy spell
    SPELL_SUMMON_TWILIGHT_WHELP                 = 58035,    // doesn't work, will spawn NPC_TWILIGHT_WHELP
    SPELL_SUMMON_SARTHARION_TWILIGHT_WHELP      = 58826,    // doesn't work, will spawn NPC_SHARTHARION_TWILIGHT_WHELP

    SPELL_HATCH_EGGS_H                          = 59189,
    SPELL_HATCH_EGGS                            = 58542,
    SPELL_HATCH_EGGS_EFFECT_H                   = 59190,
    SPELL_HATCH_EGGS_EFFECT                     = 58685,

    NPC_TWILIGHT_EGG                            = 30882,

    //Whelps
    NPC_TWILIGHT_WHELP                          = 30890,
    NPC_SHARTHARION_TWILIGHT_WHELP              = 31214,
    SPELL_FADE_ARMOR                            = 60708,    // Reduces the armor of an enemy by 1500 for 15s

    //flame tsunami
    SPELL_FLAME_TSUNAMI                         = 57494,    // the visual dummy
    SPELL_FLAME_TSUNAMI_LEAP                    = 60241,    // SPELL_EFFECT_138 some leap effect, causing caster to move in direction
    SPELL_FLAME_TSUNAMI_DMG_AURA                = 57492,    // periodic damage, npc has this aura
    SPELL_FLAME_TSUNAMI_DMG                     = 57491,    // damage players
    SPELL_FLAME_TSUNAMI_BUFF                    = 60430,    // buff Lava Blazes

    NPC_FLAME_TSUNAMI                           = 30616,    // for the flame waves
    NPC_LAVA_BLAZE                              = 30643,    // adds spawning from flame strike
    NPC_FIRE_CYCLONE                            = 30648,

    //using these custom points for dragons start and end
    POINT_ID_INIT                               = 100,
    POINT_ID_LAND                               = 200
};

struct Waypoint
{
    float m_fX, m_fY, m_fZ;
};

//each dragons special points. First where fly to before connect to connon, second where land point is.
Waypoint m_aTene[]=
{
    {3212.854f, 575.597f, 109.856f},                        //init
    {3246.425f, 565.367f, 61.249f}                          //end
};

Waypoint m_aShad[]=
{
    {3293.238f, 472.223f, 106.968f},
    {3271.669f, 526.907f, 61.931f}
};

Waypoint m_aVesp[]=
{
    {3193.310f, 472.861f, 102.697f},
    {3227.268f, 533.238f, 59.995f}
};

//points around raid "isle", counter clockwise. should probably be adjusted to be more alike
Waypoint m_aDragonCommon[]=
{
    {3214.012f, 468.932f, 98.652f},
    {3244.950f, 468.427f, 98.652f},
    {3283.520f, 496.869f, 98.652f},
    {3287.316f, 555.875f, 98.652f},
    {3250.479f, 585.827f, 98.652f},
    {3209.969f, 566.523f, 98.652f}
};

float m_afTsunamiStartLoc[5][4]=
{
    //left to right
    {3201.0f, 487.75f, 58.6f, 6.23f},
    {3201.0f, 533.54f, 58.6f, 6.23f},
    {3201.0f, 579.14f, 58.6f, 6.23f},
    //right to left
    {3287.5f, 552.53f, 58.6f, 3.19f},
    {3287.5f, 511.10f, 58.6f, 3.19f},
};

uint64 m_uiAcolyteShadronGUID;
uint64 m_uiAcolyteVesperonGUID;
std::list<uint64> m_lEggsGUIDList;

/*######
## Boss Sartharion
######*/

struct MANGOS_DLL_DECL boss_sartharionAI : public ScriptedAI
{
    boss_sartharionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bTenebronHelpedInFight = false;
        m_bShadronHelpedInFight = false;
        m_bVesperonHelpedInFight = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsBerserk;
    bool m_bIsSoftEnraged;

    uint32 m_uiEnrageTimer;
    bool m_bIsHardEnraged;

    uint32 m_uiTenebronTimer;
    uint32 m_uiShadronTimer;
    uint32 m_uiVesperonTimer;

    uint32 m_uiFlameTsunamiTimer;
    uint32 m_uiFlameBreathTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiCycloneAuraTimer;

    bool m_bHasCalledTenebron;
    bool m_bHasCalledShadron;
    bool m_bHasCalledVesperon;
    bool m_bTenebronHelpedInFight;
    bool m_bShadronHelpedInFight;
    bool m_bVesperonHelpedInFight;

    bool bCanUseWill;
    bool bFirstWill;
    uint32 m_uiSarthHealth;
    uint32 m_uiTeneHealth;
    uint32 m_uiShadHealth;
    uint32 m_uiVespHealth;

    uint32 m_uiCheckTwilightTimer;

    std::list<Creature*> m_lFireCyclones;

    void Reset()
    {
        m_bIsBerserk = false;
        m_bIsSoftEnraged = false;

        m_uiEnrageTimer = MINUTE*15*IN_MILLISECONDS;
        m_bIsHardEnraged = false;

        m_uiTenebronTimer = 30000;
        m_uiShadronTimer = 75000;
        m_uiVesperonTimer = 120000;

        m_uiFlameTsunamiTimer = 30000;
        m_uiFlameBreathTimer = 20000;
        m_uiTailSweepTimer = 5000;
        m_uiCleaveTimer = 7000;
        m_uiCycloneAuraTimer = 10000;

        m_bHasCalledTenebron = false;
        m_bHasCalledShadron = false;
        m_bHasCalledVesperon = false;

        bCanUseWill = false;
        bFirstWill = true;

        m_uiCheckTwilightTimer = 2000;

        if (m_pInstance)
        {
            Creature* pTene = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TENEBRON));
            Creature* pShad = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SHADRON));
            Creature* pVesp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VESPERON));

            if (m_bTenebronHelpedInFight && pTene)
            {
                if (pTene->isDead())
                    pTene->Respawn();
                else
                    pTene->AI()->EnterEvadeMode();
            }

            if (m_bShadronHelpedInFight && pShad)
            {
                if (pShad->isDead())
                    pShad->Respawn();
                else
                    pShad->AI()->EnterEvadeMode();
            }

            if (m_bVesperonHelpedInFight && pVesp)
            {
                if (pVesp->isDead())
                    pVesp->Respawn();
                else
                    pVesp->AI()->EnterEvadeMode();
            }
        }

        m_lFireCyclones.clear();
        GetCreatureListWithEntryInGrid(m_lFireCyclones, m_creature, NPC_FIRE_CYCLONE, 100.0f);

        m_bTenebronHelpedInFight = false;
        m_bShadronHelpedInFight = false;
        m_bVesperonHelpedInFight = false;

        m_creature->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SAR);
        m_creature->RemoveAurasDueToSpell(SPELL_TWILIGHT_REVENGE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_SARTHARION_AGGRO,m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, IN_PROGRESS);
            FetchDragons();
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            uint8 uiHardMode = 0;
            if (m_bTenebronHelpedInFight)
                ++uiHardMode;
            if (m_bShadronHelpedInFight)
                ++uiHardMode;
            if (m_bVesperonHelpedInFight)
                ++uiHardMode;

            if (uiHardMode)
                m_creature->UpdateEntry(m_creature->GetEntry()*10+uiHardMode);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_SARTHARION_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0, 2))
        {
            case 0: DoScriptText(SAY_SARTHARION_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SARTHARION_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SARTHARION_SLAY_3, m_creature); break;
        }
    }

    void FetchDragons()
    {
        Creature* pTene = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TENEBRON));
        Creature* pShad = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SHADRON));
        Creature* pVesp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VESPERON));

        //if at least one of the dragons are alive and are being called
        if (pTene && pTene->isAlive() && !pTene->getVictim())
        {
            bCanUseWill = true;
            pTene->CastSpell(pTene, SPELL_POWER_OF_TENEBRON, false);
            pTene->AddSplineFlag(SPLINEFLAG_FLYING);
            pTene->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pTene->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aTene[0].m_fX, m_aTene[0].m_fY, m_aTene[0].m_fZ);

            if (!pTene->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pTene->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pShad && pShad->isAlive() && !pShad->getVictim())
        {
            bCanUseWill = true;
            pShad->CastSpell(pShad, SPELL_POWER_OF_SHADRON, false);
            pShad->AddSplineFlag(SPLINEFLAG_FLYING);
            pShad->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pShad->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aShad[0].m_fX, m_aShad[0].m_fY, m_aShad[0].m_fZ);

            if (!pShad->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pShad->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pVesp && pVesp->isAlive() && !pVesp->getVictim())
        {
            bCanUseWill = true;
            pVesp->CastSpell(pVesp, SPELL_POWER_OF_VESPERON, false);
            pVesp->AddSplineFlag(SPLINEFLAG_FLYING);
            pVesp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pVesp->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aVesp[0].m_fX, m_aVesp[0].m_fY, m_aVesp[0].m_fZ);

            if (!pVesp->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pVesp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (bCanUseWill)
            DoCast(m_creature, SPELL_WILL_OF_SARTHARION);
    }

    void CallDragon(uint32 uiDataId)
    {
        if (m_pInstance)
        {
            Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(uiDataId));

            if (pTemp && pTemp->isAlive() && !pTemp->getVictim())
            {
                pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                m_uiSarthHealth = m_creature->GetHealth();
                DoCast(m_creature, SPELL_WILL_OF_SARTHARION, false);
                bCanUseWill = true;

                int32 iTextId = 0;

                Creature* pAdd = NULL;
                pAdd = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(DATA_TENEBRON));
                if (pAdd)
                    m_uiTeneHealth = pAdd->GetHealth();
                pAdd = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(DATA_SHADRON));
                if (pAdd)
                    m_uiShadHealth = pAdd->GetHealth();
                pAdd = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(DATA_VESPERON));
                if (pAdd)
                    m_uiVespHealth = pAdd->GetHealth();

                switch (pTemp->GetEntry())
                {
                    case NPC_TENEBRON:
                        iTextId = SAY_SARTHARION_CALL_TENEBRON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aTene[1].m_fX, m_aTene[1].m_fY, m_aTene[1].m_fZ);
                        m_bTenebronHelpedInFight = true;
                        break;
                    case NPC_SHADRON:
                        iTextId = SAY_SARTHARION_CALL_SHADRON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aShad[1].m_fX, m_aShad[1].m_fY, m_aShad[1].m_fZ);
                        m_bShadronHelpedInFight = true;
                        break;
                    case NPC_VESPERON:
                        iTextId = SAY_SARTHARION_CALL_VESPERON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aVesp[1].m_fX, m_aVesp[1].m_fY, m_aVesp[1].m_fZ);
                        m_bVesperonHelpedInFight = true;
                        break;
                }

                DoScriptText(iTextId, m_creature);
            }
        }
    }

    void SendFlameTsunami()
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive())
                        DoScriptText(WHISPER_LAVA_CHURN, m_creature, i->getSource());
                }
            }
        }

        uint8 uiTsunamiDirection = urand(0, 1);
        uint8 uiTsunamiWavesAmount = 3;
        if (uiTsunamiDirection)
            uiTsunamiWavesAmount = 2;
        for (uint8 i = uiTsunamiDirection*3; i < uiTsunamiDirection*3+uiTsunamiWavesAmount; ++i)
        {
            m_creature->SummonCreature(NPC_FLAME_TSUNAMI, m_afTsunamiStartLoc[i][0], m_afTsunamiStartLoc[i][1], m_afTsunamiStartLoc[i][2], m_afTsunamiStartLoc[i][3], TEMPSUMMON_TIMED_DESPAWN, 18000);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (bCanUseWill)
        {
            if (bFirstWill)
            {
                m_creature->SetHealth(m_creature->GetHealth()*1.25);
            }
            else
            {
                m_creature->SetHealth(m_uiSarthHealth);
                Creature* pTemp = NULL;
                pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(DATA_TENEBRON));
                if (pTemp && pTemp->isAlive())
                    pTemp->SetHealth(m_uiTeneHealth);
                pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(DATA_SHADRON));
                if (pTemp && pTemp->isAlive())
                    pTemp->SetHealth(m_uiShadHealth);
                pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(DATA_VESPERON));
                if (pTemp && pTemp->isAlive())
                    pTemp->SetHealth(m_uiVespHealth);
            }
            bCanUseWill = false;
            bFirstWill = false;
        }

        //spell will target dragons, if they are still alive at 35%
        if (!m_bIsBerserk && m_creature->GetHealthPercent() <= 35.0f)
        {
            DoScriptText(SAY_SARTHARION_BERSERK, m_creature);
            //DoCast(m_creature, SPELL_BERSERK);
            Creature* pTemp = NULL;
            pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TENEBRON));
            if (pTemp && pTemp->isAlive())
                pTemp->CastSpell(pTemp, 27680, true);
            pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SHADRON));
            if (pTemp && pTemp->isAlive())
                pTemp->CastSpell(pTemp, 27680, true);
            pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VESPERON));
            if (pTemp && pTemp->isAlive())
                pTemp->CastSpell(pTemp, 27680, true);

            m_bIsBerserk = true;
        }

        //soft enrage
        if (!m_bIsSoftEnraged && m_creature->GetHealthPercent() <= 10.0f)
        {
            if (!m_lFireCyclones.empty())
                for (std::list<Creature*>::iterator iter = m_lFireCyclones.begin(); iter != m_lFireCyclones.end(); ++iter)
                    if (*iter)
                        (*iter)->CastSpell(*iter, SPELL_CYCLONE_AURA_2, true);

            m_bIsSoftEnraged = true;
        }

        // hard enrage
        if (!m_bIsHardEnraged)
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_PYROBUFFET, true);
                m_bIsHardEnraged = true;
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        // flame tsunami
        if (m_uiFlameTsunamiTimer < uiDiff)
        {
            SendFlameTsunami();
            switch(urand(0, 3))
            {
               case 0: DoScriptText(SAY_SARTHARION_SPECIAL_1, m_creature); break;
               case 1: DoScriptText(SAY_SARTHARION_SPECIAL_2, m_creature); break;
               case 2: DoScriptText(SAY_SARTHARION_SPECIAL_3, m_creature); break;
               case 3: DoScriptText(SAY_SARTHARION_SPECIAL_4, m_creature); break;
            }

            m_uiFlameTsunamiTimer = 30000;
        }
        else
            m_uiFlameTsunamiTimer -= uiDiff;

        // flame breath
        if (m_uiFlameBreathTimer < uiDiff)
        {
            DoScriptText(SAY_SARTHARION_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H);
            m_uiFlameBreathTimer = urand(25000, 35000);
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        // Tail Sweep
        if (m_uiTailSweepTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H);
            m_uiTailSweepTimer = urand(5000, 7000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = urand(7000, 10000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Lavas Strike
        if (m_uiCycloneAuraTimer < uiDiff)
        {
            if (!m_lFireCyclones.empty())
            {
                std::list<Creature*>::iterator iter = m_lFireCyclones.begin();
                advance(iter, urand(0, m_lFireCyclones.size()-1));
                if (*iter)
                {
                    (*iter)->CastSpell(*iter, SPELL_CYCLONE_AURA_2, true);

                    switch(urand(0, 15))
                    {
                        case 0: DoScriptText(SAY_SARTHARION_SPECIAL_1, m_creature); break;
                        case 1: DoScriptText(SAY_SARTHARION_SPECIAL_2, m_creature); break;
                        case 2: DoScriptText(SAY_SARTHARION_SPECIAL_3, m_creature); break;
                    }
                }
            }
            if (m_bIsSoftEnraged)
                m_uiCycloneAuraTimer = 10000;
            else
                m_uiCycloneAuraTimer = urand(20000, 25000);
        }
        else
            m_uiCycloneAuraTimer -= uiDiff;

        // call tenebron
        if (!m_bHasCalledTenebron && m_uiTenebronTimer < uiDiff)
        {
            CallDragon(DATA_TENEBRON);
            m_bHasCalledTenebron = true;
        }
        else
            m_uiTenebronTimer -= uiDiff;

        // call shadron
        if (!m_bHasCalledShadron && m_uiShadronTimer < uiDiff)
        {
            CallDragon(DATA_SHADRON);
            m_bHasCalledShadron = true;
        }
        else
            m_uiShadronTimer -= uiDiff;

        // call vesperon
        if (!m_bHasCalledVesperon && m_uiVesperonTimer < uiDiff)
        {
            CallDragon(DATA_VESPERON);
            m_bHasCalledVesperon = true;
        }
        else
            m_uiVesperonTimer -= uiDiff;

        if (m_uiCheckTwilightTimer < uiDiff)
        {
            bool bNoAliveTwilightRealm = true;
            if (m_pInstance)
            {
                if (Creature* pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteShadronGUID))
                {
                    if (pAcolyte->isAlive())
                    {
                        bNoAliveTwilightRealm = false;
                    }
                }
                if (Creature* pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteVesperonGUID))
                {
                    if (pAcolyte->isAlive())
                    {
                        bNoAliveTwilightRealm = false;
                    }
                }
                if (!m_lEggsGUIDList.empty())
                {
                    for (std::list<uint64>::iterator itr = m_lEggsGUIDList.begin(); itr != m_lEggsGUIDList.end(); ++itr)
                    {
                        if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        {
                            if (pTemp->isAlive())
                            {
                                bNoAliveTwilightRealm = false;
                                break;
                            }
                        }
                    }
                }
                if (bNoAliveTwilightRealm)
                {
                    Map* pMap = m_creature->GetMap();

                    if (pMap && pMap->IsDungeon())
                    {
                        Map::PlayerList const &PlayerList = pMap->GetPlayers();

                        if (!PlayerList.isEmpty())
                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            {
                                if (i->getSource()->isAlive())
                                    i->getSource()->CastSpell(i->getSource(), SPELL_TWILIGHT_SHIFT_REMOVAL, true);
                            }
                    }
                }
            }
            m_uiCheckTwilightTimer = 2000;
        }
        else
            m_uiCheckTwilightTimer -= uiDiff;

        DoMeleeAttackIfReady();

        EnterEvadeIfOutOfCombatArea(uiDiff);
    }
};

CreatureAI* GetAI_boss_sartharion(Creature* pCreature)
{
    return new boss_sartharionAI(pCreature);
}

enum TeneText
{
    SAY_TENEBRON_AGGRO                      = -1615009,
    SAY_TENEBRON_SLAY_1                     = -1615010,
    SAY_TENEBRON_SLAY_2                     = -1615011,
    SAY_TENEBRON_DEATH                      = -1615012,
    SAY_TENEBRON_BREATH                     = -1615013,
    SAY_TENEBRON_RESPOND                    = -1615014,
    SAY_TENEBRON_SPECIAL_1                  = -1615015,
    SAY_TENEBRON_SPECIAL_2                  = -1615016
};

enum ShadText
{
    SAY_SHADRON_AGGRO                       = -1615000,
    SAY_SHADRON_SLAY_1                      = -1615001,
    SAY_SHADRON_SLAY_2                      = -1615002,
    SAY_SHADRON_DEATH                       = -1615003,
    SAY_SHADRON_BREATH                      = -1615004,
    SAY_SHADRON_RESPOND                     = -1615005,
    SAY_SHADRON_SPECIAL_1                   = -1615006,
    SAY_SHADRON_SPECIAL_2                   = -1615007
};

enum VespText
{
    SAY_VESPERON_AGGRO                      = -1615033,
    SAY_VESPERON_SLAY_1                     = -1615034,
    SAY_VESPERON_SLAY_2                     = -1615035,
    SAY_VESPERON_DEATH                      = -1615036,
    SAY_VESPERON_BREATH                     = -1615037,
    SAY_VESPERON_RESPOND                    = -1615038,
    SAY_VESPERON_SPECIAL_1                  = -1615039,
    SAY_VESPERON_SPECIAL_2                  = -1615040
};

//to control each dragons common abilities
struct MANGOS_DLL_DECL dummy_dragonAI : public ScriptedAI
{
    dummy_dragonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWaypointId;
    uint32 m_uiMoveNextTimer;
    bool m_bCanMoveFree;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_uiWaypointId = 0;
        m_uiMoveNextTimer = 500;
        m_bCanMoveFree = false;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
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

    void JustReachedHome()
    {
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (!m_pInstance || uiType != POINT_MOTION_TYPE)
            return;

        debug_log("dummy_dragonAI: %s reached point %u", m_creature->GetName(), uiPointId);

        //if healers messed up the raid and we was already initialized
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            EnterEvadeMode();
            return;
        }

        //this is the end (!)
        if (uiPointId == POINT_ID_LAND)
        {
            m_creature->GetMotionMaster()->Clear();
            m_bCanMoveFree = false;
            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->SetInCombatWithZone();
            return;
        }

        //get amount of common points
        uint32 uiCommonWPCount = sizeof(m_aDragonCommon)/sizeof(Waypoint);

        //increase
        m_uiWaypointId = uiPointId+1;

        //if we have reached a point bigger or equal to count, it mean we must reset to point 0
        if (m_uiWaypointId >= uiCommonWPCount)
        {
            if (!m_bCanMoveFree)
                m_bCanMoveFree = true;

            m_uiWaypointId = 0;
        }

        m_uiMoveNextTimer = 500;
    }

    //used when open portal and spawn mobs in phase
    void DoRaidWhisper(int32 iTextId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    DoScriptText(iTextId, m_creature, i->getSource());
            }
        }
    }

    //Removes debuff from players
    void RemoveDebuff(uint32 uiSpellId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                i->getSource()->RemoveAurasDueToSpell(uiSpellId);
                if (uiSpellId == SPELL_TWILIGHT_SHIFT_ENTER)
                    i->getSource()->RemoveAurasDueToSpell(SPELL_TWILIGHT_SHIFT_DMG);
            }
        }
    }

    //"opens" the portal and does the "opening" whisper
    void OpenPortal()
    {
        int32 iTextId = 0;
        int32 iPortalRespawnTime = 0;

        //there are 4 portal spawn locations, each are expected to be spawned with negative spawntimesecs in database

        //using a grid search here seem to be more efficient than caching all four guids
        //in instance script and calculate range to each.
        if (GameObject* pPortal = GetClosestGameObjectWithEntry(m_creature, GO_TWILIGHT_PORTAL, 100.0f))
        {
            Creature* pAcolyte = NULL;
            switch(m_creature->GetEntry())
            {
                case NPC_TENEBRON:
                    iTextId = WHISPER_HATCH_EGGS;
                    m_lEggsGUIDList.clear();
                    for (uint8 i=0; i<6; ++i)
                    {
                        if (Creature* pEgg = m_creature->SummonCreature(NPC_TWILIGHT_EGG, pPortal->GetPositionX()-10+urand(0, 20), pPortal->GetPositionY()-10+urand(0, 20), pPortal->GetPositionZ()+1.0f, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                        {
                            pEgg->SetPhaseMask(16, true);
                            m_lEggsGUIDList.push_back(pEgg->GetGUID());
                        }
                    }
                    iPortalRespawnTime = 20;
                    break;
                case NPC_SHADRON:
                    iTextId = WHISPER_OPEN_PORTAL;
                    if (m_pInstance)
                    {
                        pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteShadronGUID);
                        if (!pAcolyte || (pAcolyte && pAcolyte->isDead()))
                        {
                            pAcolyte = NULL;
                            if (pAcolyte = m_creature->SummonCreature(NPC_ACOLYTE_OF_SHADRON, pPortal->GetPositionX()-10+urand(0, 20), pPortal->GetPositionY()-10+urand(0, 20), pPortal->GetPositionZ()+1.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000))
                            {
                                m_uiAcolyteShadronGUID = pAcolyte->GetGUID();
                                pAcolyte->SetPhaseMask(16, true);
                            }
                        }
                        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
                        {
                            if (Creature* pSarth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SARTHARION)))
                                pSarth->CastSpell(pSarth, SPELL_GIFT_OF_TWILIGTH_SAR, true);
                        }
                        else
                        {
                            if (Creature* pShad = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SHADRON)))
                                pShad->CastSpell(pShad, SPELL_GIFT_OF_TWILIGTH_SHA, true);
                        }
                    }
                    iPortalRespawnTime = 60;
                    break;
                case NPC_VESPERON:
                    iTextId = WHISPER_OPEN_PORTAL;
                    if (m_pInstance)
                    {
                        uint32 uiTempSpell;
                        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
                            uiTempSpell = 58835;
                        else
                            uiTempSpell = 57935;

                        SpellEntry* pTempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(uiTempSpell);
                        if (pTempSpell)
                        {
                            pTempSpell->StackAmount = 1;
                            pTempSpell->procCharges = 0;
                            m_creature->CastSpell(m_creature, pTempSpell, true);
                        }

                        pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteVesperonGUID);
                        if (!pAcolyte || (pAcolyte && pAcolyte->isDead()))
                        {
                            pAcolyte = NULL;
                            if (pAcolyte = m_creature->SummonCreature(NPC_ACOLYTE_OF_VESPERON, pPortal->GetPositionX()-10+urand(0, 20), pPortal->GetPositionY()-10+urand(0, 20), pPortal->GetPositionZ()+1.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000))
                            {
                                m_uiAcolyteVesperonGUID = pAcolyte->GetGUID();
                                pAcolyte->SetPhaseMask(16, true);
                            }
                        }
                    }
                    iPortalRespawnTime = 60;
                    break;
            }

            DoRaidWhisper(iTextId);

            //By using SetRespawnTime() we will actually "spawn" the object with our defined time.
            //Once time is up, portal will disappear again.

            pPortal->SetRespawnTime(iPortalRespawnTime);
            pPortal->UpdateObjectVisibility();

            //Unclear what are expected to happen if one drake has a portal open already
            //Refresh respawnTime so time again are set to 30secs?
        }
    }


    void CheckTwilightRealm()
    {
        bool bNoAliveTwilightRealm = true;
        if (m_pInstance)
        {
            if (Creature* pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteShadronGUID))
                if (pAcolyte->isAlive())
                    bNoAliveTwilightRealm = false;
            if (Creature* pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteVesperonGUID))
                if (pAcolyte->isAlive())
                    bNoAliveTwilightRealm = false;
            if (!m_lEggsGUIDList.empty())
                for (std::list<uint64>::iterator itr = m_lEggsGUIDList.begin(); itr != m_lEggsGUIDList.end(); ++itr)
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature( *itr))
                        if (pTemp->isAlive())
                        {
                            bNoAliveTwilightRealm = false;
                            break;
                        }
            if (bNoAliveTwilightRealm)
                RemoveDebuff(SPELL_TWILIGHT_SHIFT_ENTER);
        }
    }

    void JustDied(Unit* pKiller)
    {
        int32 iTextId = 0;

        switch(m_creature->GetEntry())
        {
            case NPC_TENEBRON:
            {
                iTextId = SAY_TENEBRON_DEATH;

                if (!m_lEggsGUIDList.empty())
                    for (std::list<uint64>::iterator itr = m_lEggsGUIDList.begin(); itr != m_lEggsGUIDList.end(); ++itr)
                        if (Creature* pEgg = m_creature->GetMap()->GetCreature( *itr))
                            pEgg->ForcedDespawn();
                break;
            }
            case NPC_SHADRON:
            {
                iTextId = SAY_SHADRON_DEATH;

                if (Creature* pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteShadronGUID))
                    pAcolyte->DealDamage(pAcolyte, pAcolyte->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                break;
            }
            case NPC_VESPERON:
            {
                iTextId = SAY_VESPERON_DEATH;

                if (Creature* pAcolyte = m_pInstance->instance->GetCreature(m_uiAcolyteVesperonGUID))
                    pAcolyte->DealDamage(pAcolyte, pAcolyte->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                break;
            }
        }

        DoScriptText(iTextId, m_creature);

        if (m_pInstance)
        {
            // not if solo mini-boss fight
            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
            {
                RemoveDebuff(SPELL_TWILIGHT_SHIFT_ENTER);
                return;
            }

            // Twilight Revenge to main boss
            if (Creature* pSartharion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SARTHARION)))
            {
                if (pSartharion->isAlive())
                    m_creature->CastSpell(pSartharion, SPELL_TWILIGHT_REVENGE, true);
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanMoveFree && m_uiMoveNextTimer)
        {
            if (m_uiMoveNextTimer <= uiDiff)
            {
                m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId,
                    m_aDragonCommon[m_uiWaypointId].m_fX, m_aDragonCommon[m_uiWaypointId].m_fY, m_aDragonCommon[m_uiWaypointId].m_fZ);

                debug_log("dummy_dragonAI: %s moving to point %u", m_creature->GetName(), m_uiWaypointId);
                m_uiMoveNextTimer = 0;
            }
            else
                m_uiMoveNextTimer -= uiDiff;
        }
    }
};

/*######
## Mob Tenebron
######*/

struct MANGOS_DLL_DECL mob_tenebronAI : public dummy_dragonAI
{
    mob_tenebronAI(Creature* pCreature) : dummy_dragonAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiHatchEggTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiTailSweepTimer;

    void Reset()
    {
        m_uiShadowBreathTimer = 10000;
        m_uiShadowFissureTimer = 8000;
        m_uiHatchEggTimer = 15000;
        m_uiCheckTimer = 2000;
        m_uiTailSweepTimer = 5000;
        m_lEggsGUIDList.clear();
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_TENEBRON_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_TENEBRON_SLAY_1 : SAY_TENEBRON_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H);

            m_uiShadowFissureTimer = urand(8000, 10000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            DoScriptText(SAY_TENEBRON_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H);
            m_uiShadowBreathTimer = urand(10000, 30000);
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        if (m_uiTailSweepTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H);
            m_uiTailSweepTimer = urand(5000, 7000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        if (m_uiHatchEggTimer < uiDiff)
        {
            OpenPortal();
            m_uiHatchEggTimer = 45000;
        }
        else
            m_uiHatchEggTimer -= uiDiff;

        if (m_uiCheckTimer < uiDiff && m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            CheckTwilightRealm();
            m_uiCheckTimer = 2000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_tenebron(Creature* pCreature)
{
    return new mob_tenebronAI(pCreature);
}

/*######
## Mob Shadron
######*/

struct MANGOS_DLL_DECL mob_shadronAI : public dummy_dragonAI
{
    mob_shadronAI(Creature* pCreature) : dummy_dragonAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiAcolyteShadronTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiTailSweepTimer;

    void Reset()
    {
        m_uiShadowBreathTimer = 10000;
        m_uiShadowFissureTimer = 8000;
        m_uiAcolyteShadronTimer = 15000;
        m_uiCheckTimer = 2000;
        m_uiTailSweepTimer = 5000;
        m_uiAcolyteShadronGUID = 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_SHADRON_AGGRO,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SHADRON_SLAY_1 : SAY_SHADRON_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H);

            m_uiShadowFissureTimer = urand(8000, 10000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            DoScriptText(SAY_SHADRON_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H);
            m_uiShadowBreathTimer = urand(10000, 30000);
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        if (m_uiTailSweepTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H);
            m_uiTailSweepTimer = urand(5000, 7000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        if (m_uiAcolyteShadronTimer < uiDiff)
        {
            OpenPortal();
            m_uiAcolyteShadronTimer = 60000;
        }
        else
            m_uiAcolyteShadronTimer -= uiDiff;

        if (m_uiCheckTimer < uiDiff && m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            CheckTwilightRealm();
            m_uiCheckTimer = 2000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_shadron(Creature* pCreature)
{
    return new mob_shadronAI(pCreature);
}

/*######
## Mob Vesperon
######*/

struct MANGOS_DLL_DECL mob_vesperonAI : public dummy_dragonAI
{
    mob_vesperonAI(Creature* pCreature) : dummy_dragonAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiAcolyteVesperonTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiTailSweepTimer;

    void Reset()
    {
        m_uiShadowBreathTimer = 10000;
        m_uiShadowFissureTimer = 8000;
        m_uiAcolyteVesperonTimer = 15000;
        m_uiCheckTimer = 2000;
        m_uiTailSweepTimer = 5000;
        m_uiAcolyteVesperonGUID = 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_VESPERON_AGGRO,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_VESPERON_SLAY_1 : SAY_VESPERON_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H);

            m_uiShadowFissureTimer = urand(8000, 10000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            DoScriptText(SAY_VESPERON_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H);
            m_uiShadowBreathTimer = urand(10000, 30000);
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        if (m_uiTailSweepTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H);
            m_uiTailSweepTimer = urand(5000, 7000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        if (m_uiAcolyteVesperonTimer < uiDiff)
        {
            OpenPortal();
            m_uiAcolyteVesperonTimer = 60000;
        }
        else
            m_uiAcolyteVesperonTimer -= uiDiff;

        if (m_uiCheckTimer < uiDiff && m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            CheckTwilightRealm();
            m_uiCheckTimer = 2000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_vesperon(Creature* pCreature)
{
    return new mob_vesperonAI(pCreature);
}

/*######
## Mob Acolyte of Shadron
######*/

struct MANGOS_DLL_DECL mob_acolyte_of_shadronAI : public ScriptedAI
{
    mob_acolyte_of_shadronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void JustDied(Unit* killer)
    {
        if (m_pInstance)
        {
            Creature* pDebuffTarget = NULL;

            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            {
                //not solo fight, so main boss has deduff
                pDebuffTarget = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SARTHARION));

                if (pDebuffTarget && pDebuffTarget->isAlive() && pDebuffTarget->HasAura(SPELL_GIFT_OF_TWILIGTH_SAR))
                    pDebuffTarget->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SAR);
            }
            else
            {
                //event not in progress, then solo fight and must remove debuff mini-boss
                pDebuffTarget = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SHADRON));

                if (pDebuffTarget && pDebuffTarget->isAlive() && pDebuffTarget->HasAura(SPELL_GIFT_OF_TWILIGTH_SHA))
                    pDebuffTarget->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SHA);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_shadron(Creature* pCreature)
{
    return new mob_acolyte_of_shadronAI(pCreature);
}

/*######
## Mob Acolyte of Vesperon
######*/

struct MANGOS_DLL_DECL mob_acolyte_of_vesperonAI : public ScriptedAI
{
    mob_acolyte_of_vesperonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void JustDied(Unit* pKiller)
    {
        // remove twilight torment
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                i->getSource()->RemoveAurasDueToSpell(57935);
                i->getSource()->RemoveAurasDueToSpell(58835);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_vesperon(Creature* pCreature)
{
    return new mob_acolyte_of_vesperonAI(pCreature);
}

/*######
## Mob Twilight Eggs
######*/

struct MANGOS_DLL_DECL mob_twilight_eggsAI : public ScriptedAI
{
    mob_twilight_eggsAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiSummonWhelpTimer;

    void Reset()
    {
        m_uiSummonWhelpTimer = 20000;
    }

    void AttackStart(Unit* pWho)
    {
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiSummonWhelpTimer < uiDiff)
        {
            if (Creature* pWhelp = DoSpawnCreature(NPC_TWILIGHT_WHELP, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
            {
                pWhelp->SetPhaseMask(1, true);
                pWhelp->SetInCombatWithZone();
            }
            m_uiSummonWhelpTimer = 20000;
            m_creature->ForcedDespawn();
        }
        else
            m_uiSummonWhelpTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_twilight_eggs(Creature* pCreature)
{
    return new mob_twilight_eggsAI(pCreature);
}

/*######
## Mob Twilight Whelps
######*/

struct MANGOS_DLL_DECL mob_twilight_whelpAI : public ScriptedAI
{
    mob_twilight_whelpAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiFadeArmorTimer;

    void Reset()
    {
        m_uiFadeArmorTimer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // twilight torment
        if (m_uiFadeArmorTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FADE_ARMOR);
            m_uiFadeArmorTimer = urand(5000, 10000);
        }
        else
            m_uiFadeArmorTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_twilight_whelp(Creature* pCreature)
{
    return new mob_twilight_whelpAI(pCreature);
}

/*######
## Mob Fire Cyclone
######*/

struct MANGOS_DLL_DECL mob_fire_cycloneAI : public ScriptedAI
{
    mob_fire_cycloneAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiLavaStrikeTimer;
    uint8 m_uiLavaStrikesCount;
    bool m_bLavaStrikeAllowed;


    void Reset()
    {
        m_bLavaStrikeAllowed = false;
    }

    void AttackStart(Unit* pWho)
    {
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_CYCLONE_AURA_2)
        {
            DoCast(m_creature, 57560, true);
            m_bLavaStrikeAllowed = true;
            m_uiLavaStrikeTimer = 0;
            m_uiLavaStrikesCount = 0;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_LAVA_BLAZE)
            if (urand(0, 3))                                //25% to stay
                pSummoned->ForcedDespawn();
            else
                pSummoned->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (m_bLavaStrikeAllowed)
        {
            if (m_uiLavaStrikeTimer < uiDiff)
            {
                m_creature->CastSpell(urand(3220, 3275), urand(486, 575), 58.8f, SPELL_LAVA_STRIKE, true);
                m_uiLavaStrikeTimer = 1000;
                ++m_uiLavaStrikesCount;
                if (m_uiLavaStrikesCount>=5)
                    m_bLavaStrikeAllowed = false;
            }
            else
                m_uiLavaStrikeTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_fire_cyclone(Creature* pCreature)
{
    return new mob_fire_cycloneAI(pCreature);
}

/*######
## Mob Flame Tsunami
######*/

struct MANGOS_DLL_DECL mob_flame_tsunamiAI : public ScriptedAI
{
    mob_flame_tsunamiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiTickTimer;
    uint32 m_uiMovementStartTimer;
    uint64 m_uiDummyDamagerGUID;

    void Reset()
    {
        m_creature->SetDisplayId(11686);
        DoCast(m_creature, SPELL_FLAME_TSUNAMI, true);
        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_uiMovementStartTimer = 4000;
        m_uiTickTimer = 1000;
        m_uiDummyDamagerGUID = 0;
        if (Creature* pDummyDamager = DoSpawnCreature(31103, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 18000))
        {
            pDummyDamager->SetDisplayId(11686);
            pDummyDamager->setFaction(14);
            pDummyDamager->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pDummyDamager->SetSpeedRate(MOVE_RUN, m_creature->GetSpeedRate(MOVE_RUN));
            m_uiDummyDamagerGUID = pDummyDamager->GetGUID();
        }
    }

    void AttackStart(Unit* pWho)
    {
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiMovementStartTimer < uiDiff)
        {
            int8 uiDirection = 1;
            if (m_creature->GetPositionX() > 3240.0f)
                uiDirection = -1;
            m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()+uiDirection*86.5f, m_creature->GetPositionY(), m_creature->GetPositionZ());
            if (m_pInstance)
                if (Creature* pDummyDamager = m_pInstance->instance->GetCreature(m_uiDummyDamagerGUID))
                    pDummyDamager->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()+uiDirection*86.5f, m_creature->GetPositionY(), m_creature->GetPositionZ());
            m_uiMovementStartTimer = 30000;
        }
        else
            m_uiMovementStartTimer -= uiDiff;

        if (m_uiTickTimer < uiDiff)
        {
            if (m_pInstance)
                if (Creature* pDummyDamager = m_pInstance->instance->GetCreature(m_uiDummyDamagerGUID))
                    pDummyDamager->CastSpell(pDummyDamager, SPELL_FLAME_TSUNAMI_DMG, false);

            std::list<Creature*> lLavaBlazes;
            GetCreatureListWithEntryInGrid(lLavaBlazes, m_creature, NPC_LAVA_BLAZE, 6.0f);
            if (!lLavaBlazes.empty())
            {
                SpellEntry* pTempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_FLAME_TSUNAMI_BUFF);
                if (pTempSpell)
                {
                    pTempSpell->EffectImplicitTargetA[0] = TARGET_SELF;
                    pTempSpell->EffectImplicitTargetB[0] = 0;
                    pTempSpell->EffectImplicitTargetA[1] = TARGET_SELF;
                    pTempSpell->EffectImplicitTargetB[1] = 0;
                    pTempSpell->EffectImplicitTargetA[2] = TARGET_SELF;
                    pTempSpell->EffectImplicitTargetB[2] = 0;
                    for (std::list<Creature*>::iterator iter = lLavaBlazes.begin(); iter != lLavaBlazes.end(); ++iter)
                    {
                        (*iter)->CastSpell(*iter, pTempSpell, false);
                        (*iter)->SetHealth((*iter)->GetHealth()*4);
                    }
                }
            }

            Map* pMap = m_creature->GetMap();
            if (pMap && pMap->IsDungeon())
            {
                Map::PlayerList const &PlayerList = pMap->GetPlayers();

                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()) <= 5.0f)
                        {
                            i->getSource()->SetOrientation(m_creature->GetOrientation());
                            i->getSource()->CastSpell(i->getSource(), SPELL_FLAME_TSUNAMI_LEAP, true);
                        }
            }

            m_uiTickTimer = 1000;
        }
        else
            m_uiTickTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_flame_tsunami(Creature* pCreature)
{
    return new mob_flame_tsunamiAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_lava_blazeAI : public ScriptedAI
{
    mob_lava_blazeAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS) 
              m_creature->ForcedDespawn();
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lava_blaze(Creature* pCreature)
{
    return new mob_lava_blazeAI(pCreature);
};

void AddSC_boss_sartharion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sartharion";
    newscript->GetAI = &GetAI_boss_sartharion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vesperon";
    newscript->GetAI = &GetAI_mob_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadron";
    newscript->GetAI = &GetAI_mob_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_tenebron";
    newscript->GetAI = &GetAI_mob_tenebron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_acolyte_of_shadron";
    newscript->GetAI = &GetAI_mob_acolyte_of_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_acolyte_of_vesperon";
    newscript->GetAI = &GetAI_mob_acolyte_of_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_twilight_eggs";
    newscript->GetAI = &GetAI_mob_twilight_eggs;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_twilight_whelp";
    newscript->GetAI = &GetAI_mob_twilight_whelp;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_fire_cyclone";
    newscript->GetAI = &GetAI_mob_fire_cyclone;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_tsunami";
    newscript->GetAI = &GetAI_mob_flame_tsunami;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_lava_blaze";
    newscript->GetAI = &GetAI_mob_lava_blaze;
    newscript->RegisterSelf();
}
