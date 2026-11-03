// SPDX-License-Identifier: GPL-3.0

#include <pthread.h>

struct station {
	pthread_mutex_t lock;         
	pthread_cond_t car_arrived;   
	pthread_cond_t all_boarded;   
	int waiting;                  
	int free_seats;               
	int boarding;                 
	int car_present;              
};

void station_init(struct station *station);
void station_load_car(struct station *station, int count);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

void station_init(struct station *station)
{
	pthread_mutex_init(&station->lock, NULL);
	pthread_cond_init(&station->car_arrived, NULL);
	pthread_cond_init(&station->all_boarded, NULL);
	station->waiting = 0;
	station->free_seats = 0;
	station->boarding = 0;
	station->car_present = 0;
}

void station_load_car(struct station *station, int count)
{
	if (count <= 0)
		return;

	pthread_mutex_lock(&station->lock);

	station->car_present = 1;
	station->free_seats = count;

	pthread_cond_broadcast(&station->car_arrived);

	
	while (station->boarding > 0 ||
	       (station->waiting > 0 && station->free_seats > 0))
		pthread_cond_wait(&station->all_boarded, &station->lock);

	station->car_present = 0;
	station->free_seats = 0;

	pthread_mutex_unlock(&station->lock);
}

void station_wait_for_car(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	station->waiting++;

	while (!station->car_present || station->free_seats == 0)
		pthread_cond_wait(&station->car_arrived, &station->lock);

	station->free_seats--;
	station->waiting--;
	station->boarding++;

	pthread_mutex_unlock(&station->lock);
}

void station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	station->boarding--;
	if (station->boarding == 0)
		pthread_cond_signal(&station->all_boarded);

	pthread_mutex_unlock(&station->lock);
}
