#include "precompiled.h"
long long int money;
int costo;

bool GossipHello_mob_teleguy(Player *player, Creature *_Creature)
{
    if ( player->GetTeam() == ALLIANCE ) {
        player->ADD_GOSSIP_ITEM( 5, "Darnassus. 5 Silver"		, GOSSIP_SENDER_MAIN, 1203);
        player->ADD_GOSSIP_ITEM( 5, "Exodar. 5 Silver"			, GOSSIP_SENDER_MAIN, 1216);
        player->ADD_GOSSIP_ITEM( 5, "Stormwind. 5 Silver"		, GOSSIP_SENDER_MAIN, 1206);
        player->ADD_GOSSIP_ITEM( 5, "Ironforge. 5 Silver"		, GOSSIP_SENDER_MAIN, 1224);
        player->ADD_GOSSIP_ITEM( 5, "Gnomeregan. 5 Silver"		, GOSSIP_SENDER_MAIN, 1222);
        player->ADD_GOSSIP_ITEM( 5, "Shattrath City. 5 Silver"            , GOSSIP_SENDER_MAIN, 1287);
        player->ADD_GOSSIP_ITEM( 5, "Dalaran. 5 Silver"            , GOSSIP_SENDER_MAIN, 1205);
        player->ADD_GOSSIP_ITEM( 5, "Isle Of Quel'Danas. 5 Silver"        , GOSSIP_SENDER_MAIN, 1288);
        player->ADD_GOSSIP_ITEM( 7, "[Instances] ->"			, GOSSIP_SENDER_MAIN, 5550);
        player->ADD_GOSSIP_ITEM( 7, "[Instances WotLK] ->"			, GOSSIP_SENDER_MAIN, 5554);
        }  else {
        player->ADD_GOSSIP_ITEM( 5, "Orgrimmar. 5 Silver"		, GOSSIP_SENDER_MAIN, 1215);
        player->ADD_GOSSIP_ITEM( 5, "Silvermoon. 5 Silver"		, GOSSIP_SENDER_MAIN, 1217);
        player->ADD_GOSSIP_ITEM( 5, "Undercity. 5 Silver"		, GOSSIP_SENDER_MAIN, 1213);
        player->ADD_GOSSIP_ITEM( 5, "Thunder Bluff. 5 Silver"	, GOSSIP_SENDER_MAIN, 1225);
        player->ADD_GOSSIP_ITEM( 5, "Gnomeregan. 5 Silver"		, GOSSIP_SENDER_MAIN, 1222);
        player->ADD_GOSSIP_ITEM( 5, "Shattrath City. 5 Silver"            , GOSSIP_SENDER_MAIN, 1287);
        player->ADD_GOSSIP_ITEM( 5, "Dalaran. 5 Silver"            , GOSSIP_SENDER_MAIN, 1205);
        player->ADD_GOSSIP_ITEM( 5, "Isle Of Quel'Danas. 5 Silver"        , GOSSIP_SENDER_MAIN, 1288);
        player->ADD_GOSSIP_ITEM( 7, "[Instances] ->"			, GOSSIP_SENDER_MAIN, 5550);
        player->ADD_GOSSIP_ITEM( 7, "[Instances WotLK] ->"			, GOSSIP_SENDER_MAIN, 5554);
        }
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
    return true;
}


void SendDefaultMenu_mob_teleguy(Player *player, Creature *_Creature, uint32 action )
{
    if(!player->getAttackers().empty())
    {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You are in combat!", LANG_UNIVERSAL);
        return;
    }

    if( player->getLevel() < 8  ) 
    {
         player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be lvl 8+", LANG_UNIVERSAL);
        return;
    }

    money = player-> GetMoney();
    costo = 500;

    if (money < costo ) 
    {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You haven't enough money", LANG_UNIVERSAL);
        return;
    }

    switch(action)
    {
        case 5550: //Instances
            player->ADD_GOSSIP_ITEM( 5, "Ragefire Chasm. 10 Silver"			 , GOSSIP_SENDER_MAIN, 1248);
            player->ADD_GOSSIP_ITEM( 5, "The Wailing Caverns. 10 Silver"     , GOSSIP_SENDER_MAIN, 1249);
            player->ADD_GOSSIP_ITEM( 5, "The Stockade. 10 Silver"			 , GOSSIP_SENDER_MAIN, 1253);
            player->ADD_GOSSIP_ITEM( 5, "Deadmines. 10 Silver"               , GOSSIP_SENDER_MAIN, 1250);
            player->ADD_GOSSIP_ITEM( 5, "Shadowfang Keep. 10 Silver"         , GOSSIP_SENDER_MAIN, 1251);
            player->ADD_GOSSIP_ITEM( 5, "Blackfathom Deeps. 10 Silver"       , GOSSIP_SENDER_MAIN, 1252);
            player->ADD_GOSSIP_ITEM( 5, "Razorfen Kraul. 20 Silver"          , GOSSIP_SENDER_MAIN, 1254);
            player->ADD_GOSSIP_ITEM( 5, "Razorfen Downs. 20 Silver"          , GOSSIP_SENDER_MAIN, 1256);
            player->ADD_GOSSIP_ITEM( 5, "Scarlet Monastery. 20 Silver"       , GOSSIP_SENDER_MAIN, 1257);
            player->ADD_GOSSIP_ITEM( 7, "[More] ->"							 , GOSSIP_SENDER_MAIN, 5551);
            player->ADD_GOSSIP_ITEM( 7, "<- [Main Menu]"					 , GOSSIP_SENDER_MAIN, 5552);	
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
            break;
        case 5551: //More Instances
            player->ADD_GOSSIP_ITEM( 5, "Uldaman. 30 Silver"                   , GOSSIP_SENDER_MAIN, 1258);
            player->ADD_GOSSIP_ITEM( 5, "Zul'Farrak. 30 Silver"                , GOSSIP_SENDER_MAIN, 1259);
            player->ADD_GOSSIP_ITEM( 5, "Maraudon. 40 Silver"                  , GOSSIP_SENDER_MAIN, 1260);
            player->ADD_GOSSIP_ITEM( 5, "Maraudon. 40 Silver"                  , GOSSIP_SENDER_MAIN, 1260);
            player->ADD_GOSSIP_ITEM( 5, "The Sunken Temple. 40 Silver"		   , GOSSIP_SENDER_MAIN, 1261);
            player->ADD_GOSSIP_ITEM( 5, "Blackrock Depths. 40 Silver"          , GOSSIP_SENDER_MAIN, 1262);
            player->ADD_GOSSIP_ITEM( 5, "Dire Maul. 50 Silver"                 , GOSSIP_SENDER_MAIN, 1263);
            player->ADD_GOSSIP_ITEM( 5, "Blackrock Spire. 50 Silver"           , GOSSIP_SENDER_MAIN, 1264);
            player->ADD_GOSSIP_ITEM( 5, "Stratholme. 50 Silver"                , GOSSIP_SENDER_MAIN, 1265);
            player->ADD_GOSSIP_ITEM( 5, "Scholomance. 50 Silver"               , GOSSIP_SENDER_MAIN, 1266);
            player->ADD_GOSSIP_ITEM( 7, "[More] ->"							   , GOSSIP_SENDER_MAIN, 5553);
            player->ADD_GOSSIP_ITEM( 7, "<- [Back]"							   , GOSSIP_SENDER_MAIN, 5550);
            player->ADD_GOSSIP_ITEM( 7, "<- [Main Menu]"					   , GOSSIP_SENDER_MAIN, 5552);	
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
            break;

case 5553: //Instances 60-70
player->ADD_GOSSIP_ITEM( 5, "Karazhan. 1 Gold"                              , GOSSIP_SENDER_MAIN, 4007);
player->ADD_GOSSIP_ITEM( 5, "Gruul's Lair. 1 Gold"                          , GOSSIP_SENDER_MAIN, 4008);
player->ADD_GOSSIP_ITEM( 5, "Hellfire Citadel. 1 Gold"                      , GOSSIP_SENDER_MAIN, 4009);
player->ADD_GOSSIP_ITEM( 5, "Coilfang Reservoir. 1 Gold"                    , GOSSIP_SENDER_MAIN, 4010);
player->ADD_GOSSIP_ITEM( 5, "Tempest Keep. 1 Gold"                          , GOSSIP_SENDER_MAIN, 4011);
player->ADD_GOSSIP_ITEM( 5, "Caverns of Time. 1 Gold"                       , GOSSIP_SENDER_MAIN, 4012);
player->ADD_GOSSIP_ITEM( 5, "Zul'Aman. 1 Gold"                              , GOSSIP_SENDER_MAIN, 4016);
player->ADD_GOSSIP_ITEM( 5, "Black Temple. 1 Gold"                          , GOSSIP_SENDER_MAIN, 4013);
player->ADD_GOSSIP_ITEM( 5, "Magister's Terrace. 2 Gold"                    , GOSSIP_SENDER_MAIN, 4017);
player->ADD_GOSSIP_ITEM( 5, "Sunwell Plateau. 2 Gold"                       , GOSSIP_SENDER_MAIN, 4018);
player->ADD_GOSSIP_ITEM( 7, "<- [Back]"							   , GOSSIP_SENDER_MAIN, 5550);
player->ADD_GOSSIP_ITEM( 7, "<- [Main Menu]"					   , GOSSIP_SENDER_MAIN, 5552);


player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

case 5554: //Instances 75-80 NORTHREND
player->ADD_GOSSIP_ITEM( 5, "Utgarde Keep. 20 Gold"                        , GOSSIP_SENDER_MAIN, 4019);
player->ADD_GOSSIP_ITEM( 5, "The Nexus. 20 Gold"                           , GOSSIP_SENDER_MAIN, 4020);
player->ADD_GOSSIP_ITEM( 5, "Azjol-Nerub. 20 Gold"                         , GOSSIP_SENDER_MAIN, 4021);
player->ADD_GOSSIP_ITEM( 5, "Ahn'kahet: The Old Kingdom. 20 Gold"          , GOSSIP_SENDER_MAIN, 4022);
player->ADD_GOSSIP_ITEM( 5, "Drak'Tharon Keep. 20 Gold"                    , GOSSIP_SENDER_MAIN, 4023);
player->ADD_GOSSIP_ITEM( 5, "The Violet Hold. 20 Gold"                     , GOSSIP_SENDER_MAIN, 4024);
player->ADD_GOSSIP_ITEM( 5, "Gun' Drak. 20 Gold"                           , GOSSIP_SENDER_MAIN, 4025);
player->ADD_GOSSIP_ITEM( 5, "Utgarde Pinnacle. 20 Gold"                    , GOSSIP_SENDER_MAIN, 4026);
player->ADD_GOSSIP_ITEM( 5, "Ulduar. 20 Gold"                              , GOSSIP_SENDER_MAIN, 4027);
player->ADD_GOSSIP_ITEM( 5, "The Obsidian Sanctum. 20 Gold"                , GOSSIP_SENDER_MAIN, 4028);
player->ADD_GOSSIP_ITEM( 5, "Naxxramas. 20 Gold"                           , GOSSIP_SENDER_MAIN, 4029);
player->ADD_GOSSIP_ITEM( 7, "<- [Back]"							   , GOSSIP_SENDER_MAIN, 5550);
player->ADD_GOSSIP_ITEM( 7, "<- [Main Menu]"					   , GOSSIP_SENDER_MAIN, 5552);


player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

case 5552: //Back To Main Menu
    if ( player->GetTeam() == ALLIANCE ) {
player->ADD_GOSSIP_ITEM( 5, "Darnassus. 5 Silver"			, GOSSIP_SENDER_MAIN, 1203);
player->ADD_GOSSIP_ITEM( 5, "Exodar. 5 Silver"				, GOSSIP_SENDER_MAIN, 1216);
player->ADD_GOSSIP_ITEM( 5, "Stormwind. 5 Silver"			, GOSSIP_SENDER_MAIN, 1206);
player->ADD_GOSSIP_ITEM( 5, "Ironforge. 5 Silver"			, GOSSIP_SENDER_MAIN, 1224);
player->ADD_GOSSIP_ITEM( 5, "Gnomeregan. 5 Silver"			, GOSSIP_SENDER_MAIN, 1222);
player->ADD_GOSSIP_ITEM( 5, "Shattrath City. 5 Silver"            , GOSSIP_SENDER_MAIN, 1287);
player->ADD_GOSSIP_ITEM( 5, "Dalaran. 5 Silver"            , GOSSIP_SENDER_MAIN, 1205);
player->ADD_GOSSIP_ITEM( 5, "Isle Of Quel'Danas. 5 Silver"        , GOSSIP_SENDER_MAIN, 1288);
player->ADD_GOSSIP_ITEM( 7, "[Instances] ->"				, GOSSIP_SENDER_MAIN, 5550);
player->ADD_GOSSIP_ITEM( 7, "[Instances WotLK] ->"			, GOSSIP_SENDER_MAIN, 5554);

	}  else {


player->ADD_GOSSIP_ITEM( 5, "Orgrimmar. 5 Silver"			, GOSSIP_SENDER_MAIN, 1215);
player->ADD_GOSSIP_ITEM( 5, "Silvermoon. 5 Silver"			, GOSSIP_SENDER_MAIN, 1217);
player->ADD_GOSSIP_ITEM( 5, "Undercity. 5 Silver"			, GOSSIP_SENDER_MAIN, 1213);
player->ADD_GOSSIP_ITEM( 5, "Thunder Bluff. 5 Silver"		, GOSSIP_SENDER_MAIN, 1225);
player->ADD_GOSSIP_ITEM( 5, "Gnomeregan. 5 Silver"			, GOSSIP_SENDER_MAIN, 1222);
player->ADD_GOSSIP_ITEM( 5, "Shattrath City. 5 Silver"            , GOSSIP_SENDER_MAIN, 1287);
player->ADD_GOSSIP_ITEM( 5, "Dalaran. 5 Silver"            , GOSSIP_SENDER_MAIN, 1205);
player->ADD_GOSSIP_ITEM( 5, "Isle Of Quel'Danas. 5 Silver"        , GOSSIP_SENDER_MAIN, 1288);
player->ADD_GOSSIP_ITEM( 7, "[Instances] ->"				, GOSSIP_SENDER_MAIN, 5550);
player->ADD_GOSSIP_ITEM( 7, "[Instances WotLK] ->"			, GOSSIP_SENDER_MAIN, 5554);
	}

player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

case 1203: // Teleport to Darnassus
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(1, 9947.52f, 2482.73f, 1316.21f, 0.0f);
player->ModifyMoney(-1*costo);
break;

// Teleport to Stormwind
case 1206:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(0, -8960.14f, 516.266f, 96.3568f, 0.0f);
player->ModifyMoney(-1*costo);
break;

// Teleport to Dalaran
case 1205:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(571, 5804.14f, 624.770f, 647.7670f, 1.64f);
player->ModifyMoney(-1*costo);
break;

// Teleport to Undercity
case 1213:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(0, 1819.71f, 238.79f, 60.5321f, 0.0f);
player->ModifyMoney(-1*costo);

break;

// Teleport to Orgrimmar
case 1215:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(1, 1552.5f, -4420.66f, 8.94802f, 0.0f);
player->ModifyMoney(-1*costo);
break;

// Teleport to Exodar
case 1216:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(530, -4073.03f, -12020.4f, -1.47f, 0.0f);
player->ModifyMoney(-1*costo);
break;

// Teleport to Silvermoon
case 1217:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(530, 9338.74f, -7277.27f, 13.7895f, 0.0f);
player->ModifyMoney(-1*costo);

break;

case 1222://teleport player to Gnomeregan
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(0, -5163.43f,660.40f,348.28f,4.65f);
player->ModifyMoney(-1*costo);

break;

// Teleport to Ironforge
case 1224:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(0, -4924.07f, -951.95f, 501.55f, 5.40f);
player->ModifyMoney(-1*costo);

break;

// Teleport to Thunder Bluff
case 1225:
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(1, -1280.19f,127.21f,131.35f,5.16f); 
player->ModifyMoney(-1*costo);

break;

case 1248://teleport player to Ragefire Chasm

if( player->getLevel() >= 8)
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, 1800.53f,-4394.68f,-17.93f,5.49f);
		player->ModifyMoney(-2*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 8!", LANG_UNIVERSAL);
	}
	
break;

case 1249://teleport player to the Wailing Caverns
		
if (player->getLevel() >= 10)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, -722.53f,-2226.30f,16.94f,2.71f);
		player->ModifyMoney(-2*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 10!", LANG_UNIVERSAL);
	}
	
break;

case 1250://teleport player to the Deadmines
		
if (player->getLevel() >= 10)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -11212.04f,1658.58f,25.67f,1.45f);
		player->ModifyMoney(-2*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 10!", LANG_UNIVERSAL);
	}
	
break;

case 1251://teleport player to Shadowfang Keep
		
	if (player->getLevel() >= 15)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -254.47f,1524.68f,76.89f,1.56f);
		player->ModifyMoney(-2*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 15!", LANG_UNIVERSAL);
	}
	
break;

case 1252://teleport player to Blackfathom Deeps
		
	if (player->getLevel() >= 15)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, 4254.58f,664.74f,-29.04f,1.97f);
		player->ModifyMoney(-2*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 15!", LANG_UNIVERSAL);
	}
	
break;

case 1253://teleport player to the Stockade
		
	if (player->getLevel() >= 20)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -8769.76f,813.08f,97.63f,2.26f);
		player->ModifyMoney(-2*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 20!", LANG_UNIVERSAL);
	}
	
break;

case 1254://teleport player to Razorfen Kraul
		
	if (player->getLevel() >= 24)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, -4484.04f,-1739.40f,86.47f,1.23f);
		player->ModifyMoney(-4*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 24!", LANG_UNIVERSAL);
	}
	
break;

case 1255://teleport player to Gnomeregan
		
	if (player->getLevel() >= 20)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -5162.62f,667.81f,248.05f,1.48f);
		player->ModifyMoney(-4*costo);
		
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 20!", LANG_UNIVERSAL);
	}
	
break;

case 1256://teleport player to Razorfen Downs
		
	if (player->getLevel() >= 25)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, -4645.08f,-2470.85f,85.53f,4.39f);
		player->ModifyMoney(-4*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 25!", LANG_UNIVERSAL);
	}
	
break;

case 1257://teleport player to the Scarlet Monastery
		
	if (player->getLevel() >= 25)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, 2843.89f,-693.74f,139.32f,5.11f);
		player->ModifyMoney(-4*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 25!", LANG_UNIVERSAL);
	}
	
break;

case 1258://teleport player to Uldaman
		
	if (player->getLevel() >= 35)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -6119.70f,-2957.30f,204.11f,0.03f);
		player->ModifyMoney(-6*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 35!", LANG_UNIVERSAL);
	}
	
break;

case 1259://teleport player to Zul'Farrak
		
	if (player->getLevel() >= 35)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, -6839.39f,-2911.03f,8.87f,0.41f);
		player->ModifyMoney(-6*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 35!", LANG_UNIVERSAL);
	}
	
break;


case 1260://teleport player to Maraudon
		
	if (player->getLevel() >= 40)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, -1433.33f,2955.34f,96.21f,4.82f);
		player->ModifyMoney(-8*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 40!", LANG_UNIVERSAL);
	}
	
break;

case 1261://teleport player to the Sunken Temple
		
	if (player->getLevel() >= 45)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -10346.92f,-3851.90f,-43.41f,6.09f);
		player->ModifyMoney(-8*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 45!", LANG_UNIVERSAL);
	}
	
break;

case 1262://teleport player to Blackrock Depths
		
	if (player->getLevel() >= 45)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -7301.03f,-913.19f,165.37f,0.08f);
		player->ModifyMoney(-8*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 45!", LANG_UNIVERSAL);
	}
	
break;
	
case 1263://teleport player to Dire Maul
		
	if (player->getLevel() >= 50)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, -3982.47f,1127.79f,161.02f,0.05f);
		player->ModifyMoney(-10*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 50!", LANG_UNIVERSAL);
	}
	
break;

case 1264://teleport player to Blackrock Spire
		
	if (player->getLevel() >= 50)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -7535.43f,-1212.04f,285.45f,5.29f);
		player->ModifyMoney(-10*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 50!", LANG_UNIVERSAL);
	}
	
break;

case 1265://teleport player to Stratholme
		
	if (player->getLevel() >= 50)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, 3263.54f,-3379.46f,143.59f,0.00f);
		player->ModifyMoney(-10*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 50!", LANG_UNIVERSAL);
	}
	
break;

case 1266://teleport player to Scholomance
		
	if (player->getLevel() >= 50)
		
	{
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, 1219.01f,-2604.66f,85.61f,0.50f);
		player->ModifyMoney(-10*costo);
	} else {
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("You must be at least level 50!", LANG_UNIVERSAL);
	}
	
break;

case 1287:// Shattrath City

if( player->getLevel() >= 58)
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, -1850.209961f, 5435.821777f, -10.961435f, 3.403913f);
        player->ModifyMoney(-1*costo);
 } else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 58!", LANG_UNIVERSAL);
    }
    
break;

case 1288://teleport player to Isle Of Quel'Danas

    if (player->getLevel() >= 65)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, 12947.4f,-6893.31f,5.68398f,3.09154f);
        player->ModifyMoney(-1*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 65!", LANG_UNIVERSAL);
    }
    
break;

case 4007:// Karazhan

    if (player->getLevel() >= 70)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(0, -11118.8f, -2010.84f, 47.0807f, 0.0f);
        player->ModifyMoney(-20*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 70!", LANG_UNIVERSAL);
    }
    
break;

case 4008:// Gruul's Lair

    if (player->getLevel() >= 65)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, 3539.007568f, 5082.357910f, 1.691071f, 0.0f);
        player->ModifyMoney(-20*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 65!", LANG_UNIVERSAL);
    }
    
break;

case 4009:// Hellfire Citadel
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(530, -305.816223f, 3056.401611f, -2.473183f, 2.01f);
player->ModifyMoney(-20*costo);
break;

case 4010:// Coilfang Reservoir
player->CLOSE_GOSSIP_MENU();
player->TeleportTo(530, 517.288025f, 6976.279785f, 32.007198f, 0.0f);
player->ModifyMoney(-20*costo);
break;

case 4011:// Tempest Keep

    if (player->getLevel() >= 70)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, 3089.579346f, 1399.046509f, 187.653458f, 4.794070f);
        player->ModifyMoney(-20*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 70!", LANG_UNIVERSAL);
    }
    
break;

case 4012:// Caverns of Time

    if (player->getLevel() >= 66)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(1, -8173.66f, -4746.36f, 33.8423f, 4.93989f);
        player->ModifyMoney(-20*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 66!", LANG_UNIVERSAL);
    }
    
break;

case 4016:// Zul'Aman

    if (player->getLevel() >= 70)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, 6846.95f, -7954.5f, 170.028f, 4.61501f);
        player->ModifyMoney(-20*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 70!", LANG_UNIVERSAL);
    }
    
break;

case 4013:// Black Temple

    if (player->getLevel() >= 70)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, -3610.719482f, 324.987579f, 37.400028f, 3.282981f);
        player->ModifyMoney(-20*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 70!", LANG_UNIVERSAL);
    }
    
break;

case 4017:// magistrate

    if (player->getLevel() >= 70)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, 12884.6f, -7317.69f, 65.5023f, 4.799f);
        player->ModifyMoney(-40*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 70!", LANG_UNIVERSAL);
    }
    
break;

case 4018:// sunwell

    if (player->getLevel() >= 70)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(530, 12574.1f, -6774.81f, 15.0904f, 3.13788f);
        player->ModifyMoney(-40*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 70!", LANG_UNIVERSAL);
    }
    
break;

case 4019:// Utgarde Keep

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 1219.720f, -4865.28f, 41.25f, 0.31f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4020:// The Nexus

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 3776.950f, 6953.80f, 105.05f, 0.345f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4021:// Azjol-Nerub

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 3675.430f, 2169.00f, 35.90f, 2.29f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4022:// Ahn'kahet: The Old Kingdom

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 3646.760f, 2045.17f, 1.79f, 4.37f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4023:// Drak'Tharon Keep

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 4450.860f, -2045.25f, 162.83f, 0.00f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4024:// The Violet Hold

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 5679.820f, 486.80f, 652.40f, 4.08f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4025:// Gun' Drak

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 6937.540f, -4455.98f, 450.68f, 1.00f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4026:// Utgarde Pinnacle

    if (player->getLevel() >= 80)
        
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 1245.690f, -4856.59f, 216.86f, 3.45f);
        player->ModifyMoney(-400*costo);
	} else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

case 4027:// Ulduar

    if (player->getLevel() >= 80)
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 8976.240f, -1281.33f, 1059.01f, 0.58f);
        player->ModifyMoney(-400*costo);
        } else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
break;

case 4028:// The Obsidian Sanctum

    if (player->getLevel() >= 80)
    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 3625.780f, 280.40f, -120.14f, 3.25f);
        player->ModifyMoney(-400*costo);
        } else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
break;

case 4029:// Naxxramas

    if (player->getLevel() >= 80)

    {
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(571, 3668.719f, -1262.460f, 243.63f, 5.03f);
        player->ModifyMoney(-400*costo);
        } else {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay("You must be at least level 80!", LANG_UNIVERSAL);
    }
    
break;

}


}

bool GossipSelect_mob_teleguy(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    // Main menu
    if (sender == GOSSIP_SENDER_MAIN)
    {
        player->PlayerTalkClass->ClearMenus();
        SendDefaultMenu_mob_teleguy(player, _Creature, action   );
    }
    return true;
}

void AddSC_mob_teleguy()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_teleguy";
    newscript->pGossipHello = &GossipHello_mob_teleguy;
    newscript->pGossipSelect = &GossipSelect_mob_teleguy;
    newscript->RegisterSelf();
}
