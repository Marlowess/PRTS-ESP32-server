DROP TABLE IF EXISTS devices_timestamps_pos;

CREATE TABLE IF NOT EXISTS  devices_timestamps_pos (
    mac_address_device VARCHAR(255) NOT NULL,
    timestamp VARCHAR(255) NOT NULL,
    pos_x FLOAT NOT NULL,
    pos_y FLOAT NOT NULL,
    PRIMARY KEY (device, timestamp)
) ENGINE=INNODB;
