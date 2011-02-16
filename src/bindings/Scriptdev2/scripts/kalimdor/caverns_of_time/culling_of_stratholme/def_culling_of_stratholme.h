/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: instance_culling_of_stratholme
SD%Complete: ?%
SDComment: by MaxXx2021
SDCategory: Culling of Stratholme
EndScriptData */

#ifndef DEF_CULLING_OF_STRATHOLME_H
#define DEF_CULLING_OF_STRATHOLME_H

enum Data
{
    TYPE_QUEST           = 1,
    TYPE_INTRO           = 2,
    TYPE_CRATES_COUNT    = 3,
    TYPE_PHASE           = 4,
    TYPE_ENCOUNTER       = 5,
    TYPE_WAVE_COUNT      = 6,
    TYPE_WING            = 7,
    TYPE_BONUS           = 8,
    TYPE_MALGANIS        = 9,

    DATA_TEMPSUMMON      = 10,

    QUEST_DISPELLING_ILLUSIONS  = 13149,
    QUEST_A_ROYAL_ESCORT        = 13151,
    ITEM_ARCANE_DISRUPTOR       = 37888,

    NPC_CHROMI01            = 26527,
    NPC_CHROMI02            = 27915,
    NPC_ARTHAS              = 26499,
    NPC_JAINA               = 26497,
    NPC_UTHER               = 26528,
    NPC_KNIGHT              = 28612,
    NPC_MIKE                = 30571,
    NPC_MAL_CORICS          = 31017,
    NPC_GRIAN_STONE         = 30561,
    NPC_JAMES               = 30553,
    NPC_FRAS_FRASIABI       = 30552,
    NPC_FORRESTER           = 30551,
    NPC_ROGER               = 27903,
    NPC_CRATE               = 30996,
    NPC_MORIGAN             = 27877,
    NPC_PERELLI             = 27876,
    NPC_JENA                = 27885,
    NPC_MARTHA              = 27884,
    NPC_MALCOLM             = 27891,
    NPC_DOG                 = 27892,
    NPC_BARTLEBY            = 27907,
    NPC_MARINE              = 27745,
    NPC_PRIEST              = 27747,

    NPC_INFINITE_ADVERSARY  = 27742,
    NPC_INFINITE_HUNTER     = 27743,
    NPC_INFINITE_AGENT      = 27744,
    NPC_TIME_RIFT           = 28409,
    NPC_TIME_RIFT_2         = 28439,
    NPC_ZOMBIE              = 27737,
    NPC_GHOUL               = 28249,
    NPC_NECROMANCER         = 28200,
    NPC_STALKER             = 28199,
    NPC_FIEND               = 27734,
    NPC_GOLEM               = 28201,
    NPC_EGHOUL              = 27729,
    NPC_CONSTRUCT           = 27736,
    NPC_ACOLYTE             = 27731,
    NPC_MEATHOOK            = 26529,
    NPC_SALRAMM             = 26530,
    NPC_EPOCH               = 26532,
    NPC_MALGANIS            = 26533,
    NPC_CITY                = 28167,
    NPC_INFINITE_CORRUPTOR  = 32273,

    GO_CRATE_LIGHT         = 190117,
    GO_SHKAF_GATE          = 188686,
    GO_MALGANIS_GATE1      = 187711,
    GO_MALGANIS_GATE2      = 187723,
    GO_MALGANIS_CHEST      = 190663,
    GO_MALGANIS_CHEST_H    = 193597,
    GO_EXIT                = 191788,


    WORLD_STATE_COS_TIME_ON       = 3932,
    WORLD_STATE_COS_TIME_COUNT    = 3931,
    WORLD_STATE_COS_WAVE_COUNT    = 3504,
    WORLD_STATE_COS_CRATE_ON      = 3479,
    WORLD_STATE_COS_CRATE_COUNT   = 3480,

    RIGHT               = 1,
    LEFT                = 2
};

#endif
