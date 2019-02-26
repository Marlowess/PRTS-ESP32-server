#!/bin/bash

base_dir="./mysql_scripts_backup/"
backup_extention=".backup"

#echo "$base_dir$1$backup_extention"

if [ $# -ne 1 ] ; then
  echo "Usage: $0 soruce.sql"
  exit -1
elif [ ! -f $1 ] ; then
  echo "Error: '$1 'must be a existing file source"
  exit -2
elif [ ! -f $base_dir$1$backup_extention ] ; then
  echo "Error: '$1 'must be backuped before, has not been backuped right now"
  exit -3
else
  res=$(echo $1 | grep -e "*.sql$")
  if [ "$res" ] ; then
    echo "Error: both arg1 and arg2 must be a script with '.sql' extension"
    exit -4
  fi
fi

dest=$1
source=$1
echo "exec copy: $base_dir$source$backup_extention $dest"
cp $base_dir$source$backup_extention $dest


exit 0
