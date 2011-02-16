update `creature_template` set `ScriptName` = 'boss_malygos' where `entry` = 28859;
update `instance_template` set `ScriptName` = 'instance_eye_of_eternity' where `map` = '616';
update `gameobject` set `phaseMask`=65536, `spawntimesecs`='-604800'  where id in(193905,193967);
update `gameobject_template` set `faction`=0 where entry in (193905,193967);