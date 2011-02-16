#include "precompiled.h"
#include "Item.h"
#include "Spell.h"

struct MANGOS_DLL_DECL npc_barman_scAI: public ScriptedAI
{
	npc_barman_scAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 actiontimer;
	uint16 actionid;
	Player* invoker;
	uint32 phase;
	uint32 bezpecnostni;

	void Reset()
	{
		actiontimer = 0;
		actionid = 0;
		invoker = NULL;
		phase = 0;
		bezpecnostni = 0;
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
	}

	uint32 GetPhase()
	{
		return actiontimer?99:phase;
	}

	bool IsThatPlayer(uint64 guid)
	{
		if(!invoker) return true;

		outstring_log("Comparing source guid %u with invoker guid %u",guid,invoker->GetGUID());

		return (guid == invoker->GetGUIDLow());
	}

	void SetAction(uint32 timer, uint32 action, Player* pPlayer)
	{
		if(actiontimer || !pPlayer) return;

		actiontimer = timer;
		actionid = action;
		invoker = pPlayer;
		bezpecnostni = 50000;
	}

	void UpdateAI(const uint32 diff)
	{
		if(actiontimer)
		{
			if(actiontimer <= diff)
			{
				actiontimer = 0;
				if(invoker && invoker->isAlive() && invoker->IsWithinDistInMap(m_creature,50.0f))
				{
					switch(actionid)
					{
					case 0:
						phase = 0;
						break;
					case 1:
						invoker->MonsterSay("Zdravim, barmane. Tak jak jdou ksefty?",LANG_UNIVERSAL,0);
						SetAction(3500,2,invoker);
						break;
					case 2:
						m_creature->MonsterSay("Zdravicko, posledni dobou celkem mizerne.",LANG_UNIVERSAL,0);
						++phase;
						break;
					case 3:
						invoker->MonsterSay("Co to spolecne zapit, abychom na nase trapeni aspon trochu zapomneli?",LANG_UNIVERSAL,0);
						SetAction(4000,4,invoker);
						break;
					case 4:
						m_creature->MonsterSay("Dobre tedy, nacepuji nam jedno.",LANG_UNIVERSAL,0);
						m_creature->GetMotionMaster()->MovePoint(0,-1768.920044f,5147.930176f,-37.204353f);
						SetAction(3000,5,invoker);
						break;
					case 5:
						m_creature->GetMotionMaster()->MovePoint(0,-1769.099976f,5153.140137f,-37.204201f);
						SetAction(3000,6,invoker);
						break;
					case 6:
						m_creature->SetFacingToObject(invoker);
						m_creature->MonsterSay("Tak at slouzi",LANG_UNIVERSAL,0);
						m_creature->CastSpell(m_creature,58952,true);
						invoker->CastSpell(invoker,58952,true);
						++phase;
						break;
					case 7:
						invoker->MonsterSay("Ach jo.. bojim se o nas svet. Je to cim dal tim horsi - nejdriv Illidan, pak Lich King.. slysel sem ze prej chysta nejaky prekvapko, mluvi se o tom..",LANG_UNIVERSAL,0);
						SetAction(5000,8,invoker);
						break;
					case 8:
						m_creature->MonsterSay("O tom ..eh...vubec nic nevim",LANG_UNIVERSAL,0);
						++phase;
						break;
					case 9:
						invoker->MonsterSay("Pojdme se jeste napit",LANG_UNIVERSAL,0);
						SetAction(1500,10,invoker);
						break;
					case 10:
						m_creature->MonsterSay("Oukej, strpeni",LANG_UNIVERSAL,0);
						m_creature->GetMotionMaster()->MovePoint(0,-1768.920044f,5147.930176f,-37.204353f);
						SetAction(3000,11,invoker);
						break;
					case 11:
						m_creature->GetMotionMaster()->MovePoint(0,-1769.099976f,5153.140137f,-37.204201f);
						SetAction(3000,12,invoker);
						break;
					case 12:
						m_creature->SetFacingToObject(invoker);
						invoker->MonsterSay("Tak na zdravi",LANG_UNIVERSAL,0);
						m_creature->CastSpell(m_creature,58952,true);
						invoker->CastSpell(invoker,11009,true);
						++phase;
						break;
					case 13:
						invoker->MonsterSay("A co si vlastne delal nez si se stal barmanem?",LANG_UNIVERSAL,0);
						SetAction(2500,14,invoker);
						break;
					case 14:
						m_creature->MonsterSay("Noo.. err.. byl sem zahradnik v Dun Morogh",LANG_UNIVERSAL,0);
						SetAction(2500,15,invoker);
						break;
					case 15:
						invoker->MonsterSay("Zahradnik v Dun Moroghu? Ja myslel ze se tam nic pestovat neda",LANG_UNIVERSAL,0);
						SetAction(3000,16,invoker);
						break;
					case 16:
						m_creature->MonsterSay("Vzdycky jsem si pral byt zahradnikem..",LANG_UNIVERSAL,0);
						++phase;
						break;
					case 17:
						invoker->MonsterSay("Aha, to chapu.. to je dnes zase pocasi co?",LANG_UNIVERSAL,0);
						SetAction(4000,18,invoker);
						break;
					case 18:
						m_creature->MonsterSay("Strasny, nezazil sem jiny od ty doby co tu delam",LANG_UNIVERSAL,0);
						SetAction(4000,19,invoker);
						break;
					case 19:
						m_creature->MonsterSay("Prinesu dalsi rundu",LANG_UNIVERSAL,0);
						m_creature->GetMotionMaster()->MovePoint(0,-1768.920044f,5147.930176f,-37.204353f);
						SetAction(3000,20,invoker);
						break;
					case 20:
						m_creature->GetMotionMaster()->MovePoint(0,-1769.099976f,5153.140137f,-37.204201f);
						SetAction(3000,21,invoker);
						break;
					case 21:
						m_creature->SetFacingToObject(invoker);
						invoker->MonsterSay("Na lepsi pocasi",LANG_UNIVERSAL,0);
						m_creature->CastSpell(m_creature,58952,true);
						invoker->CastSpell(invoker,11009,true);
						++phase;
						break;
					case 22:
						invoker->MonsterSay("Jak dlouho tu uz pracujes?",LANG_UNIVERSAL,0);
						SetAction(3000,23,invoker);
						break;
					case 23:
						m_creature->MonsterSay("Od te doby co me ten starej ozrala vyhodil..",LANG_UNIVERSAL,0);
						SetAction(1800,24,invoker);
						break;
					case 24:
						m_creature->MonsterSay("Whoops",LANG_UNIVERSAL,0);
						SetAction(2200,25,invoker);
						break;
					case 25:
						m_creature->MonsterSay("Tak dobre.. hic!.. byl sem pirat nez jsem nastoupil sem",LANG_UNIVERSAL,0);
						SetAction(3500,26,invoker);
						break;
					case 26:
						m_creature->MonsterSay("Muj sef byl strasnej ozrala, jednou sem mu to proste rek no..",LANG_UNIVERSAL,0);
						SetAction(4500,27,invoker);
						break;
					case 27:
						m_creature->MonsterSay("Musime to zapit, mam tu specialitku",LANG_UNIVERSAL,0);
						m_creature->GetMotionMaster()->MovePoint(0,-1768.920044f,5147.930176f,-37.204353f);
						SetAction(3000,28,invoker);
						break;
					case 28:
						m_creature->GetMotionMaster()->MovePoint(0,-1769.099976f,5153.140137f,-37.204201f);
						SetAction(3000,29,invoker);
						break;
					case 29:
						m_creature->SetFacingToObject(invoker);
						m_creature->MonsterSay("Na lepsi zamestnavatele..hic",LANG_UNIVERSAL,0);
						m_creature->CastSpell(m_creature,58952,true);
						invoker->CastSpell(invoker,11009,true);
						SetAction(2500,30,invoker);
						break;
					case 30:
						m_creature->CastSpell(m_creature,49737,true);
						invoker->CastSpell(m_creature,49737,true);
						SetAction(2500,31,invoker);
						break;
					case 31:
						m_creature->MonsterSay("Celkem sila..",LANG_UNIVERSAL,0);
						++phase;
						break;
					case 32:
						m_creature->MonsterSay("Horky pisek more se dotyka,",LANG_UNIVERSAL,0);
						SetAction(2000,33,invoker);
						break;
					case 33:
						m_creature->MonsterSay("stary pirat doupe sve obyva.",LANG_UNIVERSAL,0);
						SetAction(2000,34,invoker);
						break;
					case 34:
						m_creature->MonsterSay("Posadka zraku lidi je ukryta,",LANG_UNIVERSAL,0);
						SetAction(2000,35,invoker);
						break;
					case 35:
						{
						m_creature->MonsterSay("lupu plna jejich jsou koryta.",LANG_UNIVERSAL,0);
						m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
						WorldPacket data;
						m_creature->BuildHeartBeatMsg(&data);
						m_creature->SendMessageToSet(&data,false);
						bezpecnostni = 30000;
						++phase;
						}
						break;
					default:
						outstring_log("npc_barman_scAI: Unhandled actionid case %u",actionid);
						break;
					}
				} else {
					m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					invoker = NULL;
					phase = 0;
				}
			} else actiontimer -= diff;
		}

		if(bezpecnostni)
		{
			if(bezpecnostni <= diff)
			{
				invoker = NULL;
				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				bezpecnostni = 0;
				phase = 0;
			} else bezpecnostni -= diff;
		}
	}
};

bool GossipHello_npc_barman(Player* pPlayer, Creature* pCreature)
{
	npc_barman_scAI* barmanAI = (npc_barman_scAI*)pCreature->AI();
	if(!barmanAI)
	{
		error_log("npc_barman: Couldn't find barmanAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(pPlayer->GetQuestStatus(91000) == QUEST_STATUS_COMPLETE)
	{

	if(!barmanAI->IsThatPlayer(pPlayer->GetGUIDLow())) return false;

	switch(barmanAI->GetPhase())
	{
	default:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pozdravit a zeptat se jak jdou ksefty",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
		break;
	case 1:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pobidnout ke spolecnemu zapiti zalu",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
		break;
	case 2:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Povzdechnout si nad situaci",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+3);
		break;
	case 3:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Opetovne pobidnout k zapiti zalu",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+4);
		break;
	case 4:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Zeptat se na puvodni povolani",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+5);
		break;
	case 5:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Vyjadrit pochopeni, zeptat se na pocasi",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+6);
		break;
	case 6:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Zeptat se, jak dlouho tu pracuje",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+7);
		break;
	case 7:
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Zeptat se kde najit jeho puvodniho zamestnavatele",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+8);
		break;
	case 8:
		pPlayer->PrepareQuestMenu(pCreature->GetGUID());
		break;
	case 99:
		return false;
		break;
	}
	pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
	return true;
	} else {
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}
}

bool GossipSelect_npc_barman(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	npc_barman_scAI* barmanAI = (npc_barman_scAI*)pCreature->AI();
	if(!barmanAI)
	{
		error_log("npc_barman: Couldn't find barmanAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(uiSender == GOSSIP_SENDER_MAIN)
	{
		switch(uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF+1:
			barmanAI->SetAction(1000,1,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+2:
			barmanAI->SetAction(1000,3,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+3:
			barmanAI->SetAction(1000,7,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+4:
			barmanAI->SetAction(1000,9,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+5:
			barmanAI->SetAction(1000,13,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+6:
			barmanAI->SetAction(1000,17,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+7:
			barmanAI->SetAction(1000,22,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+8:
			barmanAI->SetAction(1000,32,pPlayer);
			break;
		}
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}

CreatureAI* GetAI_npc_barman_sc(Creature* pCreature)
{
	return new npc_barman_scAI(pCreature);
}

struct MANGOS_DLL_DECL npc_pirat_tanaAI: public ScriptedAI
{
	npc_pirat_tanaAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 actiontimer;
	uint16 actionid;
	Player* invoker;
	uint32 phase;
	uint32 bezpecnostni;

	void Reset()
	{
		actiontimer = 0;
		actionid = 0;
		invoker = NULL;
		phase = 0;
		bezpecnostni = 0;
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
	}

	uint32 GetPhase()
	{
		return actiontimer?99:phase;
	}

	bool IsThatPlayer(uint64 guid)
	{
		if(!invoker) return true;

		outstring_log("Comparing source guid %u with invoker guid %u",guid,invoker->GetGUID());

		return (guid == invoker->GetGUIDLow());
	}

	void SetAction(uint32 timer, uint32 action, Player* pPlayer)
	{
		if(actiontimer || !pPlayer) return;

		actiontimer = timer;
		actionid = action;
		invoker = pPlayer;
		bezpecnostni = 50000;
	}

	void UpdateAI(const uint32 diff)
	{
		if(actiontimer)
		{
			if(actiontimer <= diff)
			{
				actiontimer = 0;
				if(invoker && invoker->isAlive() && invoker->IsWithinDistInMap(m_creature,50.0f))
				{
					switch(actionid)
					{
					case 0:
						phase = 0;
						break;
					case 1:
						invoker->MonsterSay("Zdravim",LANG_UNIVERSAL,0);
						SetAction(1500,2,invoker);
						break;
					case 2:
						phase = 99;
						bezpecnostni = 1000;
						if(invoker->GetItemCount(99901,false) > 0 && invoker->GetItemCount(9260,false) > 0)
						{
							m_creature->MonsterSay("To je muj papousek! A ten rum..",LANG_UNIVERSAL,0);
							SetAction(2500,3,invoker);
							phase = 1;
						} else if(invoker->GetItemCount(9260,false) > 0)
						{
							m_creature->MonsterYell("Vrat mi meho papouska! Vim zes ho schoval nekde u tech proradnych goblinu!",LANG_UNIVERSAL,0);
						} else if(invoker->GetItemCount(99901,false) > 0)
						{
							m_creature->MonsterYell("Rekl jsem papouska a rum!",LANG_UNIVERSAL,0);
						} else
						{
							m_creature->MonsterYell("Prines mi rum a vrat mi meho papouska ty zpropadeny zlodeji!",LANG_UNIVERSAL,0);
						}
						break;
					case 3:
						m_creature->MonsterSay("Za odmenu ti reknu jednu vec..",LANG_UNIVERSAL,0);
						SetAction(2500,4,invoker);
						break;
					case 4:
						m_creature->MonsterSay("Kdysi sme nasli knizku, byla divna, nemohli sme ji precist, tak sme ji flakli z lodi pryc.. mozna by se ti hodila.. no nic, je pryc",LANG_UNIVERSAL,0);
						++phase;
						break;
					case 5:
						m_creature->MonsterSay("Pod bajnou vezi,",LANG_UNIVERSAL,0);
						SetAction(1500,6,invoker);
						break;
					case 6:
						m_creature->MonsterSay("v zemi zcela puste",LANG_UNIVERSAL,0);
						SetAction(1500,7,invoker);
						break;
					case 7:
						m_creature->MonsterSay("strazce knihu strezi,",LANG_UNIVERSAL,0);
						SetAction(1500,8,invoker);
						break;
					case 8:
						m_creature->MonsterSay("nebezpeci vsak roste.",LANG_UNIVERSAL,0);
						m_creature->SetFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER);
						bezpecnostni = 30000;
						++phase;
						break;
					default:
						outstring_log("npc_pirat_tanaAI: Unhandled actionid case %u",actionid);
					}
				} else {
					m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					invoker = NULL;
					phase = 0;
				}
			} else actiontimer -= diff;
		}

		if(bezpecnostni)
		{
			if(bezpecnostni <= diff)
			{
				invoker = NULL;
				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				bezpecnostni = 0;
				phase = 0;
			} else bezpecnostni -= diff;
		}
	}
};

bool GossipHello_npc_pirat_tana(Player* pPlayer, Creature* pCreature)
{
	npc_pirat_tanaAI* piratAI = (npc_pirat_tanaAI*)pCreature->AI();
	if(!piratAI)
	{
		error_log("npc_pirat: Couldn't find piratAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(pPlayer->GetQuestStatus(91002) == QUEST_STATUS_COMPLETE || pPlayer->GetQuestRewardStatus(91002))
	{
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}
	if(pPlayer->GetQuestStatus(91001) == QUEST_STATUS_COMPLETE)// || pPlayer->GetQuestStatus(91001) == QUEST_STATUS_NONE)
	{
		if(!piratAI->IsThatPlayer(pPlayer->GetGUIDLow())) return false;

		switch(piratAI->GetPhase())
		{
		default:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pozdravit",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
			break;
		case 2:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Zeptat se, kde ta knizka zhruba je",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
			break;
		case 3:
			pPlayer->PrepareQuestMenu(pCreature->GetGUID());
			break;
		case 99:
			break;
		}
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}

	pCreature->CastSpell(pPlayer,23382,true);
	switch(urand(1,5))
	{
	case 1:
		pCreature->MonsterSay("Aaarghh...",LANG_UNIVERSAL,0);
		break;
	case 2:
		pCreature->MonsterSay("Odprejskni!",LANG_UNIVERSAL,0);
		break;
	case 3:
		pCreature->MonsterSay("Grrrr...",LANG_UNIVERSAL,0);
		break;
	case 4:
		pCreature->MonsterSay("Zmiz!",LANG_UNIVERSAL,0);
		break;
	default:
		break;
	}
	return true;
}

bool GossipSelect_npc_pirat_tana(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	npc_pirat_tanaAI* piratAI = (npc_pirat_tanaAI*)pCreature->AI();
	if(!piratAI)
	{
		error_log("npc_pirat: Couldn't find piratAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(uiSender == GOSSIP_SENDER_MAIN)
	{
		switch(uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF+1:
			piratAI->SetAction(1000,1,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+2:
			piratAI->SetAction(1000,5,pPlayer);
			break;
		}
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}

CreatureAI* GetAI_npc_pirat_tana(Creature* pCreature)
{
	return new npc_pirat_tanaAI(pCreature);
}

bool GOUse_go_klec_jau(Player* pPlayer, GameObject* pGO)
{
	ItemPosCountVec dest;
    uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 99901, 1, false);
    if (msg == EQUIP_ERR_OK)
    {
		pPlayer->SendNewItem(pPlayer->StoreNewItem(dest, 99901, 1, true),1,true,false,true);
    }
	if(Creature* zabijak = GetClosestCreatureWithEntry(pPlayer,99933,50.0f))
	{
		zabijak->setFaction(14);
		switch(urand(1,3))
		{
		case 1:
			zabijak->MonsterYell("Hej! Vrat to!",LANG_UNIVERSAL,0);
			break;
		case 2:
			zabijak->MonsterYell("Nech to byt! Hej!",LANG_UNIVERSAL,0);
			break;
		case 3:
			zabijak->MonsterYell("Vrat ho zpatky! Hej!",LANG_UNIVERSAL,0);
			break;
		}
		zabijak->AI()->AttackStart(pPlayer);
	}
	return true;
}

struct MANGOS_DLL_DECL npc_aywes_desoAI: public ScriptedAI
{
	npc_aywes_desoAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 actiontimer;
	uint16 actionid;
	Player* invoker;
	uint32 phase;
	uint32 bezpecnostni;

	void Reset()
	{
		actiontimer = 0;
		actionid = 0;
		invoker = NULL;
		phase = 0;
		bezpecnostni = 0;
		m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
	}

	uint32 GetPhase()
	{
		return actiontimer?99:phase;
	}

	bool IsThatPlayer(uint64 guid)
	{
		if(!invoker) return true;

		outstring_log("Comparing source guid %u with invoker guid %u",guid,invoker->GetGUID());

		return (guid == invoker->GetGUIDLow());
	}

	void SetAction(uint32 timer, uint32 action, Player* pPlayer)
	{
		if(actiontimer || !pPlayer) return;

		actiontimer = timer;
		actionid = action;
		invoker = pPlayer;
		bezpecnostni = 50000;
	}

	void UpdateAI(const uint32 diff)
	{
		if(actiontimer)
		{
			if(actiontimer <= diff)
			{
				actiontimer = 0;
				if(invoker && invoker->isAlive() && invoker->IsWithinDistInMap(m_creature,50.0f))
				{
					switch(actionid)
					{
					case 0:
						phase = 0;
						break;
					case 1:
						invoker->MonsterSay("Budte zdrav, hledam tady jednu knihu",LANG_UNIVERSAL,0);
						SetAction(1500,2,invoker);
						break;
					case 2:
						m_creature->MonsterSay("Aaaaarr... snad .. nemyslis .. knihu Statura Oblittero! Kdyz jsem se ... ji snazil precist.. nagy.. vzali mi ji..",LANG_UNIVERSAL,0);
						SetAction(6000,3,invoker);
						break;
					case 3:
						m_creature->MonsterSay("Prines ji.. a ja ti ji pomohu precist.. je psana ve starodavnem jazyce",LANG_UNIVERSAL,0);
						++phase;
						bezpecnostni = 20000;
						break;
					case 4:
						m_creature->SetStandState(UNIT_STAND_STATE_SIT);
						m_creature->MonsterSay("Tak jdeme na to...",LANG_UNIVERSAL,0);
						SetAction(2000,5,invoker);
						break;
					case 5:
						m_creature->MonsterSay("Na upati sneznych hor,",LANG_UNIVERSAL,0);
						SetAction(2500,6,invoker);
						break;
					case 6:
						m_creature->MonsterSay("ukryva se zvlastni tvor.",LANG_UNIVERSAL,0);
						SetAction(2500,7,invoker);
						break;
					case 7:
						m_creature->MonsterSay("Draci dech mu zivot chrani,",LANG_UNIVERSAL,0);
						SetAction(2500,8,invoker);
						break;
					case 8:
						m_creature->MonsterSay("vecnemu chladu se vsak neubrani.",LANG_UNIVERSAL,0);
						m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
						bezpecnostni = 30000;
						phase = 3;
						break;
					default:
						outstring_log("npc_aywes_desoAI: Unhandled actionid case %u",actionid);
					}
				} else {
					m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
					m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					invoker = NULL;
					phase = 0;
				}
			} else actiontimer -= diff;
		}

		if(bezpecnostni)
		{
			if(bezpecnostni <= diff)
			{
				m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
				invoker = NULL;
				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				bezpecnostni = 0;
				phase = 0;
			} else bezpecnostni -= diff;
		}
	}
};

bool GossipHello_npc_aywes_deso(Player* pPlayer, Creature* pCreature)
{
	npc_aywes_desoAI* aywesAI = (npc_aywes_desoAI*)pCreature->AI();
	if(!aywesAI)
	{
		error_log("npc_aywes: Couldn't find aywesAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(pPlayer->GetQuestStatus(91003) == QUEST_STATUS_COMPLETE || pPlayer->GetQuestRewardStatus(91003))
	{
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}
	if(pPlayer->GetQuestStatus(91002) == QUEST_STATUS_COMPLETE)// || pPlayer->GetQuestStatus(91002) == QUEST_STATUS_NONE)
	{
		if(!aywesAI->IsThatPlayer(pPlayer->GetGUIDLow())) return false;

		if(pPlayer->GetItemCount(99902,false) > 0 && aywesAI->GetPhase() != 3)
		{
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pujcit knihu",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
		} else {
			switch(aywesAI->GetPhase())
			{
			default:
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pozdravit a zeptat se na knihu",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
				break;
			case 3:
				pPlayer->PrepareQuestMenu(pCreature->GetGUID());
				break;
			case 99:
				break;
			}			
		}
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}
	return true;
}

bool GossipSelect_npc_aywes_deso(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	npc_aywes_desoAI* aywesAI = (npc_aywes_desoAI*)pCreature->AI();
	if(!aywesAI)
	{
		error_log("npc_aywes: Couldn't find aywesAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(uiSender == GOSSIP_SENDER_MAIN)
	{
		switch(uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF+1:
			aywesAI->SetAction(1000,1,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+2:
			aywesAI->SetAction(1000,4,pPlayer);
			break;
		}
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}

CreatureAI* GetAI_npc_aywes_deso(Creature* pCreature)
{
	return new npc_aywes_desoAI(pCreature);
}

struct MANGOS_DLL_DECL npc_drak_wsAI: public ScriptedAI
{
	npc_drak_wsAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 actiontimer;
	uint16 actionid;
	Player* invoker;
	uint32 phase;
	uint32 bezpecnostni;

	void Reset()
	{
		actiontimer = 0;
		actionid = 0;
		invoker = NULL;
		phase = 0;
		bezpecnostni = 0;
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
	}

	uint32 GetPhase()
	{
		return actiontimer?99:phase;
	}

	bool IsThatPlayer(uint64 guid)
	{
		if(!invoker) return true;

		outstring_log("Comparing source guid %u with invoker guid %u",guid,invoker->GetGUID());

		return (guid == invoker->GetGUIDLow());
	}

	void SetAction(uint32 timer, uint32 action, Player* pPlayer)
	{
		if(actiontimer || !pPlayer) return;

		actiontimer = timer;
		actionid = action;
		invoker = pPlayer;
		bezpecnostni = 50000;
	}

	void UpdateAI(const uint32 diff)
	{
		if(actiontimer)
		{
			if(actiontimer <= diff)
			{
				actiontimer = 0;
				if(invoker && invoker->isAlive() && invoker->IsWithinDistInMap(m_creature,50.0f))
				{
					switch(actionid)
					{
					case 0:
						phase = 0;
						break;
					case 1:
						invoker->MonsterSay("Bud pozdraven, Porcusi. Mam tu jednu knihu..",LANG_UNIVERSAL,0);
						SetAction(2500,2,invoker);
						break;
					case 2:
						m_creature->MonsterSay("Takze vas ke me zavedla ta kniha a chcete najit nekoho, kdo vam prozradi sidlo poloboha?",LANG_UNIVERSAL,0);
						SetAction(6500,3,invoker);
						break;
					case 3:
						m_creature->MonsterSay("Citim uz nejakou dobu divne sily, ktere proudi spektralnim svetem, avsak tento druh sil ja spravne vycitit nemohu.",LANG_UNIVERSAL,0);
						SetAction(7500,4,invoker);
						break;
					case 4:
						m_creature->MonsterSay("Musite najit nekoho, kdo se umi s timto svetem spojit. Znam toho praveho..",LANG_UNIVERSAL,0);
						SetAction(5500,5,invoker);
						break;
					case 5:
						m_creature->MonsterSay("Je to shaman. Najdete ho tam, kde vegetace zije a voda kopci plyne.",LANG_UNIVERSAL,0);
						phase = 3;
						m_creature->SetFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER);
						break;
					default:
						outstring_log("npc_drak_wsAI: Unhandled actionid case %u",actionid);
					}
				} else {
					m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
					m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					invoker = NULL;
					phase = 0;
				}
			} else actiontimer -= diff;
		}

		if(bezpecnostni)
		{
			if(bezpecnostni <= diff)
			{
				m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
				invoker = NULL;
				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				bezpecnostni = 0;
				phase = 0;
			} else bezpecnostni -= diff;
		}
	}
};

bool GossipHello_npc_drak_ws(Player* pPlayer, Creature* pCreature)
{
	npc_drak_wsAI* drakAI = (npc_drak_wsAI*)pCreature->AI();
	if(!drakAI)
	{
		error_log("npc_drak: Couldn't find drakAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	//spell: 38318
	if(pPlayer->GetQuestStatus(91004) == QUEST_STATUS_COMPLETE || pPlayer->GetQuestRewardStatus(91004))
	{
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}
	if(pPlayer->GetQuestStatus(91003) == QUEST_STATUS_COMPLETE)// || pPlayer->GetQuestStatus(91003) == QUEST_STATUS_NONE)
	{
		if(!drakAI->IsThatPlayer(pPlayer->GetGUIDLow())) return false;

		if(!pPlayer->HasAura(38318))
		{
			switch(urand(1,3))
			{
			case 1:
				pCreature->MonsterSay("S lidmi nemluvim o nicem vaznem!",LANG_UNIVERSAL,0);
				break;
			case 2:
				pCreature->MonsterSay("Jsi clovek! Tohle ti nereknu",LANG_UNIVERSAL,0);
				break;
			case 3:
				pCreature->MonsterSay("Lide nejsou hodni neco takoveho vedet!",LANG_UNIVERSAL,0);
				break;
			}
			return false;
		} else {
			switch(drakAI->GetPhase())
			{
			default:
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pozdravit a zeptat se na knihu",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
				break;
			case 3:
				pPlayer->PrepareQuestMenu(pCreature->GetGUID());
				break;
			case 99:
				break;
			}
			pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		}
		return true;
	}
	return true;
}

bool GossipSelect_npc_drak_ws(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	npc_drak_wsAI* drakAI = (npc_drak_wsAI*)pCreature->AI();
	if(!drakAI)
	{
		error_log("npc_drak: Couldn't find drakAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(uiSender == GOSSIP_SENDER_MAIN)
	{
		switch(uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF+1:
			drakAI->SetAction(1000,1,pPlayer);
			break;
		}
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}

CreatureAI* GetAI_npc_drak_ws(Creature* pCreature)
{
	return new npc_drak_wsAI(pCreature);
}

bool GossipHello_npc_drak_ws_drak(Player* pPlayer, Creature* pCreature)
{
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pokusit se komunikovat",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
	pPlayer->SEND_GOSSIP_MENU(1,pCreature->GetGUID());
	return true;
}

bool GossipSelect_npc_drak_ws_drak(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
	{
		pCreature->MonsterSay("..tady neni vhodne misto, nahore je to prihodnejsi..",LANG_UNIVERSAL,0);
		pPlayer->CLOSE_GOSSIP_MENU();
		return true;
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return false;
}

bool ItemUse_item_ohoz(Player* pPlayer, Item* pItem, SpellCastTargets const& scTargets)
{
    pPlayer->CastSpell(pPlayer,38318,true);
    return true;
}

struct MANGOS_DLL_DECL npc_shaman_alteracAI: public ScriptedAI
{
	npc_shaman_alteracAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 actiontimer;
	uint16 actionid;
	Player* invoker;
	uint32 phase;
	uint32 bezpecnostni;
	uint64 OrbsGUID[2];

	void Reset()
	{
		actiontimer = 0;
		actionid = 0;
		invoker = NULL;
		phase = 0;
		bezpecnostni = 0;
		OrbsGUID[0] = 0;
		OrbsGUID[1] = 0;
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
	}

	uint32 GetPhase()
	{
		return actiontimer?99:phase;
	}

	bool IsThatPlayer(uint64 guid)
	{
		if(!invoker) return true;

		outstring_log("Comparing source guid %u with invoker guid %u",guid,invoker->GetGUID());

		return (guid == invoker->GetGUIDLow());
	}

	void SetAction(uint32 timer, uint32 action, Player* pPlayer)
	{
		if(actiontimer || !pPlayer) return;

		actiontimer = timer;
		actionid = action;
		invoker = pPlayer;
		bezpecnostni = 50000;
	}

	void JustSummoned(Creature* pSummoned)
	{
		if(!actionid) return;

		pSummoned->setFaction(35);
		pSummoned->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);
		if(pSummoned->GetEntry() == 17096)
		{
			if(actionid == 8 || actionid == 9)
			{
				pSummoned->CastSpell(pSummoned, 55928, true);
				pSummoned->SetFloatValue(OBJECT_FIELD_SCALE_X,0.8f);
			}
			if(actionid == 10 || actionid == 11)
			{
				pSummoned->CastSpell(pSummoned, 38464, true);
				pSummoned->SetFloatValue(OBJECT_FIELD_SCALE_X,0.5f);
			}
			if(actionid == 16 || actionid == 17)
			{
				pSummoned->SetDisplayId(1132);
				pSummoned->SetFloatValue(OBJECT_FIELD_SCALE_X,1.5f);
			}
		}
	}

	void UpdateAI(const uint32 diff)
	{
		if(actiontimer)
		{
			if(actiontimer <= diff)
			{
				actiontimer = 0;
				if(invoker && invoker->isAlive() && invoker->IsWithinDistInMap(m_creature,50.0f))
				{
					switch(actionid)
					{
					case 0:
						phase = 0;
						break;
					case 1:
						invoker->MonsterSay("Zdravim, bratre. Potreboval bych od tebe informaci.. vse je v tehle knize",LANG_UNIVERSAL,0);
						SetAction(3000,2,invoker);
						break;
					case 2:
						m_creature->MonsterSay("Aaaaah, kniha Statura Obliterro.. hrdino! Ty se chces postavit polobohu?",LANG_UNIVERSAL,0);
						++phase;
						break;
					case 3:
						invoker->MonsterSay("Ano, samozrejme nebudu sam. Muzes mi tedy prozradit vice?",LANG_UNIVERSAL,0);
						SetAction(3000,4,invoker);
						break;
					case 4:
						m_creature->MonsterSay("Oh, ano.. obnasi to obrad, dulezity pro posilneni.. jsi si jist, ze ho chces podstoupit?",LANG_UNIVERSAL,0);
						++phase;
						break;
					case 5:
						invoker->MonsterSay("Jsem si jist a jsem pripraven obrad podstoupit",LANG_UNIVERSAL,0);
						SetAction(3000,6,invoker);
						break;
					case 6:
						m_creature->MonsterSay("Probudte se sily prirodni a pomozte posilnit mladeho hrdinu",LANG_UNIVERSAL,0);
						m_creature->CastSpell(invoker,11986,false);
						SetAction(3000,7,invoker);
						break;
					case 7:
						invoker->CastSpell(invoker,25043,true);
						m_creature->MonsterSay("Sily ohne, pristupte ke me a sverte sve sily!",LANG_UNIVERSAL,0);
						SetAction(2000,8,invoker);
						break;
					case 8:
						OrbsGUID[0] = m_creature->SummonCreature(17096,320.058258f,-1480.262939f,42.991917f,0.616683f,TEMPSUMMON_TIMED_DESPAWN,10000)->GetGUID();
						OrbsGUID[1] = m_creature->SummonCreature(17096,324.851471f,-1487.024292f,43.452103f,0.616683f,TEMPSUMMON_TIMED_DESPAWN,10000)->GetGUID();
						m_creature->CastSpell(invoker,37985,true);
						SetAction(6500,9,invoker);
						break;
					case 9:
						if(Unit* Sph1 = Unit::GetUnit(*m_creature,OrbsGUID[0]))
						{
							Sph1->RemoveAllAuras();
							Sph1->CastSpell(Sph1,55947,true);
						}
						if(Unit* Sph2 = Unit::GetUnit(*m_creature,OrbsGUID[1]))
						{
							Sph2->RemoveAllAuras();
							Sph2->CastSpell(Sph2,55947,true);
						}
						m_creature->InterruptNonMeleeSpells(false);
						m_creature->RemoveAllAuras();
						SetAction(4000,10,invoker);
						break;
					case 10:
						m_creature->MonsterSay("Sily vetru, pristupte ke me a sverte sve sily!",LANG_UNIVERSAL,0);
						SetAction(2000,11,invoker);
						break;
					case 11:
						OrbsGUID[0] = m_creature->SummonCreature(17096,320.058258f,-1480.262939f,42.991917f,0.616683f,TEMPSUMMON_TIMED_DESPAWN,10000)->GetGUID();
						OrbsGUID[1] = m_creature->SummonCreature(17096,324.851471f,-1487.024292f,43.452103f,0.616683f,TEMPSUMMON_TIMED_DESPAWN,10000)->GetGUID();
						SetAction(5500,12,invoker);
						break;
					case 12:
						if(Unit* Sph1 = Unit::GetUnit(*m_creature,OrbsGUID[0]))
						{
							Sph1->RemoveAllAuras();
							Sph1->CastSpell(Sph1,52166,true);
						}
						if(Unit* Sph2 = Unit::GetUnit(*m_creature,OrbsGUID[1]))
						{
							Sph2->RemoveAllAuras();
							Sph2->CastSpell(Sph2,52166,true);
						}
						m_creature->CastSpell(m_creature,52870,false);
						SetAction(2000,13,invoker);
						break;
					case 13:
						m_creature->MonsterSay("Zadam i vas, sily demonicke, propujcte svou silu!",LANG_UNIVERSAL,0);
						SetAction(2500,14,invoker);
						break;
					case 14:
						m_creature->CastSpell(m_creature,52952,true);
						SetAction(1500,15,invoker);
						break;
					case 15:
						m_creature->MonsterSay("Sily demonu budou nejdulezitejsim pomocnikem",LANG_UNIVERSAL,0);
						SetAction(4000,16,invoker);
						break;
					case 16:
						m_creature->MonsterSay("Demonicke sily, pristupte!",LANG_UNIVERSAL,0);
						m_creature->CastSpell(m_creature,40335,true);
						SetAction(20000,17,invoker);
						break;
					case 17:
						OrbsGUID[0] = m_creature->SummonCreature(17096,320.058258f,-1480.262939f,42.991917f,0.616683f,TEMPSUMMON_TIMED_DESPAWN,15000)->GetGUID();
						OrbsGUID[1] = m_creature->SummonCreature(17096,324.851471f,-1487.024292f,43.452103f,0.616683f,TEMPSUMMON_TIMED_DESPAWN,15000)->GetGUID();
						SetAction(3500,18,invoker);
						break;
					case 18:
						//32712,60429
						invoker->CastSpell(m_creature,32712,true);
						SetAction(1500,19,invoker);
						break;
					case 19:
						if(Unit* Sph1 = Unit::GetUnit(*m_creature,OrbsGUID[0]))
						{
							Sph1->CastSpell(Sph1,60429,true);
						}
						if(Unit* Sph2 = Unit::GetUnit(*m_creature,OrbsGUID[1]))
						{
							Sph2->CastSpell(Sph2,60429,true);
						}
						SetAction(2000,20,invoker);
						break;
					case 20:
						m_creature->MonsterSay("Ritual je skoro u konce!",LANG_UNIVERSAL,0);
						SetAction(3000,21,invoker);
						break;
					case 21:
						if(Unit* Sph1 = Unit::GetUnit(*m_creature,OrbsGUID[0]))
						{
							Sph1->CastSpell(Sph1,60429,true);
						}
						if(Unit* Sph2 = Unit::GetUnit(*m_creature,OrbsGUID[1]))
						{
							Sph2->CastSpell(Sph2,60429,true);
						}
						SetAction(2500,22,invoker);
						break;
					case 22:
						if(Unit* Sph1 = Unit::GetUnit(*m_creature,OrbsGUID[0]))
						{
							Sph1->CastSpell(Sph1,32712,true);
						}
						if(Unit* Sph2 = Unit::GetUnit(*m_creature,OrbsGUID[1]))
						{
							Sph2->CastSpell(Sph2,32712,true);
						}
						SetAction(2000,23,invoker);
						break;
					case 23:
						m_creature->MonsterSay("Nyni do tebe vstupuji tve sily. Po dokonceni meditace budes pripraven..",LANG_UNIVERSAL,0);
						m_creature->RemoveAllAuras();
						SetAction(18000,24,invoker);
						break;
					case 24:
						m_creature->MonsterSay("Najdi ducha, povede te...",LANG_UNIVERSAL,0);
						SetAction(6000,25,invoker);
						break;
					case 25:
						m_creature->MonsterSay("Mrtve vetry zemi vanou,",LANG_UNIVERSAL,0);
						SetAction(2000,26,invoker);
						break;
					case 26:
						m_creature->MonsterSay("duchove ze zeme se jiz hrabou.",LANG_UNIVERSAL,0);
						SetAction(2000,27,invoker);
						break;
					case 27:
						m_creature->MonsterSay("Pod hradem ukryva se temne misto,",LANG_UNIVERSAL,0);
						SetAction(2000,28,invoker);
						break;
					case 28:
						m_creature->MonsterSay("zlo je citit uz na jisto.",LANG_UNIVERSAL,0);
						SetAction(4000,29,invoker);
						break;
					case 29:
						m_creature->MonsterSay("Nyni jsi pripraven",LANG_UNIVERSAL,0);
						phase = 3;
						m_creature->SetFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER);
						break;
					default:
						outstring_log("npc_shaman_alteracAI: Unhandled actionid case %u",actionid);
					}
				} else {
					m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					invoker = NULL;
					phase = 0;
				}
			} else actiontimer -= diff;
		}

		if(bezpecnostni)
		{
			if(bezpecnostni <= diff)
			{
				invoker = NULL;
				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				bezpecnostni = 0;
				phase = 0;
			} else bezpecnostni -= diff;
		}
	}
};

bool GossipHello_npc_shaman_alterac(Player* pPlayer, Creature* pCreature)
{
	npc_shaman_alteracAI* ashamanAI = (npc_shaman_alteracAI*)pCreature->AI();
	if(!ashamanAI)
	{
		error_log("npc_ashaman: Couldn't find ashamanAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(pPlayer->GetQuestStatus(91005) == QUEST_STATUS_COMPLETE || pPlayer->GetQuestRewardStatus(91005))
	{
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}
	if(pPlayer->GetQuestStatus(91004) == QUEST_STATUS_COMPLETE)
	{
		if(!ashamanAI->IsThatPlayer(pPlayer->GetGUIDLow())) return false;

		switch(ashamanAI->GetPhase())
		{
		default:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Pozdravit a pozadat o informace",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
			break;
		case 1:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Odpovedet ze nebudes sam a pobidnout.",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
			break;
		case 2:
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Postoupit obrad",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+3);
			break;
		case 3:
			pPlayer->PrepareQuestMenu(pCreature->GetGUID());
			break;
		case 99:
			break;
		}
		pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetGUID());
		return true;
	}
	return false;
}

bool GossipSelect_npc_shaman_alterac(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	npc_shaman_alteracAI* ashamanAI = (npc_shaman_alteracAI*)pCreature->AI();
	if(!ashamanAI)
	{
		error_log("npc_ashaman: Couldn't find ashamanAI of creature with entry %u",pCreature->GetEntry());
		return false;
	}

	if(uiSender == GOSSIP_SENDER_MAIN)
	{
		switch(uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF+1:
			ashamanAI->SetAction(1000,1,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+2:
			ashamanAI->SetAction(1000,3,pPlayer);
			break;
		case GOSSIP_ACTION_INFO_DEF+3:
			ashamanAI->SetAction(1000,5,pPlayer);
			break;
		}
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}

CreatureAI* GetAI_npc_shaman_alterac(Creature* pCreature)
{
	return new npc_shaman_alteracAI(pCreature);
}

bool GossipHello_npc_andel_kara(Player* pPlayer, Creature* pCreature)
{
	if(pPlayer->GetQuestRewardStatus(91005))
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Teleport",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
		pPlayer->SEND_GOSSIP_MENU(1,pCreature->GetGUID());
		return true;
	} else {
		pPlayer->PrepareQuestMenu(pCreature->GetGUID());
		pPlayer->SEND_GOSSIP_MENU(1,pCreature->GetGUID());
		return true;
	}
	return false;
}

bool GossipSelect_npc_andel_kara(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	if(uiSender == GOSSIP_SENDER_MAIN)
	{
		switch(uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF+1:
			pPlayer->NearTeleportTo(-11068.096680f,-1809.167603f,52.791916f,1.579361f);
			pCreature->MonsterWhisper("Bud silny, polobuh se skryva v temnem koute - tam, kde slunce nesviti..",pPlayer->GetGUID(),true);
			break;
		}
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}

/////////////////////////////////////////////////
//  ZAVERECNY BOSS							   //
/////////////////////////////////////////////////

/*

//Zajimavy spelly:
63666 - Napalm shell (bác za 8k/12k crit, 4k/1s)
41371 - Shell of Pain (2500 melee atackerum + reflect spellu, 15s)
26134 - Eye beam (4k targetu + vsem okolo)
40946 - Molten Rain (4k v area)
24193 - Charge -- pro miniony?
46887 - Bitter Blast -- pro miniony?

//Non-special
32849 - Cyclone Strike
55978 - Heart Strike
59692 - Flame Breath


//visualy
53210 - Apocalypse - visual

*/

const float channeler_pos[7][4] = {
	{-11260.425781f,-1393.748901f,0.083149f,6.163691f},
	{-11260.479492f,-1414.066284f,-0.234322f,0.273205f},
	{-11250.992188f,-1430.355713f,-0.601785f,0.771933f},
	{-11234.532227f,-1441.558594f,-0.085914f,1.302077f},
	{-11212.124023f,-1442.470947f,0.261971f,1.855783f},
	{-11194.059570f,-1429.650024f,-0.264870f,2.401635f},
	{-11184.855469f,-1407.480103f,-0.319588f,3.053516f}
};

const float summon_skels_pos[2][4] = {
	{-11257.613281f,-1476.275879f,3.577442f,0.631707f},
	{-11184.820313f,-1473.974365f,3.576001f,2.179728f}
};

struct MANGOS_DLL_DECL boss_marathon_finalAI: public ScriptedAI
{
	boss_marathon_finalAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	//Faze 0
	uint16 phase;
	uint32 phase_timer;
	std::list<Creature*> channelers;
	bool c_summoned;
	uint32 check_timer;
	uint16 chans;
	Unit* lastkiller;
	//Faze 1
	Creature* gulnath;
	bool c_gulnath;
	uint32 spell1_timer;
	uint32 spell2_timer;
	uint32 spell3_timer;
	uint32 dejmudot_timer;
	uint32 summon_f2_timer;
	uint16 f2_summonphase;
	bool f2_banished;
	//Faze 2
	uint32 summon_tydva_timer;
	uint8 summon_tydva_phase;
	uint8 zabiti_jich_counter;
	Creature* bahagood;
	Creature* bahaevil;
	uint16 shadowvlak_counter;
	uint32 teleport_timer;
	//Faze 3
	bool f3_banished;
	uint32 transform_timer;
	uint8 transform_phase;
	uint32 silence_timer;
	uint32 firewall_timer;
	uint32 firenova_timer;
	//Faze 4 - umirani
	uint32 die_timer;
	uint32 die_phase;
	//Faze 5 - final
	//

	void Reset()
	{
		m_creature->RemoveAllAuras();
		m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,1.2f);
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, false);
		m_creature->SetDisplayId(21564);
		channelers.clear();
		c_summoned = false;
		chans = 0;
		check_timer = 1000;
		lastkiller = NULL;
		phase = 0;
		phase_timer = 0;
		gulnath = NULL;
		c_gulnath = false;
		spell1_timer = 3000;
		spell2_timer = 30000;
		summon_f2_timer = 22000;
		f2_summonphase = 0;
		f2_banished = false;
		summon_tydva_timer = 500;
		summon_tydva_phase = 1;
		zabiti_jich_counter = 0;
		bahagood = NULL;
		bahaevil = NULL;
		shadowvlak_counter = 0;
		teleport_timer = 0;
		f3_banished = false;
		transform_timer = 0;
		transform_phase = 0;
		silence_timer = 15000;
		dejmudot_timer = 20000;
	}

	void MoveInLineOfSight(Unit* pWho)
	{
		if(phase == 0 || f2_banished) return;
		ScriptedAI::MoveInLineOfSight(pWho);
	}

	void AttackStart(Unit* pWho)
	{
		if(phase == 0) return;
		ScriptedAI::AttackStart(pWho);
	}

	void KilledChanneler(Unit* killer)
	{
		--chans;
		lastkiller = killer;
	}

	void DamageTaken(Unit* pDoneBy,uint32 &damage)
	{
		if(phase == 1 && f2_banished)
			damage = 0;
	}

	void KlonZemrel()
	{
		++zabiti_jich_counter;
		if(zabiti_jich_counter >= 2)
		{
			summon_tydva_timer = 5000;
			summon_tydva_phase = 3;
		}
	}

	void NahodTamOhnivouStenu()
	{
		m_creature->SummonCreature(23920,-11210.669922f,-1487.581909f,3.577940f,1.624730f,TEMPSUMMON_TIMED_DESPAWN,15000);
		m_creature->SummonCreature(23920,-11218.814453f,-1489.995850f,3.581496f,1.649779f,TEMPSUMMON_TIMED_DESPAWN,15000);
		m_creature->SummonCreature(23920,-11227.791992f,-1489.400391f,3.580389f,1.569669f,TEMPSUMMON_TIMED_DESPAWN,15000);
		m_creature->SummonCreature(23920,-11234.217773f,-1488.374390f,3.578996f,1.569669f,TEMPSUMMON_TIMED_DESPAWN,15000);
	}

	void SpellHit(Unit* pCaster, const SpellEntry* spell)
	{
		if(spell->Id == 46960 && phase == 1 && f2_banished)
		{
			++phase;
			m_creature->MonsterSay("Aaahh, nemuzu.. se.. hybat... co jste to udelali?!",LANG_UNIVERSAL,0);
			m_creature->RemoveAllAuras();
			m_creature->InterruptNonMeleeSpells(false);
			m_creature->GetMotionMaster()->MoveIdle();
			m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
			m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			m_creature->RemoveAllAuras();
			m_creature->LoadCreaturesAddon();
			m_creature->GetMotionMaster()->Clear();
			m_creature->GetMotionMaster()->MoveIdle();
			m_creature->StopMoving();
			m_creature->CastSpell(m_creature, 36185, true);
			m_creature->CastSpell(m_creature, 65208, true);
		}
	}

	void JustSummoned(Creature* pSummoned)
	{
		switch(pSummoned->GetEntry())
		{
		case 99941:
		case 99942:
		case 99943:
			pSummoned->CastSpell(pSummoned,36400,true);
			pSummoned->GetMotionMaster()->MovePoint(0,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ());
			break;
		case 23920:
			pSummoned->CastSpell(pSummoned,43113,true);
			break;
		default:
			break;
		}
	}

	void UpdateAI(const uint32 diff)
	{
		////////////////////////// Faze 0
		if(phase == 0)
		{
			if(!m_creature->HasAura(63894))
				m_creature->CastSpell(m_creature,63894,true);

			if(!m_creature->HasAura(64022))
				m_creature->CastSpell(m_creature,64022,true);
	
			if(!c_summoned)
			{
				c_summoned = true;
				for(int i=1;i<7;++i)
				{
					Creature* pChanneler = m_creature->SummonCreature(98002,channeler_pos[i][0],channeler_pos[i][1],channeler_pos[i][2],channeler_pos[i][3],TEMPSUMMON_CORPSE_TIMED_DESPAWN,2000);
					if(pChanneler)
						channelers.push_back(pChanneler);
					++chans;
				}
			}

			if(c_summoned && channelers.empty())
				c_summoned = false;

			if(c_summoned)
			{
				if(check_timer < diff)
				{
				check_timer = 1000;
				if(chans <= 0)
				{
					m_creature->RemoveAurasDueToSpell(63894);
					m_creature->RemoveAurasDueToSpell(64022);
					m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,4.0f);
					m_creature->MonsterYell("Coditko vam rozkope drzku!",LANG_UNIVERSAL,NULL);
					m_creature->MonsterTextEmote("Bahamut Lord sbira vsechny sve sily!",NULL,true);
					m_creature->CastSpell(m_creature,50420,false);
					++phase;
					if(lastkiller && lastkiller->IsWithinDistInMap(m_creature,150.0f))
						m_creature->AI()->AttackStart(lastkiller);
				}
				} else check_timer -= diff;
			}
		}
		///////////////////// Faze 1
		if(phase != 0 && (!m_creature->getVictim() || !m_creature->SelectHostileTarget()))
		{
			m_creature->RemoveAllAuras();
			EnterEvadeMode();
			return;
		}

		if(phase == 1)
		{
			if(!c_gulnath)
			{
				gulnath = m_creature->SummonCreature(98003,-11221.870117f,-1493.421753f,3.594249f,1.534396f,TEMPSUMMON_DEAD_DESPAWN,0);
				c_gulnath = true;
			}

			if(spell1_timer < diff) //Bladestorm
			{
				m_creature->CastSpell(m_creature->getVictim(),65947,false);
				spell1_timer = 20000;
			} else spell1_timer -= diff;

			if(spell2_timer < diff) //Shell of Pain
			{
				m_creature->CastSpell(m_creature,41371,false);
				spell2_timer = 60000;
			} else spell2_timer -= diff;

			if(dejmudot_timer)
			{
				if(dejmudot_timer <= diff)
				{
					if(Unit* trg1 = SelectUnit(SELECT_TARGET_RANDOM,0))
						trg1->CastSpell(trg1,34114,true);
					if(Unit* trg2 = SelectUnit(SELECT_TARGET_RANDOM,0))
						trg2->CastSpell(trg2,34114,true);
					if(Unit* trg3 = SelectUnit(SELECT_TARGET_RANDOM,0))
						trg3->CastSpell(trg3,34114,true);
					dejmudot_timer = 0;
				} else dejmudot_timer -= diff;
			}

			if(summon_f2_timer < diff)
			{
				switch(f2_summonphase)
				{
				case 0:
				case 1:
					m_creature->SummonCreature(99941,summon_skels_pos[f2_summonphase][0],summon_skels_pos[f2_summonphase][1],summon_skels_pos[f2_summonphase][2],summon_skels_pos[f2_summonphase][3],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,60000);
					summon_f2_timer = 1000;
					++f2_summonphase;
					break;
				case 2:
				case 3:
					m_creature->SummonCreature(99942,summon_skels_pos[f2_summonphase-2][0],summon_skels_pos[f2_summonphase-2][1],summon_skels_pos[f2_summonphase-2][2],summon_skels_pos[f2_summonphase-2][3],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,60000);
					summon_f2_timer = 1000;
					++f2_summonphase;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					m_creature->SummonCreature(99943,summon_skels_pos[f2_summonphase%2][0],summon_skels_pos[f2_summonphase%2][1],summon_skels_pos[f2_summonphase%2][2],summon_skels_pos[f2_summonphase%2][3],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,60000);
					summon_f2_timer = 1000;
					++f2_summonphase;
					break;
				case 8:
					summon_f2_timer = 65000;
					f2_summonphase = 0;
					break;
				}
			} else summon_f2_timer -= diff;

			if(m_creature->GetHealth() <= m_creature->GetMaxHealth()/3*2 && !f2_banished)
				f2_banished = true;
		}

		///////////////////// Faze 2

		if(phase == 2)
		{
			if(f2_banished && summon_tydva_timer)
			{
				if(summon_tydva_timer <= diff)
				{
					switch(summon_tydva_phase)
					{
					case 1:
						m_creature->InterruptNonMeleeSpells(false);
						m_creature->GetMotionMaster()->MoveIdle();
						m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
						m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
						m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
						m_creature->RemoveAllAuras();
						m_creature->LoadCreaturesAddon();
						m_creature->GetMotionMaster()->Clear();
						m_creature->GetMotionMaster()->MoveIdle();
						m_creature->StopMoving();
						m_creature->CastSpell(m_creature, 36185, true);
						m_creature->CastSpell(m_creature, 65208, true);
						summon_tydva_timer = 6000;
						++summon_tydva_phase;
						break;
					case 2:
						bahagood = m_creature->SummonCreature(98004,0,0,0,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,60000);
						bahaevil = m_creature->SummonCreature(98005,0,0,0,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,60000);
						if(bahagood && bahaevil)
						{
							Unit* pRand1 = SelectUnit(SELECT_TARGET_RANDOM,0);
							Unit* pRand2 = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
							if(pRand1)
								bahagood->AI()->AttackStart(pRand1);
							if(pRand2)
								bahaevil->AI()->AttackStart(pRand2);
						}
						summon_tydva_timer = 0;
						break;
					case 3:
						m_creature->CastSpell(m_creature,60080,true);
						m_creature->SetDisplayId(25277);
						m_creature->LoadEquipment(0,true);
						summon_tydva_timer = 3000;
						++summon_tydva_phase;
						break;
					case 4:
						m_creature->MonsterYell("Bububu Bookyyy!",LANG_UNIVERSAL,0);
						m_creature->RemoveAllAuras();
						m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
						m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);
						m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, false);
						m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
						summon_tydva_timer = 0;
						spell1_timer = 8000;
						spell2_timer = 10000;
						spell3_timer = 2000;
						f2_banished = false;
						break;
					default:
						break;
					}
				} else summon_tydva_timer -= diff;
			}

			if(!f2_banished)
			{
				if(spell1_timer < diff)
				{
					if(shadowvlak_counter == 0)
					{
						m_creature->GetMotionMaster()->MoveIdle();
						m_creature->MonsterYell("Smazte se v pekle!",LANG_UNIVERSAL,0);
						++shadowvlak_counter;
						DoCastSpellIfCan(m_creature,52535,true);
					}
					if(shadowvlak_counter > 0 && shadowvlak_counter < 28)
					{
						++shadowvlak_counter;
						m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),38628,true);
						spell1_timer = 100;
					}
					if(shadowvlak_counter == 28)
					{
						if(m_creature->getVictim())
							m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
						spell1_timer = 40000;
						shadowvlak_counter = 0;
					}
				} else spell1_timer -= diff;

				if(spell2_timer < diff)
				{
					m_creature->CastSpell(m_creature->getVictim(),60094,true);
					spell2_timer = 16000;
				} else spell2_timer -= diff;

				if(spell3_timer <= diff)
				{
					m_creature->CastSpell(m_creature,52098,false);
					teleport_timer = 6000;
					spell3_timer = 80000;
				} else spell3_timer -= diff;

				if(teleport_timer)
				{
					if(teleport_timer <= diff)
					{
						uint32 cnt = rand()%5+5;
						outstring_log("boss_marathon_final: Teleporting %u players from threat list",cnt);
						ThreatList const &mthr = m_creature->getThreatManager().getThreatList();
						for(ThreatList::const_iterator itr = mthr.begin(); itr != mthr.end(); ++itr)
						{
							Unit* owner = (*itr)->getTarget();
							if( owner
								&& (m_creature->getVictim() != owner)
								&& (owner->GetTypeId() == TYPEID_PLAYER))
							{
								--cnt;
								outstring_log("Teleporting player %s",((Player*)owner)->GetName());
								((Player*)owner)->NearTeleportTo(-11204.016602f,-1809.158325f,-28.456039f,3.229851f);
								if(cnt <= 0)
									break;
							}
						}
						teleport_timer = 0;
					} else teleport_timer -= diff;
				}

				if(m_creature->GetHealth() <= m_creature->GetMaxHealth()/3 && !f3_banished)
				{
					phase = 3;
					f3_banished = true;
					transform_timer = 3000;
					transform_phase = 1;
					m_creature->InterruptNonMeleeSpells(false);
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
					m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
					m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					m_creature->RemoveAllAuras();
					m_creature->LoadCreaturesAddon();
					m_creature->GetMotionMaster()->Clear();
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->StopMoving();
					m_creature->CastSpell(m_creature, 36185, true);
					m_creature->CastSpell(m_creature, 65208, true);
					firewall_timer = 5000;
				}
			}

		}

		if(phase == 3)
		{
			if(f3_banished)
			{
				if(transform_timer <= diff)
				{
					switch(transform_phase)
					{
					case 1:
						m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,0.6f);
						transform_timer = 2000;
						++transform_phase;
						break;
					case 2:
						m_creature->CastSpell(m_creature,60080,true);
						m_creature->SetDisplayId(11380);
						transform_timer = 3000;
						++transform_phase;
						break;
					case 3:
						m_creature->CastSpell(m_creature,37846,false);
						transform_timer = 4000;
						++transform_phase;
						break;
					case 4:
						m_creature->MonsterYell("Hned po vas zabiju vaseho Kuniho krale!",LANG_UNIVERSAL,0);
						m_creature->RemoveAllAuras();
						m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
						m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);
						m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, false);
						m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
						transform_timer = 0;
						f3_banished = false;
						spell1_timer = 5000;
						spell2_timer = 3000;
						spell3_timer = 10000;
						silence_timer = 30000;
						firenova_timer = 8000;
						break;
					default:
						break;
					}
				} else transform_timer -= diff;
			}
			else
			{
				if(spell1_timer <= diff)
				{
					Unit* randomunit = m_creature(SELECT_TARGET_RANDOM,0);
					if(randomunit)
					{
						float x,y,z;
						randomunit->GetPosition(x,y,z);
						Creature* armag = m_creature->SummonCreature(99944,x,y,z-0.1f,0,TEMPSUMMON_TIMED_DESPAWN,5000);
						m_creature->CastSpell(randomunit,43440,true);
					}
					spell1_timer = 8000;
				} else spell1_timer -= diff;

				if(spell2_timer <= diff)
				{
					m_creature->CastSpell(m_creature->getVictim(),56908,false);
					spell2_timer = 8000;
				} else spell2_timer -= diff;

				if(spell3_timer <= diff)
				{
					if(!m_creature->IsNonMeleeSpellCasted(false))
					{
						m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),64016,false);
						spell3_timer = 10000;
					}
				} else spell3_timer -= diff;

				if(silence_timer <= diff)
				{
					if(!m_creature->IsNonMeleeSpellCasted(false))
					{
						m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),41168,false);
						silence_timer = 30000;
					}
				} else silence_timer -= diff;

				if(firenova_timer <= diff)
				{
					m_creature->CastSpell(m_creature->getVictim(),20203,true);
					firenova_timer = 15000;
				} else firenova_timer -= diff;

				if(firewall_timer <= diff)
				{
					NahodTamOhnivouStenu();
					firewall_timer = 10000;
				} else firewall_timer -= diff;
			}

			if(m_creature->GetHealth() <= 350000)
			{
				die_timer = 1000;
				die_phase = 1;
				++phase;
			}
		}

		if(phase == 4)
		{
			if(die_timer <= diff)
			{
				switch(die_phase)
				{
				case 1:
					m_creature->InterruptNonMeleeSpells(false);
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
					m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
					m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					m_creature->RemoveAllAuras();
					m_creature->LoadCreaturesAddon();
					m_creature->GetMotionMaster()->Clear();
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->StopMoving();
					m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,1.8f);
					m_creature->DeMorph();
					m_creature->CastSpell(m_creature,60080,true);
					m_creature->CastSpell(m_creature,65208,true);
					m_creature->MonsterYell("Aaarrrgghh.. jeste jsme neskoncili!",LANG_UNIVERSAL,0);
					m_creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
					die_timer = 3000;
					++die_phase;
					break;
				case 2:
					{
					m_creature->Relocate(-11223.236328f,-1405.015625f,-15.143601f,4.333558f);
					WorldPacket data;
					m_creature->BuildHeartBeatMsg(&data);
					m_creature->SendMessageToSet(&data,true);
					die_timer = 1000;
					++die_phase;
					}
					break;
				case 3:
					m_creature->CastSpell(m_creature,46928,true);
					m_creature->LoadEquipment(98001);
					die_timer = 2000;
					++die_phase;
					break;
				case 4:
					m_creature->MonsterYell("Feel my wrath!",LANG_COMMON,0);
					m_creature->CastSpell(m_creature,36455,true);
					m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
					m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,2.5f);
					die_timer = 3000;
					++die_phase;
					break;
				case 5:
					m_creature->CastSpell(m_creature,36206,true);
					m_creature->CastSpell(m_creature,50420,true);
					die_timer = 1000;
					++die_phase;
					break;
				case 6:
					m_creature->RemoveAllAuras();
					m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
					m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);
					m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, false);
					m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
					spell1_timer = 8000;
					spell2_timer = 1000;
					++phase;
					break;
				default:
					break;
				}
			} else die_timer -= diff;
		}

		if(phase == 5)
		{
			if(spell1_timer <= diff)
			{
				m_creature->CastSpell(m_creature,64443,false);
				spell1_timer = 20000;
			} else spell1_timer -= diff;

			if(spell2_timer <= diff)
			{
				if(!m_creature->IsNonMeleeSpellCasted(false))
					m_creature->CastSpell(m_creature->getVictim(),46553,false);
				spell2_timer = 1500;
			} else spell2_timer -= diff;
		}

		///////////////////// Konec
		if(phase != 0 && !m_creature->HasAura(36185))
			DoMeleeAttackIfReady();
	}
};

struct MANGOS_DLL_DECL npc_final_channelerAI: public ScriptedAI
{
	npc_final_channelerAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 drain_timer;

	void Reset()
	{
		m_creature->CastSpell(m_creature,54219,true);
		drain_timer = 3000;
	}

	void JustDied(Unit* pKiller)
	{
		Creature* boss = GetClosestCreatureWithEntry(m_creature,98001,200.0f);
		if(boss)
		{
			((boss_marathon_finalAI*)boss->AI())->KilledChanneler(pKiller);
		}
	}

	void UpdateAI(const uint32 diff)
	{
		if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
		{
			if(!m_creature->HasAura(54219))
				m_creature->CastSpell(m_creature,54219,true);
			return;
		}

		if(m_creature->HasAura(54219))
			m_creature->RemoveAurasDueToSpell(54219);

		if(drain_timer < diff)
		{
			m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),44294,false);
			drain_timer = 50000;
		} else drain_timer -= diff;

		if(drain_timer < diff+40000 && drain_timer > 30000)
		{
			m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),46153,false);
			drain_timer = 5000;
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_marathon_final(Creature* pCreature)
{
	return new boss_marathon_finalAI(pCreature);
}

CreatureAI* GetAI_npc_final_channeler(Creature* pCreature)
{
	return new npc_final_channelerAI(pCreature);
}

struct MANGOS_DLL_DECL npc_gulnathAI: public ScriptedAI
{
	npc_gulnathAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 phasetimer;
	uint32 phase;

	void Reset()
	{
		m_creature->SetSplineFlags(SPLINEFLAG_FORWARD);
		phase = 1;
		phasetimer = 1000;
	}

	void IncPhase(uint32 time)
	{
		phasetimer = time;
		++phase;
	}

	void MoveTo(uint32 id, float x, float y, float z)
	{
		m_creature->GetMotionMaster()->MovePoint(id,x,y,z);
	}

	void MovementInform(uint32 type,uint32 id)
	{
		switch(id)
		{
		case 1:
			phasetimer = 1000;
			break;
		case 2:
			phasetimer = 1000;
			break;
		}
	}

	void UpdateAI(const uint32 diff)
	{
		if(phasetimer < diff)
		{
			switch(phase)
			{
			case 1:
				MoveTo(1,-11222.273438f,-1456.422241f,2.560816f);
				IncPhase(60000);
				break;
			case 2:
				m_creature->MonsterYell("Hrdinove!",LANG_UNIVERSAL,NULL);
				IncPhase(3000);
				break;
			case 3:
				m_creature->MonsterYell("Nemuzete ho porazit! Ne bez krystalu, ktery ho oslabi",LANG_UNIVERSAL,NULL);
				IncPhase(4500);
				break;
			case 4:
				m_creature->MonsterSay("Jeden z vas musi jit, prohledat kryptu a prinest zpatky krystal, kterym ho oslabi",LANG_UNIVERSAL,NULL);
				IncPhase(5500);
				break;
			case 5:
				m_creature->MonsterYell("Rychle! Nemame moc casu!",LANG_UNIVERSAL,NULL);
				IncPhase(3000);
				break;
			case 6:
				//++phase;
				MoveTo(2,-11221.879883f,-1439.955933f,-1.400249f);
				IncPhase(60000);
				break;
			case 7:
				{
				Creature* boss = GetClosestCreatureWithEntry(m_creature,98001,200.0f);
				if(boss)
				{
					m_creature->LoadEquipment(98003,true);
					m_creature->AI()->AttackStart(boss);
				}
				IncPhase(2500);
				}
				break;
			case 8:
				if(m_creature->getVictim())
					m_creature->CastSpell(m_creature->getVictim(),17364,true);
				else
					m_creature->SetVisibility(VISIBILITY_OFF);
				phasetimer = 5000;
				break;
			}
		} else phasetimer -= diff;

		if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
		{
			if(phase > 6)
			{
				m_creature->SetVisibility(VISIBILITY_OFF);
				m_creature->DealDamage(m_creature,m_creature->GetHealth(),0,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,0,false);
			}
			return;
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_npc_gulnath(Creature* pCreature)
{
	return new npc_gulnathAI(pCreature);
}

struct MANGOS_DLL_DECL npc_final_skeletonAI: public ScriptedAI
{
	npc_final_skeletonAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 cast_timer;

	void Reset()
	{
		cast_timer = 3000;
	}

	void Aggro(Unit* pWho)
	{
		if(pWho && m_creature->GetEntry() == 99941)
			m_creature->CastSpell(pWho,24193,false);
	}

	void UpdateAI(const uint32 diff)
	{
		if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
			return;

		if(cast_timer < diff)
		{
			switch(m_creature->GetEntry())
			{
			case 99941:
				m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),28333,false);
				cast_timer = 10000;
				break;
			case 99942:
				m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),34356,false);
				cast_timer = 10000;
				break;
			case 99943:
				m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),46887,false);
				cast_timer = 5000;
				break;
			}
		} else cast_timer -= diff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_npc_final_skeleton(Creature* pCreature)
{
	return new npc_final_skeletonAI(pCreature);
}

struct MANGOS_DLL_DECL boss_bahamut_goodAI: public ScriptedAI
{
	boss_bahamut_goodAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 heal_timer;
	uint32 holynova_timer;

	void Reset()
	{
		m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,1.0f);
		heal_timer = 15000;
		holynova_timer = 7500;
	}

	void Aggro(Unit* pWho)
	{
		m_creature->CastSpell(m_creature,46565,true);
		m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,2.0f);
	}

	void JustDied(Unit* pKiller)
	{
		if(Creature* bahamain = GetClosestCreatureWithEntry(m_creature,98001,300.0f))
		{
			((boss_marathon_finalAI*)bahamain->AI())->KlonZemrel();
		}
	}

	void UpdateAI(const uint32 diff)
	{
		if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
			return;

		if(heal_timer < diff)
		{
			Unit* trget = NULL;
			trget = GetClosestCreatureWithEntry(m_creature,98005,100.0f);
			if(trget && m_creature->GetHealth() < trget->GetHealth())
				trget = m_creature;
			if(trget)
				m_creature->CastSpell(trget,36678,false);
			heal_timer = 8000;
		} else heal_timer -= diff;

		if(holynova_timer < diff)
		{
			if(!m_creature->IsNonMeleeSpellCasted(false))
			{
				m_creature->CastSpell(m_creature->getVictim(),66546,false);
				holynova_timer = 8000;
			}
		} else holynova_timer -= diff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_bahamut_good(Creature* pCreature)
{
	return new boss_bahamut_goodAI(pCreature);
}

struct MANGOS_DLL_DECL boss_bahamut_evilAI: public ScriptedAI
{
	boss_bahamut_evilAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 volley_timer;
	uint32 cleave_timer;

	void Reset()
	{
		m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,1.0f);
		volley_timer = 5000;
		cleave_timer = 8000;
	}

	void Aggro(Unit* pWho)
	{
		m_creature->CastSpell(m_creature,16592,true);
		m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X,2.0f);
	}

	void JustDied(Unit* pKiller)
	{
		if(Creature* bahamain = GetClosestCreatureWithEntry(m_creature,98001,300.0f))
		{
			((boss_marathon_finalAI*)bahamain->AI())->KlonZemrel();
		}
	}

	void UpdateAI(const uint32 diff)
	{
		if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
			return;

		if(volley_timer < diff)
		{
			m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),25586,false);
			volley_timer = 3000;
		} else volley_timer -= diff;

		if(cleave_timer < diff)
		{
			m_creature->CastSpell(m_creature->getVictim(),30495,false);
			cleave_timer = 2000;
		} else cleave_timer -= diff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_bahamut_evil(Creature* pCreature)
{
	return new boss_bahamut_evilAI(pCreature);
}

bool GOUse_bahamut_portzpet(Player* pPlayer, GameObject* pGO)
{
	pPlayer->NearTeleportTo(-11220.332031f,-1506.128174f,15.010457f,1.567004f);
	return true;
}

struct MANGOS_DLL_DECL boss_armageddon_bahaAI: public ScriptedAI
{
	boss_armageddon_bahaAI(Creature* c): ScriptedAI(c)
	{
		Reset();
	}

	uint32 drop_timer;

	void Reset()
	{
		drop_timer = 1500;

        m_creature->SetDisplayId(25206);
        m_creature->setFaction(14);
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	}

	void UpdateAI(const uint32 diff)
	{
		if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
			return;

		if(drop_timer < diff)
		{
			m_creature->CastSpell(m_creature->getVictim(),45909,false);
			drop_timer = 12000;
		} else drop_timer -= diff;

		if(drop_timer > 5000 && drop_timer < 10000)
		{
			m_creature->DealDamage(m_creature,m_creature->GetHealth(),0,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,0,false);
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_armageddon_baha(Creature* pCreature)
{
	return new boss_armageddon_bahaAI(pCreature);
}

bool ItemUse_bahamut_crystal(Player* pPlayer, Item* pItem, SpellCastTargets const& scTargets)
{
	if(!(scTargets.getUnitTarget()))
		return false;

	if(Creature* pBaha = GetClosestCreatureWithEntry(pPlayer,98001,100.0f))
	{
		pPlayer->CastSpell(pBaha,46960,false);
		pPlayer->DestroyItemCount(99904,1,true);
		return true;
	}

	return false;
}

bool GOUse_bahamut_crystal(Player* pPlayer, GameObject* pGO)
{
	if(Creature* boss = GetClosestCreatureWithEntry(pPlayer,98001,50000.0f))
	{
		if(!boss->isInCombat())
			return false;

		ItemPosCountVec dest;
	    uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 99904, 1, false);
		if (msg == EQUIP_ERR_OK)
		{
			pPlayer->SendNewItem(pPlayer->StoreNewItem(dest, 99904, 1, true),1,true,false,true);
			pPlayer->CastSpell(pPlayer,37846,true);
			return true;
	    }
		return false;
	}
	return false;
}

struct MANGOS_DLL_DECL npc_areachecker_bahamutAI: public ScriptedAI
{
	npc_areachecker_bahamutAI(Creature* pCreature): ScriptedAI(pCreature)
	{
		m_creature->SetVisibility(VISIBILITY_OFF);
		Reset();
	}

	void Reset()
	{
	}

	void MoveInLineOfSight(Unit* pWho)
	{
		if(pWho->GetTypeId() != TYPEID_PLAYER)
			return;

		//Kill pokud nema hotovy posledni q v serii
		Player* pPlayer = (Player*)pWho;
		if(pPlayer && (pPlayer->isAlive())
		   && (pPlayer->IsWithinDistInMap(m_creature,50.0f))
		   && !(pPlayer->GetQuestRewardStatus(91005)))
		{
			outstring_log("areachecker_bahamut: Hrac %s se pokusil jit k bossovi bez splneneho questu", pPlayer->GetName());
			pPlayer->DealDamage(pPlayer,pPlayer->GetHealth(),0,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,0,false);
		}

		//Teleport pokud ma hotovy || odevzdany quest na slain
		if(pPlayer && (pPlayer->isAlive())
		   && (pPlayer->IsWithinDistInMap(m_creature,50.0f))
		   && ((pPlayer->GetQuestRewardStatus(90057)) || pPlayer->GetQuestStatus(90057) == QUEST_STATUS_COMPLETE) )
		{
			outstring_log("areachecker_bahamut: Hrac %s se pokusil jit k bossovi s hotovym questem (nebo ho prave splnil)", pPlayer->GetName());
			pPlayer->TeleportTo(0,-11066.8f, -1828.45f, 60.2596f, 6.0f);
		}
	}

	void UpdateAI(const uint32 diff)
	{
	}
};

CreatureAI* GetAI_npc_areachecker_bahamut(Creature* pCreature)
{
	return new npc_areachecker_bahamutAI(pCreature);
}

void AddSC_boss_marathon()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "npc_barman_sc";
	newscript->GetAI = &GetAI_npc_barman_sc;
	newscript->pGossipHello = &GossipHello_npc_barman;
	newscript->pGossipSelect = &GossipSelect_npc_barman;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_pirat_tana";
	newscript->GetAI = &GetAI_npc_pirat_tana;
	newscript->pGossipHello = &GossipHello_npc_pirat_tana;
	newscript->pGossipSelect = &GossipSelect_npc_pirat_tana;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_aywes_deso";
	newscript->GetAI = &GetAI_npc_aywes_deso;
	newscript->pGossipHello = &GossipHello_npc_aywes_deso;
	newscript->pGossipSelect = &GossipSelect_npc_aywes_deso;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_drak_ws";
	newscript->GetAI = &GetAI_npc_drak_ws;
	newscript->pGossipHello = &GossipHello_npc_drak_ws;
	newscript->pGossipSelect = &GossipSelect_npc_drak_ws;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_drak_ws_drak";
	newscript->pGossipHello = &GossipHello_npc_drak_ws_drak;
	newscript->pGossipSelect = &GossipSelect_npc_drak_ws_drak;
	newscript->RegisterSelf();
	
	newscript = new Script;
	newscript->Name = "npc_shaman_alterac";
	newscript->GetAI = &GetAI_npc_shaman_alterac;
	newscript->pGossipHello = &GossipHello_npc_shaman_alterac;
	newscript->pGossipSelect = &GossipSelect_npc_shaman_alterac;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_andel_kara";
	newscript->pGossipHello = &GossipHello_npc_andel_kara;
	newscript->pGossipSelect = &GossipSelect_npc_andel_kara;
	newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "item_ohoz";
    newscript->pItemUse = &ItemUse_item_ohoz;
    newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "go_klec_jau";
	newscript->pGOUse = GOUse_go_klec_jau;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "boss_marathon_final";
	newscript->GetAI = &GetAI_boss_marathon_final;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "boss_bahamut_good";
	newscript->GetAI = &GetAI_boss_bahamut_good;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "boss_bahamut_evil";
	newscript->GetAI = &GetAI_boss_bahamut_evil;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_final_channeler";
	newscript->GetAI = &GetAI_npc_final_channeler;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_final_skeleton";
	newscript->GetAI = &GetAI_npc_final_skeleton;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_gulnath";
	newscript->GetAI = &GetAI_npc_gulnath;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "go_bahamut_portzpet";
	newscript->pGOUse = &GOUse_bahamut_portzpet;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "boss_armageddon_baha";
	newscript->GetAI = &GetAI_boss_armageddon_baha;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "item_bahamut_krystal";
    newscript->pItemUse = &ItemUse_bahamut_crystal;
    newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "go_svaty_krystal";
	newscript->pGOUse = &GOUse_bahamut_crystal;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_areachecker_bahamut";
	newscript->GetAI = &GetAI_npc_areachecker_bahamut;
	newscript->RegisterSelf();
}
