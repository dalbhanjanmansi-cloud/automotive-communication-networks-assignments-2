all:
	gcc src/engine_ecu.c -o engine_ecu
	gcc src/dashboard.c -o dashboard
	gcc src/can_monitor.c -o can_monitor

clean:
	rm -f engine_ecu dashboard can_monitor
