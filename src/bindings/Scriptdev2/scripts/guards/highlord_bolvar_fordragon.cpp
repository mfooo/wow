/* 
#################################
 highlord Bolvar Fordragon v 1.0
#################################

by Myav 
*/

#include "precompiled.h"

#define GENERIC_CREATURE_COOLDOWN 5000


#define MANAPOTION 43186
#define HEALINGPOTION 54572
#define POTIONCD 60000
#define EVOCATION 12051
#define EVOCATION_CD 480000

struct MANGOS_DLL_DECL highlord_bolvar_fordragonAI : public ScriptedAI
{
    highlord_bolvar_fordragonAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset(); ZoneAttackMsgTimer = 0;}

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

//    m_creature->SetMaxHealth(380000);
//    m_creature->SetHealth(380000);
    m_creature->SetMaxPower(POWER_MANA,150000);
    m_creature->SetPower(POWER_MANA,150000);
    m_creature->SetArmor(11520);
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

      if(!(Potioncd < diff))
         Potioncd -= diff;
      if(!(Evocation_cd < diff))
         Evocation_cd -= diff;
      if(!(GCD < diff))
         GCD -= diff;

    if(!m_creature->isAlive())
        return;

    if(!m_creature->HasAura(54043,EFFECT_INDEX_0)) // Retribution Aura
    {
     DoCastSpellIfCan(m_creature, 54043);
     return;
    }

    if(!m_creature->HasAura(20165,EFFECT_INDEX_0)) // Seal of Light
    {
     DoCastSpellIfCan(m_creature, 20165);
     return;
    }

    if(!m_creature->HasAura(25898,EFFECT_INDEX_0)) // Greater Blessing of Kings
    {
     DoCastSpellIfCan(m_creature, 25898);
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
                  info = 36946; // Consecration
                  break;
               case 1:
                  info = 38921; // Holy Shock
                  break;
               case 2:
                  info = 48827; // Avenger's Shield
                  break;
               case 3:
                  info = 41367; // Divine Shield
                  Change = 1;
                  break;
               case 4:
                  info = 10308; // Hammer of Justice
                  break;
               case 5:
                  info = 31904; // Holy Shield
                  Change = 1;
                  break;
               case 6:
                  info = 41450; // Blessing of Protection
                  Change = 1;
                  break;
               case 7:
                  info = 31884; // Avenging Wrath
                  Change = 1;
                  break;
               case 8:
                  info = 48932; // Blessing of Might
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
                GlobalCooldown = GENERIC_CREATURE_COOLDOWN;
            }//end 50% change
            else m_creature->AttackerStateUpdate(m_creature->getVictim());

            m_creature->resetAttackTimer();
        }//end attack ready
    }// end attack distance

}
};
CreatureAI* GetAI_highlord_bolvar_fordragon(Creature* pCreature)
{
    return new highlord_bolvar_fordragonAI (pCreature);
}

void AddSC_highlord_bolvar_fordragon()
{   
    Script *newscript;

    newscript = new Script;
    newscript->Name = "highlord_bolvar_fordragon";
    newscript->GetAI = &GetAI_highlord_bolvar_fordragon;
    newscript->RegisterSelf();
}
