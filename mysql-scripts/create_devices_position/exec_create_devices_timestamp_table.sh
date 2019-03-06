#!/bin/bash

mysql -u root \
  --password=password \
  probe_requests_db < create_devices_timestamps_table.sql
