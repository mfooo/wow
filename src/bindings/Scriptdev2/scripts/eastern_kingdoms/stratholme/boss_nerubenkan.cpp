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
SDName: Boss_Nerubenkan
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "stratholme.h"

#define SPELL_ENCASINGWEBS          4962
#define SPELL_PIERCEARMOR           6016
#define SPELL_CRYPT_SCARABS         31602
#define SPELL_RAISEUNDEADSCARAB     17235

struct MANGOS_DLL_DECL boss_nerubenkanAI : public ScriptedAI
{
    boss_nerubenkanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 EncasingWebs_Timer;
    uint32 PierceArmor_Timer;
    uint32 CryptScarabs_Timer;
    uint32 RaiseUndeadScarab_Timer;

    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void Reset()
    {
        CryptScarabs_Timer = 3000;
        EncasingWebs_Timer = 7000;
        PierceArmor_Timer = 19000;
        RaiseUndeadScarab_Timer = 3000;
    }

    void RaiseUndeadScarab(Unit* victim)
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
        Summoned = DoSpawnCreature(10876, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
        if (Summoned)
            Summoned->AI()->AttackStart(victim);
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NERUB, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //EncasingWebs
        if (EncasingWebs_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ENCASINGWEBS);
            EncasingWebs_Timer = 30000;
        }else EncasingWebs_Timer -= diff;

        //PierceArmor
        if (PierceArmor_Timer < diff)
        {
            if (rand()%100 < 75)
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_PIERCEARMOR);

            PierceArmor_Timer = 35000;
        }else PierceArmor_Timer -= diff;

        //CryptScarabs
        if (CryptScarabs_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CRYPT_SCARABS);
            CryptScarabs_Timer = 16000;
        }else CryptScarabs_Timer -= diff;

        //RaiseUndeadScarab
        if (RaiseUndeadScarab_Timer < diff)
        {
            RaiseUndeadScarab(m_creature->getVictim());
            RaiseUndeadScarab_Timer = 18000;
        }else RaiseUndeadScarab_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_nerubenkan(Creature* pCreature)
{
    return new boss_nerubenkanAI(pCreature);
}

void AddSC_boss_nerubenkan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_nerubenkan";
    newscript->GetAI = &GetAI_boss_nerubenkan;
    newscript->RegisterSelf();
}
