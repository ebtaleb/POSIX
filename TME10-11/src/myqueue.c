#include <myqueue.h>

static struct mq_attr *mq = NULL;

sem_t *blocked_r_mutex = NULL;
sem_t *blocked_w_mutex = NULL;

int
is_valid_fd(int fd)
{
	return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
}

mqd_t
mq_open(const char *nom, int mq_flags, int mode)
{
	mqd_t shmfd = shm_open(nom, mq_flags, mode);
	if (shmfd < 0) {
		perror("In shm_open()");
		return -1;
	}
	fprintf(stderr, "Created shared memory object %s\n", nom);

	ftruncate(shmfd, sizeof(struct mq_attr));

	mq = (struct mq_attr *)mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	if (mq == NULL) {
		perror("In mmap()");
		return -1;
	}
	fprintf(stderr, "Shared memory segment allocated correctly (%d bytes).\n", sizeof(struct mq_attr));

	mq->mq_flags = mq_flags & O_NONBLOCK;
	mq->mq_maxmsg = 32;
	mq->mq_msgsize = sizeof(msg);
	mq->mq_curmsgs = 0;

	// creer les semaphores d'attente
	mq->blocked_readers = "empty_queue";
	mq->blocked_writers = "full_queue";

	blocked_r_mutex = sem_open(mq->blocked_readers, O_CREAT, 0644, 0);
	blocked_w_mutex = sem_open(mq->blocked_writers, O_CREAT, 0644, mq->mq_maxmsg);

	// initialize priority queue
	mq->pq.front = mq->pq.rear = -1 ;
	int i;
	for (i = 0; i < mq->mq_maxmsg; i++)
	{
		mq->pq.queue[i].data[0] = '\0';
		mq->pq.queue[i].prio = mq->pq.queue[i].ord = 0 ;
		mq->pq.queue[i].length = 0 ;
	}

	return shmfd;
}

int
mq_close(mqd_t mqdes)
{
	return close(mqdes);
}

int
mq_unlink(const char *name)
{
	if ((munmap(mq, sizeof(struct mq_attr))) != 0) {
		return -1;
	}
	mq = NULL;

	sem_close(blocked_r_mutex);
	sem_close(blocked_w_mutex);

	sem_unlink(mq->blocked_readers);
	sem_unlink(mq->blocked_writers);

	return shm_unlink(name);
}

int
mq_getattr(mqd_t mqdes, struct mq_attr *mqstat)
{
	if (is_valid_fd(mqdes) == 0) {
		errno = EBADF;
		return -1;
	}

	mqstat->mq_flags = mq->mq_flags;
	mqstat->mq_maxmsg = mq->mq_maxmsg;
	mqstat->mq_msgsize = mq->mq_msgsize;
	mqstat->mq_curmsgs = mq->mq_curmsgs;

	return 0;
}


int
mq_setattr(mqd_t mqdes, const struct mq_attr *restrict mqstat, struct mq_attr *restrict omqstat)
{
	assert(mqstat != NULL);

	if (is_valid_fd(mqdes) == 0) {
		errno = EBADF;
		return -1;
	}

	if (omqstat != NULL) {
		omqstat->mq_flags = mq->mq_flags;
		omqstat->mq_maxmsg = mq->mq_maxmsg;
		omqstat->mq_msgsize = mq->mq_msgsize;
		omqstat->mq_curmsgs = mq->mq_curmsgs;
	}

	mq->mq_flags = mqstat->mq_flags;

	return 0;
}

int
mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio)
{
	if (is_valid_fd(mqdes) == 0) {
		errno = EBADF;
		return -1;
	}

	if ( (mq->mq_curmsgs == mq->mq_maxmsg) && (mq->mq_flags & O_NONBLOCK) ) {
		errno = EAGAIN;
		return -1;
	}

	if (msg_len > mq->mq_msgsize) {
		errno = EMSGSIZE;
		return -1;
	}

	if (msg_prio > MQ_PRIO_MAX) {
		errno = EINVAL;
		return -1;
	}


	msg *temp = malloc(sizeof(msg));
	int i, j;

	sem_wait(blocked_w_mutex);

	mq->pq.rear++ ;

	strcpy(mq->pq.queue[mq->pq.rear].data, msg_ptr);
	mq->pq.queue[mq->pq.rear].prio = msg_prio;
	mq->pq.queue[mq->pq.rear].ord = mq->mq_curmsgs + 1;
	mq->pq.queue[mq->pq.rear].length = msg_len;

	if (mq->pq.front == -1)
		mq->pq.front = 0 ;

	for (i = mq->pq.front ; i <= mq->pq.rear; i++)
	{
		for (j = i+1; j <= mq->pq.rear; j++)
		{
			if (mq->pq.queue[i].prio < mq->pq.queue[j].prio)
			{
				memcpy(temp, &mq->pq.queue[i], sizeof(msg));
				memcpy(&mq->pq.queue[i], &mq->pq.queue[j], sizeof(msg));
				memcpy(&mq->pq.queue[j], temp, sizeof(msg));
			}
			else
			{
				if (mq->pq.queue[i].prio == mq->pq.queue[j].prio)
				{
					if (mq->pq.queue[i].ord < mq->pq.queue[j].ord)
					{
						memcpy(temp, &mq->pq.queue[i], sizeof(msg));
						memcpy(&mq->pq.queue[i], &mq->pq.queue[j], sizeof(msg));
						memcpy(&mq->pq.queue[j], temp, sizeof(msg));
					}
				}
			}
		}
	}

	free(temp);

	if (mq->mq_curmsgs == 0 && mq->subscriber != -1) {
		kill(mq->subscriber, mq->notification_sig);
		mq->subscriber = -1;
		mq->notification_sig = -1;
	}

	mq->mq_curmsgs++;

	sem_post(blocked_r_mutex);

	return 0;
}

ssize_t
mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio)
{
	if (is_valid_fd(mqdes) == 0) {
		errno = EBADF;
		return -1;
	}

	if ( (mq->mq_curmsgs == 0) && (mq->mq_flags & O_NONBLOCK) ) {
		errno = EAGAIN;
		return -1;
	}

	if (msg_len < mq->mq_msgsize) {
		errno = EMSGSIZE;
		return -1;
	}

	sem_wait(blocked_r_mutex);

	strcpy(msg_ptr, mq->pq.queue[mq->pq.front].data);
	*msg_prio = mq->pq.queue[mq->pq.front].prio;

	mq->pq.queue[mq->pq.front].data[0] = '\0';
	mq->pq.queue[mq->pq.front].prio = 0;
	mq->pq.queue[mq->pq.front].length = 0;
	mq->pq.queue[mq->pq.front].ord = 0;

	if (mq->pq.front == mq->pq.rear)
		mq->pq.front = mq->pq.rear = -1;
	else
		mq->pq.front++;

	mq->mq_curmsgs--;

	sem_post(blocked_w_mutex);

	return 0;
}

mqd_t
mq_notify(mqd_t mqdes, const struct sigevent *notification)
{
	if (is_valid_fd(mqdes) == 0) {
		errno = EBADF;
		return -1;
	}

	if (notification != NULL) {
		if (mq->subscriber != -1) {
			errno = EBUSY;
			return -1;
		} else {
			mq->subscriber = getpid();
			mq->notification_sig = notification->sigev_signo;
		}
	} else {

		if (mq->subscriber == getpid()) {
			mq->subscriber = -1;
			mq->notification_sig = -1;
		}
	}

	return 0;
}

