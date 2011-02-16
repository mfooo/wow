/* ScriptData
SDName: Toravon the Ice Watcher
SDAuthor: Lutik
SD%Complete: 0%
SDComment:
SDCategory: Vault of Archavon
EndScriptData */

#include "precompiled.h"
#include "vault_of_archavon.h"

enum
{
    SP_WHITEOUT         = 72034,
    H_SP_WHITEOUT       = 72096,
    SP_FREEZING_GROUND  = 72090,
    H_SP_FREEZING_GROUND = 72104,
    SP_FROZEN_MALLET    = 71993,
    FROZEN_ORB_AURA     = 72081,

    CR_FROZEN_ORB       = 38456
};


struct MANGOS_DLL_DECL boss_toravonAI : public ScriptedAI
{
    boss_toravonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        orbsNum = m_bIsRegularMode ? 1 : 3;
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *pInstance;

    int orbsNum;
    uint32 WhiteoutTimer;
    uint32 OrbsTimer;
    uint32 FreezeTimer;

    void Reset()
    {
        WhiteoutTimer = 40000;
        OrbsTimer = 15000;
        FreezeTimer = 20000 + rand()%5000;

        if(pInstance)
            pInstance->SetData(TYPE_TORAVON, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature, SP_FROZEN_MALLET);

        if(pInstance)
            pInstance->SetData(TYPE_TORAVON, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
            pInstance->SetData(TYPE_TORAVON, DONE);
    }

    void JustSummoned(Creature *orb)
    {
        orb->CastSpell(orb, FROZEN_ORB_AURA, false);
        orb->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(WhiteoutTimer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SP_WHITEOUT : H_SP_WHITEOUT);
            WhiteoutTimer = 40000;
        }
        else
            WhiteoutTimer -= diff;

        if(OrbsTimer < diff)
        {
            for(int i=0; i<orbsNum; ++i)
            {
                if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(CR_FROZEN_ORB, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
            }
            OrbsTimer = 40000;
        }
        else
            OrbsTimer -= diff;

        if(FreezeTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SP_FREEZING_GROUND);
            FreezeTimer = 20000 + rand()%5000;
        }
        else
            FreezeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_toravon(Creature *pCreature)
{
    return new boss_toravonAI (pCreature);
};

void AddSC_boss_toravon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_toravon";
    newscript->GetAI = &GetAI_boss_toravon;
    newscript->RegisterSelf();
};
