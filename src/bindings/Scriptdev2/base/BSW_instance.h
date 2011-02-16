/* Copyright (C) 2009 - 2011 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef BSW_INSTANCE_H
#define BSW_INSTANCE_H
#define BSW_INSTANCE_VERSION 0.6.14
#include "sc_instance.h"

class MANGOS_DLL_DECL BSWScriptedInstance : public ScriptedInstance
{
    public:

        BSWScriptedInstance(Map* pMap);
        ~BSWScriptedInstance();

        //sends completed achievments to all players in instance
        void DoCompleteAchievement(uint32 uiAchievmentId);
        void DoOpenDoor(uint64 guid);
        void DoCloseDoor(uint64 guid);

        void   SetNextEvent(uint32 EventNum, uint32 creatureID, uint32 timer = 1000);
        uint32 GetEvent(uint32 creatureID);
        bool   GetEventTimer(uint32 creatureID, const uint32 diff);

    private:
        uint32 m_auiEvent;
        uint32 m_auiCreatureID;
        uint32 m_auiEventTimer;
        bool   m_auiEventLock;
        Map*   m_pMap;
};
#endif
