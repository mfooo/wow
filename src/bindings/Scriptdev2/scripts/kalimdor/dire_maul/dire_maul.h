/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_DIRE_MAUL_H
#define DEF_DIRE_MAUL_H

enum
{
    MAX_ENCOUNTER           = 4,

    TYPE_ZERVIM             = 0,
    TYPE_HYDROSPAWN         = 1,
    TYPE_LETHTENDRIS        = 2,
    TYPE_IRONBARK           = 3,

    GO_CONSERVATORY_DOOR    = 176907
};

class MANGOS_DLL_DECL instance_dire_maul : public ScriptedInstance
{
    public:
        instance_dire_maul(Map* pMap);
        ~instance_dire_maul() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiConservatoryDoorGUID;
};
#endif
