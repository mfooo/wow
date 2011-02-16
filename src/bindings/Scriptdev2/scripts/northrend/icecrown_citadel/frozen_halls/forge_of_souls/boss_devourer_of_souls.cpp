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
SDName: boss_devourer_of_souls
SD%Complete: 100%
SDComment: MaxXx2021, modified by /dev/rsa
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "def_forge.h"

enum
{
   /*Speach*/
   SAY_DEVOURER_AGGRO_MALE_01      = -1632010,
   SAY_DEVOURER_SLAY_01_MALE_01    = -1632012,
   SAY_DEVOURER_SLAY_02_MALE_01    = -1632015,
   SAY_DEVOURER_DEATH_MALE_01      = -1632018,
   SAY_DEVOURER_SUMMON_MALE_01     = -1632023,
   SAY_DEVOURER_DARK_MALE_01       = -1632027,
   SAY_DEVOURER_MIRRORED_SOUL      = -1632021, 
   SAY_DEVOURER_UNLEASHED_SOULS    = -1632022,
   SAY_DEVOURER_WELL_OF_SOULS      = -1632026,

   SAY_JAINA_FS09_EXTRO            = -1632029,
   SAY_SYLVANA_FS07_EXTRO          = -1632030,

   /*Spell And Visual Effects*/
   SPELL_PHANTOM_BLAST           = 68982,
   SPELL_MIRRORED_SOUL           = 69051,
   SPELL_WELL_OF_SOULS           = 68820,
   SPELL_UNLEASHED_SOULS         = 68939,
   SPELL_WAILING_SOULS           = 68912,  //68873
   SPELL_WELL_OF_SOULS_VIS       = 68854,
   SPELL_WELL_OF_SOUL_DM         = 68863,

   /*Units*/
   NPC_WELL_OF_SOUL              = 36536,
   NPC_UNLEASHED_SOUL            = 36595,

   /*Others*/
   MODEL_FAT                     = 30149,
   MODEL_WOMAN                   = 30150,

   /*Music*/
   Battle01                      = 6077,
   Battle02                      = 6078,
   Battle03                      = 6079,

   MAX_POINTS                    = 22,
};

static Locations SpawnLoc[]=
{
{5618.139f, 2451.873f, 705.854f}, //0 - spawn
{5590.47f, 2427.79f, 705.935f},
{5593.59f, 2428.34f, 705.935f},
{5600.81f, 2429.31f, 705.935f},
{5600.81f, 2421.12f, 705.935f},
{5601.43f, 2426.53f, 705.935f},
{5601.55f, 2418.36f, 705.935f},
{5598.0f, 2429.14f, 705.935f},
{5594.04f, 2424.87f, 705.935f},
{5597.89f, 2421.54f, 705.935f},
{5598.57f, 2434.62f, 705.935f},
{5585.46f, 2417.99f, 705.935f},
{5605.81f, 2428.42f, 705.935f},
{5591.61f, 2412.66f, 705.935f},
{5593.9f, 2410.64f, 705.935f},
{5586.76f, 2416.73f, 705.935f},
{5592.23f, 2419.14f, 705.935f},
{5594.61f, 2416.87f, 705.935f},
{5589.77f, 2421.03f, 705.935f},
{5602.58f, 2435.95f, 705.935f},
{5606.13f, 2433.16f, 705.935f},
{5606.12f, 2436.6f, 705.935f}, //21
};



struct MANGOS_DLL_DECL boss_devourer_of_soulsAI : public ScriptedAI
{
    boss_devourer_of_soulsAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 BattleMusicTimer;
    uint32 PhantomBlastTimer;
    uint32 SummonTimer;
    uint32 WellOfSoulTimer;
    uint32 MirroredTimer;
    uint32 SoulBeamTimer;
    uint32 Step;
    uint32 StepTimer;
    bool Summon;

    void Reset()
    {
      if(!m_pInstance) return;
      m_pInstance->SetData(TYPE_DEVOURER, NOT_STARTED);
      DespawnAllSummons();
      Summon = false;
      Step = 0;
      StepTimer = 100;
      PhantomBlastTimer = 5000;
      WellOfSoulTimer = 12000;
      SummonTimer = 20000;
      MirroredTimer = 28000;
      SoulBeamTimer = 33000;
    }

    void Aggro(Unit *who)
    {
       if(!m_pInstance) return;
       m_pInstance->SetData(TYPE_DEVOURER, IN_PROGRESS);
       m_creature->PlayDirectSound(Battle01);
       BattleMusicTimer = 48000;
       DoScriptText(SAY_DEVOURER_AGGRO_MALE_01, m_creature);
    }

    void DespawnAllSummons()
    {
        std::list<Creature*> m_pSouls;
        GetCreatureListWithEntryInGrid(m_pSouls, m_creature, NPC_UNLEASHED_SOUL, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pSouls.empty())
            for(std::list<Creature*>::iterator itr = m_pSouls.begin(); itr != m_pSouls.end(); ++itr)
            {
                (*itr)->ForcedDespawn();
            }

        std::list<Creature*> m_pWells;
        GetCreatureListWithEntryInGrid(m_pWells, m_creature, NPC_WELL_OF_SOUL, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pWells.empty())
            for(std::list<Creature*>::iterator iter = m_pWells.begin(); iter != m_pWells.end(); ++iter)
            {
                (*iter)->ForcedDespawn();
            }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {

       Map *map = m_creature->GetMap();
       Map::PlayerList const &PlayerList = map->GetPlayers();

       if (PlayerList.isEmpty())
          return;

       for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
               if (i->getSource()->isAlive() && i->getSource()->HasAura(SPELL_MIRRORED_SOUL))
                   m_creature->DealDamage(i->getSource(), uiDamage/2,NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
      }

    void SpawnOutro(uint32 guid)
    {
       if (Creature *pSummon = m_creature->SummonCreature(guid, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z,0, TEMPSUMMON_TIMED_DESPAWN, 300000))
          {
              uint8 i = urand(1,MAX_POINTS);

              pSummon->GetMotionMaster()->MovePoint(i, SpawnLoc[i].x, SpawnLoc[i].y, SpawnLoc[i].z);

              if (pSummon->GetEntry() == npc_jaina_extro)
                  DoScriptText(SAY_JAINA_FS09_EXTRO, pSummon);
              else if (pSummon->GetEntry() == npc_sylvana_extro)
                  DoScriptText(SAY_SYLVANA_FS07_EXTRO, pSummon);
          }
    }

    void JustDied(Unit* pKiller)
    {
       if(!m_pInstance) return;
       m_pInstance->SetData(TYPE_DEVOURER, DONE);
       DoScriptText(SAY_DEVOURER_DEATH_MALE_01, m_creature);
       DespawnAllSummons();

       Player* player = (Player*)pKiller;

       if(player->GetTeam() == ALLIANCE)
       {
            m_pInstance->SetData64(DATA_LIDER,0);
            SpawnOutro(npc_jaina_extro);
            SpawnOutro(npc_jaina_credit);
            SpawnOutro(npc_mage);
            SpawnOutro(npc_mage_woman);
            SpawnOutro(npc_cc_a_01);
            SpawnOutro(npc_cc_a_02);
            SpawnOutro(npc_cc_a_03);

       } else
       {
            m_pInstance->SetData64(DATA_LIDER,1);
            SpawnOutro(npc_sylvana_extro);
            SpawnOutro(npc_sylvana_credit);
            SpawnOutro(npc_mage);
            SpawnOutro(npc_mage_woman);
            SpawnOutro(npc_cc_h_01);
            SpawnOutro(npc_cc_h_02);
            SpawnOutro(npc_cc_h_03);
        };
    }

    void KilledUnit(Unit* victim)
    {
        switch (urand(0,1))
        {
            case 0: DoScriptText(SAY_DEVOURER_SLAY_01_MALE_01, m_creature); break;
            case 1: DoScriptText(SAY_DEVOURER_SLAY_02_MALE_01, m_creature); break;
        }
    }

   void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

      if(Summon != true)
      {
        if (PhantomBlastTimer < diff)
        {
                if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(Target, SPELL_PHANTOM_BLAST);
                PhantomBlastTimer = 8000;
        }
        else
            PhantomBlastTimer -= diff;

        if (WellOfSoulTimer < diff)
        {
                if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(Target, SPELL_WELL_OF_SOULS);
                    DoScriptText(SAY_DEVOURER_WELL_OF_SOULS, m_creature);
                WellOfSoulTimer = urand(12000,24000);
        }
        else
            WellOfSoulTimer -= diff;

        if (SummonTimer < diff)
        {
                m_creature->InterruptNonMeleeSpells(false);
                switch (urand(0,1))
                {
                    case 0: DoScriptText(SAY_DEVOURER_SUMMON_MALE_01, m_creature); break;
                    case 1: DoScriptText(SAY_DEVOURER_UNLEASHED_SOULS, m_creature); break;
                }
                DoCast(m_creature, SPELL_UNLEASHED_SOULS);
                SummonTimer = 50000;
                Summon = true;
        }
        else
            SummonTimer -= diff;

        if (MirroredTimer < diff)
        {
                if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoCast(Target, SPELL_MIRRORED_SOUL);
                    DoScriptText(SAY_DEVOURER_MIRRORED_SOUL, m_creature);
                    MirroredTimer = 25000;
                }
        }
        else
            MirroredTimer -= diff;

        if (SoulBeamTimer < diff)
        {
                DoScriptText(SAY_DEVOURER_DARK_MALE_01, m_creature);
                DoCast(m_creature->getVictim(), SPELL_WAILING_SOULS);
                SoulBeamTimer = (urand(35000, 45000));
        }
        else
            SoulBeamTimer -= diff;

      }

      if(Summon == true)
      {
        if (StepTimer < diff)
        {
           switch(Step)
           {
             case 0:
               StepTimer = 900;
               ++Step;
               break;
             case 1:
               m_creature->SetDisplayId(MODEL_FAT); //this is huck, because this spell (SPELL_UNLEASHED_SOULS) morphed boss into PIG :D
               StepTimer = 2000;
               ++Step;
               break;
             case 2:
               Summon = false;
               Step = 0;
               StepTimer = 100;
               break;
           }
        } else StepTimer -= diff;

      }

         DoMeleeAttackIfReady();

        if (BattleMusicTimer < diff && m_creature->isAlive())
        {
            m_creature->PlayDirectSound(Battle01);
            BattleMusicTimer = 49000;
        }
        else
            BattleMusicTimer -= diff;

      return;
    }
};

struct MANGOS_DLL_DECL npc_well_of_soulAI : public ScriptedAI
{
    npc_well_of_soulAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

ScriptedInstance* m_pInstance;

uint32 DamageTimer;
uint32 DeathTimer;

    void Reset()
    {
      m_creature->SetLevel(80);
      m_creature->setFaction(14);
      m_creature->SetDisplayId(11686);
      m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
      DeathTimer = 60000;
      DoCast(m_creature, SPELL_WELL_OF_SOULS_VIS);
      DamageTimer = 1000;
    }

    void AttackStart(Unit* who)
    {
        return;
    }

   void UpdateAI(const uint32 diff)
    {
      if(!m_pInstance) return;

      if(m_pInstance && m_pInstance->GetData(TYPE_DEVOURER) != IN_PROGRESS)
          m_creature->ForcedDespawn();

      if (DeathTimer < diff)
        {
                m_creature->ForcedDespawn();
        } else DeathTimer -= diff;

      if (DamageTimer < diff)
        {
                DoCast(m_creature, SPELL_WELL_OF_SOUL_DM);
                DamageTimer = 1000;
        } else DamageTimer -= diff;

    return;
    }
};

struct MANGOS_DLL_DECL npc_unleashed_soulAI : public ScriptedAI
{
    npc_unleashed_soulAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
   }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        if (m_pInstance)
        {
            if (Creature* pDevourer = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_DEVOURER)))
                if (pDevourer && pDevourer->isAlive())
                    AttackStart(pDevourer->getVictim());
        }

    }

   void UpdateAI(const uint32 diff)
    {
     if(!m_pInstance) return;

     if(m_pInstance && m_pInstance->GetData(TYPE_DEVOURER) != IN_PROGRESS)
          m_creature->ForcedDespawn();

     if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
          return;

     DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_devourer_of_souls(Creature* pCreature)
{
    return new boss_devourer_of_soulsAI(pCreature);
}

CreatureAI* GetAI_npc_well_of_soul(Creature* pCreature)
{
    return new npc_well_of_soulAI(pCreature);
}

CreatureAI* GetAI_npc_unleashed_soul(Creature* pCreature)
{
    return new npc_unleashed_soulAI(pCreature);
}

void AddSC_boss_devourer_of_souls()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_devourer_of_souls";
    newscript->GetAI = &GetAI_boss_devourer_of_souls;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_well_of_soul";
    newscript->GetAI = &GetAI_npc_well_of_soul;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_unleashed_soul";
    newscript->GetAI = &GetAI_npc_unleashed_soul;
    newscript->RegisterSelf();
}
