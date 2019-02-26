SET @atmps1 = '-1';
SET @atmps2 = '1538495182';
SET @aboards_no = -1;

SELECT pr.mac_address, pr.timestamp, count(*)
FROM probe_requests pr
WHERE pr.timestamp > @atmps1 AND pr.timestamp < @atmps2
GROUP BY pr.mac_address AND pr.timestamp;
