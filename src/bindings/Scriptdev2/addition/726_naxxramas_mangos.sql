--  Arachnid Quarter
UPDATE `creature_template` SET `ScriptName`='mob_crypt_guard' WHERE `entry` IN (16573);
UPDATE `creature_template` SET `ScriptName`='mob_worshippers' WHERE `entry`='16506';
UPDATE `creature_template` SET `ScriptName`='mob_webwrap' WHERE `entry`='16486';
UPDATE `creature_template` SET `flags_extra` = '2' WHERE `entry` IN ('16486','30183');
DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (16506);
-- Construct Quarter
UPDATE `creature_template` SET `ScriptName`='boss_grobbulus' WHERE `entry`='15931';
UPDATE `creature_template` SET `ScriptName`='npc_grobbulus_poison_cloud' WHERE `entry`='16363';
UPDATE `creature_template` SET `ScriptName`='boss_thaddius', `unit_flags`=0 WHERE `entry`='15928';
UPDATE `creature_template` SET `ScriptName`='mob_stalagg' WHERE `entry`='15929';
UPDATE `creature_template` SET `ScriptName`='mob_feugen' WHERE `entry`='15930';
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` = '16027';
UPDATE `creature_template` SET `ScriptName`='mob_zombie_chows' WHERE `entry` = '16360';
-- Military Quarter
UPDATE `creature_template` SET `attackpower` = `attackpower`*40 WHERE `entry` IN (16061,29940); -- Instructor Razuvious
UPDATE `creature_template` SET `attackpower` = `attackpower`*12 WHERE `entry` IN (16803,29941); -- Death Knight Understudy
UPDATE `creature_template` SET `spell1`=61696, `spell2`=29060, `spell3`=29061 WHERE `entry` IN (16803);
UPDATE `creature` SET `MovementType`=0, `spawndist`=0 WHERE `id`=16211;
UPDATE `creature_template` SET `ScriptName`='boss_gothik' WHERE `entry`='16060';
UPDATE `creature_template` SET `flags_extra` = '2' WHERE `entry` IN (16697);
UPDATE `gameobject` SET `spawntimesecs`=-604800 WHERE `id` IN ('181366','193426');
-- Plague Quarter
UPDATE `creature_template` SET `ScriptName`='boss_heigan' WHERE `entry`='15936';
UPDATE `creature_template` SET `ScriptName`='npc_loatheb_spores' WHERE `entry`='16286';
-- Frostwyrm Lair
UPDATE `creature_template` SET `ScriptName`='boss_kelthuzad' WHERE `entry`='15990';
UPDATE `creature_template` SET `ScriptName`='mob_shadow_issure', `flags_extra` = '2', `faction_A`=16, `faction_H`=16 WHERE `entry`='16129';
-- Adds
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry` IN (16981, 16427, 16429);
DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (16981, 16427, 16429);
INSERT INTO `creature_ai_scripts` VALUES
('1698101', '16981', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '54890', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', ''),
('1698102', '16981', '0', '0', '100', '5', '10000', '20000', '10000', '20000', '11', '54891', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', ''),
('1642701', '16427', '6', '0', '100', '3', '0', '0', '0', '0', '11', '28457', '6', '7', '0', '0', '0', '0', '0', '0', '0', '0', 'Soldiers of the Frozen Wastes - Dark Blast'),
('1642702', '16427', '6', '0', '100', '5', '0', '0', '0', '0', '11', '55714', '6', '7', '0', '0', '0', '0', '0', '0', '0', '0', 'Soldiers of the Frozen Wastes - Dark Blast'),
('1642901', '16429', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '28459', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soul Weaver - Wail of Souls'),
('1642902', '16429', '0', '0', '100', '5', '10000', '20000', '10000', '20000', '11', '55765', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soul Weaver - Wail of Souls');
UPDATE gameobject SET `state`='1' WHERE `map`=533 AND `id` IN (181225,181124);
DELETE FROM `spell_target_position` WHERE `id` IN (28444);
INSERT INTO `spell_target_position` VALUES (28444, 533, 3005.776, -3483.284, 299.551, 1.552);
-- Portal at end
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry` IN ('181575', '181576', '181577', '181578'); -- 16
DELETE FROM `gameobject` WHERE `id` in ('181575', '181576', '181577', '181578');
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(181575, 533, 3, 1, 3521.34, -3942.85, 308.106, 2.12884, 0, 0, 1, 0, -604800, 0, 1),
(181576, 533, 3, 1, 3548.85, -2958.15, 302.688, 3.16206, 0, 0, 1, 0, -604800, 0, 1),
(181577, 533, 3, 1, 2909, -4025.02, 273.475, 3.14159, 0, 0, 1, 0, -604800, 0, 1),
(181578, 533, 3, 1, 2492.16, -2921.66, 241.276, 5.52219, 0, 0, 1, 0, -604800, 0, 1);

INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) values('2','0','0','0','0','0','0','0','0','0','Erupt trigger','',NULL,'80','80','200','300','20','30','0','21','21','0','1','1','0','0','0','0','0','1','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','0','0','0','0','0','0','0','0','1','0','0','194','');
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (15384, 0, 18783, 0, 11686, 0, 'Plague Fissure', '', '', 83, 83, 4120, 4120, 0, 0, 0, 21, 21, 0, 1.1, 1, 0, 0, 0, 0, 0, 1, 2000, 0, 1, 33555206, 0, 0, 0, 0, 0, 0, 0, 1, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.35, 1, 0, 1, 0, 0, 130, 'npc_heigan_eruption');

-- Soul Weaver
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `minhealth` = 56700, `maxhealth` = 56700 WHERE `entry` = 16429;
-- Guardian of Icecrown 10 men
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `minhealth` = 2520000, `maxhealth` = 2520000, `dmg_multiplier` = 1.5, mechanic_immune_mask = 1073741823 WHERE entry = 16441;
-- Guardian of Icecrown 25 men
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `minhealth` = 6300000, `maxhealth` = 6300000, `armor` = 7369, dmg_multiplier = 1.8, mechanic_immune_mask = 1073741823 WHERE entry = 30057;
-- Shadow Fissure
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `flags_extra` = 130 WHERE `entry` = 16129;

UPDATE `gameobject_template` SET `flags` = 0 WHERE `entry` = 193426;

DELETE FROM `gameobject` WHERE `guid`=21640;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`, `animprogress`,`state`) VALUES
(21640,193426, 533, 2, 1, 2511.51, -2943.93, 245.552, 5.48707, 0, 0, 0.387631, -0.921815, -604800, 100, 1);

DELETE FROM `spell_script_target` WHERE `entry`=55479;
INSERT INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES (55479, 1, 16803);               

DELETE FROM `npc_gossip` WHERE `npc_guid` IN (128352, 128353);
DELETE FROM `npc_text` WHERE `ID`=100;
INSERT INTO `npc_gossip` (`npc_guid`, `textid`) VALUES (128352, 100);
INSERT INTO `npc_gossip` (`npc_guid`, `textid`) VALUES (128353, 100);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`) VALUES (100, 'Hi. In front of you is standing a Instructor Razuvious. To defeat this boss, you ll need to use Mind Control. If you don t have any priest in your group, press button below.', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `creature` set `spawnMask` = 1 where `id` = 29912;

DELETE FROM `creature_template` WHERE (`entry`=16803);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (16803, 29941, 0, 0, 0, 0, 16539, 0, 16539, 0, 'Death Knight Understudy', '', '', 0, 82, 82, 404430, 404430, 0, 0, 49905, 21, 21, 0, 1, 1, 1, 488, 642, 0, 782, 7.5, 2000, 0, 1, 32832, 0, 0, 0, 0, 0, 0, 363, 521, 121, 7, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 61696, 29060, 29061, 0, 0, 0, 0, 'EventAI', 0, 3, 30, 2, 0, 22708, 0, 0, 0, 0, 0, 144, 1, 1728, 0, 0, '');

DELETE FROM `creature_template` WHERE (`entry`=29941);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (29941, 0, 0, 0, 0, 0, 16539, 0, 16539, 0, 'Death Knight Understudy (1)', '', '', 0, 82, 82, 404430, 404430, 0, 0, 49905, 14, 14, 0, 1.5, 1, 1, 488, 642, 0, 782, 13, 2000, 0, 1, 32832, 0, 0, 0, 0, 0, 0, 363, 521, 121, 7, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 30, 2, 0, 22708, 0, 0, 0, 0, 0, 144, 1, 0, 0, 0, '');

DELETE FROM `creature_template` WHERE (`entry`=16061);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (16061, 29940, 0, 0, 0, 0, 16582, 0, 16582, 0, 'Instructor Razuvious', '', '', 0, 83, 83, 3346800, 3346800, 0, 0, 10673, 21, 21, 0, 1.6, 1, 3, 1000, 1200, 0, 800, 70, 3500, 0, 1, 32832, 0, 0, 0, 0, 0, 0, 371, 535, 135, 6, 76, 16061, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1253602, 1532181, '', 0, 3, 240, 2, 0, 0, 0, 0, 0, 0, 0, 167, 1, 1718, 617299803, 1, 'boss_razuvious');

DELETE FROM `creature_template` WHERE (`entry`=29940);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (29940, 0, 0, 0, 0, 0, 16582, 0, 16582, 0, 'Instructor Razuvious (1)', '', '', 0, 83, 83, 10110125, 10110125, 0, 0, 10673, 21, 21, 0, 2, 1, 3, 1000, 1200, 0, 800, 70, 3500, 0, 1, 32832, 0, 0, 0, 0, 0, 0, 371, 535, 135, 6, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2507204, 3064362, '', 0, 3, 725, 2, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 617299803, 1, '');

DELETE FROM `creature_template` WHERE (`entry`=29912);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (29912, 0, 0, 0, 0, 0, 26620, 0, 26620, 0, 'Obedience Crystal', '', 'Interact', 0, 80, 80, 12600, 12600, 0, 0, 9729, 35, 35, 1, 1, 1, 0, 422, 586, 0, 642, 1, 2000, 0, 1, 64, 0, 0, 0, 0, 0, 0, 345, 509, 103, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'npc_obedience_crystal');

UPDATE `creature` SET `MovementType` = 2 WHERE `guid` = 128312;
DELETE FROM `creature_movement` WHERE id = 128312;
INSERT INTO `creature_movement` (`id`,`POINT`,`position_x`,`position_y`,`position_z`) VALUES
(128312,1,2777.138672,-3110.880859,267.684509),
(128312,2,2781.611328,-3106.903076,267.684509),
(128312,3,2784.291748,-3098.885254,267.684509),
(128312,4,2781.093018,-3090.532959,267.684509),
(128312,5,2773.239014,-3085.609619,267.684509),
(128312,6,2765.775146,-3086.021240,267.684509),
(128312,7,2758.100586,-3091.694824,267.684509),
(128312,8,2755.643555,-3097.803467,267.684509),
(128312,9,2756.769775,-3104.073975,267.684509),
(128312,10,2760.447021,-3109.536621,267.684509),
(128312,11,2766.769531,-3112.940918,267.684509);
