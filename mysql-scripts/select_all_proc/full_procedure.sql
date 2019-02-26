DROP PROCEDURE IF EXISTS select_all_proc_my;


DELIMITER //
  CREATE PROCEDURE select_all_proc_my()
    BEGIN
    DECLARE mac VARCHAR(255) DEFAULT "";
    DECLARE tmsp VARCHAR(255) DEFAULT "";
    DECLARE data_finished INT DEFAULT 0;
    DECLARE n_occr INT DEFAULT 0;
    DECLARE val_occr INT DEFAULT 0;

    DECLARE cursor_data CURSOR FOR
      SELECT mac_address, COUNT(*) AS occr_n
      FROM probe_requests
      GROUP BY mac_address
      HAVING COUNT(*) > val_occr;

    -- declare NOT FOUND handler
    DECLARE CONTINUE HANDLER
      FOR NOT FOUND SET data_finished = 1;

    -- loop using opened cursor
    OPEN cursor_data;
      get_data : LOOP
        FETCH cursor_data INTO mac, n_occr;
        IF data_finished = 1 THEN
          LEAVE get_data;
        END IF;
        SELECT concat('mac: ', mac, ' n. occr:', n_occr);
      END LOOP get_data;
    CLOSE cursor_data;

    END; //
DELIMITER ;

SET @n = 0;

--SELECT @n := COUNT(*)
--FROM mysql.proc
--  WHERE name = 'select_all_proc_my';

IF ( @n == 0 ) THEN
  SELECT CONCAT('Procedure does not exist: ', ' select_all_proc_my');
ELSE
  SELECT CONCAT('Running', ' select_all_proc_my');
  CALL select_all_proc_my();
END IF;

