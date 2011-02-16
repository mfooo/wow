/*
##############################
 Archbishop Benedictus v 2.0
##############################

by Myav
*/

#include "precompiled.h"
#include "../../base/simple_ai.h"

enum
{
    CREATURE_COOLDOWN        = 5000,
    MANAPOTION               = 43186,
    HEALINGPOTION            = 54572,
    POTIONCD                 = 60000,
    EVOCATION                = 12051,
    EVOCATION_CD             = 480000
};

struct MANGOS_DLL_DECL archbishop_benedictusAI : public ScriptedAI
{
    archbishop_benedictusAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset(); ZoneAttackMsgTimer = 0;}

    uint32 GlobalCooldown;      //This variable acts like the global cooldown that players have (1.5 seconds)
    uint32 Help;
    uint32 ZoneAttackMsgTimer;
    uint32 Potioncd;
    uint32 Evocation_cd;
    uint32 GCD;
    uint32 InnerTimer;
    Unit* enemy;
    Creature* Helper;

void Reset()
{
    GlobalCooldown = 0;
    Help = urand(45000, 75000);
    Potioncd = 0;
    Evocation_cd = 0;
    GCD = 0;

    m_creature->SetMaxHealth(255000);
    m_creature->SetHealth(255000);
    m_creature->SetMaxPower(POWER_MANA,180000);
    m_creature->SetPower(POWER_MANA,180000);
    m_creature->SetArmor(5720);
    m_creature->SetResistance(SPELL_SCHOOL_NORMAL,810);
    m_creature->SetResistance(SPELL_SCHOOL_NATURE,810);
    m_creature->SetResistance(SPELL_SCHOOL_HOLY,810);
    m_creature->SetResistance(SPELL_SCHOOL_FIRE,810);
    m_creature->SetResistance(SPELL_SCHOOL_FROST,810);
    m_creature->SetResistance(SPELL_SCHOOL_SHADOW,810);
    m_creature->SetResistance(SPELL_SCHOOL_ARCANE,810);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
    m_creature->HandleStatModifier(UNIT_MOD_DAMAGE_MAINHAND,BASE_VALUE,+3000,true);
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

    if (InnerTimer > diff)
        InnerTimer -= diff;
    else InnerTimer = 0;

      if(!(Potioncd < diff))
         Potioncd -= diff;
      if(!(Evocation_cd < diff))
         Evocation_cd -= diff;
      if(!(GCD < diff))
         GCD -= diff;

    if(!m_creature->isAlive())
        return;

    if(!m_creature->HasAura(48073,EFFECT_INDEX_0))
    {
     // Divine Spirit 6
     DoCastSpellIfCan(m_creature, 48073);
     return;
    }
    if(!m_creature->HasAura(48168,EFFECT_INDEX_0))
    {
    // Inner Fire 9
     DoCastSpellIfCan(m_creature, 48168);
     return;
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
                  // Mass Dispel
                  info = 32375;
                  break;
               case 1:
                  // Pain Suppression
                    info = 33206;
                    Change = 1;
                    break;
               case 2:
                  // Power Infusion
                    info = 10060;
                    Change = 1;
                    break;
               case 3:
                  // Power Word: Shield
                    info = 41373;
                    Change = 1;
                    break;
               case 4:
                  // Circle of Healing
                    info = 48089;
                    Change = 1;
                    break;
               case 5:
                  // Holy Nova
                    info = 59701;
                    break;
               case 6:
                  // Renew
                    info = 37260;
                    Change = 1;
                    break;
               case 7:
                  // Empowered Smite
                    info = 41471;
                    break;
               case 8:
                  // Heal
                    info = 39013;
                    Change = 1;
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
                GlobalCooldown = CREATURE_COOLDOWN;
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
CreatureAI* GetAI_cathedral_of_light(Creature* pCreature)
{
    SimpleAI* ai = new SimpleAI (pCreature);

    uint32 CreatureID = pCreature->GetEntry();

    ai->m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
    ai->m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);

    switch (CreatureID)
    {
        //Duthorian Rall
    case 6171:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 48952;      //Holy Shield 6
        ai->Spell[0].Cooldown = 30000;
        ai->Spell[0].First_Cast = 500;
        ai->Spell[0].Cast_Target_Type = CAST_SELF;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 10308;      //Hammer of Justice 4
        ai->Spell[1].Cooldown = 45000;
        ai->Spell[1].First_Cast = 10000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 48934;      //Greater Blessing of Might 5
        ai->Spell[2].Cooldown = 120000;
        ai->Spell[2].First_Cast = 5000;
        ai->Spell[2].Cast_Target_Type = CAST_SELF;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 48825;      //Holy Shock 7
        ai->Spell[3].Cooldown = 20000;
        ai->Spell[3].First_Cast = 15000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[4].Enabled = true;
        ai->Spell[4].Spell_Id = 48782;      //Holy Light 13
        ai->Spell[4].Cooldown = 15000;
        ai->Spell[4].First_Cast = 20000;
        ai->Spell[4].Cast_Target_Type = CAST_SELF;

        ai->Spell[5].Enabled = true;
        ai->Spell[5].Spell_Id = 48801;      //Exorcism 9
        ai->Spell[5].Cooldown = 15000;
        ai->Spell[5].First_Cast = 30000;
        ai->Spell[5].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;

        //High Priestess Laurena
    case 376:
        ai->Spell[0].Enabled = true;
        ai->Spell[0].Spell_Id = 48168;      //Inner Fire 9
        ai->Spell[0].Cooldown = 120000;
        ai->Spell[0].First_Cast = 500;
        ai->Spell[0].Cast_Target_Type = CAST_SELF;

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 48078;      //Holy Nova  9
        ai->Spell[1].Cooldown = 30000;
        ai->Spell[1].First_Cast = 10000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 48066;      //Power Word: Shield 14
        ai->Spell[2].Cooldown = 15000;
        ai->Spell[2].First_Cast = 5000;
        ai->Spell[2].Cast_Target_Type = CAST_SELF;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 48123;      //Smite   12
        ai->Spell[3].Cooldown = 20000;
        ai->Spell[3].First_Cast = 15000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[4].Enabled = true;
        ai->Spell[4].Spell_Id = 48089;      //Circle of Healing 7
        ai->Spell[4].Cooldown = 15000;
        ai->Spell[4].First_Cast = 20000;
        ai->Spell[4].Cast_Target_Type = CAST_SELF;

        ai->Spell[5].Enabled = true;
        ai->Spell[5].Spell_Id = 48068;      //Renew   14
        ai->Spell[5].Cooldown = 25000;
        ai->Spell[5].First_Cast = 25000;
        ai->Spell[5].Cast_Target_Type = CAST_SELF;

        ai->Spell[6].Enabled = true;
        ai->Spell[6].Spell_Id = 48063;      //Greater Heal 9
        ai->Spell[6].Cooldown = 15000;
        ai->Spell[6].First_Cast = 30000;
        ai->Spell[6].Cast_Target_Type = CAST_SELF;

        ai->Spell[7].Enabled = true;
        ai->Spell[7].Spell_Id = 48072;      //Prayer of Healing 7
        ai->Spell[7].Cooldown = 25000;
        ai->Spell[7].First_Cast = 35000;
        ai->Spell[7].Cast_Target_Type = CAST_SELF;
        break;
    }

    ai->Reset();

    return ai;
}
CreatureAI* GetAI_archbishop_benedictus(Creature* pCreature)
{
    return new archbishop_benedictusAI (pCreature);
}

void AddSC_archbishop_benedictus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "archbishop_benedictus";
    newscript->GetAI = &GetAI_archbishop_benedictus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "cathedral_of_light";
    newscript->GetAI = &GetAI_cathedral_of_light;
    newscript->RegisterSelf();
}
