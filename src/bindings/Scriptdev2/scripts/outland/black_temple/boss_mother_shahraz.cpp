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
SDName: Boss_Mother_Shahraz
SD%Complete: 80
SDComment: Saber Lash missing, Fatal Attraction slightly incorrect; need to damage only if affected players are within range of each other
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

//Speech'n'Sounds
#define SAY_TAUNT1              -1564018
#define SAY_TAUNT2              -1564019
#define SAY_TAUNT3              -1564020
#define SAY_AGGRO               -1564021
#define SAY_SPELL1              -1564022
#define SAY_SPELL2              -1564023
#define SAY_SPELL3              -1564024
#define SAY_SLAY1               -1564025
#define SAY_SLAY2               -1564026
#define SAY_ENRAGE              -1564027
#define SAY_DEATH               -1564028

//Spells
#define SPELL_BEAM_SINISTER     40859
#define SPELL_BEAM_VILE         40860
#define SPELL_BEAM_WICKED       40861
#define SPELL_BEAM_SINFUL       40827
#define SPELL_ATTRACTION        40871
#define SPELL_SILENCING_SHRIEK  40823
#define SPELL_ENRAGE            23537
#define SPELL_SABER_LASH        43267
#define SPELL_SABER_LASH_IMM    43690
#define SPELL_TELEPORT_VISUAL   40869
#define SPELL_BERSERK           45078

uint32 PrismaticAuras[]=
{
    40880,                                                  // Shadow
    40882,                                                  // Fire
    40883,                                                  // Nature
    40891,                                                  // Arcane
    40896,                                                  // Frost
    40897,                                                  // Holy
};

struct Locations
{
    float x,y,z;
};

static Locations TeleportPoint[]=
{
    {959.996f, 212.576f, 193.843f},
    {932.537f, 231.813f, 193.838f},
    {958.675f, 254.767f, 193.822f},
    {946.955f, 201.316f, 192.535f},
    {944.294f, 149.676f, 197.551f},
    {930.548f, 284.888f, 193.367f},
    {965.997f, 278.398f, 195.777f}
};

struct MANGOS_DLL_DECL boss_shahrazAI : public ScriptedAI
{
    boss_shahrazAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 TargetGUID[3];
    uint32 BeamTimer;
    uint32 BeamCount;
    uint32 CurrentBeam;
    uint32 PrismaticShieldTimer;
    uint32 FatalAttractionTimer;
    uint32 FatalAttractionExplodeTimer;
    uint32 ShriekTimer;
    uint32 RandomYellTimer;
    uint32 EnrageTimer;
    uint32 ExplosionCount;

    bool Enraged;

    void Reset()
    {
        for(uint8 i = 0; i<3; ++i)
            TargetGUID[i] = 0;

        BeamTimer = 60000;                                  // Timers may be incorrect
        BeamCount = 0;
        CurrentBeam = 0;                                    // 0 - Sinister, 1 - Vile, 2 - Wicked, 3 - Sinful
        PrismaticShieldTimer = 0;
        FatalAttractionTimer = 60000;
        FatalAttractionExplodeTimer = 70000;
        ShriekTimer = 30000;
        RandomYellTimer = urand(70000, 110000);
        EnrageTimer = 600000;
        ExplosionCount = 0;

        Enraged = false;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SHAHRAZ, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SHAHRAZ, NOT_STARTED);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SHAHRAZ, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void TeleportPlayers()
    {
        uint32 random = urand(0, 6);
        float X = TeleportPoint[random].x;
        float Y = TeleportPoint[random].y;
        float Z = TeleportPoint[random].z;

        for(uint8 i = 0; i < 3; ++i)
        {
            Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
            if (pUnit && pUnit->isAlive() && (pUnit->GetTypeId() == TYPEID_PLAYER))
            {
                TargetGUID[i] = pUnit->GetGUID();
                pUnit->CastSpell(pUnit, SPELL_TELEPORT_VISUAL, true);
                DoTeleportPlayer(pUnit, X, Y, Z, pUnit->GetOrientation());
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() < 10.0f && !Enraged)
        {
            Enraged = true;
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE, CAST_TRIGGERED);
            DoScriptText(SAY_ENRAGE, m_creature);
        }

        //Randomly cast one beam.
        if (BeamTimer < diff)
        {
            Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (!target || !target->isAlive())
                return;

            BeamTimer = 9000;

            switch(CurrentBeam)
            {
                case 0:
                    DoCastSpellIfCan(target, SPELL_BEAM_SINISTER);
                    break;
                case 1:
                    DoCastSpellIfCan(target, SPELL_BEAM_VILE);
                    break;
                case 2:
                    DoCastSpellIfCan(target, SPELL_BEAM_WICKED);
                    break;
                case 3:
                    DoCastSpellIfCan(target, SPELL_BEAM_SINFUL);
                    break;
            }
            ++BeamCount;
            uint32 Beam = CurrentBeam;

            if (BeamCount > 3)
                while(CurrentBeam == Beam)
                    CurrentBeam = urand(0, 2);

        }else BeamTimer -= diff;

        // Random Prismatic Shield every 15 seconds.
        if (PrismaticShieldTimer < diff)
        {
            uint32 random = urand(0, 5);
            if (PrismaticAuras[random])
                DoCastSpellIfCan(m_creature, PrismaticAuras[random]);
            PrismaticShieldTimer = 15000;
        }else PrismaticShieldTimer -= diff;

        // Select 3 random targets (can select same target more than once), teleport to a random location then make them cast explosions until they get away from each other.
        if (FatalAttractionTimer < diff)
        {
            ExplosionCount = 0;

            TeleportPlayers();

            DoScriptText(urand(0, 1) ? SAY_SPELL2 : SAY_SPELL3, m_creature);

            FatalAttractionExplodeTimer = 2000;
            FatalAttractionTimer = urand(40000, 70000);
        }else FatalAttractionTimer -= diff;

        if (FatalAttractionExplodeTimer < diff)
        {
            // Just make them explode three times... they're supposed to keep exploding while they are in range, but it'll take too much code. I'll try to think of an efficient way for it later.
            if (ExplosionCount < 3)
            {
                for(uint8 i = 0; i < 3; ++i)
                {
                    if (TargetGUID[i])
                    {
                        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(TargetGUID[i]))
                            pPlayer->CastSpell(pPlayer, SPELL_ATTRACTION, true);

                        TargetGUID[i] = 0;
                    }
                }

                ++ExplosionCount;
                FatalAttractionExplodeTimer = 1000;
            }
            else
            {
                FatalAttractionExplodeTimer = FatalAttractionTimer + 2000;
                ExplosionCount = 0;
            }
        }else FatalAttractionExplodeTimer -= diff;

        if (ShriekTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SILENCING_SHRIEK);
            ShriekTimer = 30000;
        }else ShriekTimer -= diff;

        //Enrage
        if (!m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            if (EnrageTimer < diff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                DoScriptText(SAY_ENRAGE, m_creature);
            }else EnrageTimer -= diff;
        }

        //Random taunts
        if (RandomYellTimer < diff)
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_TAUNT1, m_creature); break;
                case 1: DoScriptText(SAY_TAUNT2, m_creature); break;
                case 2: DoScriptText(SAY_TAUNT3, m_creature); break;
            }

            RandomYellTimer = urand(60000, 150000);
        }else RandomYellTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_shahraz(Creature* pCreature)
{
    return new boss_shahrazAI(pCreature);
}

void AddSC_boss_mother_shahraz()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_mother_shahraz";
    newscript->GetAI = &GetAI_boss_shahraz;
    newscript->RegisterSelf();
}
