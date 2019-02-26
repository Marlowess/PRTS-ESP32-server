#!/bin/bash
# password = password

sudo mysql -h localhost \
    -u root -p  \
    probe_requests_db < procedures.sql
exit 0
