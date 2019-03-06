-- Define procedure
DELIMITER //

drop procedure if exists test_and_insert_procedure //

create procedure test_and_insert_procedure()
begin

  SET @target_x_pos = -2;
  SET @target_y_pos = -4;
  SET @target_timestamp = '1551202315';
  SET @target_device = '98f7d77f8b40';

  SET @occr = -1;

  select count(*)
  from devices_timestamps_pos dtp
  where dtp.device = @target_device
    and dtp.timestamp = @target_timestamp;

  IF (@occr != -1) THEN
    insert into devices_timestamps_pos(`device`,`timestamp`,`pos_x`,`pos_y`)
      values(@target_device, @target_timestamp, @target_x_pos, @target_y_pos);
  END IF;

end //

-- Execute the procedure
call test_and_insert_procedure();

-- Drop the procedure
drop procedure test_and_insert_procedure();

DELIMITER;
