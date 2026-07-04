#include "codexion.h"
#include <time.h>
#include <sys/time.h>
void	*execute_safely(pthread_mutex_t *mutex, void *(*func)(t_coder *),
		t_coder *arg)
{
	void	*res;

	pthread_mutex_lock(mutex);
	printf("mutex lock id:%d\n",arg->id);
	res = (void *)func(arg);
	pthread_mutex_unlock(mutex);
	printf("mutex unlock id:%d\n",arg->id);
	return (res);
}

void	*take(t_coder *coder)
{
	// int a=5;
	pthread_cond_t cond;
	pthread_cond_init(&cond,NULL);
	struct timespec ts;
	struct timeval tv;
	// ts.tv_sec=5;
	// pthread_mutex_lock(&coder->coder_mutex);
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
	printf("[定期処理] 実行中... (現在の目標秒数: %ld, id:%d)\n", ts.tv_sec,coder->id);
	ts.tv_sec += 1;
	pthread_cond_timedwait(&cond,&coder->coder_mutex,&ts);
	// printf("coder id:%d result:%d\n",coder->id,a);
	// pthread_mutex_unlock(&coder->coder_mutex);
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
	printf("[定期処理] 実行終了... (現在の目標秒数: %ld, id:%d)\n", ts.tv_sec,coder->id);


	return NULL;
}
