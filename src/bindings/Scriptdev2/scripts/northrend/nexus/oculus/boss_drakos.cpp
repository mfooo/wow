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
SDName: oculus
SD%Complete: 90%
SDComment: // dev //FallenAngelX
SDCategory: Drakos the Interrogator
ToDo:: last thing spheres need to cause damge to players when they explode & Thunder Stomp needs core support for ensnare effect ( 35% movement reduction ) and he's finished
& implent achievement 18153 kill eregos within 20 mins of drakos death  as well as finishing touches on texts
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

/*###
#Drakos
####*/

enum
{
	SPELL_MAGIC_PULL                = 50770,
	SPELL_THUNDERING_STOMP          = 50774,
	H_SPELL_THUNDERING_STOMP        = 59370,

	SAY_INTRO                  = -1000006,
	SAY_AGGRO                  = -1578000,
	SAY_SLAY_1                 = -1578001,
	SAY_SLAY_2                 = -1578002,
	SAY_SLAY_3                 = -1578003,
	SAY_STOMP_1                = -1578009,
	SAY_STOMP_2                = -1578010,
	SAY_STOMP_3                = -1578011,
	SAY_DEATH                  = -1578004,
	EMOTE_MAGIC_PULL           = -1000012
};

struct MANGOS_DLL_DECL boss_drakosAI : public ScriptedAI
{
        boss_drakosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    // variables
    // timers
    uint32 ThunderingStomp_Timer;
    uint32 Bomb_Timer;
    uint32 Global_Cooldown_Timer;
    uint32 Teleport_Timer;
    // booleans
    bool MagicPull80;
    bool MagicPull60;
    bool MagicPull40;
    bool MagicPull20;
    bool Intro;

    void Reset() // initialization
    {
        ThunderingStomp_Timer = 20000;
        Bomb_Timer = 3000;
        Teleport_Timer = 8000;
        Global_Cooldown_Timer = 0;
        MagicPull80 = false;
        MagicPull60 = false;
        MagicPull40 = false;
        MagicPull20 = false;
        Intro = false;
        m_creature->SetVisibility(VISIBILITY_ON);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_creature->getVictim() && who->isTargetableForAttack() && (m_creature->IsHostileTo(who)) && who->isInAccessablePlaceFor(m_creature))
        {
            if (!Intro && m_creature->IsWithinDistInMap(who, 100))
            {
               DoScriptText(SAY_INTRO, m_creature);
                                Intro = true;
            }

            if (!m_creature->CanFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(who);
            }
        }
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DRAKOS, IN_PROGRESS);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
		if(m_pInstance)
			m_pInstance->SetData(TYPE_DRAKOS,DONE);
    }

    void KilledUnit(Unit* victim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void TeleportPlayers() // teleport the entire group to the bosses position.
    {
                float x = m_creature->GetPositionX();
                float y = m_creature->GetPositionY();
                float z = m_creature->GetPositionZ();
                int i;

                for (i = 0; i <= 4; i++)
                {
                        Unit* Player = NULL;
                        Player = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, i);
                        if (Player)
                        {
                                Player->NearTeleportTo(x, y, z, 1);
                        }else return;
                }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())  // any valid target?
            return;

                if (Teleport_Timer < diff)
                {
                        TeleportPlayers();
                        Teleport_Timer = 8000;
                }
                else
                {
                        if (Teleport_Timer != 8000)
                        {
                                Teleport_Timer -= diff;
                        }
                }

                if(Bomb_Timer < diff)
                {
						uint32 BombCount = urand(1,m_bIsRegularMode?4:6);
						for(uint32 i = 0; i < BombCount; i++)
							DoSpawnCreature(28166, 0, 0, 0, 0, TEMPSUMMON_DEAD_DESPAWN, 0); // spawn an unstable sphere, improvisation :P
						Bomb_Timer = urand(5000, 6000);
                }else Bomb_Timer -= diff;

                if (Global_Cooldown_Timer < diff)
                {
                        if (!MagicPull80 && m_creature->GetHealthPercent() < 80.0f) // 80% hp
                        {
                                DoScriptText(EMOTE_MAGIC_PULL, m_creature);
                                DoCastSpellIfCan(m_creature, SPELL_MAGIC_PULL);
                                Teleport_Timer = 1999;
                                MagicPull80 = true;
                                Global_Cooldown_Timer = 2001;
                                return;
                        }
                        if (!MagicPull60 && m_creature->GetHealthPercent() < 60.0f) // 60% hp
                        {
                                DoScriptText(EMOTE_MAGIC_PULL, m_creature);
                                DoCastSpellIfCan(m_creature, SPELL_MAGIC_PULL);
                                Teleport_Timer = 1999;
                                MagicPull60 = true;
                                Global_Cooldown_Timer = 2001;
                                return;
                        }
                        if (!MagicPull40 && m_creature->GetHealthPercent() < 40.0f) // 40% hp
                        {
                                DoScriptText(EMOTE_MAGIC_PULL, m_creature);
                                DoCastSpellIfCan(m_creature, SPELL_MAGIC_PULL);
                                Teleport_Timer = 1999;
                                MagicPull40 = true;
                                Global_Cooldown_Timer = 2001;
                                return;
                        }
                        if (!MagicPull20 && m_creature->GetHealthPercent() < 20.0f) // 20% hp
                        {
                                DoScriptText(EMOTE_MAGIC_PULL, m_creature);
                                DoCastSpellIfCan(m_creature, SPELL_MAGIC_PULL);
                                Teleport_Timer = 1999;
                                MagicPull20 = true;
                                Global_Cooldown_Timer = 2001;
                                return;
                        }
                        if (ThunderingStomp_Timer < diff) // Thundering Stomp about every 20 sec
                        {
                                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_THUNDERING_STOMP : H_SPELL_THUNDERING_STOMP);
                                switch(urand(0, 2))
                                {
                                        case 0: DoScriptText(SAY_STOMP_1, m_creature); break;
                                        case 1: DoScriptText(SAY_STOMP_2, m_creature); break;
                                        case 2: DoScriptText(SAY_STOMP_3, m_creature); break;
                                }
                                ThunderingStomp_Timer = 20000;
                                Global_Cooldown_Timer = 2001;
                        }else ThunderingStomp_Timer -= diff;
                }
                else
                {
                        Global_Cooldown_Timer -= diff;
                }
                DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_drakos(Creature* pCreature)
{
    return new boss_drakosAI(pCreature);
}

/*###
#UnStable_Sphere
####*/
enum
{
	BOMB_SPELL_ARCANE_EXPLOSION            = 50757,
	H_BOMB_SPELL_ARCANE_EXPLOSION          = 50757,
	BOMB_SPELL_VISUAL                      = 50756,
	SPELL_UNSTABLE_BOOM                    = 50759,  // might be wrong  need spell that cause players damge from core exploding
	BOMB_SPELL_TIMER                       = 50758
};

#define X               960
#define Y               1050
#define Z               360
#define PI              3.14

struct MANGOS_DLL_DECL mob_unstable_sphereAI : public ScriptedAI
{
    mob_unstable_sphereAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		m_creature->SetDisplayId(16946);
		DetonateTimer = 16000;
		ArcaneExplosion_Timer = 60000;
		explosion = false;
		visibilityOff = false;
		VisibilityOff_Timer = 16500;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 ArcaneExplosion_Timer;
    uint32 VisibilityOff_Timer;
    uint32 AnotherMovePoint;
	uint32 DetonateTimer;
    bool explosion;
    bool visibilityOff;

	void MoveToPointInCircle(uint32 id)
    {
        float x, y, z, r, angle;
        angle = urand(0, 36000) / 100;
        r = urand(20, 320) / 10;
        (angle <= PI) ? y = Y + (sin(angle) * r) : y = Y - (sin(angle) * r);
        (angle >= (PI / 2) && angle <= (PI * 3 / 2)) ? x = X - (cos(angle) * r) : x = X + (cos(angle) * r);
        z = Z;
        m_creature->GetMotionMaster()->MovePoint(id, x, y, z);
    }

    void MovementInform(uint32 type, uint32 id)
    {
		m_creature->GetMotionMaster()->MoveIdle();
		if(!explosion)
			ArcaneExplosion_Timer = 900;
		explosion = true;
    }

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
		m_creature->CastSpell(m_creature,BOMB_SPELL_VISUAL,true);
		m_creature->CastSpell(m_creature,41232,true);
        MoveToPointInCircle(1);
    }

	void AttackStart(Unit*)
	{
		return;
	}

    void UpdateAI(const uint32 diff)
    {
        if (ArcaneExplosion_Timer < diff)
        {
            m_creature->CastSpell(m_creature, BOMB_SPELL_ARCANE_EXPLOSION, true);
			ArcaneExplosion_Timer = 900;
		} else ArcaneExplosion_Timer -= diff;

		if(DetonateTimer <= diff)
		{
			m_creature->CastSpell(m_creature,SPELL_UNSTABLE_BOOM,true);
			DetonateTimer = 99999;
		} else DetonateTimer -= diff;

		if (!visibilityOff)
		{
			if (VisibilityOff_Timer < diff)
			{
				m_creature->SetVisibility(VISIBILITY_OFF);
				m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, 0, false);
				visibilityOff = true;
			}else VisibilityOff_Timer -= diff;
		}
    }
};

CreatureAI* GetAI_mob_unstable_sphere(Creature* pCreature)
{
	return new mob_unstable_sphereAI(pCreature);
}

void AddSC_boss_drakos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_drakos";
    newscript->GetAI = &GetAI_boss_drakos;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_unstable_sphere";
    newscript->GetAI = &GetAI_mob_unstable_sphere;
    newscript->RegisterSelf();
}
