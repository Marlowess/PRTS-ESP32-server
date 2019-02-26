DELIMITER //
CREATE PROCEDURE check_by_timestamp_mac_address_proc (
    IN tmsp1 VARCHAR(255), IN tmsp2 VARCHAR(255), IN boards_no INT)
BEGIN
  DECLARE data_finished INTEGER DEFAULT 0;
  DECLARE mac VARCHAR(255) DEFAULT "";
  DECLARE tmsp VARCHAR(255) DEFAULT "";
  DECLARE n_occr INT DEFAULT 0;

  -- declare cursor for data
  DECLARE cursor_data CURSOR FOR
    SELECT mac_address, timestamp
    FROM probe_requests
    WHERE timestamp > tmsp1 AND timestamp < tmsp2
    GROUP BY mac_address AND timestamp
    HAVING COUNT(*) > 0;

  -- declare NOT FOUND handler
  DECLARE CONTINUE HANDLER 
    FOR NOT FOUND SET data_finished = 1;

  OPEN cursor_data;
    get_data : LOOP
      FETCH cursor_data INTO mac, tmsp;
      IF data_finished = 1 THEN
        LEAVE get_data;
      END IF;
      SELECT concat('mac is ', mac, ' timestamp is ', tmsp);
    END LOOP get_data;
  CLOSE cursor_data;
END; //
DELIMITER ;

