#ifndef PTHREAD_RWLOCK_H
#define PTHREAD_RWLOCK_H

#include <pthread.h>
#include <errno.h>

typedef struct {
	pthread_mutex_t rw_mutex;
	pthread_cond_t rw_cond_readers;
	pthread_cond_t rw_cond_writers;

	int rw_magic;
	int rw_nwaiting_readers;
	int rw_nwaiting_writers;
	int rw_refcount;
} pthread_rwlock_t;

#define RW_MAGIC 0x12345678

#define PTHREAD_RWLOCK_INITIALIZER \
	{PTHREAD_MUTEX_INITIALIZER, \
	PTHREAD_COND_INITIALIZER, \
	PTHREAD_COND_INITIALIZER, \
	RW_MAGIC, \
	0, \
	0, \
	0 \
	}

typedef int pthread_rwlock_attr_t;

int pthread_rwlock_init(pthread_rwlock_t*, pthread_rwlock_attr_t*);
int pthread_rwlock_destroy(pthread_rwlock_t*);

int pthread_rwlock_rdlock(pthread_rwlock_t*);
int pthread_rwlock_wrlock(pthread_rwlock_t*);
int pthread_rwlock_tryrdlock(pthread_rwlock_t*);
int pthread_rwlock_trywrlock(pthread_rwlock_t*);
int pthread_rwlock_unlock(pthread_rwlock_t*);

#endif
