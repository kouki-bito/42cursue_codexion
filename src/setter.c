#include "codexion.h"

void	set_dongle_cool_time(t_dongle *first, t_dongle *second,
		long long cool_time)
{
	long long	time;

	time = get_time_ms();
	pthread_mutex_lock(&first->cool_down_mutex);
	pthread_mutex_lock(&second->cool_down_mutex);
	first->cool_time = time + cool_time;
	second->cool_time = time + cool_time;
	pthread_mutex_unlock(&first->cool_down_mutex);
	pthread_mutex_unlock(&second->cool_down_mutex);
}
void	set_dongle_use(t_dongle *first, t_dongle *second, int use)
{
	pthread_mutex_lock(&second->mutex);
	pthread_mutex_lock(&first->mutex);
	first->take_in_use = use;
	second->take_in_use = use;
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}