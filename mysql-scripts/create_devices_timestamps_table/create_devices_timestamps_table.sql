DROP TABLE IF EXISTS devices_timestamps;

CREATE TABLE IF NOT EXISTS  devices_timestamps (
    device VARCHAR(255) NOT NULL,
    timestamp VARCHAR(255) NOT NULL,
    rssi0 INT NOT NULL,
    rssi1 INT NOT NULL,
    rssi2 INT NOT NULL,
    rssi3 INT NOT NULL,
    pos INT NOT NULL,
    PRIMARY KEY (device, timestamp)
) ENGINE=INNODB;
