-- Eye of acherus
UPDATE `creature_template` SET `InhabitType` = 3, `ScriptName` = 'npc_eye_of_acherus' WHERE `entry` = 28511;
REPLACE INTO `creature_template_addon` (`entry`,`moveflags`,`auras`) VALUES (28511,33562624,''),(28525,0,'64328 0'),(28542,0,'64328 0'),(28543,0,'64328 0'),(28544,0,'64328 0');
REPLACE INTO `spell_script_target` (`entry`,`type`,`targetEntry`) VALUES (51859,1,28525),(51859,1,28542),(51859,1,28543),(51859,1,28544);
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT guid FROM `creature` WHERE `id` IN (28511,28525,28542,28543,28544));
UPDATE `npc_spellclick_spells` SET `quest_start` = 0, `quest_start_active` = 0 WHERE `npc_entry` = 29501;
