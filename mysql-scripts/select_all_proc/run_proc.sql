SET @n = 0;

SELECT @n := COUNT(*)
  FROM mysql.proc
  WHERE name = 'select_all_proc_my';

IF ( @n == 0 ) THEN
  SELECT CONCAT('Procedure does not exist: ', ' select_all_proc_my');
ELSE
  SELECT CONCAT('Running', ' select_all_proc_my');
  CALL select_all_proc_my();
END IF;

