script_execute_query = ./execute_query.sh

drop_proc   = drop_procedure.sql
create_proc = check_by_timestamp_mac_address_proc.sql
run_proc    = run_procedure.sql

run: drop_create_proc
	sudo $(script_execute_query) $(run_proc)
drop_create_proc:
	sudo $(script_execute_query) $(drop_proc)
	sudo $(script_execute_query) $(create_proc)

