UPDATE `creature_template` SET `ScriptName`='boss_archavon' WHERE `entry`=31125;
UPDATE `creature_template` SET `ScriptName`='boss_emalon' WHERE `entry`=33993;
UPDATE `creature_template` SET `ScriptName`='npc_tempest_minion' WHERE `entry`=33998;
UPDATE `creature_template` SET `ScriptName`='npc_tempest_warder' WHERE `entry`=34015;
UPDATE `creature_template` SET `ScriptName`='boss_koralon' WHERE `entry`=35013;
UPDATE `creature_template` SET `AIName`='', ScriptName='boss_toravon' WHERE `entry`=38433;
UPDATE `instance_template` SET `ScriptName`='instance_vault_of_archavon' WHERE `map`=624;
DELETE FROM `spell_script_target` WHERE `entry` IN (72034,72096);
INSERT INTO `spell_script_target` VALUES (72034,1,38433),(72096,1,38433);
