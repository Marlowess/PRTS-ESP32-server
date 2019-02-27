#!/bin/bash

mysql -u root \
  --password=password \
  --default_character_set utf8 \
  probe_requests_db < trigger_insert_new_probe_request.sql
