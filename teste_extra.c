#include <stdio.h>
#include "sync_car.c"

int main(void){
    struct station s;
    station_init(&s);
    station_load_car(&s, 0);
	station_load_car(&s, -5);
	station_load_car(&s, 10);
	printf("Extras OK: todos retornaram imediatamente\n");
	return 0;
}