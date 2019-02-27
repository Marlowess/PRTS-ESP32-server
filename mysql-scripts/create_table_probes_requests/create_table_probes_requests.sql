-- drop to clean probe_requests table
DROP TABLE IF EXISTS probe_requests;

-- create from scratch probe_requests table
-- after having removed it in order to
-- throw away hold useless records
CREATE TABLE IF NOT EXISTS probe_requests (
    -- probe_id INT AUTO_INCREMENT,
    mac_address_board VARCHAR(255) NOT NULL,
    mac_address_device VARCHAR(255) NOT NULL,
    ssid VARCHAR(255),
    timestamp VARCHAR(255) NOT NULL,
    signal_strength INT NOT NULL,
    PRIMARY KEY (mac_address_board, mac_address_device, timestamp)
) ENGINE=INNODB;
