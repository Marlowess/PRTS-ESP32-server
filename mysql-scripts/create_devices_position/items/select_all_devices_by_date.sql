SET @target_tmsp = '1551254570';
SET @target_tmsp2 = '1551254729';
-- SET @target_device = '98f7d77f8b40';

select distinct pr.mac_address_device
from probe_requests pr
where pr.timestamp > @target_tmsp
  and pr.timestamp < @target_tmsp2
order by pr.mac_address_device;
