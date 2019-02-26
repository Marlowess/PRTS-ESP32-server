DROP PROCEDURE IF EXISTS boilerplate_proc;


DELIMITER //
  CREATE PROCEDURE boilerplate_proc()
    BEGIN
    SELECT CONCAT('Hello,', 'World!');
    END; //
DELIMITER ;

CALL boilerplate_proc();
