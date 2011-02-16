/* Copyright (C) 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/* ScriptData
SDName: oculus.h
SD%Complete: ??%
SDComment: /dev/rsa/ && dev // FallenAngelX
SDCategory:
EndScriptData */



#ifndef DEF_OCULUS_H
#define DEF_OCULUS_H
#include "BSW_ai.h"
#include "BSW_instance.h"

enum
{
    TYPE_DRAKOS,
    TYPE_VAROS,
    TYPE_UROM,
    TYPE_EREGOS,
    TYPE_ROBOTS,
    TYPE_UROM_PHASE,
    MAX_ENCOUNTERS,

    DATA_DRAKOS,
    DATA_VAROS,
    DATA_UROM,
    DATA_EREGOS,

    NPC_ROBOT              = 27641,
    NPC_BALGAR_IMAGE       = 28012,
    NPC_DRAKOS             = 27654,
    NPC_VAROS              = 27447,
    NPC_UROM               = 27655,
    NPC_EREGOS             = 27656,

    GO_DRAGON_CAGE_DOOR    = 193995,
    GO_EREGOS_CACHE        = 191349,
    GO_EREGOS_CACHE_H      = 193603,
    GO_SPOTLIGHT           = 191351,
    GO_ORB_OF_NEXUS        = 188715,
    GO_NEXUS_PORTAL        = 189985,

    BELGAR_TEXT_0          = 13267,
    BELGAR_TEXT_1          = 13268,
    BELGAR_TEXT_2          = 13269,

};

#endif
