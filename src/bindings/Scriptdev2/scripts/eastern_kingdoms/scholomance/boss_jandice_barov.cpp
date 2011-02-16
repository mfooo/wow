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
SDName: Boss_jandicebarov
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"

#define SPELL_CURSEOFBLOOD          24673
//#define SPELL_ILLUSION              17773

//Spells of Illusion of Jandice Barov
#define SPELL_CLEAVE                15584

struct MANGOS_DLL_DECL boss_jandicebarovAI : public ScriptedAI
{
    boss_jandicebarovAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 CurseOfBlood_Timer;
    uint32 Illusion_Timer;
    //uint32 Illusioncounter;
    uint32 Invisible_Timer;
    bool Invisible;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void Reset()
    {
        CurseOfBlood_Timer = 15000;
        Illusion_Timer = 30000;
        Invisible_Timer = 3000;                             //Too much too low?
        Invisible = false;
    }

    void SummonIllusions(Unit* victim)
    {
        Rand = rand()%10;
        switch(urand(0, 1))
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%10;
        switch(urand(0, 1))
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(11439, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
        if (Summoned)
            Summoned->AI()->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Invisible && Invisible_Timer < diff)
        {
            //Become visible again
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetDisplayId(11073);                //Jandice Model
            Invisible = false;
        } else if (Invisible)
        {
            Invisible_Timer -= diff;
            //Do nothing while invisible
            return;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //CurseOfBlood_Timer
        if (CurseOfBlood_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CURSEOFBLOOD);
            CurseOfBlood_Timer = 30000;
        }else CurseOfBlood_Timer -= diff;

        //Illusion_Timer
        if (!Invisible && Illusion_Timer < diff)
        {
            //Inturrupt any spell casting
            m_creature->InterruptNonMeleeSpells(false);
            m_creature->setFaction(35);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetDisplayId(11686);                // Invisible Model
            m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-99);

            //Summon 10 Illusions attacking random gamers
            Unit* target = NULL;
            for(int i = 0; i < 10; ++i)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                SummonIllusions(target);
            }
            Invisible = true;
            Invisible_Timer = 3000;

            //25 seconds until we should cast this agian
            Illusion_Timer = 25000;
        }else Illusion_Timer -= diff;

        //            //Illusion_Timer
        //            if (Illusion_Timer < diff)
        //            {
        //                  //Cast
        //                DoCastSpellIfCan(m_creature->getVictim(),SPELL_ILLUSION);
        //                  //3 Illusion will be summoned
        //                  if (Illusioncounter < 3)
        //                  {
        //                    Illusion_Timer = 500;
        //                    ++Illusioncounter;
        //                  }
        //                  else {
        //                      //15 seconds until we should cast this again
        //                      Illusion_Timer = 15000;
        //                      Illusioncounter=0;
        //                  }
        //            }else Illusion_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

// Illusion of Jandice Barov Script

struct MANGOS_DLL_DECL mob_illusionofjandicebarovAI : public ScriptedAI
{
    mob_illusionofjandicebarovAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Cleave_Timer;

    void Reset()
    {
        Cleave_Timer = urand(2000, 8000);
        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = urand(5000, 8000);
        }else Cleave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jandicebarov(Creature* pCreature)
{
    return new boss_jandicebarovAI(pCreature);
}

CreatureAI* GetAI_mob_illusionofjandicebarov(Creature* pCreature)
{
    return new mob_illusionofjandicebarovAI(pCreature);
}

void AddSC_boss_jandicebarov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_jandice_barov";
    newscript->GetAI = &GetAI_boss_jandicebarov;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_illusionofjandicebarov";
    newscript->GetAI = &GetAI_mob_illusionofjandicebarov;
    newscript->RegisterSelf();
}
