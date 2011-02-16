/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/* ScriptData
SDName: Instance_Sunwell_Plateau
SD%Complete: 70%
SDComment:
SDCategory: Sunwell_Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

/* Sunwell Plateau:
0 - Kalecgos and Sathrovarr
1 - Brutallus
2 - Felmyst
3 - Eredar Twins (Alythess and Sacrolash)
4 - M'uru
5 - Kil'Jaeden
*/

#define DRAGON_REALM_Z		(float)53.079

struct MANGOS_DLL_DECL instance_sunwell_plateau : public ScriptedInstance
{
    instance_sunwell_plateau(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    // Creatures
    uint64 m_uiKalecgos_DragonGUID;
    uint64 m_uiKalecgos_HumanGUID;
    uint64 m_uiSathrovarrGUID;
    uint64 m_uiBrutallusGUID;
    uint64 m_uiFelmystGUID;
    uint64 m_uiAlythessGUID;
    uint64 m_uiSacrolashGUID;
    uint64 m_uiMuruGUID;
    uint64 m_uiKilJaedenGUID;
    uint64 m_uiKilJaedenControllerGUID;
    uint64 m_uiAnveenaGUID;
    uint64 m_uiKalecgosGUID;
	uint64 m_uiMuruPortalTargetGUID;
	uint64 m_uiMadrigosaGUID;

    // GameObjects
    uint64 m_uiForceFieldGUID;                                      // Kalecgos Encounter
    uint64 m_uiBossCollision1GUID;
    uint64 m_uiBossCollision2GUID;
    uint64 m_uiIceBarrierGUID;                                      // Brutallus Encounter
    uint64 m_uiDoorFireBarrierGUID;
    uint64 m_uiDoorTheFirstGateGUID;                                // Felmyst Encounter
    uint64 m_uiDoorTheSecondGateGUID;                               // Alythess Encounter
    uint64 m_uiDoorRaid_Gate_07GUID;                                // Sacrolash Encounter
    uint64 m_uiDoorRaid_Gate_08GUID;                                // Muru Encounter
    uint64 m_uiDoorTheThirdGateGUID;                                // Entropius Encounter

    // Misc
    uint32 m_uiSpectralRealmTimer;
    std::list<uint64> SpectralRealmList;

	uint8 m_uiPortalTargetCount;
	uint64 m_uiShadowPortalGUID[10];

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        // Creatures
        m_uiKalecgos_DragonGUID         = 0;
        m_uiKalecgos_HumanGUID          = 0;
        m_uiSathrovarrGUID              = 0;
        m_uiBrutallusGUID               = 0;
        m_uiFelmystGUID                 = 0;
        m_uiAlythessGUID                = 0;
        m_uiSacrolashGUID               = 0;
        m_uiMuruGUID                    = 0;
        m_uiKilJaedenGUID               = 0;
        m_uiKilJaedenControllerGUID     = 0;
        m_uiAnveenaGUID                 = 0;
        m_uiKalecgosGUID                = 0;
		m_uiMuruPortalTargetGUID		= 0;
		m_uiMadrigosaGUID				= 0;

        // GameObjects
        m_uiForceFieldGUID              = 0;
        m_uiBossCollision1GUID          = 0;
        m_uiBossCollision2GUID          = 0;
        m_uiIceBarrierGUID              = 0;
        m_uiDoorFireBarrierGUID         = 0;
        m_uiDoorTheFirstGateGUID        = 0;
        m_uiDoorTheSecondGateGUID       = 0;
        m_uiDoorRaid_Gate_07GUID        = 0;
        m_uiDoorRaid_Gate_08GUID        = 0;
        m_uiDoorTheThirdGateGUID        = 0;

        // Misc
        m_uiSpectralRealmTimer = 5000;
		
		m_uiPortalTargetCount = 0;

		for(int i=0;i<10;i++)
			m_uiShadowPortalGUID[i]=0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 24850: m_uiKalecgos_DragonGUID     = pCreature->GetGUID(); break;
            case 24891: m_uiKalecgos_HumanGUID      = pCreature->GetGUID(); break;
            case 24892: m_uiSathrovarrGUID          = pCreature->GetGUID(); break;
            case 24882: m_uiBrutallusGUID           = pCreature->GetGUID(); break;
            case 25038: m_uiFelmystGUID             = pCreature->GetGUID(); break;
            case 25166: m_uiAlythessGUID            = pCreature->GetGUID(); break;
            case 25165: m_uiSacrolashGUID           = pCreature->GetGUID(); break;
            case 25741: m_uiMuruGUID                = pCreature->GetGUID(); break;
            case 25315: m_uiKilJaedenGUID           = pCreature->GetGUID(); break;
            case 25608: m_uiKilJaedenControllerGUID = pCreature->GetGUID(); break;
            case 26046: m_uiAnveenaGUID             = pCreature->GetGUID(); break;
            case 25319: m_uiKalecgosGUID            = pCreature->GetGUID(); break;
			case 25770: m_uiShadowPortalGUID[m_uiPortalTargetCount++] = pCreature->GetGUID(); break;
			case 24895: m_uiMadrigosaGUID			= pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case 188421:
                m_uiForceFieldGUID = pGo->GetGUID();
                break;
            case 188523:
                m_uiBossCollision1GUID = pGo->GetGUID();
                break;
            case 188524:
                m_uiBossCollision2GUID = pGo->GetGUID();
                break;
            case 188119:
                m_uiIceBarrierGUID = pGo->GetGUID();
                break;
            case 188075:
                m_uiDoorFireBarrierGUID = pGo->GetGUID();
                if (m_auiEncounter[0] == DONE && m_auiEncounter[1] == DONE && m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case 187766:
                m_uiDoorTheFirstGateGUID = pGo->GetGUID();
                break;
            case 187764:
                m_uiDoorTheSecondGateGUID = pGo->GetGUID();
                if (m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case 187990:
                m_uiDoorRaid_Gate_07GUID = pGo->GetGUID();
                if (m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case 188118:
                m_uiDoorRaid_Gate_08GUID = pGo->GetGUID();
                if (m_auiEncounter[4] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case 187765:
                m_uiDoorTheThirdGateGUID = pGo->GetGUID();
                if (m_auiEncounter[4] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_KALECGOS:     return m_auiEncounter[0];
            case TYPE_BRUTALLUS:    return m_auiEncounter[1];
            case TYPE_FELMYST:      return m_auiEncounter[2];
            case TYPE_EREDAR_TWINS: return m_auiEncounter[3];
            case TYPE_MURU:         return m_auiEncounter[4];
            case TYPE_KILJAEDEN:    return m_auiEncounter[5];
        }

        return 0;
    }

    uint64 GetData64(uint32 id)
    {
        switch(id)
        {
            case DATA_KALECGOS_DRAGON:      return m_uiKalecgos_DragonGUID;
            case DATA_KALECGOS_HUMAN:       return m_uiKalecgos_HumanGUID;
            case DATA_SATHROVARR:           return m_uiSathrovarrGUID;
            case DATA_BRUTALLUS:            return m_uiBrutallusGUID;
            case DATA_FELMYST:              return m_uiFelmystGUID;
            case DATA_ALYTHESS:             return m_uiAlythessGUID;
            case DATA_SACROLASH:            return m_uiSacrolashGUID;
            case DATA_MURU:                 return m_uiMuruGUID;
            case DATA_KILJAEDEN:            return m_uiKilJaedenGUID;
            case DATA_KILJAEDEN_CONTROLLER: return m_uiKilJaedenControllerGUID;
            case DATA_ANVEENA:              return m_uiAnveenaGUID;
            case DATA_KALECGOS:             return m_uiKalecgosGUID;
            case DATA_GO_FORCEFIELD:        return m_uiForceFieldGUID;
			case DATA_SHADOW_PORTAL:		return m_uiShadowPortalGUID[rand()%10];
			case DATA_MADRIGOSA:			return m_uiMadrigosaGUID;
        }
        return 0;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_KALECGOS:
				if (uiData == IN_PROGRESS)		
                    SpectralRealmList.clear();

                DoUseDoorOrButton(m_uiForceFieldGUID);
                DoUseDoorOrButton(m_uiBossCollision1GUID);
                DoUseDoorOrButton(m_uiBossCollision2GUID);

                m_auiEncounter[0] = uiData;
                break;
            case TYPE_BRUTALLUS:
                if (uiData == SPECIAL)
                    DoUseDoorOrButton(m_uiIceBarrierGUID,MINUTE);

                m_auiEncounter[1] = uiData;
                break;
            case TYPE_FELMYST:
                m_auiEncounter[2] = uiData;
                if (uiData == DONE)
                    DoUseDoorOrButton(m_uiDoorFireBarrierGUID);
                break;
            case TYPE_EREDAR_TWINS:
                m_auiEncounter[3] = uiData;
                if (uiData == DONE)
                    //instance only open up to felmyst
                    //DoUseDoorOrButton(m_uiDoorTheSecondGateGUID);
                break;
            case TYPE_MURU:
                m_auiEncounter[4] = uiData;
				if (uiData == IN_PROGRESS)
					DoUseDoorOrButton(m_uiDoorRaid_Gate_07GUID);
				else if (uiData == DONE)
				{
					DoUseDoorOrButton(m_uiDoorRaid_Gate_07GUID);
					DoUseDoorOrButton(m_uiDoorRaid_Gate_08GUID);
				}
                break;
            case TYPE_KILJAEDEN: m_auiEncounter[5] = uiData; break;
            case DATA_SET_SPECTRAL_CHECK:  m_uiSpectralRealmTimer = uiData; break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    void SetData64(uint32 uiData, uint64 uiGuid)
    {
        if (uiData == DATA_PLAYER_SPECTRAL_REALM)
            SpectralRealmList.push_back(uiGuid);
    }

    void EjectPlayer(Player* pPlayer)
    {
        debug_log("SD2: Ejecting Player %s from Spectral Realm", pPlayer->GetName());

        // Put player back in Kalecgos(Dragon)'s threat list
        if (Creature* pKalecgos = instance->GetCreature(m_uiKalecgos_DragonGUID))
        {
            if (pKalecgos->isAlive())
            {
                debug_log("SD2: Adding %s in Kalecgos' threatlist", pPlayer->GetName());
                pKalecgos->AddThreat(pPlayer);
            }
        }

				        // Remove player from Sathrovarr's threat list
        if (Creature* pSath = instance->GetCreature(m_uiSathrovarrGUID))
        {
			if (pSath->isAlive()&&pSath->getVictim()==pPlayer)
            {  
                pSath->RemoveUnitFromHostileRefManager(pPlayer);
                debug_log("SD2: Deleting %s from Sathrovarr's threatlist", pPlayer->GetName()); 
            }
        }


        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_NORMAL_REALM, true); // useless(?)
		pPlayer->TeleportTo(pPlayer->GetMapId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), DRAGON_REALM_Z, pPlayer->GetOrientation()); //teleport back to normal realm
        pPlayer->CastSpell(pPlayer, SPELL_SPECTRAL_EXHAUSTION, true);
    }

    void EjectPlayers()
    {
        if (SpectralRealmList.empty())
            return;

        Map::PlayerList const& players = instance->GetPlayers();

        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* plr = itr->getSource();
			// only choose players without spectral exhaustion debuff that are in demon realm
			if (plr && !plr->HasAura(SPELL_SPECTRAL_EXHAUSTION) && plr->GetPositionZ() < DEMON_REALM_Z+3)
            {
                SpectralRealmList.remove(plr->GetGUID());
                EjectPlayer(plr);
            }
        }

        //SpectralRealmList.clear();
    }

    void Update(uint32 uiDiff)
    {
        // Only check for Spectral Realm if Kalecgos Encounter is running
		//if (m_auiEncounter[0] == IN_PROGRESS)
        {
            if (m_uiSpectralRealmTimer <= uiDiff)
            {
                EjectPlayers();							
                m_uiSpectralRealmTimer = 1000;
            }
            else
                m_uiSpectralRealmTimer -= uiDiff;
        }
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        std::istringstream loadStream(in);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >>
            m_auiEncounter[3] >> m_auiEncounter[4] >> m_auiEncounter[5];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS||SPECIAL)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_sunwell_plateau(Map* pMap)
{
    return new instance_sunwell_plateau(pMap);
}

struct MANGOS_DLL_DECL mob_sunblade_scoutAI : public ScriptedAI
{
    mob_sunblade_scoutAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    int j;
    Creature *nearby[3];
    bool gatherOthersWhenAggro;

    void Reset() { }

    void AddBuddyToList(Creature *c)
    {
        if (c==m_creature)
            return;
        for (int i=0; i<3; ++i)
        {
            if (nearby[i] == c)
                return;
            if (!nearby[i])
            {
                nearby[i] = c;
                return;
            }
        }
    }

    void AddDeceiverNear(Unit *nears)
    {
        std::list<Creature*> assistList;
        switch (j)
        {
            case 0:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25507,40.0f);
                break;
            case 1:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25363,40.0f);
                break;
            case 2:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25369,40.0f);
                break;
            case 3:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25371,40.0f);
                break;
            case 4:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25370,40.0f);
                break;
            case 5:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25367,40.0f);
                break;
            case 6:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25368,40.0f);
                break;
            case 7:
                GetCreatureListWithEntryInGrid(assistList,m_creature,26101,40.0f);
                break;
            case 8:
                GetCreatureListWithEntryInGrid(assistList,m_creature,25867,40.0f);
                break;
            default:
                break;
        }

        if (assistList.empty())
            return;

        for(std::list<Creature*>::iterator iter = assistList.begin(); iter != assistList.end(); ++iter)
           AddBuddyToList((*iter));
    }

    void GiveBuddyMyList(Creature *c)
    {
        mob_sunblade_scoutAI *cai = (mob_sunblade_scoutAI *)(c->AI());
        for (int i=0; i<3; ++i)
            if (nearby[i] && nearby[i]!=c)
                cai->AddBuddyToList(nearby[i]);
        cai->AddBuddyToList(m_creature);
    }

    void CallBuddiesToAttack(Unit *who)
    {
        for (int i=0; i<3; ++i)
        {
            Creature *c = nearby[i];
            if (c)
            {
                if (c->GetPositionZ() - 10 < m_creature->GetPositionZ())
                {
                    if (!c->isInCombat())
                    {
                        c->SetNoCallAssistance(true);
                        if (c->AI())
                        {
                            c->AI()->AttackStart(who);
                            c->GetMotionMaster()->MoveChase(m_creature->getVictim(), 0, 0);
                        }
                            
                    }
                }
            }
        }
	} 

    void Aggro(Unit* who)
    {
        m_creature->MonsterYell("Kommt mir zu Hilfe!", LANG_UNIVERSAL); 
        
        for(j=0; j < 10; ++j)
        {
            nearby[0] = nearby[1] = nearby[2] = NULL;
	        AddDeceiverNear(m_creature);
            for (int bli = 0; bli < 3; ++bli)
            {
                if (!nearby[bli])
                    break;
                AddDeceiverNear(nearby[bli]);
                ((mob_sunblade_scoutAI *)nearby[bli]->AI())->gatherOthersWhenAggro = false;
	        }
	        for (int i=0; i<3; ++i)
                if (nearby[i])
                    GiveBuddyMyList(nearby[i]);

            CallBuddiesToAttack(who);
        } 
    }
};

CreatureAI* GetAI_mob_sunblade_scout(Creature* pCreature)
{
    return new mob_sunblade_scoutAI(pCreature);
}

void AddSC_instance_sunwell_plateau()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_sunwell_plateau";
    newscript->GetInstanceData = &GetInstanceData_instance_sunwell_plateau;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_sunblade_scout";
    newscript->GetAI = &GetAI_mob_sunblade_scout;
    newscript->RegisterSelf();
}
