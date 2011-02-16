DELETE FROM `script_texts` WHERE `entry` IN
('-1712001','-1712002','-1712003','-1712004','-1712005','-1712006','-1712007','-1712008');

INSERT IGNORE INTO `script_texts` 
(`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) 
VALUES 
('-1712001', 'Soft meat! Come to me!', 'Soft meat! Come to me!', NULL, NULL, NULL, NULL, NULL, NULL, 'Мягкие пришли! Заходите!', '0', '0', '0', '0', NULL),
('-1712002', 'Get it!', 'Get it!', NULL, NULL, NULL, NULL, NULL, NULL, 'Лови кирпич!', '0', '0', '0', '0', NULL),
('-1712003', 'Into the ice!', 'Into the ice!', NULL, NULL, NULL, NULL, NULL, NULL, 'Всех заморожу!', '0', '0', '0', '0', NULL),
('-1712004', 'Stone curse!', 'Stone curse!', NULL, NULL, NULL, NULL, NULL, NULL, 'Окаменей!', '0', '0', '0', '0', NULL),
('-1712005', 'Gr-r-r-r!', 'Gr-r-r-r!', NULL, NULL, NULL, NULL, NULL, NULL, 'Ух, как я зол!', '0', '0', '0', '0', NULL),
('-1712006', 'It is cool!', 'It is cool!', NULL, NULL, NULL, NULL, NULL, NULL, 'Что, съел?', '0', '0', '0', '0', NULL),
('-1712007', 'O, no...', 'O, no...', NULL, NULL, NULL, NULL, NULL, NULL, 'Не может быть...', '0', '0', '0', '0', NULL),
('-1712008', 'Enrage!', 'Enrage!', NULL, NULL, NULL, NULL, NULL, NULL, 'Ну все, вы меня достали...', '0', '0', '0', '0', NULL);
