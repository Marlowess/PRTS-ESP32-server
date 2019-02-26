SET @a = '1538495182';

SELECT count(*)
FROM probe_requests pr
WHERE pr.timestamp <> @a;
