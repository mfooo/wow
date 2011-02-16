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
SDName: Boss_Prince_Malchezzar
SD%Complete: 100
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"

#define SAY_AGGRO           -1532091
#define SAY_AXE_TOSS1       -1532092
#define SAY_AXE_TOSS2       -1532093
#define SAY_SPECIAL1        -1532094
#define SAY_SPECIAL2        -1532095
#define SAY_SPECIAL3        -1532096
#define SAY_SLAY1           -1532097
#define SAY_SLAY2           -1532098
#define SAY_SLAY3           -1532099
#define SAY_SUMMON1         -1532100
#define SAY_SUMMON2         -1532101
#define SAY_DEATH           -1532102

// 18 Coordinates for Infernal spawns
struct InfernalPoint
{
    float x,y;
};

#define INFERNAL_Z  275.5

static InfernalPoint InfernalPoints[] =
{
    {-10922.8f, -1985.2f},
    {-10916.2f, -1996.2f},
    {-10932.2f, -2008.1f},
    {-10948.8f, -2022.1f},
    {-10958.7f, -1997.7f},
    {-10971.5f, -1997.5f},
    {-10990.8f, -1995.1f},
    {-10989.8f, -1976.5f},
    {-10971.6f, -1973.0f},
    {-10955.5f, -1974.0f},
    {-10939.6f, -1969.8f},
    {-10958.0f, -1952.2f},
    {-10941.7f, -1954.8f},
    {-10943.1f, -1988.5f},
    {-10948.8f, -2005.1f},
    {-10984.0f, -2019.3f},
    {-10932.8f, -1979.6f},
    {-10935.7f, -1996.0f}
};

#define TOTAL_INFERNAL_POINTS 18

//Enfeeble is supposed to reduce hp to 1 and then heal player back to full when it ends
//Along with reducing healing and regen while enfeebled to 0%
//This spell effect will only reduce healing

#define SPELL_ENFEEBLE          30843                       //Enfeeble during phase 1 and 2
#define SPELL_ENFEEBLE_EFFECT   41624

#define SPELL_SHADOWNOVA        30852                       //Shadownova used during all phases
#define SPELL_SW_PAIN           30854                       //Shadow word pain during phase 1 and 3 (different targeting rules though)
#define SPELL_THRASH_PASSIVE    12787                       //Extra attack chance during phase 2
#define SPELL_SUNDER_ARMOR      30901                       //Sunder armor during phase 2
#define SPELL_THRASH_AURA       12787                       //Passive proc chance for thrash
#define SPELL_EQUIP_AXES        30857                       //Visual for axe equiping
#define SPELL_AMPLIFY_DAMAGE    39095                      //Amplifiy during phase 3
#define SPELL_CLEAVE            30131                      //Same as Nightbane.
#define SPELL_HELLFIRE          30859                       //Infenals' hellfire aura
#define NETHERSPITE_INFERNAL    17646                       //The netherspite infernal creature
#define MALCHEZARS_AXE          17650                       //Malchezar's axes (creatures), summoned during phase 3

#define INFERNAL_MODEL_INVISIBLE    11686                   //Infernal Effects
#define SPELL_INFERNAL_RELAY        30834

#define EQUIP_ID_AXE                33542                   //Axes info

//---------Infernal code first
struct MANGOS_DLL_DECL netherspite_infernalAI : public ScriptedAI
{
    netherspite_infernalAI(Creature* pCreature) : ScriptedAI(pCreature) ,
        malchezaar(0), HellfireTimer(0), CleanupTimer(0), point(NULL) {Reset();}

    uint32 HellfireTimer;
    uint32 CleanupTimer;
    uint64 malchezaar;
    InfernalPoint *point;

    void Reset() {}
    void MoveInLineOfSight(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        if (HellfireTimer)
        {
            if (HellfireTimer <= diff)
            {
                DoCastSpellIfCan(m_creature, SPELL_HELLFIRE);
                HellfireTimer = 0;
            } else HellfireTimer -= diff;
        }

        if (CleanupTimer)
        {
            if (CleanupTimer <= diff)
            {
                Cleanup();
                CleanupTimer = 0;
            } else CleanupTimer -= diff;
        }
    }

    void KilledUnit(Unit *who)
    {
        if (Creature *pMalchezaar = m_creature->GetMap()->GetCreature(malchezaar))
            pMalchezaar->AI()->KilledUnit(who);
    }

    void SpellHit(Unit *who, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_INFERNAL_RELAY)
        {
            m_creature->SetDisplayId(m_creature->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            HellfireTimer = 4000;
            CleanupTimer = 170000;
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (done_by->GetGUID() != malchezaar)
            damage = 0;
    }

    void Cleanup();                                         //below ...
};

struct MANGOS_DLL_DECL boss_malchezaarAI : public ScriptedAI
{
    boss_malchezaarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        for(uint8 i =0; i < 2; ++i)
            axes[i] = 0;

        Reset();
    }

    uint32 EnfeebleTimer;
    uint32 EnfeebleResetTimer;
    uint32 ShadowNovaTimer;
    uint32 SWPainTimer;
    uint32 SunderArmorTimer;
    uint32 AmplifyDamageTimer;
    uint32 Cleave_Timer;
    uint32 InfernalTimer;
    uint32 AxesTargetSwitchTimer;
    uint32 InfernalCleanupTimer;

    std::vector<uint64> infernals;
    std::vector<InfernalPoint*> positions;

    uint64 axes[2];
    uint64 enfeeble_targets[5];
    uint64 enfeeble_health[5];

    uint32 phase;

    void Reset()
    {
        AxesCleanup();
        ClearWeapons();
        InfernalCleanup();
        positions.clear();

        for(int i =0; i < 5; ++i)
        {
            enfeeble_targets[i] = 0;
            enfeeble_health[i] = 0;
        }

        for(int i = 0; i < TOTAL_INFERNAL_POINTS; ++i)
            positions.push_back(&InfernalPoints[i]);

        EnfeebleTimer = 30000;
        EnfeebleResetTimer = 38000;
        ShadowNovaTimer = 35500;
        SWPainTimer = 20000;
        AmplifyDamageTimer = 5000;
        Cleave_Timer = 8000;
        InfernalTimer = 45000;
        InfernalCleanupTimer = 47000;
        AxesTargetSwitchTimer = urand(7500, 20000);
        SunderArmorTimer = urand(5000, 10000);
        phase = 1;
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        AxesCleanup();
        ClearWeapons();
        InfernalCleanup();
        positions.clear();

        for(int i = 0; i < TOTAL_INFERNAL_POINTS; ++i)
            positions.push_back(&InfernalPoints[i]);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void InfernalCleanup()
    {
        //Infernal Cleanup
        for(std::vector<uint64>::iterator itr = infernals.begin(); itr!= infernals.end(); ++itr)
        {
            Creature *pInfernal = m_creature->GetMap()->GetCreature(*itr);
            if (pInfernal && pInfernal->isAlive())
            {
                pInfernal->SetVisibility(VISIBILITY_OFF);
                pInfernal->SetDeathState(JUST_DIED);
            }
        }
        infernals.clear();
    }

    void AxesCleanup()
    {
        for(int i=0; i<2;++i)
        {
            Creature *axe = m_creature->GetMap()->GetCreature(axes[i]);
            if (axe && axe->isAlive())
                axe->DealDamage(axe, axe->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            axes[i] = 0;
        }
    }

    void ClearWeapons()
    {
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);

        //damage
        const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, cinfo->mindmg);
        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, cinfo->maxdmg);
        m_creature->UpdateDamagePhysical(BASE_ATTACK);
    }

    void EnfeebleHealthEffect()
    {
        const SpellEntry *info = GetSpellStore()->LookupEntry(SPELL_ENFEEBLE_EFFECT);
        if (!info)
            return;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        std::vector<Unit *> targets;

        if (tList.empty())
            return;

        //begin + 1 , so we don't target the one with the highest threat
        ThreatList::const_iterator itr = tList.begin();
        std::advance(itr, 1);
        for(; itr!= tList.end(); ++itr)                    //store the threat list in a different container
        {
            Unit *target = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                                                            //only on alive players
            if (target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(target);
        }

        //cut down to size if we have more than 5 targets
        while(targets.size() > 5)
            targets.erase(targets.begin()+rand()%targets.size());

        int i = 0;
        for(std::vector<Unit *>::iterator iter = targets.begin(); iter!= targets.end(); ++iter, ++i)
        {
            Unit *target = *iter;
            if (target)
            {
                enfeeble_targets[i] = target->GetGUID();
                enfeeble_health[i] = target->GetHealth();

                target->CastSpell(target, SPELL_ENFEEBLE, true, 0, 0, m_creature->GetGUID());
                target->SetHealth(1);
            }
        }

    }

    void EnfeebleResetHealth()
    {
        for(int i = 0; i < 5; ++i)
        {
            Player* pTarget = m_creature->GetMap()->GetPlayer(enfeeble_targets[i]);

            if (pTarget && pTarget->isAlive())
                pTarget->SetHealth(enfeeble_health[i]);

            enfeeble_targets[i] = 0;
            enfeeble_health[i] = 0;
        }
    }

    void SummonInfernal(const uint32 diff)
    {
        InfernalPoint *point = NULL;
        float posX,posY,posZ;
        if ((m_creature->GetMapId() != 532) || positions.empty())
        {
            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 60, posX, posY, posZ);
        }
        else
        {
            std::vector<InfernalPoint*>::iterator itr = positions.begin()+rand()%positions.size();
            point = *itr;
            positions.erase(itr);

            posX = point->x;
            posY = point->y;
            posZ = INFERNAL_Z;
        }

        Creature *Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, posX, posY, posZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);

        if (Infernal)
        {
            Infernal->SetDisplayId(INFERNAL_MODEL_INVISIBLE);
            Infernal->setFaction(m_creature->getFaction());

            netherspite_infernalAI* pInfernalAI = dynamic_cast<netherspite_infernalAI*>(Infernal->AI());

            if (pInfernalAI)
            {
                if (point)
                    pInfernalAI->point = point;

                pInfernalAI->malchezaar = m_creature->GetGUID();
            }

            infernals.push_back(Infernal->GetGUID());
            DoCastSpellIfCan(Infernal, SPELL_INFERNAL_RELAY);
        }

        DoScriptText(urand(0, 1) ? SAY_SUMMON1 : SAY_SUMMON2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (EnfeebleResetTimer)
        {
            if (EnfeebleResetTimer <= diff)                  //Let's not forget to reset that
            {
                EnfeebleResetHealth();
                EnfeebleResetTimer=0;
            } else EnfeebleResetTimer -= diff;
        }

        if (m_creature->hasUnitState(UNIT_STAT_STUNNED))    //While shifting to phase 2 malchezaar stuns himself
            return;

        if (m_creature->GetUInt64Value(UNIT_FIELD_TARGET)!=m_creature->getVictim()->GetGUID())
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->getVictim()->GetGUID());

        if (phase == 1)
        {
            if (m_creature->GetHealthPercent() < 60.0f)
            {
                m_creature->InterruptNonMeleeSpells(false);

                phase = 2;

                //animation
                DoCastSpellIfCan(m_creature, SPELL_EQUIP_AXES);

                //text
                DoScriptText(SAY_AXE_TOSS1, m_creature);

                //passive thrash aura
                m_creature->CastSpell(m_creature, SPELL_THRASH_AURA, true);

                //models
                SetEquipmentSlots(false, EQUIP_ID_AXE, EQUIP_ID_AXE, EQUIP_NO_CHANGE);

                //damage
                const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, 2*cinfo->mindmg);
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, 2*cinfo->maxdmg);
                m_creature->UpdateDamagePhysical(BASE_ATTACK);

                m_creature->SetBaseWeaponDamage(OFF_ATTACK, MINDAMAGE, cinfo->mindmg);
                m_creature->SetBaseWeaponDamage(OFF_ATTACK, MAXDAMAGE, cinfo->maxdmg);
                //Sigh, updating only works on main attack , do it manually ....
                m_creature->SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, cinfo->mindmg);
                m_creature->SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, cinfo->maxdmg);

                m_creature->SetAttackTime(OFF_ATTACK, (m_creature->GetAttackTime(BASE_ATTACK)*150)/100);
            }
        }
        else if (phase == 2)
        {
            if (m_creature->GetHealthPercent() < 30.0f)
            {
                InfernalTimer = 15000;

                phase = 3;

                ClearWeapons();

                //remove thrash
                m_creature->RemoveAurasDueToSpell(SPELL_THRASH_AURA);

                DoScriptText(SAY_AXE_TOSS2, m_creature);

                Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                for(uint32 i=0; i<2; ++i)
                {
                    Creature *axe = m_creature->SummonCreature(MALCHEZARS_AXE, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
                    if (axe)
                    {
                        axe->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        axe->setFaction(m_creature->getFaction());

                        axes[i] = axe->GetGUID();
                        if (target)
                        {
                            axe->AI()->AttackStart(target);
                            // axe->getThreatManager().tauntApply(target); //Taunt Apply and fade out does not work properly
                                                            // So we'll use a hack to add a lot of threat to our target
                            axe->AddThreat(target, 10000000.0f);
                        }
                    }
                }

                if (ShadowNovaTimer > 35000)
                    ShadowNovaTimer = EnfeebleTimer + 5000;

                return;
            }

            if (SunderArmorTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SUNDER_ARMOR);
                SunderArmorTimer = urand(10000, 18000);

            }else SunderArmorTimer -= diff;

            if (Cleave_Timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                Cleave_Timer = urand(6000, 12000);

            }else Cleave_Timer -= diff;
        }
        else
        {
            if (AxesTargetSwitchTimer < diff)
            {
                AxesTargetSwitchTimer = urand(7500, 20000);

                Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                if (target)
                {
                    for(int i = 0; i < 2; ++i)
                    {
                        Creature *axe = m_creature->GetMap()->GetCreature(axes[i]);
                        if (axe)
                        {
                            float threat = 1000000.0f;
                            if (axe->getVictim() && m_creature->getThreatManager().getThreat(axe->getVictim()))
                            {
                                threat = axe->getThreatManager().getThreat(axe->getVictim());
                                axe->getThreatManager().modifyThreatPercent(axe->getVictim(), -100);
                            }
                            if (target)
                                axe->AddThreat(target, threat);
                            //axe->getThreatManager().tauntFadeOut(axe->getVictim());
                            //axe->getThreatManager().tauntApply(target);
                        }
                    }
                }
            } else AxesTargetSwitchTimer -= diff;

            if (AmplifyDamageTimer < diff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, SPELL_AMPLIFY_DAMAGE);

                AmplifyDamageTimer = urand(20000, 30000);
            }else AmplifyDamageTimer -= diff;
        }

        //Time for global and double timers
        if (InfernalTimer < diff)
        {
            SummonInfernal(diff);
            InfernalTimer =  phase == 3 ? 14500 : 44500;    //15 secs in phase 3, 45 otherwise
        }else InfernalTimer -= diff;

        if (ShadowNovaTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWNOVA);
            ShadowNovaTimer = phase == 3 ? 31000 : -1;
        } else ShadowNovaTimer -= diff;

        if (phase != 2)
        {
            if (SWPainTimer < diff)
            {
                Unit* target = NULL;
                if (phase == 1)
                    target = m_creature->getVictim();       // the tank
                else                                        //anyone but the tank
                    target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);

                if (target)
                    DoCastSpellIfCan(target, SPELL_SW_PAIN);

                SWPainTimer = 20000;
            }else SWPainTimer -= diff;
        }

        if (phase != 3)
        {
            if (EnfeebleTimer < diff)
            {
                EnfeebleHealthEffect();
                EnfeebleTimer = 30000;
                ShadowNovaTimer = 5000;
                EnfeebleResetTimer = 9000;
            }else EnfeebleTimer -= diff;
        }

        if (phase==2)
            DoMeleeAttacksIfReady();
        else
            DoMeleeAttackIfReady();
    }

    void DoMeleeAttacksIfReady()
    {
        // Check if target is valid
        if (!m_creature->getVictim())
            return;

        if (!m_creature->IsNonMeleeSpellCasted(false) && m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            //Check for base attack
            if (m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
            //Check for offhand attack
            if (m_creature->isAttackReady(OFF_ATTACK))
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim(), OFF_ATTACK);
                m_creature->resetAttackTimer(OFF_ATTACK);
            }
        }
    }

    void Cleanup(Creature *infernal, InfernalPoint *point)
    {
        for(std::vector<uint64>::iterator itr = infernals.begin(); itr!= infernals.end(); ++itr)
            if (*itr == infernal->GetGUID())
        {
            infernals.erase(itr);
            break;
        }

        positions.push_back(point);
    }
};

void netherspite_infernalAI::Cleanup()
{
    Creature* pMalchezaar = m_creature->GetMap()->GetCreature(malchezaar);

    if (pMalchezaar && pMalchezaar->isAlive())
    {
        if (boss_malchezaarAI* pMalAI = dynamic_cast<boss_malchezaarAI*>(pMalchezaar->AI()))
            pMalAI->Cleanup(m_creature, point);
    }
}

CreatureAI* GetAI_netherspite_infernal(Creature* pCreature)
{
    return new netherspite_infernalAI(pCreature);
}

CreatureAI* GetAI_boss_malchezaar(Creature* pCreature)
{
    return new boss_malchezaarAI(pCreature);
}

void AddSC_netherspite_infernal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "netherspite_infernal";
    newscript->GetAI = &GetAI_netherspite_infernal;
    newscript->RegisterSelf();
}

void AddSC_boss_malchezaar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_malchezaar";
    newscript->GetAI = &GetAI_boss_malchezaar;
    newscript->RegisterSelf();
}
