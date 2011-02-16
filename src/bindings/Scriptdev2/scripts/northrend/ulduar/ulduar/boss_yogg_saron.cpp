/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_yogg_saron
SD%Complete: 
SDComment: Implement sanity, some other spells need core support. If the topaggro target is teleported into a vision, the boss resets -> needs fixing!
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

enum
{
    //yells
    //sara
    SAY_INTRO1                  = -1603307,
    SAY_INTRO2                  = -1603308,
    SAY_AGGRO                   = -1603300,
    SAY_HELP1                   = -1603301,
    SAY_HELP2                   = -1603302,
    SAY_SPECIAL1                = -1603305,
    SAY_SPECIAL2                = -1603306,
    SAY_KILL1                   = -1603303,
    SAY_KILL2                   = -1603304,
    SAY_WIPE                    = -1603309,

    //yogg
    SAY_PHASE2                  = -1603321,
    SAY_VISION                  = -1603322,
    EMOTE_PORTALS               = -1603372,
    EMOTE_SHATTER               = -1603373,
    SAY_PHASE3                  = -1603323,
    SAY_SLAY1                   = -1603324,
    SAY_SLAY2                   = -1603325,
    SAY_DEATH                   = -1603326,
    SAY_INSANITY1               = -1603327,
    SAY_INSANITY2               = -1603328,

    //visions
    //stormwind
    SAY_GARONA1                 = -1603341,
    SAY_GARONA2                 = -1603342,
    SAY_YOGG_V1_1               = -1603343,
    SAY_YOGG_V1_2               = -1603344,
    SAY_GARONA3                 = -1603345,
    SAY_GARONA4                 = -1603346,
    SAY_YOGG_V1_3               = -1603347,

    //dragons
    SAY_NELTHARION1             = -1603336,
    SAY_YSERA                   = -1603337,
    SAY_NELTHARION2             = -1603338,
    SAY_MALYGOS                 = -1603339,
    SAY_YOGG_V2                 = -1603340,

    //lich king
    SAY_LICH_KING1              = -1603330,
    SAY_CHAMPION1               = -1603331,
    SAY_CHAMPION2               = -1603332,
    SAY_LICH_KING2              = -1603333,
    SAY_YOGG_V3_1               = -1603334,
    SAY_YOGG_V3_2               = -1603335,

    //keepers yells
    SAY_HODIR_ACTIVE            = -1603086,
    SAY_FREYA_ACTIVE            = -1603009,
    SAY_THORIM_ACTIVE           = -1603238,
    SAY_MIMIRON_ACTIVE          = -1603260,

    //vision npc
    //stormwind
    NPC_GARONA                  = 33436,
    NPC_KING_LLANE              = 33437,

    MOB_SUIT_OF_ARMOR           = 33433,

    //dragons
    NPC_NELTHARION              = 33523,
    NPC_YSERA                   = 33495,
    NPC_MALYGOS                 = 33535,
    NPC_ALEXSTRASZA             = 33536,
    GO_DRAGON_SOUL              = 194462,

    MOB_RUBY_CONSORT            = 33716,
    MOB_AZURE_CONSORT           = 33717,
    MOB_BRONZE_CONSORT          = 33718,
    MOB_EMERALD_CONSORT         = 33719,
    MOB_OBSIDIAN_CONSORT        = 33720,


    //lich king
    NPC_LICH_KING               = 33441,
    NPC_IMMOLATED_CHAMPION      = 33442,

    NPC_VOICE_OF_YOGG_SARON     = 33280,
    MOB_VISION_TENTACLE         = 33943,
    SPELL_GRIM_REPRISAL         = 63305,
    // npc hp: 15k on 10 man; 40k on 25 man
    // npc no: 10

    //spells
    //phase1
    SPELL_SARAS_FERVOR          = 63138,
    SPELL_SARAS_BLESSING        = 63134,
    SPELL_SARAS_ANGER           = 63147,

    MOB_GUARDIAN_OF_YOGG        = 33136,
    SPELL_DARK_VOLLEY           = 63038,
    SPELL_SHADOW_NOVA           = 62714,    //when dies
    SPELL_SHADOW_NOVA_H         = 65209,
    SPELL_DOMINATE_MIND         = 63713,

    // clouds
    SPELL_OMINOUS_CLOUD_VISUAL  = 63084,
    SPELL_SUMMON_GUARDIAN       = 62978,
    SPELL_SUMMON_GUARDIAN2      = 63031,
    NPC_OMINOUS_CLOUD           = 33292,

    //phase2
    SPELL_SHADOWY_BARRIER       = 64775,
    SPELL_SHADOWY_BARRIER_YOGG  = 63894,
    SPELL_SANITY                = 63050,
    SPELL_PHYCHOSIS             = 65301,
    SPELL_PHYCHOSIS_H           = 63795,
    SPELL_MALADY_OF_THE_MIND    = 63830,
    SPELL_BRAIN_LINK            = 63802,
    SPELL_BRAIN_LINK_DMG        = 63803,
    SPELL_BRAIN_LINK_NON_DMG    = 63804,
    SPELL_DEATH_RAY             = 63891,    //summons orb
    SPELL_DEATH_RAY_TRIGG       = 63883,
    SPELL_DEATH_RAY_VISUAL      = 63886,
    SPELL_DEATH_RAY_VISUAL_ORI  = 63893,
    MOB_DEATH_ORB               = 33882,

    MODEL_SARA_VALKYR           = 29182,
    MODEL_SARA_HUMAN            = 29117,

    // sanity
    SPELL_CLEAR_INSANE          = 63122,
    SPELL_INSANE                = 63120,
    SPELL_INSANE_VISUAL         = 64464,
    SPELL_INSANE_TRIGG          = 64554,

    SPELL_LOW_SANITY_EFFECT     = 63752,
    SPELL_SANITY_TRIGG          = 63786,

    // portals
    MOB_DESCEND_INTO_MADNESS    = 34072,
    SPELL_LUNATIC_GAZE          = 64167,    //affects players which take the portal to madness
    NPC_LAUGHING_SKULL          = 33990,
    SKULL_DISPLAY_ID            = 25206,

    // brain's chamber
    MOB_BRAIN_OF_YOGG_SARON     = 33890,
    SPELL_SHATTERED_ILLUSION    = 64173,
    SPELL_INDUCE_MADNESS        = 64059,
    SPELL_ILLUSION_ROOM         = 63988,    // reduce speed
	SPELL_ILLUSION_CANCEL		= 63993,


    // tentacules
    MOB_CRUSHER_TENTACLE        = 33966,
    SPELL_ERUPT                 = 64144,    //also used by the corruptor tentacle
    SPELL_DIMINISH_POWER        = 64145,
    SPELL_FOCUSED_ANGER         = 57689,
    SPELL_FOCUSED_ANGER_TRIGG   = 57688,
    SPELL_SUMMON_CRUSHER        = 64139,

    MOB_CONSTRICTOR_TENTACLE    = 33983,
    SPELL_SQUEEZE               = 64125,
    SPELL_SQUEEZE_H             = 64126,
    SPELL_SUMMON_CONSTRICTOR    = 64133,

    MOB_CORRUPTOR_TENTACLE      = 33985,
    SPELL_APATHY                = 64156,
    SPELL_BLACK_PLAGUE          = 64153,
    SPELL_CURSE_OF_DOOM         = 64157,
    SPELL_DRAINING_POISON       = 64152,

    // phase 3
    SPELL_LUNATIC_GAZE_YOGG     = 64163,
    SPELL_SHADOW_BEACON         = 64465,
    SPELL_EMPOWERING_SHADOWS    = 64468,
    SPELL_EMPOWERING_SHADOWS_H  = 64486,
    SPELL_DEAFENING_ROAR        = 64189,    //only cast on 25 player with 0-3 keepes active

    MOB_IMMORTAL_GUARDIAN       = 33988,
    SPELL_EMPOWERED             = 65294,    //starts with 9 stacks and loses 1 stak at 10% hp
	SPELL_EMPOWERED_AURA		= 64161,

    SPELL_BERSERK               = 64166,    //extinguish all life

    // keepers
    // freya
    SPELL_RESILIENCE_OF_NATURE  = 62670,
    MOB_SANITY_WELL             = 33991,
    SPELL_SANITY_WELL           = 64169,    // regen sanity
    SPELL_SANITY_WELL_VISUAL    = 63288,
    SPELL_SUMMON_SANITY_WELL    = 64170,

    // hodir
    SPELL_FORTITUDE_OF_FROST    = 62650,
    SPELL_HODIRS_PROTECTIVE_GAZE= 64174,    // saves players from killing blows ~25secs cd

    // thorim
    SPELL_SPEED_OF_INVENTION    = 62671,
    SPELL_DESTABILIZATION_MATRIX= 65210,    // cast in phase 2 on the tentacules

    // mimiron
    SPELL_FURY_OF_THE_STORM     = 62702,
    SPELL_TITANIC_STORM         = 64171,    // used in phase 3 to kill guardians
    SPELL_TITANIC_STORM_DMG     = 64172,
};

enum phases
{
    // yogg phases
    PHASE_IDLE                  = 0,
    PHASE_SARA                  = 1,
    PHASE_TENTACLES             = 2,
    PHASE_OLD_GOD               = 3,
    // vision phases
    PHASE_VISION_STORMWIND      = 1,
    PHASE_VISION_WYRMREST       = 2,
    PHASE_VISION_ICECROWN       = 3,
    PHASE_VISION_RETURN         = 4,    // used to set the portals to return to main chamber = idle
};

enum achievs
{
    ACHIEV_ALONE                = 3159,
    ACHIEV_ALONE_H              = 3164,
    ACHIEV_ONE_LIGHT            = 3158,
    ACHIEV_ONE_LIGHT_H          = 3163,
    ACHIEV_TWO_LIGHTS           = 3141,
    ACHIEV_TWO_LIGHTS_H         = 3162,
    ACHIEV_THREE_LIGHTS         = 3157,
    ACHIEV_THREE_LIGHTS_H       = 3161,
    ACHIEV_NOT_GETTING_OLDER    = 3012,
    ACHIEV_NOT_GETTING_OLDER_H  = 3013,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {1951.097412f,-25.42042f, 326.162598f},
    {1970.677490f,-0.211162f, 325.478638f},
    {2001.049805f,-4.201718f, 325.751831f},
    {2009.346924f,-26.001806f,325.603271f},
    {1998.081665f,-46.625187f,325.551605f}, 
    {1962.782715f,-51.363148f,325.458160f},
    {1988.847778f,-71.143738f,328.647614f},
    {2024.004150f,-40.749989f,327.876617f},
    {2018.806885f,-3.451158f,327.593933f},
    {1978.651001f,18.373478f,328.420532f},
    {1940.641602f,1.761525f, 327.921661f},
    {1941.203735f,-52.999535f,327.246948f},
};

static LocationsXY SanityWellLoc[]=
{
    {1901.237915f, -46.305782f, 331.960754f}, 
    {1900.753052f,  -2.259287f, 332.061249f}, 
    {1991.020996f,  43.943943f, 331.746979f},
    {2044.219482f, -21.878244f, 329.776855f},
    {1986.226807f, -95.087761f, 330.253998f},
};

static LocationsXY KeepersLoc[]=
{
    {2036.859863f,  -74.113884f, 338.415222f, 2.488684f}, //thorim
    {1938.328247f,  -90.742043f, 338.459442f, 0.992500f}, //hodir
    {2036.107056f,  25.702380f,  338.415192f, 4.019527f}, // freya
    {1939.021240f,  43.221031f,  338.460663f, 5.214388f}, // mimiron
};

static LocationsXY TeleportLoc[]=
{
    {1941.587402f, 43.526680f,  239.666336f},    // stormwind
    {2055.460938f, -25.619570f, 239.721176f},    // dragons
    {1941.131226f, -94.654694f, 239.989639f},    // icecrown
    {1951.097f, -25.420f, 326.162f},             // yogg
};

static LocationsXY YoggPortalLoc[]=
{
    {1959.765137f, -20.697853f, 325.352966f},   
    {1973.407837f,  -6.656567f, 324.889526f},  
    {1990.271851f, -47.992981f, 324.959991f}, 
    {1978.893433f, -49.858326f, 324.777618f},
    {1967.471924f, -46.298458f, 324.840759f},
    {1959.654297f, -39.954502f, 324.997253f},
    {1956.343872f, -25.642859f, 325.188354f},
    {1985.072021f,  -2.515451f, 325.20010f},
    {1993.541626f, -10.527716f, 324.889587f}, 
    {2003.674316f, -23.050785f, 325.384064f}, 
};

static LocationsXY MadnessPortalLoc[]=
{
    {2001.015f,   4.185f, 242.747f},    
    {1999.690f, -54.931f, 242.418f},    
    {1946.898f, -25.769f, 242.169f},    
};
// vison pos
//Positional defines 
struct VisionLocXY
{
    float x, y, z, o;
    uint32 id;
};
// dragons
const float PosYsera[4] = {2114.504f, -16.118f, 242.646f, 3.91f};
const float PosMalygos[4] = {2113.388f, -34.381f, 242.646f, 2.26f};
const float PosNeltharion[4] = {2117.588f, -25.318f,  242.646f, 3.15f};
const float PosAlexstrasza[4] = {2091.679f, -25.289f, 242.646f, 6.282f};
const float PosVoiceDragon[3] = {2104.555f, -25.635f, 242.646f};
const float DisplayDragons[10] = {2718, 2718, 2717, 2717, 12869, 12869, 1687, 1687, 2719, 2719};

static VisionLocXY DragonLoc[]=
{
    {2071.951660f,   1.881840f, 239.794922f, 5.590341f},
    {2093.910156f,  19.939915f, 239.766830f, 4.962034f},
    {2114.737061f,  20.441664f, 239.757309f, 4.259104f},
    {2136.709473f,   4.874056f, 239.718658f, 3.889961f},
    {2147.894287f, -12.416141f, 239.757980f, 3.210588f},
    {2148.479004f, -37.655373f, 239.720169f, 3.033874f},
    {2136.296631f, -56.800838f, 239.754654f, 2.287745f},
    {2114.370117f, -68.110947f, 239.721100f, 1.789017f},
    {2093.946289f, -67.447899f, 239.720734f, 1.305995f},
    {2071.001709f, -54.414040f, 239.719345f, 0.528450f},
};

static VisionLocXY SkullDragonLoc[]=
{
    {2075.898193f, -5.637041f,  239.787735f},
    {2137.949219f, -26.778023f, 239.717712f},
    {2084.131836f, -52.716999f, 239.720703f},
};

// stormwind
const float PosGarona[4] = {1935.398926f, 54.017738f, 240.376465f, 2.008213f};
const float PosKing[4] = {1930.465670f, 62.674065f, 242.376373f, 5.196925f};
const float PosVoiceStormwind[3] = {1927.326f, 68.120f, 242.376f}; 

static VisionLocXY KeepLoc[]=
{
    {1930.854370f, 39.910034f, 239.666443f, 1.641476f},
    {1909.771240f, 45.685230f, 239.666443f, 0.962106f},
    {1898.966309f, 64.644989f, 239.666443f, 0.157073f},
    {1904.273926f, 85.369118f, 239.666443f, 5.662714f},
    {1923.474487f, 96.419815f, 239.666443f, 4.826267f},
    {1944.612061f, 91.062439f, 239.666443f, 4.076213f},
    {1955.231079f, 71.870926f, 239.666443f, 3.255475f},
    {1949.701416f, 51.040390f, 239.666443f, 2.481856f},
};

static VisionLocXY SkullKeepLoc[]=
{
    {1908.942261f, 58.934380f, 239.666382f},
    {1916.902954f, 86.755638f, 239.66662f},
    {1944.789307f, 78.614716f, 239.666382f},
};

// lich king
const float PosLichKing[4] = {1910.499268f,-147.709961f,239.989639f, 0.943203f};
const float PosChampion[4] = {1915.371094f,-139.934219f,239.989639f, 4.159409f};
const float PosVoiceIcecrown[3] = {1914.332f, -139.317f, 239.989f}; 

static VisionLocXY IcecrownLoc[]=
{
    {1952.521606f, -137.052094f, 239.989716f, 2.513270f},
    {1946.220337f, -130.236008f, 239.989716f, 5.387829f},
    {1942.029541f, -136.833328f, 239.989716f, 0.192416f},
    {1896.965210f, -104.922951f, 239.989716f, 5.647007f},
    {1904.356079f, -113.879349f, 239.989716f, 2.163766f},
    {1907.664795f, -106.186508f, 239.989716f, 2.976653f},
    {1919.831421f, -131.184784f, 239.989716f, 4.213656f},
    {1919.120728f, -145.960281f, 239.989716f, 1.908511f},
    {1907.462891f, -139.149307f, 239.989716f, 0.176708f},
};

static VisionLocXY SkullIcecrownLoc[]=
{
    {1962.658569f, -111.356392f, 239.98986f},
    {1940.515625f, -152.933945f, 239.989868f},
    {1889.130371f, -122.932549f, 239.98986f},
    {1908.828003f, -88.593613f, 239.98986f},
};

// location of the minds eye:
// X: 1981.422974 Y: -22.442831 Z: 236.104813

// transfer from brain
//  X: 1951.097412 Y: -25.420420 Z: 326.162598 Orientation: 0.131792
// brain room portal loc: 
// sara -> type_flags = 108; original

// SanityAura, needs core support, not used here
class MANGOS_DLL_DECL SanityAura : public Aura
{
public:
	SanityAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, SpellAuraHolder *holder, Unit *target, Unit *caster) : Aura(spell, eff, bp, holder, target, caster, NULL)
    {}
};

// Yogg Saron, main event controller
struct MANGOS_DLL_DECL boss_yogg_saronAI : public ScriptedAI
{
    boss_yogg_saronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBerserkTimer;
    uint32 m_uiSanityTimer;
    uint32 m_uiKeepersActive;

    uint32 m_uiLunaticGazaTimer;
    uint32 m_uiShadowBeaconTimer;
    uint32 m_uiEmpoweringShadowsTimer;
    uint32 m_uiSummonTimer;
    uint32 m_uiDeafeningRoarTimer;
    bool m_bIsShatter;

    uint32 m_uiAchievTimer;

    std::list<Creature*> lClouds;

    void Reset()
    {
        m_uiSanityTimer             = 10000;
        m_uiKeepersActive           = 0;

		// phase 3
        m_uiLunaticGazaTimer        = 20000;
        m_uiShadowBeaconTimer       = 15000;
        m_uiEmpoweringShadowsTimer  = 60000;
        m_uiSummonTimer             = 40000;
        m_uiDeafeningRoarTimer      = 30000;
        m_uiAchievTimer             = 0;
        m_bIsShatter				= false;
        m_uiBerserkTimer            = 900000;   // 15 min

        m_creature->SetVisibility(VISIBILITY_OFF);

        if(m_creature->HasAura(SPELL_SHADOWY_BARRIER_YOGG))
            m_creature->RemoveAurasDueToSpell(SPELL_SHADOWY_BARRIER_YOGG);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_YOGG_PHASE, PHASE_IDLE);
            m_pInstance->SetData(TYPE_VISION_PHASE, PHASE_VISION_RETURN);
        }

		// respawn clouds
        GetCreatureListWithEntryInGrid(lClouds, m_creature, NPC_OMINOUS_CLOUD, DEFAULT_VISIBILITY_INSTANCE);
        if (!lClouds.empty())
        {
            for(std::list<Creature*>::iterator iter = lClouds.begin(); iter != lClouds.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
    }

    void Aggro(Unit *who) 
    {
        m_creature->SetInCombatWithZone();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        //StartSanity();

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_YOGGSARON, IN_PROGRESS);
            m_pInstance->SetData(TYPE_YOGG_PHASE, PHASE_SARA);
            // summon thorim
            if(m_pInstance->GetData(TYPE_KEEPER_THORIM) == DONE)
            {
                m_creature->SummonCreature(KEEPER_THORIM, KeepersLoc[0].x, KeepersLoc[0].y, KeepersLoc[0].z, KeepersLoc[0].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }
            // summon hodir
            if(m_pInstance->GetData(TYPE_KEEPER_HODIR) == DONE)
            {
                m_creature->SummonCreature(KEEPER_HODIR, KeepersLoc[1].x, KeepersLoc[1].y, KeepersLoc[1].z, KeepersLoc[1].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }
            // summon freya
            if(m_pInstance->GetData(TYPE_KEEPER_FREYA) == DONE)
            {
                m_creature->SummonCreature(KEEPER_FREYA, KeepersLoc[2].x, KeepersLoc[2].y, KeepersLoc[2].z, KeepersLoc[2].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }
            // summon mimiron
            if(m_pInstance->GetData(TYPE_KEEPER_MIMIRON) == DONE)
            {
                m_creature->SummonCreature(KEEPER_MIMIRON, KeepersLoc[3].x, KeepersLoc[3].y, KeepersLoc[3].z, KeepersLoc[3].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }

            if(Creature* pSara = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_SARA)))
                DoScriptText(SAY_AGGRO, pSara);
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_YOGGSARON, NOT_STARTED);

            if(Creature* pSara = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_SARA)))
            {
                if(!pSara->isAlive())
                    pSara->Respawn();
                else
                    pSara->AI()->EnterEvadeMode();
            }

            if (Creature* pYoggBrain = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_YOGG_BRAIN)))
            {
                if(!pYoggBrain->isAlive())
                    pYoggBrain->Respawn();
                else
                    pYoggBrain->AI()->EnterEvadeMode();
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_YOGGSARON, DONE);
            m_pInstance->SetData(TYPE_YOGGSARON_HARD, 0);

            // hacky way to complete achievements; use only if you have this function
            if(m_uiKeepersActive == 0)
            {
                m_pInstance->SetData(TYPE_YOGGSARON_HARD, 4);
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_ALONE : ACHIEV_ALONE_H);
            }
            else if (m_uiKeepersActive == 1)
            {
                m_pInstance->SetData(TYPE_YOGGSARON_HARD, 3);
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_ONE_LIGHT : ACHIEV_ONE_LIGHT_H);
            }
            else if (m_uiKeepersActive == 2)
            {
                m_pInstance->SetData(TYPE_YOGGSARON_HARD, 2);
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_TWO_LIGHTS : ACHIEV_TWO_LIGHTS_H);
            }
            else if (m_uiKeepersActive == 3)
            {
                m_pInstance->SetData(TYPE_YOGGSARON_HARD, 1);
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_THREE_LIGHTS : ACHIEV_THREE_LIGHTS_H);
            }

            // under 7 min
            if(m_uiAchievTimer < 420000)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_NOT_GETTING_OLDER : ACHIEV_NOT_GETTING_OLDER_H);

        }

        if(Creature* pSara = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_SARA)))
        {
            if(pSara->isAlive())
                pSara->DealDamage(pSara, pSara->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        if (Creature* pYoggBrain = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_YOGG_BRAIN)))
        {
            if(pYoggBrain->isAlive())
                pYoggBrain->DealDamage(pYoggBrain, pYoggBrain->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void StartSecondPhase()
    {
        m_creature->SetVisibility(VISIBILITY_ON);
        DoCast(m_creature, SPELL_SHADOWY_BARRIER_YOGG);
        StartSanity();
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        GetCreatureListWithEntryInGrid(lClouds, m_creature, NPC_OMINOUS_CLOUD, DEFAULT_VISIBILITY_INSTANCE);
        if (!lClouds.empty())
        {
            for(std::list<Creature*>::iterator iter = lClouds.begin(); iter != lClouds.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }

		m_uiSummonTimer = urand(3000, 7000);
    }

    void StartThirdPhase()
    {
        DoScriptText(SAY_PHASE3, m_creature);
        m_creature->RemoveAurasDueToSpell(SPELL_SHADOWY_BARRIER_YOGG);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.3);

		m_uiSummonTimer = urand(15000, 20000);

        if(Creature* pSara = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_SARA)))
            pSara->ForcedDespawn();

        if (Creature* pYoggBrain = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_YOGG_BRAIN)))
            pYoggBrain->DealDamage(pYoggBrain, pYoggBrain->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

	// hacky way of doing sanity before the recent aura changes
    void StartSanity()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            SpellEntry* spell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_SANITY);
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive())
                {
                   /* if(i->getSource()->HasAura(SPELL_SANITY, EFFECT_INDEX_0))
                        i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0)->SetStackAmount(100);
                    else
                    {
                        if(i->getSource()->AddAura(new SanityAura(spell, EFFECT_INDEX_0, NULL, i->getSource(), m_creature)))
                            i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0)->SetStackAmount(100);
                    }*/
                }
            }
        }
    }

    void DoCastSanity()
    {
        uint8 m_uiStacks;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive())
                {
                    // reduce sanity
                    if(i->getSource()->HasAura(SPELL_SANITY, EFFECT_INDEX_0))
                    {
                        if(Aura *aura = i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0))
                        {
                            m_uiStacks = aura->GetStackAmount();
                            if(m_uiStacks == 100)
                                DoCast(i->getSource(), SPELL_INSANE);
                            /*if(m_uiStacks > 1)
                            i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0)->SetStackAmount(stack - 1);
                            else
                            {
                            i->getSource()->RemoveAurasDueToSpell(SPELL_SANITY);
                            DoCast(i->getSource(), SPELL_INSANE);
                            }*/
                        }
                    }
                }
            }
        }
    }

    Creature* SelectRandomGuardian(float fRange)
    {
        std::list<Creature* > lGuardiansList;
        GetCreatureListWithEntryInGrid(lGuardiansList, m_creature, MOB_IMMORTAL_GUARDIAN, fRange);

        //This should not appear!
        if (lGuardiansList.empty()){
            m_uiEmpoweringShadowsTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lGuardiansList.begin();
        advance(iter, urand(0, lGuardiansList.size()-1));

        return *iter;
    }

	void SummonTentacles()
	{
		float homeZ = 0;
		float creatureDist = 0;
		// set tentacle entry
		uint32 m_uiTentacles[] = {MOB_CONSTRICTOR_TENTACLE, MOB_CORRUPTOR_TENTACLE, MOB_CRUSHER_TENTACLE};
		uint32 m_uiEntry = urand(0, 2);
		if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, MOB_CRUSHER_TENTACLE, 100.0f))
		{
			if(pTemp->isAlive())
				m_uiEntry = urand(0, 1);
		}		
		uint32 m_uiTentacleEntry = m_uiTentacles[m_uiEntry];

		// set distance
		// Z factor needs some fixing, in some cases it should be smaller
		switch(urand(0, 1))
		{
		case 0:
			creatureDist = urand(20, 30);
			homeZ = 326 + creatureDist * 0.05f;
			break;
		case 1:
			creatureDist = urand(30, 45);
			homeZ = 328 + creatureDist * 0.05f;
			break;
		}

		// set coordonates
		float angle = (float) rand()*360/RAND_MAX + 1;
		float homeX = m_creature->GetPositionX() + creatureDist*cos(angle*(M_PI/180));
		float homeY = m_creature->GetPositionY() + creatureDist*sin(angle*(M_PI/180));
		// summon tentacle
		if(Creature *pTemp = m_creature->SummonCreature(m_uiTentacleEntry, homeX, homeY, homeZ, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
			pTemp->SetInCombatWithZone();
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // achiev timer
        m_uiAchievTimer += uiDiff;

        switch(m_pInstance->GetData(TYPE_YOGG_PHASE))
        {
        case PHASE_SARA:
            // Friendly Sara phase: see script below
            break;
        case PHASE_TENTACLES:
            {
                if (m_uiSanityTimer < uiDiff)
                {
                    //DoCastSanity();
                    //DoCast(m_creature, SPELL_SANITY);
                    m_uiSanityTimer = 20000;
                }
                else m_uiSanityTimer -= uiDiff;

				// summon tentacles
				if (m_uiSummonTimer < uiDiff && !m_bIsShatter)
                {
                    SummonTentacles();
                    m_uiSummonTimer = urand(10000, 15000);
                }
                else m_uiSummonTimer -= uiDiff;

                break;
            }
        case PHASE_OLD_GOD:
            {
                if (m_uiSummonTimer < uiDiff)
                {
                    if(Creature *pTemp = m_creature->SummonCreature(MOB_IMMORTAL_GUARDIAN, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
						pTemp->SetInCombatWithZone();
                    m_uiSummonTimer = 30000;
                }
                else m_uiSummonTimer -= uiDiff;

                if (m_uiLunaticGazaTimer < uiDiff)
                {
                    DoCast(m_creature, SPELL_LUNATIC_GAZE_YOGG);
                    m_uiLunaticGazaTimer = urand(10000, 13000);
                }
                else m_uiLunaticGazaTimer -= uiDiff;

                if (m_uiEmpoweringShadowsTimer < uiDiff)
                {
                    if (Creature* pGuardian = SelectRandomGuardian(80.0f))
                        DoCast(pGuardian, SPELL_SHADOW_BEACON);
                    m_uiEmpoweringShadowsTimer = 45000;
                }
                else m_uiEmpoweringShadowsTimer -= uiDiff;

                if (m_uiDeafeningRoarTimer < uiDiff && !m_bIsRegularMode && m_uiKeepersActive < 4)
                {
                    DoCast(m_creature, SPELL_DEAFENING_ROAR);
                    m_uiDeafeningRoarTimer = 30000;
                }
                else m_uiDeafeningRoarTimer -= uiDiff;

                DoMeleeAttackIfReady();

                break;
            }
        }

        // extinguish all life
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserkTimer = 10000;
        }
        else m_uiBerserkTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_madness_portalAI : public ScriptedAI
{
    mob_madness_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint8 m_uiDestination;
    uint32 m_uiCheckTimer;
    bool m_bHasTeleported;

    void Reset()
    {
        m_uiCheckTimer      = 500;
        m_bHasTeleported    = false;
        m_creature->SetRespawnDelay(DAY);
        if(m_pInstance)
            m_uiDestination = m_pInstance->GetData(TYPE_VISION_PHASE) - 1;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_creature->GetPositionZ() < 245.0f)
            m_uiDestination = 3;

		// hacky way. This should be done by a spell
		// this uses the Type_vision_phase in order to set vision destination
        if(m_uiCheckTimer < uiDiff && !m_bHasTeleported)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                    {
                        i->getSource()->TeleportTo(i->getSource()->GetMapId(), TeleportLoc[m_uiDestination].x, TeleportLoc[m_uiDestination].y, TeleportLoc[m_uiDestination].z, i->getSource()->GetOrientation());
                        if(m_uiDestination < 3)
						{
							i->getSource()->CastSpell(i->getSource(), SPELL_ILLUSION_ROOM, false);
                            m_creature->ForcedDespawn();
						}
						else
							i->getSource()->RemoveAurasDueToSpell(SPELL_ILLUSION_ROOM);
                    }
                }
            } 
            m_uiCheckTimer = 500;
        }else m_uiCheckTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_brain_of_yogg_saronAI : public ScriptedAI
{
    boss_brain_of_yogg_saronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiVisionPhase;
    uint32 m_uiSpeechTimer;
    uint32 m_uiMadnessTimer;
    uint32 m_uiTentacleCheckTimer;

    bool m_bIsPhaseFinished;
    bool m_bIsVisionFinished;
    bool m_bHasShattered;

    uint64 m_uiLichKingGUID;
    uint64 m_uiChampionGUID;
    uint64 m_uiGaronaGUID;
    uint64 m_uiKingLlaneGUID;
    uint64 m_uiNeltharionGUID;
    uint64 m_uiMalygosGUID;
    uint64 m_uiYseraGUID;
    uint64 m_uiVoiceOfYoggGUID;

    void Reset()
    {
        m_bIsPhaseFinished  = false;
        m_bIsVisionFinished = false;
        m_bHasShattered     = false;
        m_uiLichKingGUID    = 0;
        m_uiChampionGUID    = 0;
        m_uiGaronaGUID      = 0;
        m_uiKingLlaneGUID   = 0;
        m_uiNeltharionGUID  = 0;
        m_uiMalygosGUID     = 0;
        m_uiYseraGUID       = 0;
        m_uiVoiceOfYoggGUID = 0;

        m_uiVisionPhase     = 0;
        m_uiSpeechTimer     = 1000;
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_YOGG_BRAIN, NOT_STARTED);

        // close doors on reset
        if(GameObject* pVisionDoor = GetClosestGameObjectWithEntry(m_creature, GO_BRAIN_DOOR1, 100.0f))
            pVisionDoor->SetGoState(GO_STATE_READY);
        if(GameObject* pVisionDoor = GetClosestGameObjectWithEntry(m_creature, GO_BRAIN_DOOR2, 100.0f))
            pVisionDoor->SetGoState(GO_STATE_READY);
        if(GameObject* pVisionDoor = GetClosestGameObjectWithEntry(m_creature, GO_BRAIN_DOOR3, 100.0f))
            pVisionDoor->SetGoState(GO_STATE_READY);
    }

    // for debug
    void Aggro(Unit *who) 
    {
		// For vision debug only
        //StartVisions();
		//m_pInstance->SetData(TYPE_VISION_PHASE, PHASE_VISION_STORMWIND);
        //m_creature->GetMotionMaster()->MoveIdle();
        //m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), 255.011f, 0.0f);
        //m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), 255.011f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > 0 && !m_bHasShattered)
        {
            if (Creature* pYogg = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_YOGGSARON)))
			{
				((boss_yogg_saronAI*)pYogg->AI())->m_bIsShatter = true;
				// spell bugged, need core fix. It should be cast on tentacles, not on players!
				//pYogg->CastSpell(pYogg, SPELL_SHATTERED_ILLUSION, false);
			}
            m_bHasShattered = true;
        }
    }

    void SummonPortals()
    {
        for(uint8 i = 0; i < 3; i++)
            m_creature->SummonCreature(MOB_DESCEND_INTO_MADNESS, MadnessPortalLoc[i].x, MadnessPortalLoc[i].y, MadnessPortalLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
    }

    void StartVisions()
    {
        DoCast(m_creature, SPELL_INDUCE_MADNESS);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_uiVisionPhase         = 0;
        m_uiSpeechTimer         = 1000;
        m_uiTentacleCheckTimer  = 1000;
        m_uiMadnessTimer        = 60000;
        m_bIsVisionFinished     = false;
        m_bHasShattered         = false;
    }

	// check if all the tentacles are dead
    bool IsThereAnyAdd(WorldObject *pSource)
    {
        if(GetClosestCreatureWithEntry(pSource, MOB_VISION_TENTACLE, 80.0f))
            return true;

        if(m_pInstance)
        {
            // open doors
            switch(m_pInstance->GetData(TYPE_VISION_PHASE))
            {
            case PHASE_VISION_STORMWIND:
                if(GameObject* pVisionDoor = GetClosestGameObjectWithEntry(m_creature, GO_BRAIN_DOOR3, 100.0f))
                    pVisionDoor->SetGoState(GO_STATE_ACTIVE);
                break;
            case PHASE_VISION_WYRMREST:
                if(GameObject* pVisionDoor = GetClosestGameObjectWithEntry(m_creature, GO_BRAIN_DOOR1, 100.0f))
                    pVisionDoor->SetGoState(GO_STATE_ACTIVE);
                break;
            case PHASE_VISION_ICECROWN:
                if(GameObject* pVisionDoor = GetClosestGameObjectWithEntry(m_creature, GO_BRAIN_DOOR2, 100.0f))
                    pVisionDoor->SetGoState(GO_STATE_ACTIVE);
                break;
            }
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        switch(m_pInstance->GetData(TYPE_VISION_PHASE))
        {
        case PHASE_VISION_STORMWIND:
            {
                if(m_uiSpeechTimer < uiDiff)
                {
                    switch(m_uiVisionPhase)
                    {
                    case 0:
                        if(Creature* Garona = m_creature->SummonCreature(NPC_GARONA, PosGarona[0], PosGarona[1], PosGarona[2], PosGarona[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                            m_uiGaronaGUID = Garona->GetGUID();
                        if(Creature* KingLlane = m_creature->SummonCreature(NPC_KING_LLANE, PosKing[0], PosKing[1], PosKing[2], PosKing[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                            m_uiKingLlaneGUID = KingLlane->GetGUID();
                        if(Creature* VoiceOfYogg = m_creature->SummonCreature(NPC_VOICE_OF_YOGG_SARON, PosVoiceStormwind[0], PosVoiceStormwind[1], PosVoiceStormwind[2], 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                        {
                            m_uiVoiceOfYoggGUID = VoiceOfYogg->GetGUID();
                            VoiceOfYogg->SetDisplayId(11686);     // make invisible
                        }
                        for(uint8 i = 0; i < 8; i++)
                        {
                            if(Creature *pTemp = m_creature->SummonCreature(MOB_VISION_TENTACLE, KeepLoc[i].x, KeepLoc[i].y, KeepLoc[i].z, KeepLoc[i].o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 80000))
                            {
                                pTemp->SetDisplayId(28621);
                                pTemp->SetMaxHealth(m_bIsRegularMode ? 15000 : 40000);
                                pTemp->setFaction(7); 
                                pTemp->CastSpell(pTemp, SPELL_GRIM_REPRISAL, false);
                            }
                        }
                        for(uint8 i = 0; i < 3; i++)
                            m_creature->SummonCreature(NPC_LAUGHING_SKULL, SkullKeepLoc[i].x, SkullKeepLoc[i].y, SkullKeepLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 1000;
                        break;
                    case 1:
                        if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                            DoScriptText(SAY_GARONA1, Garona);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 12000;
                        break;
                    case 2:
                        if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                            DoScriptText(SAY_GARONA2, Garona);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 12000;
                        break;
                    case 3:
                        if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                            DoScriptText(SAY_YOGG_V1_1, VoiceOfYogg);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 4000;
                        break;
                    case 4:
                        if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                            DoScriptText(SAY_YOGG_V1_2, VoiceOfYogg);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 4000;
                        break;
                    case 5:
                        if(Creature* KingLlane = m_pInstance->instance->GetCreature(m_uiKingLlaneGUID))
                            DoScriptText(SAY_GARONA3, KingLlane);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 6000;
                        break;
                    case 6:
                        if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                            Garona->GetMotionMaster()->MovePoint(0, 1931.348f, 61.033f, 241.709f);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 6000;
                        break;
                    case 7:
                        if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                        {
                            DoScriptText(SAY_GARONA4, Garona);
                            if(Creature* KingLlane = m_pInstance->instance->GetCreature(m_uiKingLlaneGUID))
                            {
                                KingLlane->SetStandState(UNIT_STAND_STATE_DEAD);
                                KingLlane->SetHealth(0);
                                Garona->Attack(KingLlane, true);
                            }
                        }
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 6000;
                        break;
                    case 8:
                        if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                            Garona->AttackStop();
                        if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                            DoScriptText(SAY_YOGG_V1_3, VoiceOfYogg);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 8000;
                        break;
                    default:
                        m_uiSpeechTimer = 100000;
                    }
                }
                else m_uiSpeechTimer -= uiDiff;

                break;
            }
        case PHASE_VISION_WYRMREST:
            {
                if(m_uiSpeechTimer < uiDiff)
                {
                    switch(m_uiVisionPhase)
                    {
                    case 0:
                        m_creature->SummonCreature(NPC_ALEXSTRASZA, PosAlexstrasza[0], PosAlexstrasza[1], PosAlexstrasza[2], PosAlexstrasza[3], TEMPSUMMON_TIMED_DESPAWN, 60000);
                        if(Creature* Neltharion = m_creature->SummonCreature(NPC_NELTHARION, PosNeltharion[0], PosNeltharion[1], PosNeltharion[2], PosNeltharion[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                            m_uiNeltharionGUID = Neltharion->GetGUID();
                        if(Creature* Malygos = m_creature->SummonCreature(NPC_MALYGOS, PosMalygos[0], PosMalygos[1], PosMalygos[2], PosMalygos[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                            m_uiMalygosGUID = Malygos->GetGUID();
                        if(Creature* Ysera = m_creature->SummonCreature(NPC_YSERA, PosYsera[0], PosYsera[1], PosYsera[2], PosYsera[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                            m_uiYseraGUID = Ysera->GetGUID();
                        if(Creature* VoiceOfYogg = m_creature->SummonCreature(NPC_VOICE_OF_YOGG_SARON, PosVoiceDragon[0], PosVoiceDragon[1], PosVoiceDragon[2], 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                        {
                            m_uiVoiceOfYoggGUID = VoiceOfYogg->GetGUID();
                            VoiceOfYogg->SetVisibility(VISIBILITY_OFF);
                        }
                        for(uint8 i = 0; i < 10; i++)
                        {
                            if(Creature *pTemp = m_creature->SummonCreature(MOB_VISION_TENTACLE, DragonLoc[i].x, DragonLoc[i].y, DragonLoc[i].z, DragonLoc[i].o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 80000))
                            {
                                pTemp->SetDisplayId(DisplayDragons[i]);
                                pTemp->SetMaxHealth(m_bIsRegularMode ? 15000 : 40000);
                                pTemp->setFaction(7);
                                pTemp->CastSpell(pTemp, SPELL_GRIM_REPRISAL, false);
                            }
                        }
                        for(uint8 i = 0; i < 3; i++)
                            m_creature->SummonCreature(NPC_LAUGHING_SKULL, SkullDragonLoc[i].x, SkullDragonLoc[i].y, SkullDragonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 1000;
                        break;
                    case 1:
                        if(Creature* Neltharion = m_pInstance->instance->GetCreature(m_uiNeltharionGUID))
                            DoScriptText(SAY_NELTHARION1, Neltharion);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 10000;
                        break;
                    case 2:
                        if(Creature* Ysera = m_pInstance->instance->GetCreature(m_uiYseraGUID))
                            DoScriptText(SAY_YSERA, Ysera);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 7000;
                        break;
                    case 3:
                        if(Creature* Neltharion = m_pInstance->instance->GetCreature(m_uiNeltharionGUID))
                            DoScriptText(SAY_NELTHARION2, Neltharion);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 6000;
                        break;
                    case 4:
                        if(Creature* Malygos = m_pInstance->instance->GetCreature(m_uiMalygosGUID))
                            DoScriptText(SAY_MALYGOS, Malygos);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 9000;
                        break;
                    case 5:
                        if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                            DoScriptText(SAY_YOGG_V2, VoiceOfYogg);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 10000;
                        break;
                    default:
                        m_uiSpeechTimer = 100000;
                    }
                }else m_uiSpeechTimer -= uiDiff;

                break;
            }
        case PHASE_VISION_ICECROWN:
            {
                if(m_uiSpeechTimer < uiDiff)
                {
                    switch(m_uiVisionPhase)
                    {
                    case 0:
                        if(Creature* LichKing = m_creature->SummonCreature(NPC_LICH_KING, PosLichKing[0], PosLichKing[1], PosLichKing[2], PosLichKing[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                            m_uiLichKingGUID = LichKing->GetGUID();
                        if(Creature* Champion = m_creature->SummonCreature(NPC_IMMOLATED_CHAMPION, PosChampion[0], PosChampion[1], PosChampion[2], PosChampion[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                            m_uiChampionGUID = Champion->GetGUID();
                        if(Creature* VoiceOfYogg = m_creature->SummonCreature(NPC_VOICE_OF_YOGG_SARON, PosVoiceIcecrown[0], PosVoiceIcecrown[1], PosVoiceIcecrown[2], 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                        {
                            m_uiVoiceOfYoggGUID = VoiceOfYogg->GetGUID();
                            VoiceOfYogg->SetVisibility(VISIBILITY_OFF);
                        }
                        for(uint8 i = 0; i < 9; i++)
                        {
                            if(Creature *pTemp = m_creature->SummonCreature(MOB_VISION_TENTACLE, IcecrownLoc[i].x, IcecrownLoc[i].y, IcecrownLoc[i].z, IcecrownLoc[i].o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 80000))
                            {
                                pTemp->SetMaxHealth(m_bIsRegularMode ? 15000 : 40000);
                                pTemp->SetDisplayId(25627);
                                pTemp->setFaction(7);
                                pTemp->CastSpell(pTemp, SPELL_GRIM_REPRISAL, false);
                            }
                        }
                        for(uint8 i = 0; i < 4; i++)
                            m_creature->SummonCreature(NPC_LAUGHING_SKULL, SkullIcecrownLoc[i].x, SkullIcecrownLoc[i].y, SkullIcecrownLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 1000;
                        break;
                    case 1:
                        if(Creature* Champion = m_pInstance->instance->GetCreature(m_uiChampionGUID))
                        {
                            Champion->SetStandFlags(UNIT_STAND_STATE_KNEEL);
                            if(Creature* LichKing = m_pInstance->instance->GetCreature(m_uiLichKingGUID))
                            {
                                LichKing->CastSpell(Champion, 54142, false);
                                DoScriptText(SAY_LICH_KING1, LichKing);
                            }
                        }
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 5000;
                        break;
                    case 2:
                        if(Creature* Champion = m_pInstance->instance->GetCreature(m_uiChampionGUID))
                            DoScriptText(SAY_CHAMPION1, Champion);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 8000;
                        break;
                    case 3:
                        if(Creature* Champion = m_pInstance->instance->GetCreature(m_uiChampionGUID))
                            DoScriptText(SAY_CHAMPION2, Champion);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 8000;
                        break;
                    case 4:
                        if(Creature* LichKing = m_pInstance->instance->GetCreature(m_uiLichKingGUID))
                            DoScriptText(SAY_LICH_KING2, LichKing);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 7000;
                        break;
                    case 5:
                        if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                            DoScriptText(SAY_YOGG_V3_1, VoiceOfYogg);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 5000;
                        break;
                    case 6:
                        if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                            DoScriptText(SAY_YOGG_V3_2, VoiceOfYogg);
                        ++m_uiVisionPhase;
                        m_uiSpeechTimer = 10000;
                        break;
                    default:
                        m_uiSpeechTimer = 100000;
                    }
                }else m_uiSpeechTimer -= uiDiff;

                break;
            }
        }

        // End phase after 30%
        if(m_creature->GetHealthPercent() < 30 && !m_bIsPhaseFinished)
        {
            if(m_pInstance) 
            {
                m_pInstance->SetData(TYPE_YOGG_BRAIN, DONE);
                m_pInstance->SetData(TYPE_YOGG_PHASE, PHASE_OLD_GOD);
            }
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (Creature* pYogg = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_YOGGSARON)))
			{
                ((boss_yogg_saronAI*)pYogg->AI())->StartThirdPhase();
				pYogg->RemoveAurasDueToSpell(SPELL_SHATTERED_ILLUSION);
			}
			m_creature->CastStop();
            m_bIsPhaseFinished = true;
        }

        // check if tentacles are dead
        if (m_uiTentacleCheckTimer < uiDiff && !m_bIsVisionFinished)
        {
            if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
            {
                if(!IsThereAnyAdd(VoiceOfYogg))
                {
                    SummonPortals();
					m_pInstance->SetData(TYPE_VISION_PHASE, PHASE_VISION_RETURN);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_uiTentacleCheckTimer = 300000;
                }
                else
                    m_uiTentacleCheckTimer = 500;
            }
        }
        else m_uiTentacleCheckTimer -= uiDiff;

        // make boss unattackable -> exploit check
        // Induced madness need core support, it should make the players insane
        if (m_uiMadnessTimer < uiDiff)
        {
            if (Creature* pYogg = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_YOGGSARON)))
			{
				((boss_yogg_saronAI*)pYogg->AI())->m_bIsShatter = false;
				((boss_yogg_saronAI*)pYogg->AI())->m_uiSummonTimer = urand(5000, 8000);
				pYogg->RemoveAurasDueToSpell(SPELL_SHATTERED_ILLUSION);
			}
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_bIsVisionFinished = true;
            m_uiMadnessTimer = 300000;
        }
        else m_uiMadnessTimer -= uiDiff;
    }
};

// Sara script
struct MANGOS_DLL_DECL boss_saraAI : public ScriptedAI
{
    boss_saraAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bIsIntroDone = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    // intro
    bool m_bIsIntro;
    uint32 m_uiIntro_Phase;
    uint32 m_uiSpeech_Timer;
    bool m_bIsIntroDone;
    bool m_bIsFactionSet;

    // phase 1
    uint32 m_uiPhaseYellTimer;
    uint32 m_uiSummonTimer;
    uint32 m_uiSarasFervorTimer;
    uint32 m_uiSarasBlessingTimer;
    uint32 m_uiSarasAngerTimer;
	uint32 m_uiAllVisions;

    // transition
    bool m_bIsOutro;
    uint32 m_uiOutroTimer;
    uint32 m_uiOutroStep;

    // phase 2
    uint32 m_uiPsychosisTimer;
	uint32 m_uiPortalsTimer;
    uint32 m_uiMaladyTimer;
    uint32 m_uiBrainLinkTimer;
    uint32 m_uiDeathRayTimer;
    uint32 m_uiBrainLinkEndTimer;
    uint32 m_uiBrainLinkTickTimer;
    bool m_bIsBrainLink;

	uint32 m_uiFirstVision;
	uint32 m_uiSecondVision;

    uint64 m_uiLinkTarget1GUID;
    uint64 m_uiLinkTarget2GUID;

    void Reset()
    {
        // intro
        m_bIsIntro              = false;
        m_uiIntro_Phase         = 0;
        m_uiSpeech_Timer        = 1000;
        m_bIsFactionSet         = false;

        // phase 1
        m_uiPhaseYellTimer      = 30000;
        m_uiSummonTimer         = 10000 + urand (1000, 5000);
        m_uiSarasFervorTimer    = urand(10000, 15000);
        m_uiSarasBlessingTimer  = urand(15000, 20000);
        m_uiSarasAngerTimer     = urand(20000, 25000);
		m_uiAllVisions			= 0;

        // transition
        m_bIsOutro              = false;
        m_uiOutroTimer          = 10000;
        m_uiOutroStep           = 1;

        // phase 2
		m_uiPortalsTimer		= 60000;
        m_uiPsychosisTimer      = 15000;     
        m_uiMaladyTimer         = 20000;
        m_uiBrainLinkTimer      = 25000;
        m_uiDeathRayTimer       = 30000;
        m_uiBrainLinkTickTimer  = 1000;
        m_uiBrainLinkEndTimer   = 0;
        m_bIsBrainLink          = false;

		m_uiFirstVision			= 0;
		m_uiSecondVision		= 0;

        m_uiLinkTarget1GUID     = 0;
        m_uiLinkTarget2GUID     = 0;

        if(m_creature->HasAura(SPELL_SHADOWY_BARRIER))
            m_creature->RemoveAurasDueToSpell(SPELL_SHADOWY_BARRIER);

        m_creature->SetDisplayId(MODEL_SARA_HUMAN);
        m_creature->setFaction(35);
        m_creature->SetMaxHealth(199999);
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        m_creature->SetSplineFlags(SPLINEFLAG_FLYING);
        m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), 329.397f, 5.9f);
        m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), 329.397f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_KILL1, m_creature);
        else
            DoScriptText(SAY_KILL2, m_creature);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            m_bIsOutro = true;
            m_creature->SetHealth(m_creature->GetMaxHealth());
            uiDamage = 0;
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // start intro speech
        if(m_pInstance->GetData(TYPE_YOGG_PHASE) == PHASE_IDLE)
        {
            if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && !m_bIsIntro && !m_bIsIntroDone &&
                pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 110) && m_creature->IsWithinLOSInMap(pWho))
                m_bIsIntro = true;
        }
    }

    // return a random npc from the selected entry
    Creature* SelectRandomNpc(float fRange, uint32 uiEntry)
    {
        std::list<Creature* > lNpcList;
        GetCreatureListWithEntryInGrid(lNpcList, m_creature, uiEntry, fRange);

        //This should not appear!
        if (lNpcList.empty()){
            return NULL;
        }

        std::list<Creature* >::iterator iter = lNpcList.begin();
        advance(iter, urand(0, lNpcList.size()-1));

        return *iter;
    }

    void DoBrainLink()
    {
		// workaround here
    }

	void SummonPortals()
    {
        uint8 maxPortals = m_bIsRegularMode ? 4 : 10;
        for(uint8 i = 0; i < maxPortals; i++)
            m_creature->SummonCreature(MOB_DESCEND_INTO_MADNESS, YoggPortalLoc[i].x, YoggPortalLoc[i].y, YoggPortalLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
    }

	// set a random vision
	uint32 GetCurrentVision()
	{
		switch(urand(0, 2))
		{
		case 0:
			return PHASE_VISION_STORMWIND;
			break;
		case 1:
			return PHASE_VISION_WYRMREST;
			break;
		case 2:
			return PHASE_VISION_ICECROWN;
			break;
		}

		return 0;
	}

	// start a vision
	void SetVisionPhase()
	{
		uint32 m_uiVisionType = 0;

		// set random different destination
		if(m_uiFirstVision == 0 && m_uiSecondVision == 0)
		{
			m_uiVisionType = GetCurrentVision();
			m_uiFirstVision = m_uiVisionType;
		}
		else if(m_uiSecondVision == 0)
		{
			do m_uiVisionType = GetCurrentVision();
			while(m_uiVisionType == m_uiFirstVision);
			m_uiSecondVision = m_uiVisionType;
		}
		else
		{
			do m_uiVisionType = GetCurrentVision();
			while(m_uiVisionType == m_uiFirstVision || m_uiVisionType == m_uiSecondVision);
		}
		
		// set instance data for vision
		if(m_uiVisionType != 0)
			m_pInstance->SetData(TYPE_VISION_PHASE, m_uiVisionType);

		// summon portals
		SummonPortals();
		// start the vision
		if (Creature* pYoggBrain = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_YOGG_BRAIN)))
		{
			if(pYoggBrain->isAlive())
				((boss_brain_of_yogg_saronAI*)pYoggBrain->AI())->StartVisions();
		}
		// increase the vision no.
		m_uiAllVisions += 1;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        switch(m_pInstance->GetData(TYPE_YOGG_PHASE))
        {
        case PHASE_IDLE:
            {
                // intro
                if (m_bIsIntro && !m_bIsIntroDone)
                {
                    if(m_uiSpeech_Timer < uiDiff)
                    {
                        switch(m_uiIntro_Phase)
                        {
                        case 0:
                            DoScriptText(SAY_INTRO1, m_creature);
                            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                            m_creature->GetMotionMaster()->MoveIdle();
                            SetCombatMovement(false);
                            m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), 329.397f, 5.9f);
                            m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), 329.397f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                            ++m_uiIntro_Phase;
                            m_uiSpeech_Timer = 8000;
                            break;
                        case 1:
                            DoScriptText(SAY_INTRO2, m_creature);
                            ++m_uiIntro_Phase;
                            m_uiSpeech_Timer = 8000;
                            break;
                        case 2:
                            m_bIsIntro      = false;
                            m_bIsIntroDone  = true;
                            m_uiSpeech_Timer = 12000;
                            break;
                        default:
                            m_uiSpeech_Timer = 100000;
                        }
                    }
                    else m_uiSpeech_Timer -= uiDiff;
                }
                break;
            }
        case PHASE_SARA:
            {
                if(!m_bIsOutro)
                {
					// wipe check
                    if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
                    {
                        if(!pYogg->getVictim() || !pYogg->SelectHostileTarget() || pYogg->getVictim() == m_creature)
                        {
                            pYogg->AI()->EnterEvadeMode();
                            DoScriptText(SAY_WIPE, m_creature);
                        }
                    }

                    if (m_uiPhaseYellTimer < uiDiff)
                    {
                        switch(urand(0, 1))
                        {
                        case 0: DoScriptText(SAY_HELP1, m_creature); break;
                        case 1: DoScriptText(SAY_HELP2, m_creature); break;
                        }
                        m_uiPhaseYellTimer = 30000;
                    }else m_uiPhaseYellTimer -= uiDiff;

                    if (m_uiSummonTimer < uiDiff)
                    {
                        if (Creature* pCloud = SelectRandomNpc(120.0f, NPC_OMINOUS_CLOUD))
                            pCloud->CastSpell(pCloud, SPELL_SUMMON_GUARDIAN2, false);
                        m_uiSummonTimer = urand(15000, 20000);
                    }else m_uiSummonTimer -= uiDiff;

                    if (m_uiSarasFervorTimer < uiDiff)
                    {
                        if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
                        {
                            if (Unit* pTarget = pYogg->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
							{
								if(pTarget != m_creature)
								{
									DoCast(pTarget, SPELL_SARAS_FERVOR);
									m_uiSarasFervorTimer = urand(20000, 30000);
								}
							}
                        } 
                    }else m_uiSarasFervorTimer -= uiDiff;

                    if (m_uiSarasBlessingTimer < uiDiff)
                    {
                        if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
                        {
                            if (Unit* pTarget = pYogg->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
							{
								if(pTarget != m_creature)
								{
									DoCast(pTarget, SPELL_SARAS_BLESSING);
									m_uiSarasBlessingTimer = urand(20000, 30000);
								}
							}
                        }
                    }else m_uiSarasBlessingTimer -= uiDiff;

                    if (m_uiSarasAngerTimer < uiDiff)
                    {
                        if (Creature* pGuardian = SelectRandomNpc(80.0f, MOB_GUARDIAN_OF_YOGG))
                            DoCast(pGuardian, SPELL_SARAS_ANGER);
                        m_uiSarasAngerTimer = urand(20000, 30000);
                    }else m_uiSarasAngerTimer -= uiDiff;
                }
				// Phase 1 outro
                if(m_bIsOutro)
                {
                    switch(m_uiOutroStep)
                    {
                    case 1:
                        m_creature->SetHealth(m_creature->GetMaxHealth());
                        m_creature->CombatStop(true);
                        m_creature->InterruptNonMeleeSpells(false);
                        ++m_uiOutroStep;
                        m_uiOutroTimer = 5000;
                        break;
                    case 3:
                        DoScriptText(SAY_PHASE2, m_creature);
                        ++m_uiOutroStep;
                        m_uiOutroTimer = 15000;
                        break;
                    case 5:
                        if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
                        {
                            if (Unit* pTarget = pYogg->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                m_creature->AddThreat(pTarget, 100.0f);
                                m_creature->AI()->AttackStart(pTarget);
                            }
                        }
                        m_creature->setFaction(14);
                        m_creature->SetInCombatWithZone();
						SetCombatMovement(false);
						m_creature->GetMotionMaster()->MoveIdle();
                        m_creature->SetDisplayId(MODEL_SARA_VALKYR);
                        DoCast(m_creature, SPELL_SHADOWY_BARRIER);
                        m_creature->SetHealth(m_creature->GetMaxHealth());
                        m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  10, 5.9f);
                        m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  10, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                        if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
                            ((boss_yogg_saronAI*)pYogg->AI())->StartSecondPhase();
                        m_uiPhaseYellTimer = 30000 + urand(5000, 10000);
                        m_pInstance->SetData(TYPE_YOGG_PHASE, PHASE_TENTACLES);
                        ++m_uiOutroStep;
                        m_uiOutroTimer = 3000;
                        break;
                    }
                }
                else return;

                if (m_uiOutroTimer <= uiDiff)
                {
                    ++m_uiOutroStep;
                    m_uiOutroTimer = 330000;
                } 
                m_uiOutroTimer -= uiDiff;

                break;
            }
        case PHASE_TENTACLES:
            {
				// wipe check
                if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
                {
                    if(!pYogg->getVictim() || !pYogg->SelectHostileTarget())
                        pYogg->AI()->EnterEvadeMode();
                }

                if (m_uiPhaseYellTimer < uiDiff)
                {
                    switch(urand(0, 1))
                    {
                    case 0: DoScriptText(SAY_SPECIAL1, m_creature); break;
                    case 1: DoScriptText(SAY_SPECIAL2, m_creature); break;
                    }
                    m_uiPhaseYellTimer = 30000 + urand(5000, 10000);
                }
                else m_uiPhaseYellTimer -= uiDiff;

				// summon madness portals
				if (m_uiPortalsTimer < uiDiff && m_uiAllVisions < 3)
				{
					SetVisionPhase();
					if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
						DoScriptText(SAY_VISION, pYogg);
					m_uiPortalsTimer = 85000;
				}
				else m_uiPortalsTimer -= uiDiff;

                if (m_uiPsychosisTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_PHYCHOSIS : SPELL_PHYCHOSIS_H);
                    m_uiPsychosisTimer = urand(15000, 20000);
                }
                else m_uiPsychosisTimer -= uiDiff;

                if (m_uiMaladyTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_MALADY_OF_THE_MIND);
                    m_uiMaladyTimer = urand(20000, 25000);
                }
                else m_uiMaladyTimer -= uiDiff;

                if (m_uiBrainLinkTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_uiLinkTarget1GUID = pTarget->GetGUID();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_uiLinkTarget2GUID = pTarget->GetGUID();
                    DoCast(m_creature, SPELL_BRAIN_LINK);
                    m_bIsBrainLink          = true;
                    m_uiBrainLinkEndTimer   = 30000;
                    m_uiBrainLinkTickTimer  = 1000;
                    m_uiBrainLinkTimer      = urand(25000, 30000);
                }
                else m_uiBrainLinkTimer -= uiDiff;

                // workaround for brainlink
				// spell need core support
				// REMOVE FOR REVISION!
                if (m_uiBrainLinkTickTimer < uiDiff && m_bIsBrainLink)
                {
                    DoBrainLink();
                    m_uiBrainLinkTickTimer = 1000;
                }
                else m_uiBrainLinkTickTimer -= uiDiff;

                if (m_uiBrainLinkEndTimer < uiDiff && m_bIsBrainLink)
                    m_bIsBrainLink = false;
                else m_uiBrainLinkEndTimer -= uiDiff;

				// workaround, should be done by spell
                if (m_uiDeathRayTimer < uiDiff)
                {
                    if(Creature* pYogg = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_YOGGSARON)))
                    {
                        for(int i = 0; i < irand(3, 4); i++)
                        {
                            float angle = (float) rand()*360/RAND_MAX + 1;
                            float homeX = pYogg->GetPositionX() + urand(20, 40)*cos(angle*(M_PI/180));
                            float homeY = pYogg->GetPositionY() + urand(20, 40)*sin(angle*(M_PI/180));
                            m_creature->SummonCreature(MOB_DEATH_ORB, homeX, homeY, pYogg->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
                        }
                    }
                    //DoCast(m_creature, SPELL_DEATH_RAY);
                    m_uiDeathRayTimer = 30000;
                }
                else m_uiDeathRayTimer -= uiDiff;

                break;
            }
        }
    }
};

/*
*   Keepers
*/
struct MANGOS_DLL_DECL keeper_hodirAI : public ScriptedAI
{
    keeper_hodirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiHodirProtectivGazeTimer;

    void Reset()
    {
        m_uiHodirProtectivGazeTimer = 30000;
        m_creature->SetRespawnDelay(DAY);
        DoCast(m_creature, SPELL_FORTITUDE_OF_FROST);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_uiHodirProtectivGazeTimer < uiDiff)
        {
            if(Unit* pTemp = DoSelectLowestHpFriendly(100.0f))
                DoCast(pTemp, SPELL_HODIRS_PROTECTIVE_GAZE);
            m_uiHodirProtectivGazeTimer = 30000 + urand(10000,30000);
        }
        else m_uiHodirProtectivGazeTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL keeper_freyaAI : public ScriptedAI
{
    keeper_freyaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        DoCast(m_creature, SPELL_RESILIENCE_OF_NATURE);
        m_creature->SetRespawnDelay(DAY);
        // Summon Wells
        SummonSanityWells();
    }

    void SummonSanityWells()
    {
        for(uint8 i = 0; i < 5; i++)
            m_creature->SummonCreature(MOB_SANITY_WELL, SanityWellLoc[i].x, SanityWellLoc[i].y, SanityWellLoc[i].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 30000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
    }
};

struct MANGOS_DLL_DECL keeper_thorimAI : public ScriptedAI
{
    keeper_thorimAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bHasTitanicStorm;

    void Reset()
    {
        m_bHasTitanicStorm = false;
        m_creature->SetRespawnDelay(DAY);
        DoCast(m_creature, SPELL_FURY_OF_THE_STORM);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_pInstance->GetData(TYPE_YOGG_PHASE) == PHASE_OLD_GOD && !m_bHasTitanicStorm)
        {
            DoCast(m_creature, SPELL_TITANIC_STORM);
            m_bHasTitanicStorm = true;
        }
    }
};

struct MANGOS_DLL_DECL keeper_mimironAI : public ScriptedAI
{
    keeper_mimironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiDestabilizationMatrixTimer;
    std::list<Creature*> lTentacleList;

    void Reset()
    {
        lTentacleList.clear();
        m_uiDestabilizationMatrixTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
        DoCast(m_creature, SPELL_SPEED_OF_INVENTION);
    }

    Creature* SelectRandomTentacle(float fRange)
    {
        GetCreatureListWithEntryInGrid(lTentacleList, m_creature, MOB_CRUSHER_TENTACLE, fRange);
        GetCreatureListWithEntryInGrid(lTentacleList, m_creature, MOB_CORRUPTOR_TENTACLE, fRange);

        if (lTentacleList.empty()){
            m_uiDestabilizationMatrixTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lTentacleList.begin();
        advance(iter, urand(0, lTentacleList.size()-1));

        return *iter;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_pInstance->GetData(TYPE_YOGG_PHASE) == PHASE_TENTACLES)
        {
            if (m_uiDestabilizationMatrixTimer < uiDiff)
            {
                if(Unit* pTentacle = SelectRandomTentacle(120.0f))
                    DoCast(pTentacle, SPELL_DESTABILIZATION_MATRIX);
                m_uiDestabilizationMatrixTimer = 30000 + urand(10000,30000);
            }
            else m_uiDestabilizationMatrixTimer -= uiDiff;
        }
    }
};

/*
*   Guardians
*/
struct MANGOS_DLL_DECL mob_immortal_guardianAI : public ScriptedAI
{
    mob_immortal_guardianAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStack;
    uint32 m_uiHealth;
	bool m_bHasAura;

    void Reset()
    {
        m_uiStack   = 0;
        m_uiHealth  = 90;
		m_bHasAura	= false;
        m_creature->SetRespawnDelay(DAY);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealthPercent() < 5)
        {
            if(uiDamage > m_creature->GetHealth())
                uiDamage = 0;
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(m_creature->GetHealthPercent() < 5)
        {
            if(spell->Id == SPELL_TITANIC_STORM_DMG && caster->GetEntry() == KEEPER_THORIM)
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void Aggro(Unit *who) 
    {
		DoCast(m_creature, SPELL_EMPOWERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		// hacky way of stacking aura, needs fixing
		if(SpellAuraHolder* empoweredAura = m_creature->GetSpellAuraHolder(SPELL_EMPOWERED))
		{
			if(empoweredAura->GetStackAmount() < 9 && !m_bHasAura)
			{
				m_bHasAura = true;
				empoweredAura->SetStackAmount(9);
			}
		}

        if(m_creature->GetHealthPercent() > 10)
        {
            if(m_creature->GetHealthPercent() < m_uiHealth)
            {
				if(SpellAuraHolder* empoweredAura = m_creature->GetSpellAuraHolder(SPELL_EMPOWERED))
				{
					if(empoweredAura->ModStackAmount(-1))
						m_creature->RemoveAurasDueToSpell(SPELL_EMPOWERED);
				}
                m_uiHealth -= 10;
            }
        }

        // empowering shadows, needs more research and core fix
        if(m_creature->GetHealthPercent() > m_uiHealth + 10)
        {
			DoCast(m_creature, SPELL_EMPOWERED);
            m_uiHealth += 10;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_guardian_of_yogg_saronAI : public ScriptedAI
{
    mob_guardian_of_yogg_saronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 m_uiDarkVolleyTimer;
    uint32 m_uiDieTimer;
    uint32 m_uiDominateMindTimer;
    bool m_bHasCasted;

    void Reset()
    {
        m_uiDarkVolleyTimer     = 10000;
        m_uiDominateMindTimer   = 30000;
        m_bHasCasted            = false;
        m_creature->SetRespawnDelay(DAY);
    }

	void DamageTaken(Unit *done_by, uint32 &uiDamage)
	{
		if(uiDamage > m_creature->GetHealth())
		{
			uiDamage = 0;
			if(!m_bHasCasted)
			{
				DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_NOVA : SPELL_SHADOW_NOVA_H);
				m_bHasCasted = true;
				m_uiDieTimer = 500;
				m_creature->SetHealth(m_creature->GetMaxHealth());

				// workaround for dmg Sara
				// need to find a way to damage Sara by spell!
				if(Creature* pSara = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_SARA)))
				{
					if(m_creature->IsWithinDist2d(pSara->GetPositionX(), pSara->GetPositionY(), 15.0f))
					{
						uint32 maxHealth = m_bIsRegularMode ? 21994 : 27500;
						if(pSara->GetHealth() > maxHealth)
							pSara->DealDamage(pSara, m_bIsRegularMode ? urand(20000, 21994) : urand(25000, 27500), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, NULL, false);
						else
						{
							((boss_saraAI*)pSara->AI())->m_bIsOutro = true;
							pSara->SetHealth(pSara->GetMaxHealth());
						}
					}
				}
			}
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(Creature* pSara = (Creature*)m_creature->GetMap()->GetUnit(m_pInstance->GetData64(NPC_SARA)))
        {
            if(m_creature->getVictim() == pSara)
            {
                m_creature->AddThreat(pSara, -100000.0f);
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_BOTTOMAGGRO, 0))
                    m_creature->AddThreat(pTarget, 1000000.0f);
            }
        }

        if (m_uiDarkVolleyTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DARK_VOLLEY);
            m_uiDarkVolleyTimer = 15000;
        }
        else m_uiDarkVolleyTimer -= uiDiff;

        if (m_uiDominateMindTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_DOMINATE_MIND);
            m_uiDominateMindTimer = 30000;
        }
        else m_uiDominateMindTimer -= uiDiff;

        if (m_uiDieTimer < uiDiff && m_bHasCasted)
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDieTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*
*   Tentacules
*/
struct MANGOS_DLL_DECL mob_corruptor_tentacleAI : public ScriptedAI
{
    mob_corruptor_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 m_uiEruptTimer;
    bool m_bHasErupted;
    uint32 m_uiSpellTimer;

    void Reset()
    {
        m_uiEruptTimer  = 500;
        m_bHasErupted   = false;
        m_uiSpellTimer  = 10000;
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEruptTimer < uiDiff && !m_bHasErupted)
        {
            DoCast(m_creature, SPELL_ERUPT);
            m_bHasErupted = true;
        }else m_uiEruptTimer -= uiDiff;

        if (m_uiSpellTimer < uiDiff)
        {
            switch(urand(0, 3))
            {
            case 0:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_BLACK_PLAGUE);
                break;
            case 1:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_APATHY);
                break;
            case 2:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_CURSE_OF_DOOM);
                break;
            case 3:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_DRAINING_POISON);
                break;
            }
            m_uiSpellTimer = urand(5000, 10000);
        }
        else m_uiSpellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_constrictor_tentacleAI : public ScriptedAI
{
    mob_constrictor_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 m_uiSqueezeTimer;
    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiSqueezeTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = m_creature->GetMap()->GetUnit( m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_SQUEEZE : SPELL_SQUEEZE_H);
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (Unit* pVictim = m_creature->GetMap()->GetUnit( m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_SQUEEZE : SPELL_SQUEEZE_H);

        if (pKiller)
            pKiller->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_SQUEEZE : SPELL_SQUEEZE_H);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) 
            pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_SQUEEZE : SPELL_SQUEEZE_H);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS)
        {
            if (Unit* pVictim = m_creature->GetMap()->GetUnit( m_uiVictimGUID))
                pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_SQUEEZE : SPELL_SQUEEZE_H);
            m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSqueezeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
				// spell needs vehicles
                //pTarget->CastSpell(pTarget, m_bIsRegularMode ? SPELL_SQUEEZE : SPELL_SQUEEZE_H, false);
                m_uiVictimGUID = pTarget->GetGUID();
            }
            m_uiSqueezeTimer = 30000;
        }else m_uiSqueezeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_crusher_tentacleAI : public ScriptedAI
{
    mob_crusher_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiEruptTimer;
    bool m_bHasErupted;
    uint32 m_uiDiminishPowerTimer;

    void Reset()
    {
        m_uiEruptTimer          = 500;
        m_bHasErupted           = false;
        m_uiDiminishPowerTimer  = 10000;
        m_creature->SetRespawnDelay(DAY);
        DoCast(m_creature, SPELL_FOCUSED_ANGER_TRIGG);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEruptTimer < uiDiff && !m_bHasErupted)
        {
            DoCast(m_creature, SPELL_ERUPT);
            m_bHasErupted = true;
        }
        else m_uiEruptTimer -= uiDiff;

        if (m_uiDiminishPowerTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DIMINISH_POWER);
            m_uiDiminishPowerTimer = 10000 + urand(1000, 5000);
        }
        else m_uiDiminishPowerTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_vision_tentacleAI : public ScriptedAI
{
    mob_vision_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    void Reset()
    { 
        DoCast(m_creature, SPELL_GRIM_REPRISAL);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > 0 && m_creature->GetDisplayId() != 28813)
            m_creature->SetDisplayId(28813);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

/*
*   Other
*/
struct MANGOS_DLL_DECL mob_death_orbAI : public ScriptedAI
{
    mob_death_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        pCreature->setFaction(14);
        Reset();
    }

    uint32 m_uiAuraTimer;
    bool m_bHasAura;

    void Reset()
    {
        m_uiAuraTimer   = 500;
        m_bHasAura      = false;
        DoCast(m_creature, SPELL_DEATH_RAY_VISUAL);
        m_creature->GetMotionMaster()->MoveConfused();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiAuraTimer < uiDiff && !m_bHasAura)
        {
            DoCast(m_creature, SPELL_DEATH_RAY_TRIGG);
            m_bHasAura = true;
        }
        else m_uiAuraTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_sanity_wellAI : public ScriptedAI
{
    mob_sanity_wellAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiAuraDelayTimer;
    bool m_bHasAura;

    void Reset()
    {
        m_uiAuraDelayTimer  = 1000;
        m_bHasAura          = false;
        DoCast(m_creature, SPELL_SANITY_WELL_VISUAL);
        m_creature->SetRespawnDelay(DAY);
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_uiAuraDelayTimer < uiDiff && !m_bHasAura)
        {
            // spell partially broken, it should be triggered by the visual aura
            //DoCast(m_creature, SPELL_SANITY_WELL);
            m_bHasAura = true;
        }
        else m_uiAuraDelayTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_laughing_skullAI : public ScriptedAI
{
    mob_laughing_skullAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetDisplayId(SKULL_DISPLAY_ID);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        pCreature->GetMotionMaster()->MoveConfused();
        Reset();
    }

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
		DoCast(m_creature, SPELL_LUNATIC_GAZE);
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
    }
};

struct MANGOS_DLL_DECL mob_ominous_cloudAI : public ScriptedAI
{
    mob_ominous_cloudAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        pCreature->setFaction(14);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 m_uiRangeCheckTimer;
    bool m_bSummonGuardian;

    void Reset()
    {
        m_uiRangeCheckTimer = 1000;
        m_bSummonGuardian   = false;
        DoCast(m_creature, SPELL_OMINOUS_CLOUD_VISUAL);
		m_creature->SetRespawnDelay(DAY);
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void JustSummoned(Creature* pSummon)
    {
        pSummon->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiRangeCheckTimer < uiDiff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) <= 5)
                        m_bSummonGuardian = true;

                }
            } 
            // cast summon guard
            if(m_bSummonGuardian)
            {
                DoCast(m_creature, SPELL_SUMMON_GUARDIAN2);
                m_uiRangeCheckTimer = 11000;
                m_bSummonGuardian = false;
            }
            else
                m_uiRangeCheckTimer = 1000;
        }
        else m_uiRangeCheckTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_yogg_saron(Creature* pCreature)
{
    return new boss_yogg_saronAI(pCreature);
}

CreatureAI* GetAI_boss_sara(Creature* pCreature)
{
    return new boss_saraAI(pCreature);
}

CreatureAI* GetAI_mob_madness_portal(Creature* pCreature)
{
    return new mob_madness_portalAI(pCreature);
}

CreatureAI* GetAI_boss_brain_of_yogg_saron(Creature* pCreature)
{
    return new boss_brain_of_yogg_saronAI(pCreature);
}

CreatureAI* GetAI_mob_corruptor_tentacle(Creature* pCreature)
{
    return new mob_corruptor_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_constrictor_tentacle(Creature* pCreature)
{
    return new mob_constrictor_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_crusher_tentacle(Creature* pCreature)
{
    return new mob_crusher_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_vision_tentacle(Creature* pCreature)
{
    return new mob_vision_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_guardian_of_yogg_saron(Creature* pCreature)
{
    return new mob_guardian_of_yogg_saronAI(pCreature);
}

CreatureAI* GetAI_mob_immortal_guardian(Creature* pCreature)
{
    return new mob_immortal_guardianAI(pCreature);
}

CreatureAI* GetAI_keeper_hodir(Creature* pCreature)
{
    return new keeper_hodirAI(pCreature);
}

CreatureAI* GetAI_keeper_freya(Creature* pCreature)
{
    return new keeper_freyaAI(pCreature);
}

CreatureAI* GetAI_keeper_thorim(Creature* pCreature)
{
    return new keeper_thorimAI(pCreature);
}

CreatureAI* GetAI_keeper_mimiron(Creature* pCreature)
{
    return new keeper_mimironAI(pCreature);
}

CreatureAI* GetAI_mob_death_orb(Creature* pCreature)
{
    return new mob_death_orbAI(pCreature);
}

CreatureAI* GetAI_mob_sanity_well(Creature* pCreature)
{
    return new mob_sanity_wellAI(pCreature);
}

CreatureAI* GetAI_mob_laughing_skull(Creature* pCreature)
{
    return new mob_laughing_skullAI(pCreature);
}

CreatureAI* GetAI_mob_ominous_cloud(Creature* pCreature)
{
    return new mob_ominous_cloudAI(pCreature);
}

void AddSC_boss_yogg_saron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_yogg_saron";
    newscript->GetAI = &GetAI_boss_yogg_saron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_sara";
    newscript->GetAI = &GetAI_boss_sara;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_madness_portal";
    newscript->GetAI = &GetAI_mob_madness_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_brain_of_yogg_saron";
    newscript->GetAI = &GetAI_boss_brain_of_yogg_saron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_corruptor_tentacle";
    newscript->GetAI = &GetAI_mob_corruptor_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_constrictor_tentacle";
    newscript->GetAI = &GetAI_mob_constrictor_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_crusher_tentacle";
    newscript->GetAI = &GetAI_mob_crusher_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vision_tentacle";
    newscript->GetAI = &GetAI_mob_vision_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_guardian_of_yogg_saron";
    newscript->GetAI = &GetAI_mob_guardian_of_yogg_saron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_immortal_guardian";
    newscript->GetAI = &GetAI_mob_immortal_guardian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_hodir";
    newscript->GetAI = &GetAI_keeper_hodir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_freya";
    newscript->GetAI = &GetAI_keeper_freya;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_thorim";
    newscript->GetAI = &GetAI_keeper_thorim;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_mimiron";
    newscript->GetAI = &GetAI_keeper_mimiron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_death_orb";
    newscript->GetAI = &GetAI_mob_death_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_sanity_well";
    newscript->GetAI = &GetAI_mob_sanity_well;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_laughing_skull";
    newscript->GetAI = &GetAI_mob_laughing_skull;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ominous_cloud";
    newscript->GetAI = &GetAI_mob_ominous_cloud;
    newscript->RegisterSelf();
}
