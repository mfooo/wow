/* Copyright (C) 2009 - 2011 Project Dark-iCE <http://projectSD2.clanice.com/>
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
SDName: custom_npc_spelltrainer
SD%Complete: 100
SDComment: Spell Trainer - One-Click spell teacher.
SDCategory: NPC
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
EndScriptData */

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../config.h"

#define MSG_COMBAT                  "You are in combat!"
#define MSG_LEARNED_SPELL           "Congratulations, you've learned all of your class spells!"
#define MSG_LEARNED_WEAPON          "Congratulations, you've learned all your class weapon skills!"
#define MSG_LEARNED_LANGUAGES       "Congratulations, you've learned all languages!"
#define MSG_MAXED_WEAPON            "Congratulations, you've maxed out your weapon skills!"
#define MSG_MAXED_LANGUAGES         "Congratulations, you've maxed out all languages!"
#define MSG_MAXED_RIDING            "Congratulations, you've maxed out your riding skills!"
#define CLASS_WARRIOR               1
#define CLASS_PALADIN               2
#define CLASS_HUNTER                3
#define CLASS_ROGUE                 4
#define CLASS_PRIEST                5
#define CLASS_DEATHKNIGHT           6
#define CLASS_SHAMAN                7
#define CLASS_MAGE                  8
#define CLASS_WARLOCK               9
#define CLASS_DRUID                 11


bool GossipHello_custom_npc_spellmaster(Player* pPlayer, Creature* pCreature)
{
    Config SD2Config;
    if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
        error_log("SD2: Unable to open configuration file");
        
    bool LearnAllWeapons            = SD2Config.GetBoolDefault("Spellmaster.LearnAllWeapons", true);
    bool LearnAllLanguages          = SD2Config.GetBoolDefault("Spellmaster.LearnAllLanguages", true);
    bool MaxOutWeaponSkills         = SD2Config.GetBoolDefault("Spellmaster.MaxOutWeaponSkills", true);
    bool MaxOutRidingSkills         = SD2Config.GetBoolDefault("Spellmaster.MaxOutRidingSkills", true);
    bool MaxOutLanguageSkills       = SD2Config.GetBoolDefault("Spellmaster.MaxOutAllLanguageSkills", true);

    if(pPlayer->getLevel() > (SD2Config.GetFloatDefault("SpellMaster.MinimumLevel",0)))
    {
        // Warrior Spells
        if(pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Warrior spells"            , GOSSIP_SENDER_MAIN, 1000);
        // Paladin Spells
        if(pPlayer->getClass() == CLASS_PALADIN)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Paladin spells"            , GOSSIP_SENDER_MAIN, 2000);
        // Hunter Spells
        if(pPlayer->getClass() == CLASS_HUNTER)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Hunter spells"            , GOSSIP_SENDER_MAIN, 3000);
        // Rogue Spells
        if(pPlayer->getClass() == CLASS_ROGUE)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Rogue spells"            , GOSSIP_SENDER_MAIN, 4000);
        // Priest Spells
        if(pPlayer->getClass() == CLASS_PRIEST)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Priest spells"            , GOSSIP_SENDER_MAIN, 5000);
        // Death Knight Spells
        if(pPlayer->getClass() == CLASS_DEATHKNIGHT)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Death Knight spells"    , GOSSIP_SENDER_MAIN, 6000);
        // Shaman Spells
        if(pPlayer->getClass() == CLASS_SHAMAN)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Shaman spells"            , GOSSIP_SENDER_MAIN, 7000);
        // Mage Spells
        if(pPlayer->getClass() == CLASS_MAGE)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Mage spells"            , GOSSIP_SENDER_MAIN, 8000);
        // Warlock Spells
        if(pPlayer->getClass() == CLASS_WARLOCK)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Warlock spells"            , GOSSIP_SENDER_MAIN, 9000);
        // Druid Spells
        if(pPlayer->getClass() == CLASS_DRUID)
            pPlayer->ADD_GOSSIP_ITEM( 3, "Learn all Druid spells"            , GOSSIP_SENDER_MAIN, 10000);
        // Weapon Skills
        if(LearnAllWeapons)
            pPlayer->ADD_GOSSIP_ITEM( 9, "Learn my class weapon skills"        , GOSSIP_SENDER_MAIN, 11000);
        // Max out all weapon skills
        if(MaxOutWeaponSkills)
            pPlayer->ADD_GOSSIP_ITEM( 9, "Max out weapon skills"            , GOSSIP_SENDER_MAIN, 12000);
        // Max out all riding skills
        if(MaxOutRidingSkills)
            pPlayer->ADD_GOSSIP_ITEM( 2, "Max out riding skills"            , GOSSIP_SENDER_MAIN, 13000);
        // Learn all languages
        if(LearnAllLanguages)
            pPlayer->ADD_GOSSIP_ITEM( 0, "Learn all languages"                , GOSSIP_SENDER_MAIN, 14000);
        // Max out all languages
        if(MaxOutLanguageSkills)
            pPlayer->ADD_GOSSIP_ITEM( 7, "Max out language skills"            , GOSSIP_SENDER_MAIN, 15000);
        
        pPlayer->ADD_GOSSIP_ITEM( 0, "Not Interested"                    , GOSSIP_SENDER_MAIN, 99000);
    }
    else
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterWhisper("You need more levels to learn the spells!", pPlayer, false);
    }

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
    return true;
}

void SendDefaultMenu_custom_npc_spellmaster(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

    // Not allow in combat
    if (pPlayer->isInCombat())
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterSay(MSG_COMBAT, LANG_UNIVERSAL, NULL);
        return;
    }

    switch(uiAction)
    {
        case 1000: //Warrior Spells
            pPlayer->learnSpell(47436, false);      // Battle Shout  [Rank 9]
            pPlayer->learnSpell(18499, false);      // Beserker Rage
            pPlayer->learnSpell(2458, false);       // Beserker Stance
            pPlayer->learnSpell(2687, false);       // Bloodrage
            pPlayer->learnSpell(1161, false);       // Challenging Shout
            pPlayer->learnSpell(11578, false);      // Charge [Rank 3]
            pPlayer->learnSpell(47520, false);      // Cleave [Rank 8]
            pPlayer->learnSpell(469, false);        // Commanding Shout [Rank 1]
            pPlayer->learnSpell(47439, false);      // Commanding Shout [Rank 2]
            pPlayer->learnSpell(47440, false);      // Commanding Shout [Rank 3]
            pPlayer->learnSpell(71, false);         // Defensive Stance
            pPlayer->learnSpell(47437, false);      // Demoralizing Shout [Rank 8]
            pPlayer->learnSpell(676, false);        // Disarm
            pPlayer->learnSpell(55694, false);      // Enraged Regeneration
            pPlayer->learnSpell(47471, false);      // Execute [Rank 9]
            pPlayer->learnSpell(1715, false);       // Hamstring
            pPlayer->learnSpell(47450, false);      // Heroic Strike [Rank 13]
            pPlayer->learnSpell(57755, false);      // Heroic Throw
            pPlayer->learnSpell(20252, false);      // Intercept
            pPlayer->learnSpell(3411, false);       // Intervene
            pPlayer->learnSpell(5246, false);       // Intimidating Shout
            pPlayer->learnSpell(694, false);        // Mocking Blow
            pPlayer->learnSpell(7384, false);       // Overpower
            pPlayer->learnSpell(6552, false);       // Pummel
            pPlayer->learnSpell(1719, false);       // Recklessness
            pPlayer->learnSpell(47471, false);      // Rend [Rank 10]
            pPlayer->learnSpell(20230, false);      // Retaliation
            pPlayer->learnSpell(57823, false);      // Revenge  [Rank 9]
            pPlayer->learnSpell(64382, false);      // Shattering Throw
            pPlayer->learnSpell(72, false);         // Shield Bash        
            pPlayer->learnSpell(2565, false);       // Shield Block
            pPlayer->learnSpell(47488, false);      // Shield Slam [Rank 8]
            pPlayer->learnSpell(871, false);        // Shield Wall
            pPlayer->learnSpell(47475, false);      // Slam [Rank 8]
            pPlayer->learnSpell(23920, false);      // Spell Reflection
            pPlayer->learnSpell(12678, false);      // Stance Mastery
            pPlayer->learnSpell(7386, false);       // Sunder Armor 
            pPlayer->learnSpell(355, false);        // Taunt
            pPlayer->learnSpell(47502, false);      // Thunder Clap [Rank 9]
            pPlayer->learnSpell(34428, false);      // Victory Rush
            pPlayer->learnSpell(1680, false);       // Whirlwind
            pPlayer->learnSpell(750, false);        // Plate Mail
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);

        break;

        case 2000: //Paladin Spells
            pPlayer->learnSpell(31884, false);      // Avenging Wrath
            pPlayer->learnSpell(20217, false);      // Blessing of Kings
            pPlayer->learnSpell(48932, false);      // Blessing of Might [Rank 10]
            pPlayer->learnSpell(48936, false);      // Blessing of Wisdom [Rank 9]
            pPlayer->learnSpell(4987, false);       // Cleanse
            pPlayer->learnSpell(19746, false);      // Concentration Aura
            pPlayer->learnSpell(48810, false);      // Consecration [Rank 8]
            pPlayer->learnSpell(32223, false);      // Crusader Aura
            pPlayer->learnSpell(48942, false);      // Devotion Aura [Rank 10]
            pPlayer->learnSpell(19752, false);      // Divine Intervention
            pPlayer->learnSpell(54428, false);      // Divine Plea
            pPlayer->learnSpell(498, false);        // Divine Protection
            pPlayer->learnSpell(642, false);        // Divine Shield
            pPlayer->learnSpell(48801, false);      // Exorcism [Rank 9]
            pPlayer->learnSpell(48947, false);      // Fire Resistance Aura [Rank 5]
            pPlayer->learnSpell(48785, false);      // Flash of Light [Rank 9]
            pPlayer->learnSpell(48945, false);      // Frost Resistance Aura [Rank 5]
            pPlayer->learnSpell(25898, false);      // Greater Blessing of Kings
            pPlayer->learnSpell(48934, false);      // Greater Blessing of Might [Rank 5]
            pPlayer->learnSpell(25899, false);      // Greater Blessing of Sanctuary
            pPlayer->learnSpell(48938, false);      // Greater Blessing of Wisdom [Rank 5]
            pPlayer->learnSpell(48806, false);      // Hammer of Wrath [Rank 6]
            pPlayer->learnSpell(1044, false);       // Hand of Freedom
            pPlayer->learnSpell(10278, false);      // Hand of Protection [Rank 3]
            pPlayer->learnSpell(62124, false);      // Hand of Reckoning
            pPlayer->learnSpell(6940, false);       // Hand of Sacrifice
            pPlayer->learnSpell(1038, false);       // Hand of Salvation
            pPlayer->learnSpell(48782, false);      // Holy Light [Rank 13]
            pPlayer->learnSpell(48817, false);      // Holy Wrath [Rank 5]
            pPlayer->learnSpell(53407, false);      // Judgement of Justice
            pPlayer->learnSpell(20271, false);      // Judgement of Light
            pPlayer->learnSpell(53408, false);      // Judgement of Wisdom
            pPlayer->learnSpell(48788, false);      // Lay on Hands [Rank 5]
            pPlayer->learnSpell(48950, false);      // Redemption [Rank 7]
            pPlayer->learnSpell(54043, false);      // Retribution Aura [Rank 7]
            pPlayer->learnSpell(31789, false);      // Righteous Defense
            pPlayer->learnSpell(25780, false);      // Righteous Fury
            pPlayer->learnSpell(53601, false);      // Sacred Shield [Rank 1]
            pPlayer->learnSpell(20164, false);      // Seal of Justice
            pPlayer->learnSpell(20165, false);      // Seal of Light
            pPlayer->learnSpell(21084, false);      // Seal of Righteousness
            pPlayer->learnSpell(20166, false);      // Seal of Wisdom
            pPlayer->learnSpell(48943, false);      // Shadow Resistance Aura [Rank 5]
            pPlayer->learnSpell(5502, false);       // Sense Undead
            pPlayer->learnSpell(10326, false);      // Turn Evil
            if(pPlayer->getRace() == 10)
            pPlayer->learnSpell(34769, false);      // Summon Warhorse Horde
            else if(pPlayer->getRace() == 1 || 3 || 11)
            pPlayer->learnSpell(13819, false);      // Summon Warhorse Alliance
            if(pPlayer->getRace() == 10) 
            pPlayer->learnSpell(2825, false);       // Seal of Corruption
            else if(pPlayer->getRace() == 1 || 3 || 11) 
            pPlayer->learnSpell(32182, false);      // Seal of Vengeance
            if(pPlayer->getRace() == 10) 
            pPlayer->learnSpell(34767, false);      // Summon Charger Horde
            else if(pPlayer->getRace() == 1 || 3 || 11)
            pPlayer->learnSpell(23214, false);      // Summon Charger Alliance
            pPlayer->learnSpell(750, false);        // Plate Mail
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 3000: //Hunter Spells
            pPlayer->learnSpell(49045, false);      // Arcane Shot [Rank 11]
            pPlayer->learnSpell(13161, false);      // Aspect of the Beast
            pPlayer->learnSpell(5118, false);       // Aspect of the Cheetah
            pPlayer->learnSpell(61847, false);      // Aspect of the Dragonhawk [Rank 2]
            pPlayer->learnSpell(27044, false);      // Aspect of the Hawk [Rank 8]
            pPlayer->learnSpell(13163, false);      // Aspect of the Monkey
            pPlayer->learnSpell(13159, false);      // Aspect of the Pack
            pPlayer->learnSpell(34074, false);      // Aspect of the Viper
            pPlayer->learnSpell(49071, false);      // Aspect of the Wild [Rank 4] 
            pPlayer->learnSpell(1462, false);       // Beast Lore 
            pPlayer->learnSpell(883, false);        // Call Pet
            pPlayer->learnSpell(62757, false);      // Call Stabled Pet
            pPlayer->learnSpell(5116, false);       // Concussive Shot
            pPlayer->learnSpell(19263, false);      // Deterrence
            pPlayer->learnSpell(781, false);        // Disengage 
            pPlayer->learnSpell(2641, false);       // Dismiss Pet
            pPlayer->learnSpell(20736, false);      // Distracting Shot [Rank 1]
            pPlayer->learnSpell(6197, false);       // Eagle Eye
            pPlayer->learnSpell(49067, false);      // Explosive Trap [Rank 6]
            pPlayer->learnSpell(1002, false);       // Eyes of the Beast
            pPlayer->learnSpell(6991, false);       // Feed Pet
            pPlayer->learnSpell(5384, false);       // Feign Death
            pPlayer->learnSpell(1543, false);       // Flare
            pPlayer->learnSpell(14311, false);      // Freezing Trap [Rank 3]
            pPlayer->learnSpell(13809, false);      // Frost Trap
            pPlayer->learnSpell(53338, false);      // Hunter's Mark [Rank 5]
            pPlayer->learnSpell(49056, false);      // Immolation Trap [Rank 8]
            pPlayer->learnSpell(34026, false);      // Kill Command
            pPlayer->learnSpell(61006, false);      // Kill Shot [Rank 3]
            pPlayer->learnSpell(53271, false);      // Master's Call    
            pPlayer->learnSpell(48990, false);      // Mend Pet [Rank 10]
            pPlayer->learnSpell(34477, false);      // Misdirection
            pPlayer->learnSpell(53339, false);      // Mongoose Bite [Rank 6]
            pPlayer->learnSpell(49048, false);      // Multi-Shot [Rank 8]
            pPlayer->learnSpell(3045, false);       // Rapid Fire
            pPlayer->learnSpell(48996, false);      // Raptor Strike [Rank 11]
            pPlayer->learnSpell(982, false);        // Revive Pet
            pPlayer->learnSpell(14327, false);      // Scare Beast [Rank 3]
            pPlayer->learnSpell(3043, false);       // Scorpid Sting
            pPlayer->learnSpell(49001, false);      // Serpent Sting [Rank 12]
            pPlayer->learnSpell(34600, false);      // Snake Trap
            pPlayer->learnSpell(49052, false);      // Steady Shot [Rank 4]
            pPlayer->learnSpell(1515, false);       // Tame Beast
            pPlayer->learnSpell(19801, false);      // Tranquilizing Shot
            pPlayer->learnSpell(3034, false);       // Viper Sting
            pPlayer->learnSpell(58434, false);      // Volley [Rank 6]
            pPlayer->learnSpell(2974, false);       // Wing Clip
            pPlayer->learnSpell(1494, false);       // Track Beasts
            pPlayer->learnSpell(19878, false);      // Track Demons
            pPlayer->learnSpell(19879, false);      // Track Dragonkin
            pPlayer->learnSpell(19880, false);      // Track Elementals
            pPlayer->learnSpell(19882, false);      // Track Giants
            pPlayer->learnSpell(19885, false);      // Track Hidden
            pPlayer->learnSpell(19883, false);      // Track Humanoids
            pPlayer->learnSpell(19884, false);      // Track Undead
            pPlayer->learnSpell(8737, false);       // Mail
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 4000: // Rogue Spells
            pPlayer->learnSpell(48691, false);      // Ambush [Rank 10]
            pPlayer->learnSpell(48657, false);      // Backstab [Rank 12]
            pPlayer->learnSpell(2094, false);       // Blind
            pPlayer->learnSpell(1833, false);       // Cheap Shot
            pPlayer->learnSpell(31224, false);      // Cloak of Shadows
            pPlayer->learnSpell(48674, false);      // Deadly Throw [Rank 3]
            pPlayer->learnSpell(2836, false);       // Detect Traps
            pPlayer->learnSpell(1842, false);       // Disarm Trap
            pPlayer->learnSpell(51722, false);      // Dismantle
            pPlayer->learnSpell(1725, false);       // Distract
            pPlayer->learnSpell(57993, false);      // Envenom [Rank 4]
            pPlayer->learnSpell(26669, false);      // Evasion [Rank 2]
            pPlayer->learnSpell(48668, false);      // Eviscerate [Rank 12]
            pPlayer->learnSpell(8647, false);       // Expose Armor
            pPlayer->learnSpell(51723, false);      // Fan of Knives
            pPlayer->learnSpell(48659, false);      // Feint [Rank 8]
            pPlayer->learnSpell(48676, false);      // Garrote [Rank 10]
            pPlayer->learnSpell(1776, false);       // Gouge
            pPlayer->learnSpell(1766, false);       // Kick
            pPlayer->learnSpell(8643, false);       // Kidney Shot [Rank 2]
            pPlayer->learnSpell(1804, false);       // Pick Lock
            pPlayer->learnSpell(921, false);        // Pick Pocket
            pPlayer->learnSpell(48672, false);      // Rupture [Rank 9]
            pPlayer->learnSpell(1860, false);       // Safe Fall
            pPlayer->learnSpell(51724, false);      // Sap [Rank 4]
            pPlayer->learnSpell(5938, false);       // Shiv
            pPlayer->learnSpell(48638, false);      // Sinister Strike [Rank 12]
            pPlayer->learnSpell(6774, false);       // Slice and Dice [Rank 2]
            pPlayer->learnSpell(11305, false);      // Sprint [Rank 3]
            pPlayer->learnSpell(1787, false);       // Stealth [Rank 4]
            pPlayer->learnSpell(57934, false);      // Tricks of the Trade
            pPlayer->learnSpell(26889, false);      // Vanish [Rank 3]
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 5000: // Priest Spells
            pPlayer->learnSpell(552, false);        // Abolish Disease
            pPlayer->learnSpell(48120, false);      // Binding Heal [Rank 3]
            pPlayer->learnSpell(528, false);        // Cure Disease
            pPlayer->learnSpell(48300, false);      // Devouring Plague [Rank 9]
            pPlayer->learnSpell(988, false);        // Dispel Magic [Rank 2]
            pPlayer->learnSpell(64843, false);      // Divine Hymn
            pPlayer->learnSpell(48073, false);      // Divine Spirit [Rank 6]
            pPlayer->learnSpell(586, false);        // Fade
            pPlayer->learnSpell(6346, false);       // Fear Ward
            pPlayer->learnSpell(48071, false);      // Flash Heal [Rank 11]
            pPlayer->learnSpell(48063, false);      // Greater Heal [Rank 9]
            pPlayer->learnSpell(6064, false);       // Heal [Rank 4]
            pPlayer->learnSpell(48135, false);      // Holy Fire [Rank 11]
            pPlayer->learnSpell(48078, false);      // Holy Nova [Rank 9]
            pPlayer->learnSpell(64901, false);      // Hymn of Hope
            pPlayer->learnSpell(48168, false);      // Inner Fire [Rank 9]
            pPlayer->learnSpell(2053, false);       // Lesser Heal [Rank 3]
            pPlayer->learnSpell(1706, false);       // Levitate
            pPlayer->learnSpell(8129, false);       // Mana Burn
            pPlayer->learnSpell(32375, false);      // Mass Dispel
            pPlayer->learnSpell(48127, false);      // Mind Blast [Rank 13]
            pPlayer->learnSpell(605, false);        // Mind Control
            pPlayer->learnSpell(53023, false);      // Mind Sear [Rank 2]
            pPlayer->learnSpell(453, false);        // Mind Soothe
            pPlayer->learnSpell(10909, false);      // Mind Vision [Rank 2]
            pPlayer->learnSpell(48161, false);      // Power Word: Fortitude [Rank 8]
            pPlayer->learnSpell(48066, false);      // Power Word: Shield [Rank 14]
            pPlayer->learnSpell(48162, false);      // Prayer of Fortitude [Rank 4]
            pPlayer->learnSpell(48072, false);      // Prayer of Healing [Rank 7]
            pPlayer->learnSpell(48113, false);      // Prayer of Mending [Rank 3]
            pPlayer->learnSpell(48170, false);      // Prayer of Shadow Protection [Rank 3]
            pPlayer->learnSpell(48074, false);      // Prayer of Spirit [Rank 3]
            pPlayer->learnSpell(10890, false);      // Psychic Scream [Rank 4]
            pPlayer->learnSpell(48068, false);      // Renew [Rank 14]
            pPlayer->learnSpell(48171, false);      // Resurrection [Rank 7]
            pPlayer->learnSpell(10955, false);      // Shackle Undead [Rank 3]
            pPlayer->learnSpell(48169, false);      // Shadow Protection [Rank 5]
            pPlayer->learnSpell(48158, false);      // Shadow Word: Death [Rank 4]
            pPlayer->learnSpell(48125, false);      // Shadow Word: Pain [Rank 12]
            pPlayer->learnSpell(34433, false);      // Shadowfiend
            pPlayer->learnSpell(48123, false);      // Smite [Rank 12]
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 6000: // Death Knight Spells
            pPlayer->learnSpell(48778, false);      // Acherus Deathcharger
            pPlayer->learnSpell(48707, false);      // Anti-Magic Shell
            pPlayer->learnSpell(42650, false);      // Army of the Dead
            pPlayer->learnSpell(49941, false);      // Blood Boil [Rank 4]
            pPlayer->learnSpell(49930, false);      // Blood Strike [Rank 6]
            pPlayer->learnSpell(45529, false);      // Blood Tap
            pPlayer->learnSpell(45524, false);      // Chains of Ice
            pPlayer->learnSpell(56222, false);      // Dark Command
            pPlayer->learnSpell(49938, false);      // Death and Decay [Rank 4]
            pPlayer->learnSpell(62904, false);      // Death Coil [Rank 5]
            pPlayer->learnSpell(50977, false);      // Death Gate
            pPlayer->learnSpell(48743, false);      // Death Pact
            pPlayer->learnSpell(49924, false);      // Death Strike [Rank 5]
            pPlayer->learnSpell(47568, false);      // Empower Rune Weapon
            pPlayer->learnSpell(48263, false);      // Frost Presence
            pPlayer->learnSpell(57623, false);      // Horn of Winter [Rank 2]
            pPlayer->learnSpell(48792, false);      // Icebound Fortitude
            pPlayer->learnSpell(49909, false);      // Icy Touch [Rank 5]
            pPlayer->learnSpell(47528, false);      // Mind Freeze
            pPlayer->learnSpell(51425, false);      // Obliterate [Rank 4]
            pPlayer->learnSpell(3714, false);       // Path of Frost
            pPlayer->learnSpell(50842, false);      // Pestilence
            pPlayer->learnSpell(49221, false);      // Plague Strike [Rank 6]
            pPlayer->learnSpell(61999, false);      // Raise Ally
            pPlayer->learnSpell(46584, false);      // Raise Dead
            pPlayer->learnSpell(56815, false);      // Rune Strike
            pPlayer->learnSpell(53428, false);      // Runeforging
            pPlayer->learnSpell(53341, false);      // Rune of Cinderglacier
            pPlayer->learnSpell(53331, false);      // Rune of Lichbane
            pPlayer->learnSpell(53343, false);      // Rune of Razorice
            pPlayer->learnSpell(54447, false);      // Rune of Spellbreaking
            pPlayer->learnSpell(53342, false);      // Rune of Spellshattering
            pPlayer->learnSpell(54446, false);      // Rune of Swordbreaking
            pPlayer->learnSpell(53323, false);      // Rune of Swordshattering
            pPlayer->learnSpell(53344, false);      // Rune of the Fallen Crusader
            pPlayer->learnSpell(62158, false);      // Rune of the Stoneskin Gargoyle
            pPlayer->learnSpell(47476, false);      // Strangulate
            pPlayer->learnSpell(48265, false);      // Unholy Presence
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 7000: // Shaman Spells
            pPlayer->learnSpell(49277, false);      // Ancestral Spirit [Rank 7]    
            pPlayer->learnSpell(556, false);        // Astral Recall    
            pPlayer->learnSpell(66843, false);      // Call of the Ancestors
            pPlayer->learnSpell(66842, false);      // Call of the Elements
            pPlayer->learnSpell(66844, false);      // Call of the Spirits    
            pPlayer->learnSpell(55459, false);      // Chain Heal [Rank 7]
            pPlayer->learnSpell(49271, false);      // Chain Lightning [Rank 8]
            pPlayer->learnSpell(8170, false);       // Cleansing Totem
            pPlayer->learnSpell(526, false);        // Cure Toxins
            pPlayer->learnSpell(2062, false);       // Earth Elemental Totem
            pPlayer->learnSpell(49231, false);      // Earth Shock [Rank 10]
            pPlayer->learnSpell(2484, false);       // Earthbind Totem
            pPlayer->learnSpell(51994, false);      // Earthliving Weapon [Rank 6]
            pPlayer->learnSpell(6196, false);       // Far Sight
            pPlayer->learnSpell(2894, false);       // Fire Elemental Totem    
            pPlayer->learnSpell(61657, false);      // Fire Nova Totem [Rank 9]
            pPlayer->learnSpell(58739, false);      // Fire Resistance Totem [Rank 6]
            pPlayer->learnSpell(49233, false);      // Flame Shock [Rank 9]
            pPlayer->learnSpell(58656, false);      // Flametongue Totem [Rank 8]
            pPlayer->learnSpell(58790, false);      // Flametongue Weapon [Rank 10]
            pPlayer->learnSpell(58745, false);      // Frost Resistance Totem [Rank 6]
            pPlayer->learnSpell(49236, false);      // Frost Shock [Rank 7]
            pPlayer->learnSpell(58796, false);      // Frostbrand Weapon [Rank 9]
            pPlayer->learnSpell(2645, false);       // Ghost Wolf
            pPlayer->learnSpell(8177, false);       // Grounding Totem    
            pPlayer->learnSpell(58757, false);      // Healing Stream Totem [Rank 9]
            pPlayer->learnSpell(49273, false);      // Healing Wave [Rank 14]
            pPlayer->learnSpell(51514, false);      // Hex    
            pPlayer->learnSpell(60043, false);      // Lava Burst [Rank 2]
            pPlayer->learnSpell(49276, false);      // Lesser Healing Wave [Rank 9]    
            pPlayer->learnSpell(49238, false);      // Lightning Bolt [Rank 14]
            pPlayer->learnSpell(49281, false);      // Lightning Shield [Rank 11]
            pPlayer->learnSpell(58734, false);      // Magma Totem [Rank 7]
            pPlayer->learnSpell(58774, false);      // Mana Spring Totem [Rank 8]
            pPlayer->learnSpell(58749, false);      // Nature Resistance Totem [Rank 6]
            pPlayer->learnSpell(8012, false);       // Purge [Rank 2]
            pPlayer->learnSpell(20608, false);      // Reincarnation
            pPlayer->learnSpell(10399, false);      // Rockbiter Weapon [Rank 4]
            pPlayer->learnSpell(58704, false);      // Searing Totem [Rank 10]
            pPlayer->learnSpell(6495, false);       // Sentry Totem
            pPlayer->learnSpell(58582, false);      // Stoneclaw Totem [Rank 10]
            pPlayer->learnSpell(58753, false);      // Stoneskin Totem [Rank 10]
            pPlayer->learnSpell(58643, false);      // Strength of Earth Totem [Rank 8]
            pPlayer->learnSpell(36936, false);      // Totemic Recall
            pPlayer->learnSpell(8143, false);       // Tremor Totem
            pPlayer->learnSpell(131, false);        // Water Breathing
            pPlayer->learnSpell(57960, false);      // Water Shield [Rank 9]
            pPlayer->learnSpell(546, false);        // Water Walking
            pPlayer->learnSpell(57994, false);      // Wind Shear
            pPlayer->learnSpell(8512, false);       // Windfury Totem
            pPlayer->learnSpell(58804, false);      // Windfury Weapon [Rank 8]
            if(pPlayer->getRace() == 2 || 6 || 8) 
            pPlayer->learnSpell(2825, false);       // Bloodlust
            else if(pPlayer->getRace() == 11)
            pPlayer->learnSpell(32182, false);      // Heroism
            pPlayer->learnSpell(8737, false);       // Mail
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 8000: // Mage Spells
            pPlayer->learnSpell(43017, false);      // Amplify Magic [Rank 7]
            pPlayer->learnSpell(42897, false);      // Arcane Blast [Rank 4]
            pPlayer->learnSpell(43002, false);      // Arcane Brilliance [Rank 3]
            pPlayer->learnSpell(42921, false);      // Arcane Explosion [Rank 10]
            pPlayer->learnSpell(42995, false);      // Arcane Intellect [Rank 7]
            pPlayer->learnSpell(42846, false);      // Arcane Missles [Rank 13]
            pPlayer->learnSpell(1953, false);       // Blink
            pPlayer->learnSpell(42940, false);      // Blizzard [Rank 9]
            pPlayer->learnSpell(42931, false);      // Cone of Cold [Rank 8]
            pPlayer->learnSpell(33717, false);      // Conjure Food [Rank 8]
            pPlayer->learnSpell(42985, false);      // Conjure Mana Gem [Rank 6]
            pPlayer->learnSpell(42956, false);      // Conjure Refreshment [Rank 2]
            pPlayer->learnSpell(27090, false);      // Conjure Water [Rank 9]
            pPlayer->learnSpell(2139, false);       // Counterspell
            pPlayer->learnSpell(61316, false);      // Dalaran Brilliance [Rank 3]
            pPlayer->learnSpell(61024, false);      // Dalaran Intellect [Rank 7]
            pPlayer->learnSpell(43015, false);      // Dampen Magic [Rank 7]
            pPlayer->learnSpell(12051, false);      // Evocation
            pPlayer->learnSpell(42873, false);      // Fire Blast [Rank 11]
            pPlayer->learnSpell(43010, false);      // Fire Ward [Rank 7]
            pPlayer->learnSpell(42833, false);      // Fireball [Rank 16]
            pPlayer->learnSpell(42926, false);      // Flamestrike [Rank 9]
            pPlayer->learnSpell(7301, false);       // Frost Armor [Rank 3]
            pPlayer->learnSpell(42917, false);      // Frost Nova [Rank 6]
            pPlayer->learnSpell(43012, false);      // Frost Ward [Rank 7]
            pPlayer->learnSpell(42842, false);      // Frostbolt [Rank 16]
            pPlayer->learnSpell(47610, false);      // Frostfire Bolt [Rank 2]
            pPlayer->learnSpell(43008, false);      // Ice Armor [Rank 6]
            pPlayer->learnSpell(45438, false);      // Ice Block
            pPlayer->learnSpell(42914, false);      // Ice Lance [Rank 3]
            pPlayer->learnSpell(66, false);         // Invisibility
            pPlayer->learnSpell(43024, false);      // Mage Armor [Rank 6]
            pPlayer->learnSpell(43020, false);      // Mana Shield [Rank 9]
            pPlayer->learnSpell(55342, false);      // Mirror Image
            pPlayer->learnSpell(43046, false);      // Molten Armor [Rank 3]
            pPlayer->learnSpell(12826, false);      // Polymorph [Rank 4]
            pPlayer->learnSpell(61305, false);      // Polymorph: Black Cat
            pPlayer->learnSpell(28272, false);      // Polymorph: Pig
            pPlayer->learnSpell(61721, false);      // Polymorph: Rabbit
            pPlayer->learnSpell(61780, false);      // Polymorph: Turkey
            pPlayer->learnSpell(28271, false);      // Polymorph: Turtle
            pPlayer->learnSpell(53142, false);      // Portal: Dalaran
            if(pPlayer->getRace() == 1 || 3 || 4 || 7 || 11) 
                pPlayer->learnSpell(11419, false),  // Portal: Darnassus
                pPlayer->learnSpell(32266, false),  // Portal: Exodar
                pPlayer->learnSpell(11416, false),  // Portal: Ironforge
                pPlayer->learnSpell(33691, false),  // Portal: Shattrath
                pPlayer->learnSpell(11059, false),  // Portal: Stormwind
                pPlayer->learnSpell(49360, false);  // Portal: Theramore
            else if(pPlayer->getRace() == 2 || 5 || 6 || 8 || 10)
                pPlayer->learnSpell(11417, false),  // Portal: Orgrimmar
                pPlayer->learnSpell(35717, false),  // Portal: Shattrath
                pPlayer->learnSpell(32267, false),  // Portal: Silvermoon
                pPlayer->learnSpell(49361, false),  // Portal: Stonard
                pPlayer->learnSpell(11420, false),  // Portal: Thunder Bluff
                pPlayer->learnSpell(11418, false);  // Portal: Undercity
            if(pPlayer->getRace() == 1 || 3 || 4 || 7 || 11) 
                pPlayer->learnSpell(3565, false),   // Teleport: Darnassus
                pPlayer->learnSpell(32271, false),  // Teleport: Exodar
                pPlayer->learnSpell(3562, false),   // Teleport: Ironforge
                pPlayer->learnSpell(33690, false),  // Teleport: Shattrath
                pPlayer->learnSpell(3561, false),   // Teleport: Stormwind
                pPlayer->learnSpell(49359, false);  // Teleport: Theramore
            else if(pPlayer->getRace() == 2 || 5 || 6 || 8 || 10)
                pPlayer->learnSpell(3567, false),   // Teleport: Orgrimmar
                pPlayer->learnSpell(35715, false),  // Teleport: Shattrath
                pPlayer->learnSpell(32272, false),  // Teleport: Silvermoon
                pPlayer->learnSpell(49358, false),  // Teleport: Stonard
                pPlayer->learnSpell(3566, false),   // Teleport: Thunder Bluff
                pPlayer->learnSpell(3563, false);   // Teleport: Undercity
            pPlayer->learnSpell(475, false);        // Remove Curse
            pPlayer->learnSpell(58659, false);      // Ritual of Refreshment [Rank 2]
            pPlayer->learnSpell(42859, false);      // Scorch [Rank 11]
            pPlayer->learnSpell(130, false);        // Slow Fall
            pPlayer->learnSpell(30449, false);      // Spellsteal
            pPlayer->learnSpell(53140, false);      // Teleport: Dalaran    
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 9000: // Warlock Spells
            pPlayer->learnSpell(60220, false);      // Create Firestone [Rank 7]
            pPlayer->learnSpell(47884, false);      // Create Soulstone [Rank 7]
            pPlayer->learnSpell(47888, false);      // Create Spellstone [Rank 6]
            pPlayer->learnSpell(18647, false);      // Banish [Rank 2]
            pPlayer->learnSpell(47813, false);      // Corruption [Rank 10]
            pPlayer->learnSpell(47878, false);      // Create Healthstone [Rank 8]
            pPlayer->learnSpell(47864, false);      // Curse of Agony [Rank 9]
            pPlayer->learnSpell(11719, false);      // Curse of Tongues [Rank 2]
            pPlayer->learnSpell(47867, false);      // Curse of Doom [Rank 3]
            pPlayer->learnSpell(50511, false);      // Curse of Weakness [Rank 9]
            pPlayer->learnSpell(47865, false);      // Curse of the Elements [Rank 5]
            pPlayer->learnSpell(47860, false);      // Death Coil [Rank 6]
            pPlayer->learnSpell(47889, false);      // Demon Armor [Rank 8]
            pPlayer->learnSpell(696, false);        // Demon Skin [Rank 2]
            pPlayer->learnSpell(48018, false);      // Demonic Circle: Summon
            pPlayer->learnSpell(48020, false);      // Demonic Circle: Teleport
            pPlayer->learnSpell(132, false);        // Detect Invisibility
            pPlayer->learnSpell(47857, false);      // Drain Life [Rank 9]
            pPlayer->learnSpell(5138, false);       // Drain Mana
            pPlayer->learnSpell(47855, false);      // Drain Soul [Rank 6]
            pPlayer->learnSpell(23161, false);      // Dreadsteed
            pPlayer->learnSpell(61191, false);      // Enslave Demon [Rank 4]
            pPlayer->learnSpell(126, false);        // Eye of Kilrogg
            pPlayer->learnSpell(6215, false);       // Fear [Rank 3]
            pPlayer->learnSpell(47893, false);      // Fel Armor [Rank 4]
            pPlayer->learnSpell(5784, false);       // Felsteed
            pPlayer->learnSpell(47856, false);      // Health Funnel [Rank 9]
            pPlayer->learnSpell(47823, false);      // Hellfire [Rank 5]
            pPlayer->learnSpell(17928, false);      // Howl of Terror [Rank 2]
            pPlayer->learnSpell(47811, false);      // Immolate [Rank 11]
            pPlayer->learnSpell(47838, false);      // Incinerate [Rank 4]
            pPlayer->learnSpell(57946, false);      // Life Tap [Rank 8]
            pPlayer->learnSpell(47820, false);      // Rain of Fire [Rank 7]
            pPlayer->learnSpell(18540, false);      // Ritual of Doom
            pPlayer->learnSpell(58887, false);      // Ritual of Souls [Rank 2]
            pPlayer->learnSpell(698, false);        // Ritual of Summoning
            pPlayer->learnSpell(47815, false);      // Searing Pain [Rank 10]
            pPlayer->learnSpell(47836, false);      // Seed of Corruption [Rank 3]
            pPlayer->learnSpell(5500, false);       // Sense Demons
            pPlayer->learnSpell(47809, false);      // Shadow Bolt [Rank 13]
            pPlayer->learnSpell(47891, false);      // Shadow Ward [Rank 6]
            pPlayer->learnSpell(61290, false);      // Shadowflame [Rank 2]
            pPlayer->learnSpell(47825, false);      // Soul Fire [Rank 6]
            pPlayer->learnSpell(29858, false);      // Soulshatter
            pPlayer->learnSpell(691, false);        // Summon Felhunter
            pPlayer->learnSpell(688, false);        // Summon Imp
            pPlayer->learnSpell(712, false);        // Summon Succubus
            pPlayer->learnSpell(697, false);        // Summon Voidwalker
            pPlayer->learnSpell(5697, false);       // Unending Breath
            
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 10000: // Druid Spells
            pPlayer->learnSpell(2893, false);       // Abolish Poison
            pPlayer->learnSpell(1066, false);       // Aquatic Form
            pPlayer->learnSpell(22812, false);      // Barkskin
            pPlayer->learnSpell(8983, false);       // Bash [Rank 3]
            pPlayer->learnSpell(768, false);        // Cat Form
            pPlayer->learnSpell(5209, false);       // Challenging Roar    
            pPlayer->learnSpell(48570, false);      // Claw [Rank 8]
            pPlayer->learnSpell(48575, false);      // Cower [Rank 6]
            pPlayer->learnSpell(8946, false);       // Cure Poison
            pPlayer->learnSpell(33786, false);      // Cyclone
            pPlayer->learnSpell(33357, false);      // Dash [Rank 3]
            pPlayer->learnSpell(48560, false);      // Demoralizing Roar [Rank 8]
            pPlayer->learnSpell(9634, false);       // Dire Bear Form
            pPlayer->learnSpell(5229, false);       // Enrage
            pPlayer->learnSpell(53308, false);      // Entangling Roots [Rank 8]
            pPlayer->learnSpell(770, false);        // Faerie Fire
            pPlayer->learnSpell(16857, false);      // Faerie Fire (Feral)
            pPlayer->learnSpell(20719, false);      // Feline Grace 
            pPlayer->learnSpell(16979, false);      // Feral Charge - Bear
            pPlayer->learnSpell(49376, false);      // Feral Charge - Cat 
            pPlayer->learnSpell(48577, false);      // Ferocious Bite [Rank 8]
            pPlayer->learnSpell(33943, false);      // Flight Form
            pPlayer->learnSpell(48470, false);      // Gift of the Wild [Rank 4]
            pPlayer->learnSpell(6795, false);       // Growl 
            pPlayer->learnSpell(22842, false);      // Frenzied Regeneration [Rank 1]
            pPlayer->learnSpell(48378, false);      // Healing Touch [Rank 15]
            pPlayer->learnSpell(48467, false);      // Hurricane [Rank 5]
            pPlayer->learnSpell(29166, false);      // Innervate
            pPlayer->learnSpell(48451, false);      // Lifebloom [Rank 3]
            pPlayer->learnSpell(48568, false);      // Lacerate [Rank 3]
            pPlayer->learnSpell(49802, false);      // Maim [Rank 2]
            pPlayer->learnSpell(48564, false);      // Mangle (Bear, false); [Rank 5]
            pPlayer->learnSpell(48566, false);      // Mangle (Cat, false); [Rank 5]
            pPlayer->learnSpell(48469, false);      // Mark of the Wild [Rank 9]
            pPlayer->learnSpell(48480, false);      // Maul [Rank 10]
            pPlayer->learnSpell(48463, false);      // Moonfire [Rank 14]
            pPlayer->learnSpell(53312, false);      // Nature's Grasp [Rank 8]
            pPlayer->learnSpell(50464, false);      // Nourish [Rank 1]
            pPlayer->learnSpell(49803, false);      // Pounce [Rank 5]
            pPlayer->learnSpell(9913, false);       // Prowl [Rank 3]
            pPlayer->learnSpell(48574, false);      // Rake [Rank 7]
            pPlayer->learnSpell(48579, false);      // Ravage [Rank 7]
            pPlayer->learnSpell(48477, false);      // Rebirth [Rank 7]
            pPlayer->learnSpell(48443, false);      // Regrowth [Rank 12]
            pPlayer->learnSpell(48441, false);      // Rejuvenation [Rank 15]
            pPlayer->learnSpell(2782, false);       // Remove Curse    
            pPlayer->learnSpell(50763, false);      // Revive [Rank 7]
            pPlayer->learnSpell(49800, false);      // Rip [Rank 9]
            pPlayer->learnSpell(62600, false);      // Savage Defense
            pPlayer->learnSpell(48572, false);      // Shred [Rank 9]
            pPlayer->learnSpell(26995, false);      // Soothe Animal [Rank 4]
            pPlayer->learnSpell(48465, false);      // Starfire [Rank 10]
            pPlayer->learnSpell(40120, false);      // Swift Flight Form
            pPlayer->learnSpell(62078, false);      // Swipe (Cat) [Rank 1]
            pPlayer->learnSpell(48562, false);      // Swipe (Bear) [Rank 8]
            pPlayer->learnSpell(18960, false);      // Teleport: Moonglade
            pPlayer->learnSpell(53307, false);      // Thorns [Rank 8]
            pPlayer->learnSpell(50213, false);      // Tiger's Fury [Rank 6]
            pPlayer->learnSpell(5225, false);       // Track Humanoids
            pPlayer->learnSpell(48447, false);      // Tranquility [Rank 7]
            pPlayer->learnSpell(783, false);        // Travel Form
            pPlayer->learnSpell(48461, false);      // Wrath [Rank 12]
                
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_SPELL, pPlayer, false);
            
        break;

        case 11000: // Weapon Skills
            if(pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(264, false),        // Bows
            pPlayer->learnSpell(5011, false),       // Crossbows
            pPlayer->learnSpell(266, false),        // Guns
            pPlayer->learnSpell(3018, false),       // Shoot (Ranged)
            pPlayer->learnSpell(2764, false),       // Throw
            pPlayer->learnSpell(2567, false);       // Thrown
            
            if(pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARLOCK || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(1180, false),       // Daggers
            pPlayer->learnSpell(227, false);        // Staves
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(674, false);        // Dual Wield
            
            if(pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(15590, false);      // Fist Weapons
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(196, false),        // One-Handed Axes
            pPlayer->learnSpell(3127, false);       // Parry
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(198, false);        // One-Handed Maces
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_WARLOCK || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(201, false);        // One-Handed Swords
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(200, false);        // Polearms
            
            if(pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_WARLOCK)
            pPlayer->learnSpell(5009, false),       // Wands
            pPlayer->learnSpell(5019, false);       // Shoot (Wand)
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(197, false);        // Two-Handed Axes
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(199, false);        // Two-Handed Maces
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->learnSpell(202, false);        // Two-Handed Swords

            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_WEAPON, pPlayer, false);
            
        break;

        case 12000: // Max out all weapon skills

            if(pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_BOWS, 450, 450),            // Max Out Bows
            pPlayer->SetSkill(SKILL_CROSSBOWS, 450, 450),       // Max Out Crossbows
            pPlayer->SetSkill(SKILL_GUNS, 450, 450),            // Max Out Guns
            pPlayer->SetSkill(SKILL_THROWN, 450, 450);          // Max Out Thrown

            if(pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARLOCK || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_DAGGERS, 450, 450),         // Max Out Daggers
            pPlayer->SetSkill(SKILL_STAVES, 450, 450);          // Max Out Staves

            if(pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_FIST_WEAPONS, 450, 450);    // Max Out Fist Weapons

            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_AXES, 450, 450);            // Max Out Axes
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_MACES, 450, 450);           // Max Out Maces 
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_ROGUE || pPlayer->getClass() == CLASS_WARLOCK || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_SWORDS, 450, 450);          // Max Out Swords
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_POLEARMS, 450, 450);        // Max Out Polearms
            
            if(pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_WARLOCK)
            pPlayer->SetSkill(SKILL_WANDS, 450, 450);           // Max Out Wands
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_2H_AXES, 450, 450);         // Max Out Two-Handed Axes
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_SHAMAN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_2H_MACES, 450, 450);        // Max Out Two-Handed Maces
            
            if(pPlayer->getClass() == CLASS_DEATHKNIGHT || pPlayer->getClass() == CLASS_HUNTER || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_WARRIOR)
            pPlayer->SetSkill(SKILL_2H_SWORDS, 450, 450);       // Max Out Two-Handed Swords

            pPlayer->SetSkill(SKILL_UNARMED, 450, 450);         // Max Out Unarmed

            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_MAXED_WEAPON, pPlayer, false);
            
        break;

        case 13000: // Max out all riding skills

            pPlayer->learnSpell(34091, false);          // Artisan Riding
            pPlayer->learnSpell(54197, false);          // Cold Weather Flying

            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_MAXED_RIDING, pPlayer, false);
            
        break;

        case 14000: // Learn all languages

            pPlayer->learnSpell(668, false);            // Language Common
            pPlayer->learnSpell(671, false);            // Language Darnassian
            pPlayer->learnSpell(29932, false);          // Language Draenei
            pPlayer->learnSpell(672, false);            // Language Dwarven
            pPlayer->learnSpell(7340, false);           // Language Gnomish
            pPlayer->learnSpell(17737, false);          // Language Gutterspeak
            pPlayer->learnSpell(669, false);            // Language Orcish
            pPlayer->learnSpell(670, false);            // Language Taurahe
            pPlayer->learnSpell(813, false);            // Language Thalassian
            pPlayer->learnSpell(7341, false);           // Language Troll

            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_LEARNED_LANGUAGES, pPlayer, false);
            
        break;

        case 15000: // Max out all languages
            
            pPlayer->UpdateSkill(98,300);               // Max Out Language Common
            pPlayer->UpdateSkill(113,300);              // Max Out Language Darnassian
            pPlayer->UpdateSkill(759,300);              // Max Out Language Draenei
            pPlayer->UpdateSkill(111,300);              // Max Out Language Dwarven
            pPlayer->UpdateSkill(313,300);              // Max Out Language Gnomish
            pPlayer->UpdateSkill(673,300);              // Max Out Language Gutterspeak
            pPlayer->UpdateSkill(109,300);              // Max Out Language Orcish
            pPlayer->UpdateSkill(115,300);              // Max Out Language Taurahe
            pPlayer->UpdateSkill(137,300);              // Max Out Language Thalassian
            pPlayer->UpdateSkill(315,300);              // Max Out Language Troll

            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterWhisper(MSG_MAXED_LANGUAGES, pPlayer, false);
            
        break;

        // Close gossip menu
        case 99000:
            pPlayer->CLOSE_GOSSIP_MENU();
        break;
    }
}

bool GossipSelect_custom_npc_spellmaster(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiSender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_custom_npc_spellmaster(pPlayer, pCreature, uiAction);

    return true;
}

void AddSC_custom_npc_spellmaster()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_npc_spellmaster";
    newscript->pGossipHello     = &GossipHello_custom_npc_spellmaster;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_spellmaster;
    newscript->RegisterSelf();
}