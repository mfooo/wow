/* 
##############################
 High Sorcerer Andromath v 1.0
##############################

by Myav 
*/

#include "precompiled.h"

#define GENERIC_CREATURE_COOLDOWN 5000

#define MANAPOTION 43186
#define HEALINGPOTION 54572
#define POTIONCD 60000
#define EVOCATION 12051
#define EVOCATION_CD 480000

struct MANGOS_DLL_DECL high_sorcerer_andromathAI : public ScriptedAI
{
    high_sorcerer_andromathAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset(); ZoneAttackMsgTimer = 0;}

    uint32 GlobalCooldown;      //This variable acts like the global cooldown that players have (1.5 seconds)
    uint32 Help;
    uint32 ZoneAttackMsgTimer;
    uint32 Potioncd;
    uint32 Evocation_cd;
    uint32 GCD;
    Unit* enemy;
    Creature* Helper;

void Reset()
{
    GlobalCooldown = 0;
    Help = urand(45000, 75000);
    Potioncd = 0;
    Evocation_cd = 0;
    GCD = 0;

//    m_creature->SetMaxHealth(120000);
//   m_creature->SetHealth(120000);
    m_creature->SetMaxPower(POWER_MANA,90000);
    m_creature->SetPower(POWER_MANA,90000);
    m_creature->SetArmor(5720);
    m_creature->SetResistance(SPELL_SCHOOL_NORMAL,810);
    m_creature->SetResistance(SPELL_SCHOOL_NATURE,810);
    m_creature->SetResistance(SPELL_SCHOOL_HOLY,810);
    m_creature->SetResistance(SPELL_SCHOOL_FIRE,810);
    m_creature->SetResistance(SPELL_SCHOOL_FROST,810);
    m_creature->SetResistance(SPELL_SCHOOL_SHADOW,810);
    m_creature->SetResistance(SPELL_SCHOOL_ARCANE,810);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_KNOCKOUT, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
    m_creature->HandleStatModifier(UNIT_MOD_DAMAGE_MAINHAND,BASE_VALUE,+1000,true);
}

void Aggro(Unit *who)
{
    //Send Zone Under Attack message to the LocalDefense and WorldDefense Channels
    if (who->GetTypeId() == TYPEID_PLAYER && !ZoneAttackMsgTimer)
    {
        m_creature->SendZoneUnderAttackMessage((Player*)who);
        ZoneAttackMsgTimer = 30000;
    }
}

void UpdateAI(const uint32 diff)
{
      if (m_creature->GetHealthPercent() < 30.0f && Potioncd<diff && GCD<diff)
      {
         DoCastSpellIfCan (m_creature,HEALINGPOTION);
         Potioncd = POTIONCD;
         GCD = 1500;
      }
      if (m_creature->GetPower(POWER_MANA)<m_creature->GetMaxPower(POWER_MANA)*0.3 && Evocation_cd<diff && GCD<diff)
      {
         DoCastSpellIfCan (m_creature,EVOCATION);
         Evocation_cd = EVOCATION_CD;
         GCD = 1500;
      }
      if (m_creature->GetPower(POWER_MANA)<m_creature->GetMaxPower(POWER_MANA)*0.3 && Potioncd<diff && GCD<diff)
      {
         DoCastSpellIfCan (m_creature,MANAPOTION);
         Potioncd = POTIONCD;
         GCD = 1500;
      }

    //Always decrease our global cooldown first
    if (GlobalCooldown > diff)
        GlobalCooldown -= diff;
    else GlobalCooldown = 0;

    //Always decrease ZoneAttackMsgTimer
    if (ZoneAttackMsgTimer > diff)
        ZoneAttackMsgTimer -= diff;
    else ZoneAttackMsgTimer = 0;

      if(!(Potioncd < diff))
         Potioncd -= diff;
      if(!(Evocation_cd < diff))
         Evocation_cd -= diff;
      if(!(GCD < diff))
         GCD -= diff;

    if(!m_creature->isAlive())
        return;
    // Ice Armor 6
    if(!m_creature->HasAura(43008,EFFECT_INDEX_0))
    {
     DoCastSpellIfCan(m_creature, 43008);
    }
    //Return since we have no target
    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        return;

    Unit *target = m_creature->getVictim();

       if(target == m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO,0) && !target->isAlive() && !m_creature->IsNonMeleeSpellCasted(false))
       {
          Unit* target2 = NULL;
          target2 = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO,1);
          m_creature->getThreatManager().modifyThreatPercent(target,-100);
          if(target2 && target2->isAlive())
              {
                 m_creature->AddThreat(target2,1);
                 m_creature->Attack(target2,true);
                 AttackStart(target2);
                 return;
              }
          else
          {
              Reset();
              return;
          }
       }

    if(m_creature->IsHostileTo(target))
           enemy = target;

    if(enemy)
        if(!enemy->isAlive())
            enemy = NULL;


    //Always decrease Help when in combat
    if (Help > diff)
        Help -= diff;
    else Help = 0;

    //If we are within range melee the target
    if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
    {
        //Make sure our attack is ready and we arn't currently casting
        if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
        {
           int Change = 0;
           int info = 0;

                 if(enemy)
                  {
                   switch(urand(0, 8))
                   {
               case 0:
                  // Ice Barrier 8
                  info = 43039;
                  Change = 1;
                  break;
               case 1:
                   // Blizzard
                    info = 41382;
                    break;
               case 2:
                   // Knockdown
                    info = 5164;
                    break;
               case 3:
                   // Charged Arcane Explosion
                    info = 37106;
                    break;
               case 4:
                   // Cone of Cold
                    info = 29717;
                    break;
               case 5:
                   // Frost Nova
                    info = 31250;
                    break;
               case 6:
                   // Arcane Blast
                    info = 24857;
                    break;
               case 7:
                   // Frostbolt
                    info = 41486;
                    break;
               case 8:
                   // Arcane Missiles
                    info = 39414;
                    break;
               }//switch
            }

            //65% chance to replace our white hit with a spell
            if (info && urand(0, 1) == 0 && !GlobalCooldown)
            {
                //Cast the spell   
                if (Change == 1)DoCastSpellIfCan(m_creature, info);
                else DoCastSpellIfCan(enemy, info);

                //Set our global cooldown
                GlobalCooldown = GENERIC_CREATURE_COOLDOWN;
            }//end 50% change
            else m_creature->AttackerStateUpdate(m_creature->getVictim());

            m_creature->resetAttackTimer();
        }//end attack ready
    }// end attack distance
/*    else 
    { 
     if(!m_creature->IsNonMeleeSpellCasted(false))
     {
         if ((*m_creature).GetMotionMaster()->top()->GetMovementGeneratorType()!=TARGETED_MOTION_TYPE)
         {
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->MoveChase(m_creature->getVictim());
         }
     }
    }//end else */
}
};
CreatureAI* GetAI_high_sorcerer_andromath(Creature* pCreature)
{
    return new high_sorcerer_andromathAI (pCreature);
}

void AddSC_high_sorcerer_andromath()
{   
    Script *newscript;

    newscript = new Script;
    newscript->Name = "high_sorcerer_andromath";
    newscript->GetAI = &GetAI_high_sorcerer_andromath;
    newscript->RegisterSelf();
}
