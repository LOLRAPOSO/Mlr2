// SPDX-License-Identifier: GPL-3.0
/*
 * Sincronizacao do Metro: um carro, varios passageiros (threads).
 * Usa somente 1 mutex e 2 variaveis de condicao por station.
 *
 * Declaracao da struct station e implementacao das funcoes em um unico arquivo.
 */

#include <pthread.h>

struct station {
	pthread_mutex_t lock;         /* unico lock da estacao */
	pthread_cond_t car_arrived;   /* passageiros esperam carro com vaga */
	pthread_cond_t all_boarded;   /* carro espera passageiros sentarem */
	int waiting;                  /* passageiros esperando na plataforma */
	int free_seats;               /* assentos ainda nao reservados */
	int boarding;                 /* reservaram assento, ainda nao sentaram */
	int car_present;              /* 1 se ha carro na estacao */
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
	/* Sem assentos (ou valor invalido): nada a fazer, retorna logo. */
	if (count <= 0)
		return;

	pthread_mutex_lock(&station->lock);

	station->car_present = 1;
	station->free_seats = count;

	/* Acorda todos os passageiros; cada um confere se ha vaga. */
	pthread_cond_broadcast(&station->car_arrived);

	/*
	 * Retorna quando ninguem esta embarcando e (o carro encheu ou
	 * nao ha mais ninguem esperando). Se nao ha ninguem esperando,
	 * a condicao ja e falsa e retorna imediatamente.
	 */
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

	/* while (nao if): reconfere apos acordar, outro pode ter pego a vaga */
	while (!station->car_present || station->free_seats == 0)
		pthread_cond_wait(&station->car_arrived, &station->lock);

	/* Reserva o assento: permite varios passageiros retornarem juntos. */
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
