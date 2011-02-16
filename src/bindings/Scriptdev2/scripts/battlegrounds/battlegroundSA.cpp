#include "precompiled.h"
#include "BattleGroundSA.h"
#include "Vehicle.h"
 
#define Spell_Boom        52408

struct MANGOS_DLL_DECL npc_sa_bombAI : public ScriptedAI
{
    npc_sa_bombAI(Creature* pCreature) : ScriptedAI(pCreature) { SetCombatMovement(false); Reset(); }
    uint32 event_bomb;
    float fx, fy, fz;
    void Reset() { m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE); event_bomb = 10000; }
    void Aggro(Unit* who){}
    void JustDied(Unit* Killer){ m_creature->ForcedDespawn(); }
    void KilledUnit(Unit *victim){}
    void UpdateAI(const uint32 diff)
    {
        if (event_bomb < diff)
        {
            m_creature->GetPosition(fx, fy, fz);
            m_creature->CastSpell(m_creature, 34602, true);
            m_creature->CastSpell(m_creature, 71495, true);
            m_creature->CastSpell(fx, fy, fz, Spell_Boom, true, 0, 0, m_creature->GetCharmerGuid());
            m_creature->ForcedDespawn();
        } else event_bomb -= diff;
    }
};
 
CreatureAI* GetAI_npc_sa_bomb(Creature* pCreature)
{
    return new npc_sa_bombAI (pCreature);
}

struct MANGOS_DLL_DECL npc_sa_demolisherAI : public ScriptedAI
{
    npc_sa_demolisherAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    void Reset()
    {
        done = false;
    }

    bool done;
    BattleGround *bg;

    void Aggro(Unit* who){ m_creature->CombatStop(); }

    void StartEvent(Player* pPlayer)
    {
        if (BattleGround *bg = pPlayer->GetBattleGround())
        {
            if (((BattleGroundSA*)bg)->GetDefender() == pPlayer->GetTeam() || bg->GetStatus() == STATUS_WAIT_JOIN)
                return;

            if (VehicleKit *vehicle = m_creature->GetVehicleKit())
                pPlayer->EnterVehicle(vehicle);
        }
    }

    bool mustDespawn(BattleGround *bg)
    {
        if (bg->GetStatus() == STATUS_WAIT_JOIN && ((BattleGroundSA*)bg)->GetDefender() == ALLIANCE)
        {
            float x = m_creature->GetPositionX();
            if (x < 1400.0f)
                return true;
        }
        return false;
    }
 
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->isCharmed())
        {
            if (m_creature->isInCombat())
                m_creature->CombatStop();

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);

            if (!done)  // Get the BattleGround only once
            {
                Map* pMap = m_creature->GetMap();

                if (!pMap || !pMap->IsBattleGround())
                    return;

                Map::PlayerList const &PlayerList = pMap->GetPlayers();
                Map::PlayerList::const_iterator itr = PlayerList.begin();
                Player *player = itr->getSource();
                if (player)
                {
                    bg = player->GetBattleGround();
                    done = true;
                }
            }

            if (bg)
            {
                m_creature->setFaction(bg->GetVehicleFaction(VEHICLE_SA_DEMOLISHER));
                if (mustDespawn(bg))
                    m_creature->ForcedDespawn();
            }
        }
    }
};
 
CreatureAI* GetAI_npc_sa_demolisher(Creature* pCreature)
{
    return new npc_sa_demolisherAI(pCreature);
}
 
bool GossipHello_npc_sa_demolisher(Player* pPlayer, Creature* pCreature)
{
     pPlayer->CLOSE_GOSSIP_MENU();
     ((npc_sa_demolisherAI*)pCreature->AI())->StartEvent(pPlayer);
         return true;
}
 
struct MANGOS_DLL_DECL npc_sa_cannonAI : public ScriptedAI
{
    npc_sa_cannonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    void Reset()
    {
        done = false;
    }

    bool done;
    BattleGround *bg;

    void Aggro(Unit* who){ m_creature->CombatStop(); }

    void StartEvent(Player* pPlayer)
    {
        if (BattleGround *bg = pPlayer->GetBattleGround())
        {
            if (bg->GetDefender() != pPlayer->GetTeam() || bg->GetStatus() == STATUS_WAIT_JOIN)
                return;

            if (VehicleKit *vehicle = m_creature->GetVehicleKit())
                pPlayer->EnterVehicle(vehicle);
        }
    }
 
    void UpdateAI(const uint32 diff)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (!m_creature->isCharmed())
        {
            if (m_creature->isInCombat())
                m_creature->CombatStop();

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);

            if (!done)  // Get the BattleGround only once
            {
                Map* pMap = m_creature->GetMap();

                if (!pMap || !pMap->IsBattleGround())
                    return;

                Map::PlayerList const &PlayerList = pMap->GetPlayers();
                Map::PlayerList::const_iterator itr = PlayerList.begin();
                Player *player = itr->getSource();
                if (player)
                {
                    bg = player->GetBattleGround();
                    done = true;
                }
            }

            if (bg)
                m_creature->setFaction(bg->GetVehicleFaction(VEHICLE_SA_CANNON));
        }
    }
};
 
CreatureAI* GetAI_npc_sa_cannon(Creature* pCreature)
{
    return new npc_sa_cannonAI(pCreature);
}
 
bool GossipHello_npc_sa_cannon(Player* pPlayer, Creature* pCreature)
{
     pPlayer->CLOSE_GOSSIP_MENU();
     ((npc_sa_cannonAI*)pCreature->AI())->StartEvent(pPlayer);
         return true;
}
 
#define GOSSIP_START_EVENT_1        "Start building the Demolisher."
#define GOSSIP_START_EVENT_2        "You have nothing to do now!"
#define GOSSIP_EVENT_STARTED        "Im working on it."

#define NPC_DEMILISHER        28781

#define SA_MESSAGE_0        "Begin the manufracture of Demolisher"
#define SA_MESSAGE_1        "Time Left: 1 minute"
#define SA_MESSAGE_1_1      "Time Left: 40 seconds"
#define SA_MESSAGE_1_2      "Time Left: 35 seconds"
#define SA_MESSAGE_2        "Time Left: 30 seconds"
#define SA_MESSAGE_3        "Time Left: 25 seconds"
#define SA_MESSAGE_4        "Time Left: 20 seconds"
#define SA_MESSAGE_5        "Time Left: 15 seconds"
#define SA_MESSAGE_6        "Time Left: 10 seconds"
#define SA_MESSAGE_7        "Time Left: 5 seconds"
#define SA_MESSAGE_8        "Ready!"

static float SummonLocations[2][4]=
{
     {1371.06f, -317.046f, 35.001f, 1.916f},
     {1353.24f, 223.91f, 35.2535f, 4.366f},
};

bool build;

struct MANGOS_DLL_DECL npc_sa_vendorAI : public ScriptedAI
{
    npc_sa_vendorAI(Creature* pCreature) : ScriptedAI(pCreature){Reset();}
    uint32 build_time;
    uint8 gydId;
    void Reset(){ build=false; }
    void StartEvent(Player* pPlayer, uint8 gyd)
    {
        build_time = 60000;
        gydId = gyd;
        build = true;
        m_creature->MonsterSay(SA_MESSAGE_0,LANG_UNIVERSAL,0);
        m_creature->MonsterSay(SA_MESSAGE_1,LANG_UNIVERSAL,0);
    }

    void UpdateAI(const uint32 diff)
    {
        if (build)
        {
            if (build_time < diff)
            {
                m_creature->SummonCreature(NPC_DEMILISHER,SummonLocations[gydId][0],SummonLocations[gydId][1],SummonLocations[gydId][2],SummonLocations[gydId][3], TEMPSUMMON_DEAD_DESPAWN, 0);
                m_creature->MonsterSay(SA_MESSAGE_8,LANG_UNIVERSAL,0);
                build=false;
            }else build_time -= diff;
 
            switch(build_time/2)
            {
                case 15000: m_creature->MonsterSay(SA_MESSAGE_2,LANG_UNIVERSAL,0); break;
                case 7500: m_creature->MonsterSay(SA_MESSAGE_5,LANG_UNIVERSAL,0); break;
            }
        }
    }
};
 
CreatureAI* GetAI_npc_sa_vendor(Creature* pCreature)
{
    return new npc_sa_vendorAI(pCreature);
}
 
bool GossipHello_npc_sa_vendor(Player* pPlayer, Creature* pCreature)
{
    uint8 gyd = NULL;
    if (pCreature->GetEntry() == 29260)
        gyd = 0;    
    if (pCreature->GetEntry() == 29262)
        gyd = 1;
    if (!build)
    {
        if (pPlayer->GetMapId() == 607)
            if (BattleGround *bg = pPlayer->GetBattleGround())
                if (bg->GetDefender() != pPlayer->GetTeam())
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START_EVENT_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START_EVENT_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }
    else
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START_EVENT_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    
    return true;
}

bool GossipSelect_npc_sa_vendor(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        uint8 gyd = NULL;
        if (pCreature->GetEntry() == 29260)
            gyd = 0;
        if (pCreature->GetEntry() == 29262)
            gyd = 1;
         pPlayer->CLOSE_GOSSIP_MENU();
         ((npc_sa_vendorAI*)pCreature->AI())->StartEvent(pPlayer, gyd);
     }
     if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
         pPlayer->CLOSE_GOSSIP_MENU();
     return true;
}

/*######
## go_sa_def_portal
######*/

static float SpawnLocation[7][3]=
{
    {1468.12f, -225.7f, 30.8969f},
    {1394.07f, 72.3632f, 31.0541f},
    {1216.12f, 47.7665f, 54.2785f},
    {1255.73f, -233.153f, 56.4357f},
    {1065.02f, -89.9522f, 81.0758f},
    {880.162f, -95.979f, 109.835f},
    {880.68f, -120.799f, 109.835f},
};
 
static float TeleLocation[7][3]=
{
    {1451.72f, -224.863f, 41.9564f},
    {1401.6f, 88.6693f, 41.1833f},
    {1210.68f, 60.3558f, 64.7388f},
    {1245.03f, -226.439f, 66.7201f},
    {1062.83f, -87.1955f, 93.8061f},
    {808.447f, -109.192f, 109.835f},
    {808.447f, -109.192f, 109.835f},
};
 
bool GOHello_go_sa_def_portal(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetMapId() == 607)
    {
        if (BattleGround *bg = pPlayer->GetBattleGround())
        {
            if (pPlayer->GetTeam() == bg->GetDefender())
            {
                for (uint32 i=0; i<7; ++i)
                {
                    if ((pGo->GetPositionX() == SpawnLocation[i][0]) && 
                    (pGo->GetPositionY() == SpawnLocation[i][1]) &&
                    (pGo->GetPositionZ() == SpawnLocation[i][2]))
                    {
                        pPlayer->TeleportTo(bg->GetMapId(),TeleLocation[i][0],TeleLocation[i][1],TeleLocation[i][2],0);
                        return true;
                    }
                }
            } else pPlayer->MonsterSay("No puedes usarlo",LANG_UNIVERSAL, pPlayer);
        }
    }
    return false;
}
 
void AddSC_battlegroundSA()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name="npc_sa_bomb";
    pNewScript->GetAI = &GetAI_npc_sa_bomb;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_sa_demolisher";
    pNewScript->GetAI = &GetAI_npc_sa_demolisher;
    pNewScript->pGossipHello = &GossipHello_npc_sa_demolisher;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_sa_cannon";
    pNewScript->GetAI = &GetAI_npc_sa_cannon;
    pNewScript->pGossipHello = &GossipHello_npc_sa_cannon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_sa_vendor";
    pNewScript->GetAI = &GetAI_npc_sa_vendor;
    pNewScript->pGossipHello = &GossipHello_npc_sa_vendor;
    pNewScript->pGossipSelect = &GossipSelect_npc_sa_vendor;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_sa_def_portal";
    pNewScript->pGOUse = &GOHello_go_sa_def_portal;
    pNewScript->RegisterSelf();
}
