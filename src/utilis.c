#include "codexion.h"

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
	int	a;

	a = coder->id;
	a *= 10;
	printf("coder id:%d result:%d\n",coder->id,a);
	return NULL;
}
