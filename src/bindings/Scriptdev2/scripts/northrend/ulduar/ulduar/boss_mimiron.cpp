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
SDName: boss_mimiron
SD%Complete: 
SDComment: needs vehicles
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

enum
{
    //yells
    SAY_AGGRO           = -1603241,
    SAY_HARD_MODE       = -1603242,
    SAY_BERSERK         = -1603243,
    SAY_TANK_ACTIVE     = -1603244,
    SAY_TANK_SLAY1      = -1603245,
    SAY_TANK_SLAY2      = -1603246,
    SAY_TANK_DEATH      = -1603247,
    SAY_TORSO_ACTIVE    = -1603248,
    SAY_TORSO_SLAY1     = -1603249,
    SAY_TORSO_SLAY2     = -1603250,
    SAY_TORSO_DEATH     = -1603251,
    SAY_HEAD_ACTIVE     = -1603252,
    SAY_HEAD_SLAY1      = -1603253,
    SAY_HEAD_SLAY2      = -1603254,
    SAY_HEAD_DEATH      = -1603255,
    SAY_ROBOT_ACTIVE    = -1603256,
    SAY_ROBOT_SLAY1     = -1603257,
    SAY_ROBOT_SLAY2     = -1603258,
    SAY_ROBOT_DEATH     = -1603259,

    EMOTE_PLASMA_BLAST      = -1603371,

    SPELL_JET_PACK          = 63341, // used by mimiron to change seats
    SPELL_SELF_REPAIR       = 64383,

    // hard mode spells
    SPELL_SELF_DESTRUCTION  = 64613,    // visual aura
    SPELL_SELF_DESTRUCT     = 64610,    // damage aura
    SPELL_EMERGENCY_MODE_AURA   = 65101,
    NPC_MIMIRON_INFERNO     = 33370,    // used to cast the self destruct

    SPELL_FLAMES            = 64561,    // may be the fires spells
    SPELL_FLAMES_SUMMON     = 64563,    // 64567
    SPELL_FLAMES_SPREAD     = 64562,
    NPC_FLAME               = 34121,
    NPC_FLAME_INITIAL       = 34363,

    //spells
    //leviathan
    SPELL_PROXIMITY_MINES   = 63016, // also in phase 4
    SPELL_MINE_SUMMON       = 65347,
    MOB_PROXIMITY_MINE      = 34362,
    SPELL_EXPLOSION         = 66351,
    SPELL_EXPLOSION_H       = 63009,
    SPELL_NAPALM_SHELL      = 63666,
    SPELL_NAPALM_SHELL_H    = 65026,
    SPELL_PLASMA_BLAST      = 62997,
    SPELL_PLASMA_BLAST_H    = 64529,
    SPELL_SHOCK_BLAST       = 63631, // also in phase 4
    SPELL_FLAME_SUPRESSANT  = 64570, // hard mode
    LEVIATHAN_TURRET        = 34071,

    //vx001
    SPELL_RAPID_BURST       = 63387,
    SPELL_RAPID_BURST_H     = 64531,
    SPELL_LASER_BARRAGE     = 63293, // also in phase 4
    SPELL_LASER_VISUAL      = 63300,
    SPELL_LASER_TRIGG       = 63274,
    SPELL_ROCKET_STRIKE     = 64064,
    NPC_MIMIRON_FOCUS       = 33835,    //33369
    SPELL_HEAT_WAVE         = 63677,
    SPELL_HEAT_WAVE_H       = 64533,
    SPELL_HAND_PULSE        = 64348, // only in phase 4
    SPELL_FLAME_SUPRESS     = 65192,    // used by robot in melee range
    SPELL_HAND_PULSE_H      = 64536,

    // frostbomb
    SPELL_FROST_BOMB_EXPL   = 64626,
    SPELL_FROST_BOMB_AURA   = 64624,    // before explode
    SPELL_FROST_BOMB_VISUAL = 64625,    // bomb grows
    SPELL_FROST_BOMB_SUMMON = 64627,    // summon the frostbomb

    //aerial unit
    SPELL_PLASMA_BALL       = 63689, // also in phase 4
    SPELL_PLASMA_BALL_H     = 64535, // also in phase 4
    MOB_ASSALT_BOT          = 34057,
    MOB_BOMB_BOT            = 33836,
    MOB_BOMB_BOT_321        = 33346,
    MOB_BOMB_BOT_500        = 34192,
    MOB_JUNK_BOT            = 33855,
    SPELL_MAGNETIC_FIELD    = 64668,
    SPELL_MAGNETIC_CORE     = 64436, // increase dmg taken by 50%, used by magnetic core
    MOB_MAGNETIC_CORE       = 34068,
    ITEM_MAGNETIC_CORE      = 46029,
    SPELL_BOMB_BOT_SUMMON   = 63811,
    SPELL_BOMB_BOT          = 63767,

    //hard mode
    // summons fires
    SPELL_EMERGENCY_MODE    = 64582,
    MOB_FROST_BOMB          = 34149,
    MOB_EMERGENCY_FIRE_BOT  = 34147,
    SPELL_DEAFENING_SIREN   = 64616,
    SPELL_WATER_SPRAY       = 64619,

    SPELL_MIMIRONS_INFERNO  = 62910,  // maybe used by rocket
    SPELL_MIMIRONS_INFERNO2 = 62909,  // maybe hard mode
    SPELL_BERSERK           = 26662,

    ACHIEV_FIREFIGHTER      = 3180,
    ACHIEV_FIREFIGHTER_H    = 3189,
};

enum MimironPhase
{
    PHASE_IDLE      = 0,
    PHASE_INTRO     = 1,
    PHASE_LEVIATHAN = 2,
    PHASE_TRANS_1   = 3,
    PHASE_VX001     = 4,
    PHASE_TRANS_2   = 5,
    PHASE_AERIAL    = 6,
    PHASE_TRANS_3   = 7,
    PHASE_ROBOT     = 8,
    PHASE_OUTRO     = 9,
};

#define CENTER_X            2744.732f
#define CENTER_Y            2569.479f
#define CENTER_Z            364.312f 

const float PosTankHome[2]= {2794.86f, 2597.83f};
struct LocationsXY
{
    float x, y;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {2753.665f, 2584.712f},
    {2754.150f, 2554.445f},
    {2726.966f, 2569.032f},
    {2759.085f, 2594.249f},
    {2759.977f, 2544.345f},
    {2715.542f, 2569.160f},
    {2765.070f, 2604.337f},
    {2765.676f, 2534.558f},
    {2703.810f, 2569.132f},
};

// Leviathan Mk script
struct MANGOS_DLL_DECL boss_leviathan_mkAI : public ScriptedAI
{
    boss_leviathan_mkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    bool m_bStartAttack;

    uint32 m_uiMinesTimer;
    uint32 m_uiNapalmTimer;
    uint32 m_uiPlasmaBlastTimer;
    uint32 m_uiShockBlastTimer;

    bool m_bHasSuppresed;
    uint32 m_uiSupressTimer;
    uint32 m_uiSetFireTimer;

    // outro
    bool m_bIsOutro;
    uint32 m_uiOutroTimer;
    uint32 m_uiOutroStep;

    bool m_bMustRepair;
    uint32 m_uiRepairTimer;

    void Reset()
    {
        m_bStartAttack          = false;
        m_uiMinesTimer          = 5000;
        m_uiNapalmTimer         = 20000;
        m_uiPlasmaBlastTimer    = 10000;
        m_uiShockBlastTimer     = 30000;
        m_bHasSuppresed         = false;
        m_uiSetFireTimer        = 10000;
        m_uiSupressTimer        = 10000;

        m_uiOutroTimer          = 10000;
        m_uiOutroStep           = 1;
        m_bIsOutro              = false;
        m_bMustRepair           = false;
        m_uiRepairTimer         = 15000;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN_MK, NOT_STARTED);
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_LEVIATHAN)
                DoStartMovement(pWho);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_LEVIATHAN)
        {
            if(m_creature->GetHealthPercent() < 1.0f)
            {
                uiDamage = 0;
                m_bIsOutro = true;
            }
        }
        // hacky way for feign death
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_ROBOT)
        {
            if(uiDamage > m_creature->GetHealth() && !m_bMustRepair)
            {
                uiDamage = 0;
                m_creature->SetHealth(0);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovementExpired(false);
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->CombatStop();
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                m_bMustRepair   = true;
                m_uiRepairTimer = 15000;
                DoCast(m_creature, SPELL_SELF_REPAIR);

                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_LEVIATHAN_MK, SPECIAL);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Creature* pMimiron = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_MIMIRON)))
        {
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_LEVIATHAN)
            {
                if(irand(0,1))
                    DoScriptText(SAY_TANK_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_TANK_SLAY2, pMimiron);
            }
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_ROBOT)
            {
                if(irand(0,1))
                    DoScriptText(SAY_ROBOT_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_ROBOT_SLAY2, pMimiron);
            }
        }
    }

    // hacky way for phase 4. needs rewriging when vehicles are fixed
    void SetPhase()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bIsOutro          = false;
        m_uiMinesTimer      = 10000;
        m_uiShockBlastTimer = 30000;
        m_bStartAttack      = true;
        // look like a robot
        SetCombatMovement(false);
    }

    // hacky way for phase 4. needs rewriging when vehicles are fixed
    void Repair()
    {
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());
        SetPhase();

        if(m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN_MK, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummon)
    {
        pSummon->SetInCombatWithZone();
    }

    void SuppressFires()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, DEFAULT_VISIBILITY_INSTANCE);
        if (!lFires.empty())
        {
            for(std::list<Creature*>::iterator iter = lFires.begin(); iter != lFires.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_bIsOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if(m_uiRepairTimer < uiDiff && m_bMustRepair)
            {
                SetPhase();
                Repair();
                m_bMustRepair = false;
            }
            else m_uiRepairTimer -= uiDiff;

            // return if repairing
            if(m_bMustRepair)
                return;

            // this should be removed when vehicles are implemented! The are casted by the turret
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_LEVIATHAN)
            {
                if(m_uiPlasmaBlastTimer < uiDiff)
                {
                    DoScriptText(EMOTE_PLASMA_BLAST, m_creature);
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_PLASMA_BLAST : SPELL_PLASMA_BLAST_H);
                    m_uiPlasmaBlastTimer = 30000;
                }
                else m_uiPlasmaBlastTimer -= uiDiff;

                if(m_uiNapalmTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if(!m_creature->IsWithinDistInMap(pTarget, 15))
                        {
                            DoCast(pTarget, m_bIsRegularMode ? SPELL_NAPALM_SHELL : SPELL_NAPALM_SHELL_H);
                            m_uiNapalmTimer = 7000;
                        }
                    }
                }
                else m_uiNapalmTimer -= uiDiff;
            }

            // proximity mines
            if(m_uiMinesTimer < uiDiff)
            {
                //DoCast(m_crreature, SPELL_PROXIMITY_MINES);
                for(uint8 i = 0; i < urand(8, 10); i++)
                {
                    float angle = (float) rand()*360/RAND_MAX + 1;
                    float homeX = m_creature->GetPositionX() + 15*cos(angle*(M_PI/180));
                    float homeY = m_creature->GetPositionY() + 15*sin(angle*(M_PI/180));
                    m_creature->SummonCreature(MOB_PROXIMITY_MINE, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 10000);
                }
                m_uiMinesTimer = 30000;
            }
            else m_uiMinesTimer -= uiDiff;

            // shock blast
            if(m_uiShockBlastTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_SHOCK_BLAST);
                m_uiShockBlastTimer = 50000;
            }
            else m_uiShockBlastTimer -= uiDiff;

            // hard mode script
            if(m_pInstance->GetData(TYPE_MIMIRON_HARD) == IN_PROGRESS && m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_LEVIATHAN)
            {
                if(m_creature->GetHealthPercent() < 50.0f && !m_bHasSuppresed)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    DoCast(m_creature, SPELL_FLAME_SUPRESSANT);
                    m_bHasSuppresed = true;
                    m_uiSupressTimer = 2000;
                    m_uiSetFireTimer = 10000;
                }

                if(m_uiSupressTimer < uiDiff && m_bHasSuppresed)
                {
                    SuppressFires();
                    m_uiSupressTimer = 600000;
                }
                else m_uiSupressTimer -= uiDiff;

                if(m_uiSetFireTimer < uiDiff && m_bHasSuppresed)
                {
                    // start again 3 fires
                    for(uint8 i = 0; i < 3; i++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTarget->InterruptNonMeleeSpells(true);
                            pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                        }
                    }
                    m_uiSetFireTimer = 600000;
                }
                else m_uiSetFireTimer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
        // outro for phase 1
        if(m_bIsOutro)
        {
            switch(m_uiOutroStep)
            {
            case 1:
                m_bStartAttack = false;
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->GetMotionMaster()->MovePoint(0, PosTankHome[0], PosTankHome[1], CENTER_Z); 
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (Creature* pMimiron = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_MIMIRON)))
                    DoScriptText(SAY_TANK_DEATH, pMimiron);
                ++m_uiOutroStep;
                m_uiOutroTimer = 12000;
                break;
            case 3:
                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_TRANS_1);
                // reset the miniboss for phase 4
                EnterEvadeMode();
                ++m_uiOutroStep;
                m_uiOutroTimer = 3000;
                break;
            }
        }
        else return;

        if (m_uiOutroTimer <= uiDiff)
        {
            ++m_uiOutroStep;
            m_uiOutroTimer = 330000;
        } m_uiOutroTimer -= uiDiff;
    }
};

// VX001 script
struct MANGOS_DLL_DECL boss_vx001AI : public ScriptedAI
{
    boss_vx001AI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    uint32 m_uiRapidBurstTimer;
    uint32 m_uiLaserBarrageTimer;
    uint32 m_uiRocketStrikeTimer;
    uint32 m_uiHeatWaveTimer;
    uint32 m_uiHandPulseTimer;

    uint32 m_uiFlameSuppressTimer;
    uint32 m_uiFrostBombTimer;
    uint32 m_uiSpreadFiresTimer;

    uint32 m_uiRepairTimer;
    bool m_bMustRepair;

    void Reset()
    {
        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 12000;
        m_bMustRepair           = false;
        m_uiRepairTimer         = 15000;

        m_uiRapidBurstTimer     = 1000;
        m_uiLaserBarrageTimer   = 60000;
        m_uiRocketStrikeTimer   = 25000;
        m_uiHeatWaveTimer       = 20000;
        m_uiHandPulseTimer      = 1000;

        m_uiFlameSuppressTimer  = urand(10000, 15000);
        m_uiFrostBombTimer      = urand(25000, 30000);
        m_uiSpreadFiresTimer    = urand(40000, 50000);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VX001, NOT_STARTED);
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Creature* pMimiron = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_MIMIRON)))
        {
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_VX001)
            {
                if(irand(0,1))
                    DoScriptText(SAY_TORSO_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_TORSO_SLAY2, pMimiron);
            }
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_ROBOT)
            {
                if(irand(0,1))
                    DoScriptText(SAY_ROBOT_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_ROBOT_SLAY2, pMimiron);
            }
        }
    }

    // hacky way for phase 4. needs rewriging when vehicles are fixed
    void SetPhase()
    {
        //SetCombatMovement(true);
        m_uiLaserBarrageTimer   = 60000;
        m_uiRocketStrikeTimer   = 25000;
        m_uiHandPulseTimer      = 1000;

        // look like a robot
        m_creature->GetMotionMaster()->MoveIdle();
        SetCombatMovement(false);
        m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  3, 0.0f);
        m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  3, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_VX001)
            {
                if (Creature* pMimiron = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_MIMIRON)))
                    DoScriptText(SAY_TORSO_DEATH, pMimiron);
                m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_TRANS_2);
                m_pInstance->SetData(TYPE_VX001, DONE);
            }
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_VX001)
            return;

        // hacky way for feign death, needs fixing
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_ROBOT)
        {
            if(uiDamage > m_creature->GetHealth())
            {
                uiDamage = 0;
                m_creature->SetHealth(0);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovementExpired(false);
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->CombatStop();
                m_bMustRepair   = true;
                m_uiRepairTimer = 15000;
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_VX001, SPECIAL);
            }
        }
    }

    // hacky way for phase 4. needs rewriging when vehicles are fixed
    void Repair()
    {
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());
        SetPhase();

        if(m_pInstance)
            m_pInstance->SetData(TYPE_VX001, IN_PROGRESS);
    }

    void SuppressFires()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, 10.0f);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, 10.0f);
        if (!lFires.empty())
        {
            for(std::list<Creature*>::iterator iter = lFires.begin(); iter != lFires.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    Creature* SelectRandomFire()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, DEFAULT_VISIBILITY_INSTANCE);

        //This should not appear!
        if (lFires.empty()){
            m_uiFrostBombTimer = 5000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lFires.begin();
        advance(iter, urand(0, lFires.size()-1));

        if((*iter)->isAlive())
            return *iter;
        else
        {
            m_uiFrostBombTimer = 500;
            return NULL;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            if(GameObject* pLift = GetClosestGameObjectWithEntry(m_creature, GO_MIMIRON_ELEVATOR, DEFAULT_VISIBILITY_INSTANCE))
                pLift->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
        }
        else m_uiAttackStartTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiRepairTimer < uiDiff && m_bMustRepair)
        {
            SetPhase();
            Repair();
            m_bMustRepair = false;
        }
        else m_uiRepairTimer -= uiDiff;

        // return if repairing
        if(m_bMustRepair)
            return;

        // only in VX001 phase
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_VX001)
        {
            if(m_uiRapidBurstTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_RAPID_BURST : SPELL_RAPID_BURST_H);
                m_uiRapidBurstTimer = 1000;
            }
            else m_uiRapidBurstTimer -= uiDiff;

            if(m_uiHeatWaveTimer < uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_HEAT_WAVE : SPELL_HEAT_WAVE_H);
                m_uiHeatWaveTimer = 10000;
            }
            else m_uiHeatWaveTimer -= uiDiff;
        }

        // only in robot phase
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_ROBOT)
        {
            if(m_uiHandPulseTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_HAND_PULSE : SPELL_HAND_PULSE_H);
                m_uiHandPulseTimer = 1000;
            }
            else m_uiHandPulseTimer -= uiDiff;
        }

        if(m_uiLaserBarrageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_LASER_TRIGG);
            m_uiLaserBarrageTimer = urand(50000, 60000);
        }
        else m_uiLaserBarrageTimer -= uiDiff;

        // this needs vehicles in order to make the rocket move to the target
        if(m_uiRocketStrikeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Creature* pTemp = m_creature->SummonCreature(NPC_MIMIRON_FOCUS, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000))
                {
                    pTemp->setFaction(14);
                    pTemp->GetMotionMaster()->MoveIdle();
                    pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pTemp->CombatStop();
                    pTemp->SetDisplayId(11686);     // make invisible
                    pTemp->CastSpell(pTemp, SPELL_ROCKET_STRIKE, false);
                }
            }
            m_uiRocketStrikeTimer = urand(25000, 30000);
        }
        else m_uiRocketStrikeTimer -= uiDiff;

        // hard mode
        if(m_pInstance->GetData(TYPE_MIMIRON_HARD) == IN_PROGRESS)
        {
            // only in VX001 phase
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_VX001)
            {
                if(m_uiFlameSuppressTimer < uiDiff)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    DoCast(m_creature, SPELL_FLAME_SUPRESS);
                    SuppressFires();
                    m_uiFlameSuppressTimer = urand(9000, 10000);
                }
                else m_uiFlameSuppressTimer -= uiDiff;
            }

            if(m_uiFrostBombTimer < uiDiff)
            {
                if(Creature* pFire = SelectRandomFire())
                    m_creature->SummonCreature(MOB_FROST_BOMB, pFire->GetPositionX(), pFire->GetPositionY(), pFire->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 10000);
                m_uiFrostBombTimer = urand(50000, 60000);
                m_uiSpreadFiresTimer = urand(15000, 20000);
            }
            else m_uiFrostBombTimer -= uiDiff;

            if(m_uiSpreadFiresTimer < uiDiff)
            {
                // start again 3 fires
                for(uint8 i = 0; i < 3; i++)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        pTarget->InterruptNonMeleeSpells(true);
                        pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                    }
                }
                m_uiSpreadFiresTimer = 60000;
            }
            else m_uiSpreadFiresTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

// Aerial command unit script
struct MANGOS_DLL_DECL boss_aerial_command_unitAI : public ScriptedAI
{
    boss_aerial_command_unitAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    uint32 m_uiPlasmaBallTimer;
    uint32 m_uiSummonWavesTimer;
    uint32 m_uiGroundTimer;
    bool m_bIsGrounded;
    uint32 m_uiSpreadFiresTimer;

    uint32 m_uiRepairTimer;
    bool m_bMustRepair;

    void Reset()
    {
        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 5000;
        m_uiSpreadFiresTimer    = urand(40000, 50000);

        m_uiPlasmaBallTimer     = 3000;
        m_uiSummonWavesTimer    = 10000;
        m_bIsGrounded = false;

        m_bMustRepair           = false;
        m_uiRepairTimer         = 15000;

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_AERIAL_UNIT, NOT_STARTED);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_AERIAL)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
            m_creature->GetMotionMaster()->MoveIdle();
            SetCombatMovement(false);
            m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, 0.0f);
            m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_AERIAL)
        {
            if(m_creature->HasAura(SPELL_MAGNETIC_CORE, EFFECT_INDEX_0))
                uiDamage += uiDamage;
            return;
        }

        // hacky way for feign death
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_ROBOT)
        {
            if(uiDamage > m_creature->GetHealth())
            {
                uiDamage = 0;
                m_creature->SetHealth(0);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovementExpired(false);
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->CombatStop();
                m_bMustRepair   = true;
                m_uiRepairTimer = 15000;
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_AERIAL_UNIT, SPECIAL);
            }
        }
    }

    // hacky way for phase 4. needs rewriging when vehicles are fixed
    void Repair()
    {
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());
        SetPhase();

        if(m_pInstance)
            m_pInstance->SetData(TYPE_AERIAL_UNIT, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_AERIAL)
            {
                if (Creature* pMimiron = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_MIMIRON)))
                    DoScriptText(SAY_HEAD_DEATH, pMimiron);
                m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_TRANS_3);
                m_pInstance->SetData(TYPE_AERIAL_UNIT, DONE);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Creature* pMimiron = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_MIMIRON)))
        {
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_AERIAL)
            {
                if(irand(0,1))
                    DoScriptText(SAY_HEAD_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_HEAD_SLAY2, pMimiron);
            }
            if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_ROBOT)
            {
                if(irand(0,1))
                    DoScriptText(SAY_ROBOT_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_ROBOT_SLAY2, pMimiron);
            }
        }
    }

    // hacky way for phase 4. needs rewriging when vehicles are fixed
    void SetPhase()
    {
        m_uiPlasmaBallTimer = 3000;

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        SetCombatMovement(false);
        m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  6.5f, 0.0f);
        m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 6.5f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

    // get the boss down by the magnetic core
    void SetToGround()
    {
        m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), CENTER_Z, 0);
        m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), CENTER_Z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
        m_bIsGrounded = true;
        // make boss land
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        m_uiGroundTimer = 20000;
    }

    void JustSummoned(Creature* pSummon)
    {
        pSummon->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
        }
        else m_uiAttackStartTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiRepairTimer < uiDiff && m_bMustRepair)
        {
            SetPhase();
            Repair();
            m_bMustRepair = false;
        }
        else m_uiRepairTimer -= uiDiff;

        // return if repairing
        if(m_bMustRepair)
            return;

        if (m_creature->HasAura(SPELL_MAGNETIC_CORE, EFFECT_INDEX_0))
            return;

        if (m_uiGroundTimer < uiDiff && m_bIsGrounded)
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, 0.0f);
            m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
            m_bIsGrounded = false;
            // make boss fly
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_BOMB_BOT_SUMMON);
        }else m_uiGroundTimer -= uiDiff;

        if(m_uiPlasmaBallTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_PLASMA_BALL : SPELL_PLASMA_BALL_H);
            m_uiPlasmaBallTimer = urand(3000, 5000);
        }
        else m_uiPlasmaBallTimer -= uiDiff;

        // spawn adds in arena, only in phase 3
        if(m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_AERIAL)
        {
            if(m_uiSummonWavesTimer < uiDiff)
            {
                uint32 m_uiCreatureEntry;
                // summon emergency boots
                if(m_pInstance->GetData(TYPE_MIMIRON_HARD) == IN_PROGRESS)
                {
                    switch(urand(0, 4))
                    {
                    case 0:
                    case 1:
                        m_uiCreatureEntry = MOB_JUNK_BOT;
                        break;
                    case 2:
                    case 3:
                        m_uiCreatureEntry = MOB_EMERGENCY_FIRE_BOT;
                        break;
                    case 4:
                        m_uiCreatureEntry = MOB_ASSALT_BOT;
                        break;
                    }
                }
                else
                {
                    switch(urand(0, 2))
                    {
                    case 0:
                    case 1:
                        m_uiCreatureEntry = MOB_JUNK_BOT;
                        break;
                    case 2:
                        m_uiCreatureEntry = MOB_ASSALT_BOT;
                        break;
                    }
                }
                uint8 m_uiSummonLoc = urand(0, 8);
                if(m_uiCreatureEntry != 0)
                    m_creature->SummonCreature(m_uiCreatureEntry, SummonLoc[m_uiSummonLoc].x, SummonLoc[m_uiSummonLoc].y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);

                m_uiSummonWavesTimer = urand (10000, 15000);
            }
            else m_uiSummonWavesTimer -= uiDiff; 

            if(m_pInstance->GetData(TYPE_MIMIRON_HARD) == IN_PROGRESS)
            {
                if(m_uiSpreadFiresTimer < uiDiff)
                {
                    // start again 3 fires
                    for(uint8 i = 0; i < 3; i++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTarget->InterruptNonMeleeSpells(true);
                            pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                        }
                    }
                    m_uiSpreadFiresTimer = urand(40000, 50000);
                }
                else m_uiSpreadFiresTimer -= uiDiff;
            }
        }
    }
};

// Mimiron, event controller
// boss should be placed inside the vehicles when they are supported by mangos
struct MANGOS_DLL_DECL boss_mimironAI : public ScriptedAI
{
    boss_mimironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    bool m_bIsHardMode;
    uint32 m_uiSelfDestructTimer;
    uint32 m_uiUseLiftTimer;
    uint32 m_uiPhaseDelayTimer;
    uint32 m_uiRobotDelayTimer;
    uint32 m_uiSelfRepairTimer;
    uint32 m_uiBerserkTimer;
    bool m_bHasChecked;
    uint32 m_uiOutroTimer;
    uint32 m_uiHpCheckTimer;
    bool m_bHasMoreHp;

    uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;
    bool m_bIsIntro;
    bool m_bIsRobotReady;

    uint64 m_uiTankGUID;
    uint64 m_uiTorsoGUID;
    uint64 m_uiHeadGUID;

    bool m_bIsTankDead;
    bool m_bIsTorsoDead;
    bool m_bIsHeadDead;

    void Reset()
    {
        m_bIsHardMode           = false;
        m_uiSelfDestructTimer   = 460000;  // 8 min
        m_bIsIntro              = true;
        m_uiPhaseDelayTimer     = 7000;
        m_uiUseLiftTimer        = 4000;
        m_uiBerserkTimer        = 900000;   // 15 min
        m_bHasChecked           = false;
        m_bHasMoreHp            = false;
        m_bIsRobotReady         = false;

        m_uiIntroTimer          = 10000;
        m_uiIntroStep           = 1;

        m_bIsTankDead           = false;
        m_bIsTorsoDead          = false;
        m_bIsHeadDead           = false;

        m_uiTankGUID            = 0;
        m_uiTorsoGUID           = 0;
        m_uiHeadGUID            = 0;

		// reset button
        if(GameObject* pButton = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_MIMIRON_BUTTON)))
            pButton->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);

		// reset elevator
        if(GameObject* pLift = GetClosestGameObjectWithEntry(m_creature, GO_MIMIRON_ELEVATOR, DEFAULT_VISIBILITY_INSTANCE))
            pLift->SetGoState(GO_STATE_ACTIVE);

		// kill torso and Head
        if(Creature* pTorso = GetClosestCreatureWithEntry(m_creature, NPC_VX001, 80.0f))
            pTorso->DealDamage(pTorso, pTorso->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if(Creature* pHead = GetClosestCreatureWithEntry(m_creature, NPC_AERIAL_UNIT, 80.0f))
            pHead->DealDamage(pHead, pHead->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

		// reset tank
        if (Creature* pTank = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEVIATHAN_MK)))
        {
            if(pTank->isAlive())
                pTank->AI()->EnterEvadeMode();
            else
                pTank->Respawn();
        }

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_IDLE);
            m_pInstance->SetData(TYPE_MIMIRON_HARD, NOT_STARTED);
        }

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_MIMIRON, NOT_STARTED);
    }

	// start event
    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_MIMIRON, IN_PROGRESS);
			// activate teleporter
            if(m_pInstance->GetData(TYPE_MIMIRON_TP) != DONE)
                m_pInstance->SetData(TYPE_MIMIRON_TP, DONE);
			// start intro
            if(m_pInstance->GetData(TYPE_MIMIRON) != DONE)
                m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_INTRO);
        }
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DoOutro()
    {
        if(m_pInstance) 
        {
            if(m_bIsHardMode)
            {
                m_pInstance->SetData(TYPE_MIMIRON_HARD, DONE);
                // hacky way to complete achievements; use only if you have this function
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_FIREFIGHTER : ACHIEV_FIREFIGHTER_H);
            }
            m_pInstance->SetData(TYPE_MIMIRON, DONE);
        }
        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
       if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_MIMIRON, DONE);
            if(m_bIsHardMode)
                m_pInstance->SetData(TYPE_MIMIRON_HARD, DONE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(m_pInstance->GetData(TYPE_MIMIRON_PHASE))
        {
        case PHASE_INTRO:
            {
                if(m_bIsIntro)
                {
                    //hard mode check
                    switch(m_uiIntroStep)
                    {
                    case 1:
                        ++m_uiIntroStep;
                        m_uiIntroTimer = 10000;
                        break;
                    case 3:
                        if(GameObject* pButton = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_MIMIRON_BUTTON)))
                            pButton->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                        if(m_bIsHardMode)
                        {
                            DoScriptText(SAY_HARD_MODE, m_creature);
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 15000;
                        }
                        else
                        {
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 1000;
                        }
                        break;
                    case 5:
                        if (Creature* pTank = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEVIATHAN_MK)))
                        {
                            if(pTank->isAlive())
                            {
                                DoScriptText(SAY_TANK_ACTIVE, m_creature);
                                pTank->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, CENTER_Z);
                                m_uiTankGUID = pTank->GetGUID();
                            }
                            else
                                EnterEvadeMode();
                        }
                        ++m_uiIntroStep;
                        m_uiIntroTimer = 18000;
                        break;
                    case 7:
                        if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                        {
                            if(m_bIsHardMode)
                            {
                                pTank->CastSpell(pTank, SPELL_EMERGENCY_MODE, false);
                                ++m_uiIntroStep;
                                m_uiIntroTimer = 1000;
                            }
                            else
                            {
                                if(m_pInstance)
                                    m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_LEVIATHAN);
                                        ((boss_leviathan_mkAI*)pTank->AI())->m_bStartAttack = true;
                                pTank->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pTank->SetInCombatWithZone();
                                m_uiBerserkTimer        = 900000;   // 15 min
                                m_bIsIntro = false;
                                ++m_uiIntroStep;
                                m_uiIntroTimer = 9000;
                            }
                        }
                        break;
                    case 9:
                        if(m_bIsHardMode)
                        {
                            if(m_pInstance)
                                m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_LEVIATHAN);
                            if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                            {
                                pTank->SetHealth(pTank->GetMaxHealth()+ (pTank->GetMaxHealth() * 0.3));
                                ((boss_leviathan_mkAI*)pTank->AI())->m_bStartAttack = true;
                                pTank->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pTank->SetInCombatWithZone();
                            }
                            m_uiSelfDestructTimer   = 460000;  // 8 min
                            m_bIsIntro = false;
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 9000;
                        }
                        break;
                    }
                }
                else return;

                if (m_uiIntroTimer <= uiDiff)
                {
                    ++m_uiIntroStep;
                    m_uiIntroTimer = 330000;
                } 
                m_uiIntroTimer -= uiDiff;

                break;
            }
        case PHASE_LEVIATHAN:
            // leviathan MK phase: see above script
            break;
        case PHASE_TRANS_1:
            {
                if(m_uiUseLiftTimer < uiDiff)
                {
                    if(GameObject* pLift = GetClosestGameObjectWithEntry(m_creature, GO_MIMIRON_ELEVATOR, DEFAULT_VISIBILITY_INSTANCE))
                        m_pInstance->DoUseDoorOrButton(pLift->GetGUID());
                    m_uiUseLiftTimer = 60000;
                }
                else m_uiUseLiftTimer -= uiDiff;

                if(m_uiPhaseDelayTimer < uiDiff && !m_bHasMoreHp)
                {
                    DoScriptText(SAY_TORSO_ACTIVE, m_creature);
                    if(Creature* pTorso = m_creature->SummonCreature(NPC_VX001, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                    {
                        if(m_bIsHardMode)
                        {
                            pTorso->CastSpell(pTorso, SPELL_EMERGENCY_MODE, false);
                            m_bHasMoreHp        = true;
                            m_uiHpCheckTimer    = 1000;
                            m_uiTorsoGUID = pTorso->GetGUID();
                        }
                        else
                        {
                            m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_VX001);
                            m_uiPhaseDelayTimer = 10000;
                        }
                    }
                }
                else m_uiPhaseDelayTimer -= uiDiff;

                if (m_uiHpCheckTimer <= uiDiff && m_bHasMoreHp)
                {
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                        pTorso->SetHealth(pTorso->GetMaxHealth()+ (pTorso->GetMaxHealth() * 0.3));
                    m_bHasMoreHp        = false;
                    m_uiPhaseDelayTimer = 10000;
                    m_uiHpCheckTimer    = 10000;
                    m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_VX001);
                    // start again 3 fires
                    for(uint8 i = 0; i < 3; i++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTarget->InterruptNonMeleeSpells(true);
                            pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                        }
                    }
                }
                else m_uiHpCheckTimer -= uiDiff;

                break;
            }
        case PHASE_VX001:
            // VX001 phase: see above script
            break;
        case PHASE_TRANS_2:
            {
                if(m_uiPhaseDelayTimer < uiDiff && !m_bHasMoreHp)
                {
                    DoScriptText(SAY_HEAD_ACTIVE, m_creature);
                    if(Creature* pHead = m_creature->SummonCreature(NPC_AERIAL_UNIT, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                    {
                        if(m_bIsHardMode)
                        {
                            pHead->CastSpell(pHead, SPELL_EMERGENCY_MODE, false);
                            m_bHasMoreHp        = true;
                            m_uiHpCheckTimer    = 1000;
                        }
                        else
                        {
                            m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_AERIAL);
                            m_uiPhaseDelayTimer = 15000;
                        }
                        m_uiHeadGUID = pHead->GetGUID();
                    }
                }
                else m_uiPhaseDelayTimer -= uiDiff;

                if (m_uiHpCheckTimer <= uiDiff && m_bHasMoreHp)
                {
                    if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                        pHead->SetHealth(pHead->GetMaxHealth()+ (pHead->GetMaxHealth() * 0.3));
                    m_bHasMoreHp        = false;
                    m_uiPhaseDelayTimer = 15000;
                    m_uiHpCheckTimer    = 10000;
                    m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_AERIAL);
                    // start again 3 fires
                    for(uint8 i = 0; i < 3; i++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTarget->InterruptNonMeleeSpells(true);
                            pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                        }
                    }
                }
                else m_uiHpCheckTimer -= uiDiff;

                break;
            }
        case PHASE_AERIAL:
            // Aerial Unit phase: see above script
            break;
        case PHASE_TRANS_3:
            {
                if(m_uiPhaseDelayTimer < uiDiff && !m_bIsRobotReady)
                {
                    if (Creature* pTank = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEVIATHAN_MK)))
                    {
                        if(pTank->isAlive())
                        {
                            pTank->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, CENTER_Z);
                            m_uiTankGUID = pTank->GetGUID();
                            m_bIsRobotReady = true;
                            m_uiRobotDelayTimer = 15000;
                        }
                        else
                            EnterEvadeMode();
                    }
                    m_uiPhaseDelayTimer = 100000;
                }
                else m_uiPhaseDelayTimer -= uiDiff;

                if(m_uiRobotDelayTimer < uiDiff && m_bIsRobotReady && !m_bHasMoreHp)
                {
                    DoScriptText(SAY_ROBOT_ACTIVE, m_creature);
                    if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                        ((boss_leviathan_mkAI*)pTank->AI())->SetPhase();

                    if(Creature* pTorso = m_creature->SummonCreature(NPC_VX001, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                    {
                        ((boss_vx001AI*)pTorso->AI())->SetPhase();
                        m_uiTorsoGUID = pTorso->GetGUID();
                    }

                    if(Creature* pHead = m_creature->SummonCreature(NPC_AERIAL_UNIT, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                    {
                        ((boss_aerial_command_unitAI*)pHead->AI())->SetPhase();
                        m_uiHeadGUID = pHead->GetGUID();
                    }

                    if(m_bIsHardMode)
                    {
                        if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                            pTorso->CastSpell(pTorso, SPELL_EMERGENCY_MODE, false);
                        if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                            pHead->CastSpell(pHead, SPELL_EMERGENCY_MODE, false);
                        if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                            pTank->CastSpell(pTank, SPELL_EMERGENCY_MODE, false);
                        m_bHasMoreHp = true;
                        m_uiHpCheckTimer = 1000;
                    }
                    else
                    {
                        if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                            pTorso->SetHealth(pTorso->GetMaxHealth() * 0.5);
                        if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                            pHead->SetHealth(pHead->GetMaxHealth() * 0.5);
                        if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                            pTank->SetHealth(pTank->GetMaxHealth() * 0.5);
                        m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_ROBOT);
                    }
                    m_uiRobotDelayTimer = 100000;
                }
                else m_uiRobotDelayTimer -= uiDiff;

                if (m_uiHpCheckTimer <= uiDiff && m_bHasMoreHp)
                {
                    if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                        pHead->SetHealth(pHead->GetMaxHealth() * 0.5);
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                        pTorso->SetHealth(pTorso->GetMaxHealth()* 0.5);
                    if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                        pTank->SetHealth(pTank->GetMaxHealth()* 0.5);   
                    m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_ROBOT);
                    // start again 3 fires
                    for(uint8 i = 0; i < 3; i++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTarget->InterruptNonMeleeSpells(true);
                            pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                        }
                    }
                    m_uiHpCheckTimer = 10000;
                }
                else m_uiHpCheckTimer -= uiDiff;

                break;
            }
        case PHASE_ROBOT:
            {
                if(m_pInstance->GetData(TYPE_LEVIATHAN_MK) == SPECIAL && !m_bHasChecked)
                {
                    m_uiSelfRepairTimer = 15000;
                    m_bIsTankDead       = true;
                    m_bHasChecked       = true;
                }
                if(m_pInstance->GetData(TYPE_VX001) == SPECIAL && !m_bHasChecked)
                {
                    m_uiSelfRepairTimer = 15000;
                    m_bIsTorsoDead      = true;
                    m_bHasChecked       = true;
                }
                if(m_pInstance->GetData(TYPE_AERIAL_UNIT) == SPECIAL && !m_bHasChecked)
                {
                    m_uiSelfRepairTimer = 15000;
                    m_bIsHeadDead       = true;
                    m_bHasChecked       = true;
                }

                if(m_uiSelfRepairTimer < uiDiff && m_bHasChecked)
                {
                    if(m_pInstance->GetData(TYPE_LEVIATHAN_MK) == SPECIAL)
                        m_bIsTankDead = true;
                    if(m_pInstance->GetData(TYPE_VX001) == SPECIAL)
                        m_bIsTorsoDead = true;
                    if(m_pInstance->GetData(TYPE_AERIAL_UNIT) == SPECIAL)
                        m_bIsHeadDead = true;

                    if(m_bIsTankDead && m_bIsTorsoDead && m_bIsHeadDead)
                    {
                        DoScriptText(SAY_ROBOT_DEATH, m_creature);
                        m_uiOutroTimer = 15000;
                        m_pInstance->SetData(TYPE_MIMIRON_PHASE, PHASE_OUTRO);
                    }
                    else
                    {
                        m_bHasChecked = false;
                        if(m_bIsTankDead)
                        {
                            if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                                ((boss_leviathan_mkAI*)pTank->AI())->Repair();
                        }
                        if(m_bIsTorsoDead)
                        {
                            if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                                ((boss_vx001AI*)pTorso->AI())->Repair();
                        }
                        if(m_bIsHeadDead)
                        {
                            if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                                ((boss_aerial_command_unitAI*)pHead->AI())->Repair();
                        }
                    }
                    m_uiSelfRepairTimer = 1000;
                }
                else m_uiSelfRepairTimer -= uiDiff;

                break;
            }
        case PHASE_OUTRO:
            {
                if(m_uiOutroTimer < uiDiff)
                {
                    if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                        m_creature->DealDamage(pTank, pTank->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                        m_creature->DealDamage(pHead, pHead->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                        m_creature->DealDamage(pTorso, pTorso->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    DoOutro();
                    m_uiOutroTimer = 60000;
                }
                else m_uiOutroTimer -= uiDiff;

                break;
            }
        }

        // berserk
        if (m_uiBerserkTimer <= uiDiff)
        {
            if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
            {
                if(pTank && pTank->isAlive())
                    pTank->CastSpell(pTank, SPELL_BERSERK, false);
            }

            if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
            {
                if(pTorso && pTorso->isAlive())
                    pTorso->CastSpell(pTorso, SPELL_BERSERK, false);
            }

            if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
            {
                if(pHead && pHead->isAlive())
                    pHead->CastSpell(pHead, SPELL_BERSERK, false);
            }

            m_uiBerserkTimer = 330000;
        } 
        else
            m_uiBerserkTimer -= uiDiff;

        // self destruct platform in hard mode
        if (m_uiSelfDestructTimer < uiDiff && m_bIsHardMode)
        {
            m_creature->SummonCreature(NPC_MIMIRON_INFERNO, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
            // visual part, hacky way
            if(Creature* pTemp = m_creature->SummonCreature(NPC_MIMIRON_FOCUS, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
            {
                pTemp->GetMotionMaster()->MoveIdle();
                pTemp->CombatStop();
                pTemp->SetDisplayId(11686);     // make invisible
                pTemp->CastSpell(pTemp, SPELL_SELF_DESTRUCTION, false);
            }
            m_uiSelfDestructTimer = 60000;
        }
        else m_uiSelfDestructTimer -= uiDiff;
    }
};

// Leviathan MK turret
// used in phase 1; should be attached by a vehicle seat to the Leviathan MK
struct MANGOS_DLL_DECL leviathan_turretAI : public ScriptedAI
{
    leviathan_turretAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetVisibility(VISIBILITY_OFF);   
        //pCreature->setFaction(14);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPlasmaBlastTimer;
    uint32 m_uiNapalmShellTimer;

    void Reset()
    {
        m_uiPlasmaBlastTimer = 20000;
        m_uiNapalmShellTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiPlasmaBlastTimer < uiDiff)
        {
            if (Creature* pTank = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEVIATHAN_MK)))
            {
                DoScriptText(EMOTE_PLASMA_BLAST, m_creature);
                if (Unit* pTarget = pTank->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_PLASMA_BLAST : SPELL_PLASMA_BLAST_H);
            }
            m_uiPlasmaBlastTimer = 30000;
        }
        else m_uiPlasmaBlastTimer -= uiDiff;

        if(m_uiNapalmShellTimer < uiDiff)
        {
            if (Creature* pTank = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_LEVIATHAN_MK)))
            {
                if (Unit* pTarget = pTank->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(!pTank->IsWithinDistInMap(pTarget, 15))
                    {
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_NAPALM_SHELL : SPELL_NAPALM_SHELL_H);
                        m_uiNapalmShellTimer = 7000;
                    }
                }
            }
        }
        else m_uiNapalmShellTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_proximity_mineAI : public ScriptedAI
{
    mob_proximity_mineAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiExplosionTimer;
    uint32 m_uiRangeCheckTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiExplosionTimer  = 60000;
        m_uiRangeCheckTimer = 1000;
        m_uiDieTimer        = 65000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MIMIRON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDieTimer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else
            m_uiDieTimer -= uiDiff;

        if(m_uiExplosionTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_EXPLOSION : SPELL_EXPLOSION_H);
            m_uiDieTimer        = 500;
            m_uiExplosionTimer = 20000;
        }
        else m_uiExplosionTimer -= uiDiff;

        if (m_uiRangeCheckTimer < uiDiff)
        {
            if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 2))
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_EXPLOSION : SPELL_EXPLOSION_H);
                m_uiDieTimer = 500;
                m_uiRangeCheckTimer = 5000;
            }
            else
                m_uiRangeCheckTimer = 500;
        }
        else m_uiRangeCheckTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_bomb_botAI : public ScriptedAI
{
    mob_bomb_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiRangeCheckTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiRangeCheckTimer = 1000;
        m_uiDieTimer        = 600000;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            DoCast(m_creature, SPELL_BOMB_BOT);
            m_creature->SetHealth(m_creature->GetMaxHealth());
            uiDamage = 0;
            m_uiDieTimer = 500;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDieTimer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else
            m_uiDieTimer -= uiDiff;

        if (m_uiRangeCheckTimer < uiDiff)
        {
            if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 2))
            {
                DoCast(m_creature, SPELL_BOMB_BOT);
                m_uiDieTimer = 500;
                m_uiRangeCheckTimer = 5000;
            }
            else
                m_uiRangeCheckTimer = 500;
        }
        else m_uiRangeCheckTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_assault_botAI : public ScriptedAI
{
    mob_assault_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiMagneticFieldTimer;

    void Reset()
    {
        m_uiMagneticFieldTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMagneticFieldTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_MAGNETIC_FIELD);
            m_uiMagneticFieldTimer = urand(10000, 15000);
        }
        else m_uiMagneticFieldTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_emergency_botAI : public ScriptedAI
{
    mob_emergency_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiWaterSprayTimer;

    void Reset()
    {
        m_uiWaterSprayTimer = urand(5000, 10000);
        if(!m_bIsRegularMode)
            DoCast(m_creature, SPELL_DEAFENING_SIREN);
    }

    void SuppressFires()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, 15.0f);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, 15.0f);
        if (!lFires.empty())
        {
            for(std::list<Creature*>::iterator iter = lFires.begin(); iter != lFires.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiWaterSprayTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_WATER_SPRAY);
            SuppressFires();
            m_uiWaterSprayTimer = urand(7000, 12000);
        }
        else m_uiWaterSprayTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_frost_bomb_ulduarAI : public ScriptedAI
{
    mob_frost_bomb_ulduarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiExplosionTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiExplosionTimer  = 10000;
        m_uiDieTimer        = 15000;
        DoCast(m_creature, SPELL_FROST_BOMB_VISUAL);
    }

    void SuppressFires()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, 30.0f);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, 30.0f);
        if (!lFires.empty())
        {
            for(std::list<Creature*>::iterator iter = lFires.begin(); iter != lFires.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiDieTimer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else
            m_uiDieTimer -= uiDiff;

        if (m_uiExplosionTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_FROST_BOMB_EXPL);
            SuppressFires();
            m_uiExplosionTimer = 100000;
            m_uiDieTimer = 500;
        }
        else m_uiExplosionTimer -= uiDiff;
    }
};

// Flames used in hard mode
struct MANGOS_DLL_DECL mob_mimiron_flamesAI : public ScriptedAI
{
    mob_mimiron_flamesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiFlamesSpreadTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_FLAMES);
        m_uiFlamesSpreadTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MIMIRON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		// spread flames
        if(m_uiFlamesSpreadTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAMES_SPREAD);
            m_uiFlamesSpreadTimer = urand(2000, 5000);
        }
        else m_uiFlamesSpreadTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_mimiron_infernoAI : public ScriptedAI
{
    mob_mimiron_infernoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->setFaction(14);
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiFlamesTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_SELF_DESTRUCTION);
        m_uiFlamesTimer = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MIMIRON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(m_uiFlamesTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_SELF_DESTRUCT);
            m_uiFlamesTimer = 1000;
        }
        else m_uiFlamesTimer -= uiDiff;
    }
};

// item script, used to bring the aerial unit down
struct MANGOS_DLL_DECL mob_magnetic_coreAI : public ScriptedAI
{
    mob_magnetic_coreAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpellTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiSpellTimer = 2000;
        m_uiDieTimer = 23000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiSpellTimer < uiDiff)
        {
            if(Creature* pAerial = GetClosestCreatureWithEntry(m_creature, NPC_AERIAL_UNIT, 10.0f))
            {
                DoCast(pAerial, SPELL_MAGNETIC_CORE);
                ((boss_aerial_command_unitAI*)pAerial->AI())->SetToGround();
            }
            m_uiSpellTimer = 100000;
        }
        else m_uiSpellTimer -= uiDiff;

        if (m_uiDieTimer < uiDiff)
            m_creature->ForcedDespawn();
        else
            m_uiDieTimer -= uiDiff;
    }
};

// Red button -> used to start the hard mode
bool GOUse_go_red_button(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!m_pInstance)
        return false;

    if (Creature* pMimiron = pGo->GetMap()->GetCreature(m_pInstance->GetData64(NPC_MIMIRON)))
    {
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1); 
        pPlayer->CastSpell(pPlayer, SPELL_FLAMES_SUMMON, false);
        if(pMimiron->isAlive() && m_pInstance->GetData(TYPE_MIMIRON_PHASE) == PHASE_INTRO)
            ((boss_mimironAI*)pMimiron->AI())->m_bIsHardMode = true;
        m_pInstance->SetData(TYPE_MIMIRON_HARD, IN_PROGRESS);
    }

    return false;
}

CreatureAI* GetAI_boss_mimiron(Creature* pCreature)
{
    return new boss_mimironAI(pCreature);
}

CreatureAI* GetAI_boss_leviathan_mk(Creature* pCreature)
{
    return new boss_leviathan_mkAI(pCreature);
}

CreatureAI* GetAI_boss_vx001(Creature* pCreature)
{
    return new boss_vx001AI(pCreature);
}

CreatureAI* GetAI_boss_aerial_command_unit(Creature* pCreature)
{
    return new boss_aerial_command_unitAI(pCreature);
}

CreatureAI* GetAI_leviathan_turret(Creature* pCreature)
{
    return new leviathan_turretAI(pCreature);
}

CreatureAI* GetAI_mob_proximity_mine(Creature* pCreature)
{
    return new mob_proximity_mineAI(pCreature);
}

CreatureAI* GetAI_mob_bomb_bot(Creature* pCreature)
{
    return new mob_bomb_botAI(pCreature);
}

CreatureAI* GetAI_mob_assault_bot(Creature* pCreature)
{
    return new mob_assault_botAI(pCreature);
}

CreatureAI* GetAI_mob_emergency_bot(Creature* pCreature)
{
    return new mob_emergency_botAI(pCreature);
}

CreatureAI* GetAI_mob_frost_bomb_ulduar(Creature* pCreature)
{
    return new mob_frost_bomb_ulduarAI(pCreature);
}

CreatureAI* GetAI_mob_mimiron_flames(Creature* pCreature)
{
    return new mob_mimiron_flamesAI(pCreature);
}

CreatureAI* GetAI_mob_mimiron_inferno(Creature* pCreature)
{
    return new mob_mimiron_infernoAI(pCreature);
}

CreatureAI* GetAI_mob_magnetic_core(Creature* pCreature)
{
    return new mob_magnetic_coreAI(pCreature);
}

void AddSC_boss_mimiron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_mimiron";
    newscript->GetAI = &GetAI_boss_mimiron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_leviathan_mk";
    newscript->GetAI = &GetAI_boss_leviathan_mk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "leviathan_turret";
    newscript->GetAI = &GetAI_leviathan_turret;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_vx001";
    newscript->GetAI = &GetAI_boss_vx001;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_aerial_command_unit";
    newscript->GetAI = &GetAI_boss_aerial_command_unit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_proximity_mine";
    newscript->GetAI = &GetAI_mob_proximity_mine;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bomb_bot";
    newscript->GetAI = &GetAI_mob_bomb_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_assault_bot";
    newscript->GetAI = &GetAI_mob_assault_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_emergency_bot";
    newscript->GetAI = &GetAI_mob_emergency_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frost_bomb_ulduar";
    newscript->GetAI = &GetAI_mob_frost_bomb_ulduar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_mimiron_flames";
    newscript->GetAI = &GetAI_mob_mimiron_flames;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_mimiron_inferno";
    newscript->GetAI = &GetAI_mob_mimiron_inferno;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_magnetic_core";
    newscript->GetAI = &GetAI_mob_magnetic_core;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_red_button";
    newscript->pGOUse = &GOUse_go_red_button;
    newscript->RegisterSelf();
}
