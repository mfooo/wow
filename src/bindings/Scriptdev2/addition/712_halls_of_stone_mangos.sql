UPDATE `creature_template` SET `ScriptName` = 'boss_krystallus' WHERE `entry` =27977 LIMIT 1 ;

UPDATE `gameobject` SET `state` = '1' WHERE `guid` =53556;
UPDATE `gameobject` SET `state` = '1' WHERE `guid` =53560;

UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `entry` =191293;
UPDATE `gameobject_template` SET `faction` = '0', `flags` = '0'  WHERE `entry` IN (193996,190586);
UPDATE `gameobject` SET `phaseMask` = '65535' WHERE `guid` =37577;
UPDATE `gameobject` SET `phaseMask` = '65535' WHERE `guid` =37583;
