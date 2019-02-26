script_sql_drop="drop_procedure.sql"
script_sql_create="check_by_timestamp_mac_address_proc.sql"
script_sql_run="run_procedure.sql"

res="full_procedure.sql"
run_script="./execute_query.sh"

rm -f $res

cat $script_sql_drop \
  $script_sql_create \
  $script_sql_run > $res

sudo $run_script $res

