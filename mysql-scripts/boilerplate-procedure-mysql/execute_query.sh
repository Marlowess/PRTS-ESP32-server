#!/bin/bash
# password = password

if [ $# -ne 1 ] ; then
  echo "Usage: $0 query_script.sql"
  exit -1
elif [ ! -f $1 ] ; then
  echo "Error: '$1' not exist or is not a file"
  exit -2
else
  res=$(echo $1 | grep -e ".sql$")
  if [ "res" = "" ] ; then
    echo "Error: '$1' must be a '.sql' source script"
    exit -3
  fi
fi

query=$1

sudo mysql -h localhost \
    -u root -p  \
    probe_requests_db < $query
exit 0
