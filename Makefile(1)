CC     = gcc
CFLAGS = -Wall -Wextra -pthread


sync_car: sync_car.c sync_car-runner.c
	sed 's/"sync_car.h"/"sync_car.c"/' sync_car-runner.c > runner_build.c
	$(CC) $(CFLAGS) -o sync_car runner_build.c
	rm -f runner_build.c

clean:
	rm -f sync_car runner_build.c

.PHONY: clean
