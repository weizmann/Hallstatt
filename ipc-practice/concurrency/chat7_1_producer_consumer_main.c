#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 1000000
#define MAX_THREADS 100

int nitems; // read only variable
struct {
	pthread_mutex_t mutex;
	int buffer[MAX_ITEMS];
	int nputs;
	int nval;
} shared = {
	PTHREAD_MUTEX_INITIALIZER
};

void *producer(void*), *consumer(void*);

int main(int argc, const char** argv)
{
	int nthreads, count[MAX_THREADS];
	pthread_t tid_producer[MAX_THREADS], tid_consumer;

	if (argc != 3)
	{
		printf("usage: prodcons <#items> <#threads>\n");
		return -1;
	}

	nitems = (atoi(argv[1]) < MAX_ITEMS) ? atoi(argv[1]) : MAX_ITEMS;
	nthreads = (atoi(argv[2]) < MAX_THREADS) ? atoi(argv[2]) : MAX_THREADS;

	for (int index = 0; index < nthreads; index++)
	{
		count[index] = 0;
		pthread_create(&tid_producer[index], NULL, producer, &count[index]);
	}

	for (int index = 0; index < nthreads; index++)
	{
		pthread_join(tid_producer[index], NULL);
		printf("count[%d]=%d\n", index, count[index]);
	}

	pthread_create(&tid_consumer, NULL, consumer, NULL);
	pthread_join(tid_consumer, NULL);

	return 0;
}

void* producer(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&shared.mutex);
		if (shared.nputs >= nitems)
		{
			pthread_mutex_unlock(&shared.mutex);
			return NULL;
		}
		shared.buffer[shared.nputs] = shared.nval;
		shared.nputs++;
		shared.nval++;
		pthread_mutex_unlock(&shared.mutex);
		*((int *)arg) += 1;
	}
}

void* consumer(void* arg)
{
	for (int index = 0; index < nitems; index++)
	{
		if (shared.buffer[index] != index)
		{
			printf("buffer[%d] error!!!", index);
		}
	}
	return NULL;
}
