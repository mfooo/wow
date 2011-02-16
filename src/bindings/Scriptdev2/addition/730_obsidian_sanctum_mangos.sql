-- Obsidian sanctum (from PSZ and MaxXx2021)
UPDATE creature_template SET ScriptName='mob_fire_cyclone' WHERE entry = 30648;
UPDATE creature_template SET ScriptName='mob_flame_tsunami' WHERE entry = 30616;
UPDATE creature_template SET ScriptName='mob_lava_blaze' WHERE entry = 30643;

-- CUSTOM hack to different hard mode loot
REPLACE INTO creature_template (entry, difficulty_entry_1, modelid_1, modelid_3, faction_A, faction_H, NAME, subname, minhealth, maxhealth, rank, lootid) VALUES
(288601, 313111, 27035, 27035, 103, 103, 'Sartharion', 'The Onyx Guardian', 1, 1, 3, 288601),
(288602, 313112, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 288602),
(288603, 313113, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 288603),
(313111, 0, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 313111),
(313112, 0, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 313112),
(313113, 0, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 313113);

-- boss loot
DELETE FROM creature_loot_template WHERE entry IN (30452, 30451, 30449, 28860, 288601, 288602, 288603, 31534, 31520, 31535, 31311, 313111, 313112, 313113);
-- Tenebron, Shadron, Vesperon
UPDATE creature_template SET lootid=30449 WHERE entry IN (30452, 30451, 30449, 31534, 31520, 31535);
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
(30449, 47241, 100, 0, 1, 1, 0, 0, 0);
-- Sartharion
-- references
DELETE FROM reference_loot_template WHERE entry IN (615000, 615001, 615002, 615010, 615011, 615012);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
-- normal, no drakes
(615000, 40426, 0, 2, 1, 1, 0, 0, 0),
(615000, 40427, 0, 2, 1, 1, 0, 0, 0),
(615000, 40428, 0, 2, 1, 1, 0, 0, 0),
(615000, 40429, 0, 2, 1, 1, 0, 0, 0),
(615000, 40430, 0, 2, 1, 1, 0, 0, 0),
(615000, 40613, 0, 1, 1, 1, 0, 0, 0),
(615000, 40614, 0, 1, 1, 1, 0, 0, 0),
(615000, 40615, 0, 1, 1, 1, 0, 0, 0),
(615000, 43345, 100, 0, 1, 1, 0, 0, 0),
(615000, 43347, 100, 0, 1, 1, 0, 0, 0),
-- normal, 1 drake
(615001, 43992, 0, 1, 1, 1, 0, 0, 0),
(615001, 43988, 0, 1, 1, 1, 0, 0, 0),
(615001, 43990, 0, 1, 1, 1, 0, 0, 0),
(615001, 43989, 0, 1, 1, 1, 0, 0, 0),
(615001, 43991, 0, 1, 1, 1, 0, 0, 0),
-- normal, 2 drakes
(615002, 43995, 0, 1, 1, 1, 0, 0, 0),
(615002, 43998, 0, 1, 1, 1, 0, 0, 0),
(615002, 43994, 0, 1, 1, 1, 0, 0, 0),
(615002, 43996, 0, 1, 1, 1, 0, 0, 0),
(615002, 43993, 0, 1, 1, 1, 0, 0, 0),
-- heroic, no drakes
(615010, 40431, 0, 2, 1, 1, 0, 0, 0),
(615010, 40432, 0, 3, 1, 1, 0, 0, 0),
(615010, 40433, 0, 3, 1, 1, 0, 0, 0),
(615010, 40437, 0, 2, 1, 1, 0, 0, 0),
(615010, 40438, 0, 3, 1, 1, 0, 0, 0),
(615010, 40439, 0, 3, 1, 1, 0, 0, 0),
(615010, 40446, 0, 2, 1, 1, 0, 0, 0),
(615010, 40451, 0, 2, 1, 1, 0, 0, 0),
(615010, 40453, 0, 3, 1, 1, 0, 0, 0),
(615010, 40455, 0, 2, 1, 1, 0, 0, 0),
(615010, 40628, 0, 1, 1, 1, 0, 0, 0),
(615010, 40629, 0, 1, 1, 1, 0, 0, 0),
(615010, 40630, 0, 1, 1, 1, 0, 0, 0),
(615010, 43345, 100, 0, 1, 1, 0, 0, 0),
(615010, 43346, 100, 0, 1, 1, 0, 0, 0),
-- heroic, 1 drake
(615011, 44003, 0, 1, 1, 1, 0, 0, 0),
(615011, 44002, 0, 1, 1, 1, 0, 0, 0),
(615011, 44000, 0, 1, 1, 1, 0, 0, 0),
(615011, 44004, 0, 1, 1, 1, 0, 0, 0),
-- heroic, 2 drakes
(615012, 44007, 0, 1, 1, 1, 0, 0, 0),
(615012, 44006, 0, 1, 1, 1, 0, 0, 0),
(615012, 44005, 0, 1, 1, 1, 0, 0, 0),
(615012, 44008, 0, 1, 1, 1, 0, 0, 0),
(615012, 44011, 0, 1, 1, 1, 0, 0, 0);
UPDATE creature_template SET lootid=entry WHERE entry IN (28860, 288601, 288602, 288603, 31311, 313111, 313112, 313113);
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
-- normal, no drakes
(28860, 615000, 100, 0, -615000, 1, 0, 0, 0),
(28860, 47241, 100, 0, 1, 1, 0, 0, 0),
-- normal, 1 drake
(288601, 615000, 100, 0, -615000, 1, 0, 0, 0),
(288601, 615001, 100, 0, -615001, 1, 0, 0, 0),
(288601, 47241, 100, 0, 2, 2, 0, 0, 0),
-- normal, 2 drakes
(288602, 615000, 100, 0, -615000, 1, 0, 0, 0),
(288602, 615001, 100, 0, -615001, 1, 0, 0, 0),
(288602, 615002, 100, 0, -615002, 1, 0, 0, 0),
(288602, 47241, 100, 0, 3, 3, 0, 0, 0),
-- normal, 3 drakes
(288603, 615000, 100, 0, -615000, 1, 0, 0, 0),
(288603, 615001, 100, 0, -615001, 1, 0, 0, 0),
(288603, 615002, 100, 0, -615002, 1, 0, 0, 0),
(288603, 43986, 100, 0, 1, 1, 0, 0, 0),
(288603, 47241, 100, 0, 4, 4, 0, 0, 0),
-- heroic, no drakes
(31311, 615010, 100, 0, -615010, 1, 0, 0, 0),
(31311, 47241, 100, 0, 1, 1, 0, 0, 0),
-- heroic, 1 drake
(313111, 615010, 100, 0, -615010, 1, 0, 0, 0),
(313111, 615011, 100, 0, -615011, 1, 0, 0, 0),
(313111, 47241, 100, 0, 2, 2, 0, 0, 0),
-- heroic, 2 drakes
(313112, 615010, 100, 0, -615010, 1, 0, 0, 0),
(313112, 615011, 100, 0, -615011, 1, 0, 0, 0),
(313112, 615012, 100, 0, -615012, 1, 0, 0, 0),
(313112, 47241, 100, 0, 3, 3, 0, 0, 0),
-- heroic, 3 drakes
(313113, 615010, 100, 0, -615010, 1, 0, 0, 0),
(313113, 615011, 100, 0, -615011, 1, 0, 0, 0),
(313113, 615012, 100, 0, -615012, 1, 0, 0, 0),
(313113, 43954, 100, 0, 1, 1, 0, 0, 0),
(313113, 47241, 100, 0, 4, 4, 0, 0, 0);
