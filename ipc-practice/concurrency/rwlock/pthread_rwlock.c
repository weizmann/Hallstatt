#include "pthread_rwlock.h"

int pthread_rwlock_init(pthread_rwlock_t* rw, pthread_rwlock_attr_t* attr) {
	int result;
	if (attr != NULL) {
		return EINVAL;
	}

	if ((result = pthread_mutex_init(&rw->rw_mutex), NULL) != 0) {
		goto err1;
	}
	if ((result = pthread_cond_init(&rw->rw_cond_readers, NULL)) != 0) {
		goto err2;
	}
	if ((result = pthread_cond_init(&rw->rw_cond_writers, NULL)) != 0) {
		goto err3;
	}

	rw->rw_cond_readers = 0;
	rw->rw_cond_writers = 0;
	rw->rw_nwaiting_readers = 0;
	rw->rw_nwaiting_writers = 0;
	rw->rw_refcount = 0;
	rw->rw_magic = RW_MAGIC;

err3:
	pthread_cond_destroy(&rw->rw_cond_readers);
err2:
	pthread_mutex_destroy(&rw->rw_mutex);
err1:
	return result;
}

int pthread_rwlock_destroy(pthread_rwlock_t* rw) {
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}
	if (rw->rw_cond_readers != 0
			|| rw->rw_cond_writers != 0
			|| rw->rw_refcount != 0) {
		return EBUSY;
	}

	pthread_mutex_destroy(&rw->rw_mutex);
	pthread_cond_destroy(&rw->rw_cond_readers);
	pthread_cond_destroy(&rw->rw_cond_writers);
	rw->rw_magic = RW_MAGIC;
	return 0;
}

int pthread_rwlock_rdlock(pthread_rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	while (rw->rw_refcount < 0 || rw->rw_nwaiting_writers >  0) {
		rw->rw_nwaiting_readers++;
		result = pthread_cond_wait(&rw->rw_cond_readers, &rw->rw_mutex);
		rw->rw_nwaiting_readers--;
		if (result != 0) {
			break;
		}
	}

	if (result == 0) {
		rw->rw_refcount++;
	}

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	if (rw->rw_refcount < 0 || rw->rw_nwaiting_writers > 0) {
		result = EBUSY;
	} else {
		rw->rw_refcount++;
	}

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int pthread_rwlock_wrlock(pthread_rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}
	if ((result = pthread_mutex_lock(rw->rw_mutex)) != 0) {
		return result;
	}

	while (rw->rw_refcount != 0) {
		rw->rw_nwaiting_writers++;
		result = pthread_cond_wait(&rw->rw_cond_writers, &rw->rw_mutex);
		rw->rw_nwaiting_writers--;
		if (result != 0) {
			break;
		}
	}

	if (result == 0) {
		rw->rw_refcount = -1;
	}

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	if (rw->rw_refcount != 0) {
		return EBUSY;
	} else {
		rw->rw_refcount = -1;
	}

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int pthread_rwlock_unlock(pthread_rwlock_t* rw) {
	int result;
	if (rw->rw_magic != RW_MAGIC) {
		return EINVAL;
	}

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0) {
		return result;
	}

	if (rw->rw_refcount > 0) {
		rw->rw_refcount--;
	} else if (rw->rw_refcount == -1) {
		rw->rw_refcount = 0;
	} else {
		printf("error ref_count");
		return EINVAL;
	}

	if (rw->rw_nwaiting_writers > 0) {
		if (rw->rw_refcount == 0) {
			result = pthread_cond_signal(&rw->rw_cond_writers);
		}
	} else if (rw->rw_nwaiting_readers > 0) {
		result = pthread_cond_broadcast(&rw->rw_cond_readers);
	}

	return result;
}

