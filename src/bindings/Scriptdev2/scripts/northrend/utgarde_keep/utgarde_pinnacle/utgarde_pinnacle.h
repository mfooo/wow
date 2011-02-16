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
SDName: instance_pinnacle
SD%Complete: %
SDComment:
SDCategory: Utgarde Pinnacle
TODO::
// dev // FallenAngelX
EndScriptData */

#ifndef DEF_NEXUS_H
#define DEF_NEXUS_H

enum
{
    MAX_ENCOUNTER                   = 5,

    TYPE_SVALA                      = 0,
    TYPE_GORTOK                     = 1,
    TYPE_SKADI                      = 2,
    TYPE_YMIRON                     = 3,
    TYPE_HARPOONLUNCHER             = 4,

    DATA_BJORN                      = 10,
    DATA_HALDOR                     = 11,
    DATA_RANULF                     = 12,
    DATA_TORGYN                     = 13,
    DATA_YMIRON                     = 14,

    GO_STASIS_GENERATOR             = 188593,
    GO_DOOR_SKADI                   = 192173,
    GO_DOOR_YMIRON                  = 192174,
    GO_HARPOON1                     = 192175,
    GO_HARPOON2                     = 192176,
    GO_HARPOON3                     = 192177,

    NPC_FURBOLG                     = 26684,
    NPC_WORGEN                      = 26683,
    NPC_JORMUNGAR                   = 26685,
    NPC_RHINO                       = 26686,
    NPC_BJORN                       = 27303,
    NPC_HALDOR                      = 27307,
    NPC_RANULF                      = 27308,
    NPC_TORGYN                      = 27309,
    NPC_YMIRON                      = 26861
};

#endif
