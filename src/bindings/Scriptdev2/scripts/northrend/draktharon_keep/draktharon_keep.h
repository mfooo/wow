/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_DRAKTHARON_KEEP_H
#define DEF_DRAKTHARON_KEEP_H

enum
{
    MAX_ENCOUNTER                   = 4,

    TYPE_TROLLGORE                  = 0,
    TYPE_NOVOS                      = 1,
    TYPE_KING_DRED                  = 2,
    TYPE_THARONJA                   = 3,

    NPC_KING_DRED                   = 27483,

    TYPE_CRYSTAL_1                = 5,
    TYPE_CRYSTAL_2                = 6,
    TYPE_CRYSTAL_3                = 7,
    TYPE_CRYSTAL_4                = 8,
    TYPE_NOVOS_PHASE2_CHECK       = 9,
    TYPE_NOVOS_EVENT              = 10,

    NPC_CRYSTAL_CHANNEL_TARGET    = 26710,
    NPC_CRYSTAL_CHANNEL           = 26712,
    NPC_TRIGGER_TARGET            = 26714,
    NPC_NOVOS                     = 26631,

    NPC_CRYSTAL_HANDLER           = 26627,
    NPC_HULKING_CORPSE            = 27597,
    NPC_FETID_TROLL_CORPSE        = 27598,
    NPC_RISEN_SHADOWCASTER        = 27600,

    // Adds of King Dred Encounter - deaths counted for achievement
    NPC_DRAKKARI_GUTRIPPER          = 26641,
    NPC_DRAKKARI_SCYTHECLAW         = 26628,

    // Achievement Criterias to be handled with SD2
    ACHIEV_CRIT_BETTER_OFF_DREAD    = 7318,
    ACHIEV_CRIT_CONSUME_JUNCTION    = 7579,
    ACHIEV_CRIT_OH_NOVOS            = 7361,
};

class MANGOS_DLL_DECL instance_draktharon_keep : public ScriptedInstance
{
    public:
        instance_draktharon_keep(Map* pMap);
        ~instance_draktharon_keep() {}

        void Initialize();

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint32 m_uiDreadAddsKilled;
        bool m_bNovosAddGrounded;
        bool m_bTrollgoreConsume;
};

#endif
