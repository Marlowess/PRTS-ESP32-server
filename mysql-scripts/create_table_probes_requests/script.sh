#!/bin/bash
# password = password

sudo mysql -h localhost \
    -u root -p  \
    probe_requests_db < create_table_probes_requests.sql
exit 0
