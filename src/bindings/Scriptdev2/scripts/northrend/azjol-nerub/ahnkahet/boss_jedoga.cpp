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
SDName: Boss_Jedoga
SD%Complete: 90%
SDAuthor: Tassadar
SDComment: Correct Timers
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

struct Locations
{
    float x, y, z, o;
    uint32 id;
};
enum
{
    SPELL_SPHERE_VISUAL                 = 56075,
    SPELL_GIFT_OF_THE_HERALD            = 56219,
    SPELL_CYCLONE_STRIKE                = 60030,
    SPELL_CYCLONE_STRIKE_H              = 56855,
    SPELL_LIGHTNING_BOLT                = 56891,
    SPELL_LIGHTNING_BOLT_H              = 60032,
    SPELL_THUNDERSHOCK                  = 56926,
    SPELL_THUNDERSHOCK_H                = 60029,

    SAY_AGGRO                           = -1619017,
    SAY_CALL_SACRIFICE_1                = -1619018,
    SAY_CALL_SACRIFICE_2                = -1619019,
    SAY_SACRIFICE_1                     = -1619020,
    SAY_SACRIFICE_2                     = -1619021,
    SAY_SLAY_1                          = -1619022,
    SAY_SLAY_2                          = -1619023,
    SAY_SLAY_3                          = -1619024,
    SAY_DEATH                           = -1619025,
    SAY_PREACHING_1                     = -1619026,
    SAY_PREACHING_2                     = -1619027,
    SAY_PREACHING_3                     = -1619028,
    SAY_PREACHING_4                     = -1619029,
    SAY_PREACHING_5                     = -1619030,

    SAY_VOLUNTEER_1                     = -1619031,         //said by the volunteer image
    SAY_VOLUNTEER_2                     = -1619032,

    NPC_TWILIGHT_INITIATE               = 30114,
    NPC_TWILIGHT_VOLUNTEER              = 30385,

    GO_CIRCLE                           = 194394,           // Propably wrong id

    //Jedoga Shadowseeker's phases
    PHASE_FIGHT                         = 3,
    PHASE_SACRIFACE                     = 4,
        SUBPHASE_FLY_UP                 = 41,
        SUBPHASE_CALL_VOLUNTEER         = 42,
        SUBPHASE_WAIT_FOR_VOLUNTEER     = 43,
        SUBPHASE_SACRIFACE              = 44,

    VOLUNTEER_COUNT                     = 29,
};
#define CENTER_X                        372.331f
#define CENTER_Y                        -705.278f
#define GROUND_Z                        -16.17f
#define AIR_Z                           -0.624178f
#define JEDOGA_O                        5.42797f

static Locations VolunteerLoc[]=
{
    //29 Volunteers
    {365.68f, -735.95f, -16.17f, 1.607f}, // Right, first line
    {367.12f, -736.13f, -16.17f, 1.607f},
    {369.03f, -736.06f, -16.17f, 1.607f},
    {371.66f, -735.97f, -16.17f, 1.607f},
    {373.47f, -735.63f, -16.17f, 1.607f},

    {365.45f, -739.03f, -16.00f, 1.607f}, // Right, second line
    {367.56f, -738.62f, -16.00f, 1.607f},
    {369.62f, -738.22f, -16.17f, 1.607f},
    {371.66f, -737.82f, -16.06f, 1.607f},
    {373.75f, -737.41f, -16.00f, 1.607f},
    
    {400.99f, -705.41f, -16.00f, 2.491f}, // Center, from right
    {398.07f, -710.02f, -16.00f, 2.491f},
    {395.34f, -713.76f, -16.00f, 2.491f},
    {393.42f, -716.39f, -16.00f, 2.491f},
    {391.48f, -718.94f, -16.00f, 2.491f},
    {388.80f, -722.46f, -16.00f, 2.491f},
    {386.19f, -725.89f, -16.00f, 2.491f},
    {383.61f, -729.29f, -16.00f, 2.491f},
    {380.37f, -733.55f, -16.00f, 2.491f},
    
    {402.72f, -700.79f, -16.00f, 3.046f}, // Left, first line
    {402.63f, -698.86f, -16.18f, 3.149f},
    {402.62f, -697.10f, -16.17f, 3.149f},
    {402.61f, -695.50f, -16.17f, 3.059f},
    {402.20f, -693.39f, -16.17f, 3.159f},

    {405.31f, -701.29f, -16.00f, 2.924f}, // Left, second line
    {405.46f, -699.25f, -16.00f, 3.198f},
    {405.40f, -697.19f, -16.00f, 3.150f},
    {405.35f, -695.30f, -16.00f, 3.150f},
    {405.29f, -693.26f, -16.00f, 3.150f}
};

/*######
## boss_jedoga
######*/

struct MANGOS_DLL_DECL boss_jedogaAI : public ScriptedAI
{
    boss_jedogaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiEvadeCheckCooldown;
    bool m_bDelay;

    bool m_bVolunteerDied;
    bool m_bIsIntroDone;
    uint8 m_uiPhase;
    uint8 m_uiSubPhase;
    uint8 m_uiPreachingText;
    uint64 m_uiVolunteerGUID;
    uint8 m_uiLastSacrifaceHP;

    uint32 m_uiPreachingTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiSacrifaceTimer;
    uint32 m_uiCycloneStrikeTimer;
    uint32 m_uiLightningBoltTimer;
    uint32 m_uiThundershockTimer;


    void Reset()
    {
        m_uiEvadeCheckCooldown = 2000;
        m_bDelay = false;
        m_uiPhase = PHASE_FIGHT;
        m_uiSubPhase = 0;
        m_uiPreachingText = 0;
        m_uiVolunteerGUID = 0;
        m_bVolunteerDied = false;
        m_uiLastSacrifaceHP = 0;
        m_bIsIntroDone = false;

        m_uiCheckTimer = 1000;
        m_uiSacrifaceTimer = 2000;
        m_uiPreachingTimer = 0;
        m_uiCycloneStrikeTimer = 17000;
        m_uiLightningBoltTimer = 3000;
        m_uiThundershockTimer = 30000;

        DoCast(m_creature, SPELL_SPHERE_VISUAL);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        std::list<Creature*> lUnitList;
        GetCreatureListWithEntryInGrid(lUnitList, m_creature, NPC_TWILIGHT_INITIATE, 100.0f);
        if (!lUnitList.empty())
            for(std::list<Creature*>::iterator iter = lUnitList.begin(); iter != lUnitList.end(); ++iter)
                if ((*iter)->isDead())
                    (*iter)->Respawn(); 

        lUnitList.clear();
        GetCreatureListWithEntryInGrid(lUnitList, m_creature, NPC_TWILIGHT_VOLUNTEER, 100.0f);
        if (!lUnitList.empty())
            for(std::list<Creature*>::iterator iter = lUnitList.begin(); iter != lUnitList.end(); ++iter)
                (*iter)->ForcedDespawn(); 
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bIsIntroDone)
        {
            if (!GetClosestCreatureWithEntry(m_creature, NPC_TWILIGHT_INITIATE, 100.0f))
            {
                m_bIsIntroDone = true;
                m_creature->RemoveAurasDueToSpell(SPELL_SPHERE_VISUAL);
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMap()->CreatureRelocation(m_creature, CENTER_X, CENTER_Y, GROUND_Z, JEDOGA_O);
                m_creature->SendMonsterMove(CENTER_X, CENTER_Y, GROUND_Z, SPLINETYPE_NORMAL, SPLINEFLAG_NONE, 0);
                m_bDelay = true;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetInCombatWithZone();
                
                //Spawn Volunteers
                for(int i = 0; i < VOLUNTEER_COUNT; i++)
                    if(Creature *pTemp = m_creature->SummonCreature(NPC_TWILIGHT_VOLUNTEER, VolunteerLoc[i].x, VolunteerLoc[i].y, VolunteerLoc[i].z, VolunteerLoc[i].o, TEMPSUMMON_CORPSE_DESPAWN, 0))
                        pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                m_uiSubPhase = 0;
                m_uiPhase = PHASE_FIGHT;
            }
            return;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_JEDOGA, DONE);

        std::list<Creature*> lUnitList;
        GetCreatureListWithEntryInGrid(lUnitList, m_creature, NPC_TWILIGHT_VOLUNTEER, 100.0f);
        if (!lUnitList.empty())
            for(std::list<Creature*>::iterator iter = lUnitList.begin(); iter != lUnitList.end(); ++iter)
                (*iter)->ForcedDespawn(); 
    }
   
    uint64 SelectRandomVolunteer(float fRange)
    {
        std::list<Creature* > lVolunteerList;
        GetCreatureListWithEntryInGrid(lVolunteerList, m_creature, NPC_TWILIGHT_VOLUNTEER, fRange);

        //This should not appear!
        if (lVolunteerList.empty()){
            EnterEvadeMode();
            debug_log("SD2: AhnKahet: No volunteer to sacriface!");
            return 0;
        }
            
        std::list<Creature* >::iterator iter = lVolunteerList.begin();
        advance(iter, urand(0, lVolunteerList.size()-1));

        return (*iter)->GetGUID();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (m_bIsIntroDone)
                return;

            if (m_uiPreachingTimer > uiDiff)
            {
                m_uiPreachingTimer -= uiDiff;
                return;
            }

            switch (m_uiPreachingText)
            {
               case 0:
                   DoScriptText(SAY_PREACHING_1, m_creature);
                   m_uiPreachingText++;
                   m_uiPreachingTimer = 9500;
                   break;
               case 1:
                   DoScriptText(SAY_PREACHING_2, m_creature);
                   m_uiPreachingText++;
                   m_uiPreachingTimer = 6500;
                   break;
               case 2:
                   DoScriptText(SAY_PREACHING_3, m_creature);
                   m_uiPreachingText++;
                   m_uiPreachingTimer = 8500;
                   break;
               case 3:
                   DoScriptText(SAY_PREACHING_4, m_creature);
                   m_uiPreachingText++;
                   m_uiPreachingTimer = 7500;
                   break;
               case 4:
                   DoScriptText(SAY_PREACHING_5, m_creature);
                   m_uiPreachingText = 0;
                   m_uiPreachingTimer = 12000;
                   break;
            }

            return;
        }

        if (m_bDelay)
        {
            m_bDelay = false;
            return;
        }

        if (m_uiEvadeCheckCooldown < uiDiff)
        {
            if (m_creature->GetDistance2d(CENTER_X, CENTER_Y) > 46.0f)
                EnterEvadeMode();
            m_uiEvadeCheckCooldown = 2000;
        }
        else
            m_uiEvadeCheckCooldown -= uiDiff;

        if (m_uiPhase == PHASE_FIGHT)
        {
            //Spells
            //Cyclone Strike
            if (m_uiCycloneStrikeTimer <= uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_CYCLONE_STRIKE : SPELL_CYCLONE_STRIKE_H);
                m_uiCycloneStrikeTimer = 10000 + rand()%10000;
            }
            else
                m_uiCycloneStrikeTimer -= uiDiff;

            //Lightning Bolt
            if (m_uiLightningBoltTimer <= uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_LIGHTNING_BOLT : SPELL_LIGHTNING_BOLT_H);
                m_uiLightningBoltTimer = 3000 + rand()%2000;
            }
            else
                m_uiLightningBoltTimer -= uiDiff;

            //Thundershock
            if (m_uiThundershockTimer <= uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_THUNDERSHOCK : SPELL_THUNDERSHOCK_H);
                m_uiThundershockTimer = 20000 + rand()%10000;
            }
            else
                m_uiThundershockTimer -= uiDiff;

            //Health check
            if (m_uiCheckTimer <= uiDiff)
            {
                uint8 health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();                    
                if (m_uiLastSacrifaceHP == 0 && health <= 75)
                {
                    m_uiLastSacrifaceHP = 75;
                    m_uiPhase = PHASE_SACRIFACE;
                    m_uiSubPhase = SUBPHASE_FLY_UP;
                    return;
                }
                else if (m_uiLastSacrifaceHP == 75 && health <= 50)
                {
                    m_uiLastSacrifaceHP = 50;
                    m_uiPhase = PHASE_SACRIFACE;
                    m_uiSubPhase = SUBPHASE_FLY_UP;
                    return;
                }
                else if (m_uiLastSacrifaceHP == 50 && health <= 25)
                {
                    m_uiLastSacrifaceHP = 25;
                    m_uiPhase = PHASE_SACRIFACE;
                    m_uiSubPhase = SUBPHASE_FLY_UP;
                    return;
                }
                m_uiCheckTimer = 1000;
            }
            else
                m_uiCheckTimer -= uiDiff; 

            DoMeleeAttackIfReady();
        }
        else if (m_uiPhase == PHASE_SACRIFACE)
        {
            if (m_uiSubPhase == SUBPHASE_FLY_UP)
            {
                SetCombatMovement(false);
                m_creature->GetMotionMaster()->MovementExpired();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->GetMap()->CreatureRelocation(m_creature, CENTER_X, CENTER_Y, AIR_Z, JEDOGA_O);
                m_creature->SendMonsterMove(CENTER_X, CENTER_Y, AIR_Z, SPLINETYPE_NORMAL, SPLINEFLAG_NONE, 0);
                m_bDelay = true;
                DoCast(m_creature, SPELL_SPHERE_VISUAL);
                m_uiSubPhase = SUBPHASE_CALL_VOLUNTEER;
                GameObject* pCircle = GetClosestGameObjectWithEntry(m_creature, GO_CIRCLE, 100.0f);
                if (m_pInstance && pCircle && !pCircle->isSpawned())
                    m_pInstance->DoRespawnGameObject(pCircle->GetGUID(), 10000);
            }
            else if (m_uiSubPhase == SUBPHASE_CALL_VOLUNTEER)
            {
                m_uiVolunteerGUID = SelectRandomVolunteer(100.0f);
                if (Creature* pVolunteer = m_creature->GetMap()->GetCreature(m_uiVolunteerGUID))
                {
                    switch(urand(0, 1))
                    {
                        case 0: DoScriptText(SAY_CALL_SACRIFICE_1, m_creature); break;
                        case 1: DoScriptText(SAY_CALL_SACRIFICE_2, m_creature); break;
                    }
                    switch (urand(0, 1))
                    {
                        case 0: DoScriptText(SAY_VOLUNTEER_1, pVolunteer); break;
                        case 1: DoScriptText(SAY_VOLUNTEER_2, pVolunteer); break;
                    }
                    pVolunteer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pVolunteer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pVolunteer->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, GROUND_Z);
                    m_uiSubPhase = SUBPHASE_WAIT_FOR_VOLUNTEER;
                }
            }
            else if (m_uiSubPhase == SUBPHASE_WAIT_FOR_VOLUNTEER)
            {
                if (m_uiCheckTimer <= uiDiff)
                {
                    m_bVolunteerDied = true;
                    if (Creature* pVolunteer = m_creature->GetMap()->GetCreature(m_uiVolunteerGUID))
                        if (pVolunteer->isAlive())
                        {
                            m_bVolunteerDied = false;
                            if (pVolunteer->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE)
                                m_uiSubPhase = SUBPHASE_SACRIFACE;
                        }
                        else
                            m_uiSubPhase = SUBPHASE_SACRIFACE;
                    else
                        m_uiSubPhase = SUBPHASE_SACRIFACE;
                       
                    m_uiCheckTimer = 1000;
                }
                else
                    m_uiCheckTimer -= uiDiff;
            }
            else if (m_uiSubPhase == SUBPHASE_SACRIFACE)
            {
                switch(urand(0, 1))
                {
                    case 0: DoScriptText(SAY_SACRIFICE_1, m_creature); break;
                    case 1: DoScriptText(SAY_SACRIFICE_2, m_creature); break;
                }
                
                if (!m_bVolunteerDied)
                {
                    DoCast(m_creature, SPELL_GIFT_OF_THE_HERALD);
                    if (Creature* pVolunteer = m_creature->GetMap()->GetCreature(m_uiVolunteerGUID))
                    {
                        m_creature->SetHealth(m_creature->GetHealth() + pVolunteer->GetHealth());
                        pVolunteer->DealDamage(pVolunteer, pVolunteer->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }

                m_creature->RemoveAurasDueToSpell(SPELL_SPHERE_VISUAL);
                m_creature->GetMap()->CreatureRelocation(m_creature, CENTER_X, CENTER_Y, GROUND_Z, JEDOGA_O);
                m_creature->SendMonsterMove(CENTER_X, CENTER_Y, GROUND_Z, SPLINETYPE_NORMAL, SPLINEFLAG_NONE, 0);
                m_bDelay = true;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                SetCombatMovement(true);
                m_uiPhase = PHASE_FIGHT;
                if (m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
        }
    }
};

/*######
## npc_twilight_volunteer
######*/
struct MANGOS_DLL_DECL npc_twilight_volunteerAI : public ScriptedAI
{
    npc_twilight_volunteerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    void Reset() { }

    void AttackStart(Unit* pWho) { }

    void UpdateAI(const uint32 uiDiff) { }
};

CreatureAI* GetAI_boss_jedoga(Creature* pCreature)
{
    return new boss_jedogaAI(pCreature);
}

CreatureAI* GetAI_npc_twilight_volunteer(Creature* pCreature)
{
    return new npc_twilight_volunteerAI(pCreature);
}

void AddSC_boss_jedoga()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_jedoga";
    newscript->GetAI = &GetAI_boss_jedoga;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_twilight_volunteer";
    newscript->GetAI = &GetAI_npc_twilight_volunteer;
    newscript->RegisterSelf();
}
