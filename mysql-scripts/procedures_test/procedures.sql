DROP PROCEDURE IF EXISTS procedure1;

DELIMITER //
CREATE PROCEDURE procedure1(OUT param INT)
BEGIN
  DECLARE b INT DEFAULT 0;
  SET param = 0;
  SELECT count(*) INTO b
  FROM probe_requests;
  SET param = b;
END; //
DELIMITER ;

CALL procedure1 (@a);
SELECT @a;
