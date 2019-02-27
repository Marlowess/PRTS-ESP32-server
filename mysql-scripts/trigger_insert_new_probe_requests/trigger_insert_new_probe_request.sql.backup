DROP TRIGGER IF EXISTS insert_probe_request;
DELIMITER $$

CREATE
    TRIGGER `insert_probe_request` AFTER INSERT
    ON `probe_requests_db`.`probe_requests`
    FOR EACH ROW
    BEGIN
        DECLARE n INT;
        
        SELECT pos INTO n
        FROM devices_timestamps
        WHERE devices_timestamps.device = NEW.mac_address_device
            AND devices_timestamps.timestamp = NEW.timestamp;
        
        IF n = NULL THEN
            insert into devices_timestamps(`device`, `rssi0`, `rssi1`, `rssi2`, `rssi3`, `pos`)
                values('NEW.mac_address_device', NEW.signal_strength, 0,0,0, 1);
        END IF;
        IF n = 1 THEN
            UPDATE devices_timestamps
            SET devices_timestamps.pos = 2, devices_timestamps.rssi1 = 'NEW.signal_strength'
            WHERE devices_timestamps.device = NEW.mac_address_device
            AND devices_timestamps.timestamp = NEW.timestamp;
        END IF;
        IF n = 2 THEN
            UPDATE devices_timestamps
            SET devices_timestamps.pos = 3, devices_timestamps.rssi2 = 'NEW.signal_strength'
            WHERE devices_timestamps.device = NEW.mac_address_device
            AND devices_timestamps.timestamp = NEW.timestamp;
        END IF;
        IF n = 3 THEN
            UPDATE devices_timestamps
            SET devices_timestamps.pos = 4, devices_timestamps.rssi3 = 'NEW.signal_strength'
            WHERE devices_timestamps.device = NEW.mac_address_device
            AND devices_timestamps.timestamp = NEW.timestamp;
        END IF;

END $$

DELIMITER ;
