/* 
#####################
 City Officers v 4
#####################

by Myav
*/

#include "precompiled.h"
#include "../../base/guard_ai.h"

#define GENERIC_CREATURE_COOLDOWN 5000

#define HEALINGPOTION 54572
#define POTIONCD 60000

struct MANGOS_DLL_DECL city_officerAI : public ScriptedAI
{
    city_officerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset(); ZoneAttackMsgTimer = 0; _Aggro = 0;}

    uint32 GlobalCooldown;      //This variable acts like the global cooldown that players have (1.5 seconds)
    uint32 Help;
    uint32 OffHand;
    uint32 ZoneAttackMsgTimer;
    uint32 Potioncd;
    bool yell;
    int _Aggro;
    float temp1;
    float temp2;
    Unit* enemy;
    Creature* Helper;

void Reset()
{
    GlobalCooldown = 0;
    Help = 5000;
    OffHand = 0;
    Potioncd = 0;
    yell = false;

    m_creature->setPowerType(POWER_RAGE); 
    m_creature->SetMaxPower(POWER_RAGE,500);
    m_creature->SetPower(POWER_RAGE,0); 
    if(m_creature->GetEntry() != 2041 && m_creature->GetEntry() != 18103)   
        m_creature->SetMaxHealth(33600);
    else 
    {
    if(m_creature->GetEntry() == 18103)
         m_creature->SetMaxHealth(47400);
     else m_creature->SetMaxHealth(67000);
    }
    if(m_creature->GetEntry() != 2041 && m_creature->GetEntry() != 18103)
        m_creature->SetHealth(33600);
    else
    {
    if(m_creature->GetEntry() == 18103)
         m_creature->SetHealth(47400);
     else m_creature->SetHealth(67000);
    }
    if(m_creature->GetEntry() == 21970 || m_creature->GetEntry() == 21971)
        m_creature->SetMaxHealth(45725);
    if(m_creature->GetEntry() == 21970 || m_creature->GetEntry() == 21971)
        m_creature->SetHealth(45725);
    m_creature->SetArmor(9600);
    m_creature->SetResistance(SPELL_SCHOOL_NORMAL,540);
    m_creature->SetResistance(SPELL_SCHOOL_HOLY,540);
    m_creature->SetResistance(SPELL_SCHOOL_FIRE,540);
    m_creature->SetResistance(SPELL_SCHOOL_NATURE,540);
    m_creature->SetResistance(SPELL_SCHOOL_FROST,540);
    m_creature->SetResistance(SPELL_SCHOOL_SHADOW,540);
    m_creature->SetResistance(SPELL_SCHOOL_ARCANE,540);
    m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
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

void DamageDeal(Unit *done_to, uint32 &damage)
{
   if(m_creature->GetEntry() == 2041 || m_creature->GetEntry() == 18103)
       return;
   int temp1 = m_creature->GetPower(POWER_RAGE);
   int temp2 = 0;
   temp2 = temp1;
   switch(urand(0, 3))
   {
      case 0: temp1 = temp2 + 40;break;
      case 1: temp1 = temp2 + 50;break;
      case 2: temp1 = temp2 + 60;break;
      case 3: temp1 = temp2 + 70;break;
   }
   if(temp1 > 500)
      m_creature->SetPower(POWER_RAGE,500);   
   else
      m_creature->SetPower(POWER_RAGE,temp1);
}

void DamageTaken(Unit *done_by, uint32 &damage)
{
    if(damage > m_creature->GetHealth())
        return;

   if(m_creature->GetEntry() == 2041 || m_creature->GetEntry() == 18103)
       return;

   int temp1 = m_creature->GetPower(POWER_RAGE);
   int temp2 = 0;
   temp2 = temp1;
   switch(urand(0, 3))
   {
      case 0:temp1 = temp2 + 20;break;
      case 1:temp1 = temp2 + 30;break;
      case 2:temp1 = temp2 + 40;break;
      case 3:temp1 = temp2 + 50;break;
   }
   if(temp1 > 500)
      m_creature->SetPower(POWER_RAGE,500);   
   else
      m_creature->SetPower(POWER_RAGE,temp1); 
}


void guards()
{
       int entry = 0;
       float x;
       float y;
       float z;
       float X;
       float Y;
       float Z;
       m_creature->GetPosition(x,y,z);
       m_creature->getVictim()->GetPosition(X,Y,Z);
       if(x > X)
           X = x + urand(20, 30);
       else
           X = x - urand(20, 30);
       if(y > Y)
           Y = y + urand(20, 30);
       else
           Y = y - urand(20, 30);
       m_creature->GetRespawnCoord(x,y,z); 
       if(m_creature->getFaction() == 85)
           entry = 3296;
       if(m_creature->getFaction() == 79)
           entry = 4262;
       if(m_creature->getFaction() == 11)
           entry = 68;
       if(m_creature->getFaction() == 71)
           entry = 5624;
       if(m_creature->getFaction() == 57)
           entry = 5595;
       if(m_creature->getFaction() == 105)
           entry = 3084;    
       if(m_creature->getFaction() == 1638)
           entry = 16733;
       if(m_creature->getFaction() == 1604)
           entry = 16222;
       Helper = DoSpawnCreature(entry, (float) (X - x), (float) (Y - y), (float) (Z -z), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
    /*   Helper = DoSpawnCreature(entry, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000); */
       if(Helper && enemy)
          ((CreatureAI*)Helper->AI())->AttackStart(enemy); 
}

void UpdateAI(const uint32 diff)
{

      if (m_creature->GetHealthPercent() < 30.0f && Potioncd<diff)
      {
         DoCastSpellIfCan (m_creature,HEALINGPOTION);
         Potioncd = POTIONCD;
      }

    //Always decrease our global cooldown first
    if (GlobalCooldown > diff)
        GlobalCooldown -= diff;
    else GlobalCooldown = 0;

    //Always decrease our OffHand cooldown first
    if (OffHand > diff)
        OffHand -= diff;
    else OffHand = 0;

    //Always decrease ZoneAttackMsgTimer
    if (ZoneAttackMsgTimer > diff)
        ZoneAttackMsgTimer -= diff;
    else ZoneAttackMsgTimer = 0;

      if(!(Potioncd < diff))
         Potioncd -= diff;

    if(!m_creature->isAlive())
        return;

    //Buff timer (only buff when we are alive and not in combat
    if (!m_creature->isInCombat())
    {
       int temp1 = m_creature->GetPower(POWER_RAGE);
       int temp2 = temp1;
       temp1 = temp2 - 3;
       if(temp1 > 0)
          m_creature->SetPower(POWER_RAGE,temp1);
       else
          m_creature->SetPower(POWER_RAGE,0);
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

    if(!Help && m_creature->GetEntry() != 2041 && m_creature->GetEntry() != 18103)
    {
       if(m_creature->getFaction() == 85)
        m_creature->MonsterSay("Orgrimmar is under Invasion! Guards!! Help me!",LANG_ORCISH);
       if(m_creature->getFaction() == 79)
        m_creature->MonsterSay("Darnassus is under Invasion! Guards!! Help me!",LANG_DARNASSIAN);
       if(m_creature->getFaction() == 11)
        m_creature->MonsterSay("Stormwind is under Invasion! Guards!! Help me!",LANG_COMMON);
       if(m_creature->getFaction() == 71)
        m_creature->MonsterSay("Undercity is under Invasion! Guards!! Help me!",LANG_GUTTERSPEAK);
       if(m_creature->getFaction() == 57)
        m_creature->MonsterSay("Ironforge is under Invasion! Guards!! Help me!",LANG_DWARVISH);
       if(m_creature->getFaction() == 105)
        m_creature->MonsterSay("Thunder Bluff is under Invasion! Guards!! Help me!",LANG_TAURAHE);
       if(m_creature->getFaction() == 1638)
        m_creature->MonsterSay("The Exodar is under Invasion! Guards!! Help me!",LANG_DRAENEI);
       if(m_creature->getFaction() == 1604)
        m_creature->MonsterSay("Silvermoon is under Invasion! Guards!! Help me!",LANG_THALASSIAN);
       m_creature->HandleEmoteCommand(EMOTE_ONESHOT_BATTLEROAR);
       for(int i = 0; i < 3;i++)
       {
            guards();
       }
       Help = urand(60000, 105000);
    }

    //If we are within range melee the target
    if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
    {
            if(m_creature->getVictim() && m_creature->SelectHostileTarget() && OffHand > 0 && OffHand < 500)
            {
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ATTACKOFF);
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
                OffHand = 0;
            }
        //Make sure our attack is ready and we arn't currently casting
        if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
        {

           if(_Aggro != 2 && m_creature->GetEntry() != 2041 && m_creature->GetEntry() != 18103)
           {
              _Aggro = 2;
              DoCastSpellIfCan(m_creature,2457);
              return;
           }

           int Change = 0;
           int info = 0;

       /*     if(!m_creature->HasAura(47440,0) && m_creature->GetPower(POWER_RAGE) > 50)
            {
                info = 47440;
                Change = 1;
            }
            else
            { */
                if(!m_creature->HasAura(47436,EFFECT_INDEX_0) && m_creature->GetPower(POWER_RAGE) > 50)
                {
                    info = 47436;
                    Change = 1;
                }
                else
                 if(enemy && m_creature->GetPower(POWER_RAGE) > 100)
                  {
                   switch(urand(0, 8))
                   {
               case 0:
                  // Heroic Strike
                  info = 47450;
                  break;
               case 1:
                  // Bloodthirst
                    info = 30335;
                    break;
               case 2:
                  // Piercing Howl
                    info = 12323;
                    break;
               case 3:
                  // Intimidating Shout
                    info = 5246;
                    break;
               case 4:
                  // Concussion Blow
                    info = 12809;
                    break;
               case 5:
                  // Cleave
                    info = 40504;
                    break;
               case 6:
                  // Disarm
                    info = 31955;
                    break;
               case 7:
                  // Hamstring
                    info = 30989;
                    break;
               case 8:
                  // Whirlwind
                    info = 13736;
                    break;
               }//switch
            }//if rage
        /*    } */

            if(m_creature->GetEntry() == 2041 || m_creature->GetEntry() == 18103)
                info = 0;
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
            OffHand = 1000;
        }//end attack ready
    }// end attack distance
}
};
CreatureAI* GetAI_city_officer(Creature* pCreature)
{
    return new city_officerAI (pCreature);
}
CreatureAI* GetAI_city_guard(Creature* pCreature)
{
    return new guardAI (pCreature);
}
void AddSC_city_officer()
{   
    Script *newscript;

    newscript = new Script;
    newscript->Name = "city_officer";
    newscript->GetAI = &GetAI_city_officer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "city_guard";
    newscript->GetAI = &GetAI_city_guard;
    newscript->RegisterSelf();
}
