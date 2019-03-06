
SET @target_x_pos = -2;
SET @target_y_pos = -4;

SET @target_timestamp = '1551202225';
SET @target_device = '98f7d55f8b40';


insert into devices_timestamps_pos(`device`,`timestamp`,`pos_x`,`pos_y`)
  values(@target_device, @target_timestamp, @target_x_pos, @target_y_pos);
