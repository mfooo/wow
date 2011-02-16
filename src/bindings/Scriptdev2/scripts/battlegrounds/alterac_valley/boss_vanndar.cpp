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
SDName: Boss_Vanndar
SD%Complete: 
SDComment: Some spells listed on wowwiki but doesn't exist on wowhead
EndScriptData */

#include "precompiled.h"

#define YELL_AGGRO                  -2100008

#define YELL_EVADE                  -2100009
#define YELL_RESPAWN1		    -2100010
#define YELL_RESPAWN2               -2100011

#define YELL_RANDOM1		    -2100012
#define YELL_RANDOM2		    -2100013
#define YELL_RANDOM3		    -2100014
#define YELL_RANDOM4		    -2100015
#define YELL_RANDOM5		    -2100016
#define YELL_RANDOM6		    -2100017
#define YELL_RANDOM7		    -2100018


#define SPELL_AVATAR                19135
#define SPELL_THUNDERCLAP	    15588
#define SPELL_STORMBOLT             20685 // not sure


struct MANGOS_DLL_DECL boss_vanndarAI : public ScriptedAI
{
    boss_vanndarAI(Creature *c) : ScriptedAI(c) {Reset();}


    uint32 AvatarTimer;
    uint32 ThunderclapTimer;
    uint32 StormboltTimer;
    uint32 ResetTimer;
    uint32 YellTimer;


    void Reset()
    {
        AvatarTimer			    = 3000;
        ThunderclapTimer		= 4000;
        StormboltTimer			= 6000;
        ResetTimer			    = 5000;
        YellTimer		        = (20+rand()%10)*1000; //20 to 30 seconds
    }

    void Aggro(Unit *who)
    {
        DoScriptText(YELL_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        switch(rand()%1)
        {
        case 0: DoScriptText(YELL_RESPAWN1, m_creature); break;
        case 1: DoScriptText(YELL_RESPAWN2, m_creature); break;
        }
    }

    void KilledUnit(Unit* victim){}

    void JustDied(Unit* Killer){}

    void UpdateAI(const uint32 diff)
    {

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (AvatarTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_AVATAR);
            AvatarTimer =  (15+rand()%5)*1000;
        }else AvatarTimer -= diff;

        if (ThunderclapTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_THUNDERCLAP);
            ThunderclapTimer = (5+rand()%10)*1000;
        }else ThunderclapTimer -= diff;

        if (StormboltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_STORMBOLT);
            StormboltTimer = (10+rand()%15)*1000;
        }else StormboltTimer -= diff;

        if (YellTimer < diff) {
            switch(rand()%6)
            {
            case 0: DoScriptText(YELL_RANDOM1, m_creature); break;
            case 1: DoScriptText(YELL_RANDOM2, m_creature); break;
            case 2: DoScriptText(YELL_RANDOM3, m_creature); break;
            case 3: DoScriptText(YELL_RANDOM4, m_creature); break;
            case 4: DoScriptText(YELL_RANDOM5, m_creature); break;
            case 5: DoScriptText(YELL_RANDOM6, m_creature); break;
            case 6: DoScriptText(YELL_RANDOM7, m_creature); break;
            }
            YellTimer = (20+rand()%10)*1000; //20 to 30 seconds
        } else YellTimer -= diff;

        // check if creature is not outside of building
        if(ResetTimer < diff)
        {
            float x, y, z;
            m_creature->GetPosition(x, y, z);
            if(x < 678)
            {
                DoScriptText(YELL_EVADE, m_creature);
                EnterEvadeMode();
            }
            ResetTimer = 5000;
        }else ResetTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vanndar(Creature *_Creature)
{
    return new boss_vanndarAI (_Creature);
}

void AddSC_boss_vanndar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_vanndar";
    newscript->GetAI = &GetAI_boss_vanndar;
    newscript->RegisterSelf();
}
