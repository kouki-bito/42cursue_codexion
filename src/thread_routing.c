#include "codexion.h"

long long	get_time_ms(void);
int			check_count_compile(t_coder *coder);
void	wait_for_start(t_coder *coder);
static void	execute_coder_cycle(t_coder *coder)
{
	take_dongles(coder);
	start_compile(coder);
	leave_dongle(coder);
	start_debuging(coder);
	start_refactor(coder);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_for_start(coder);
	while (!check_simulation_status(coder) && !check_count_compile(coder))
	{
		execute_coder_cycle(coder);
	}
	return (NULL);
}
int	check_count_compile(t_coder *coder)
{
	int	count;

	pthread_mutex_lock(&coder->coder_mutex);
	count = coder->count_compile;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (count <= coder->data->number_of_compiles_required);
}
void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->data_mutex);
	coder->data->start_flag = coder->data->start_flag | 1 << (coder->id-1);
	pthread_mutex_unlock(&coder->data->data_mutex);
	action_usleep(0, coder);
}
void	leave_dongle(t_coder *coder)
{
	if (check_simulation_status(coder))
		return ;
	set_dongle_use(coder->right_dongle, coder->left_dongle, 0);
	set_dongle_cool_time(coder->right_dongle, coder->left_dongle,
		coder->data->dongle_cooldown);
}

void	start_compile(t_coder *coder)
{
	long long	time;

	time = get_time_ms();
	if (!check_simulation_status(coder))
	{
		pthread_mutex_lock(&coder->coder_mutex);
		coder->last_compile = time;
		pthread_mutex_unlock(&coder->coder_mutex);
		printf("%lld %d is compiling", time, coder->id);
		action_usleep(coder->data->time_to_compile, coder);
		pthread_mutex_lock(&coder->coder_mutex);
		pthread_mutex_lock(&coder->data->data_mutex);
		if (!coder->data->is_simulation_ended)
			coder->count_compile += 1;
		pthread_mutex_unlock(&coder->data->data_mutex);
		pthread_mutex_unlock(&coder->coder_mutex);
	}
}
void	start_debuging(t_coder *coder)
{
	if (!check_simulation_status(coder))
	{
		printf("%lld %d is debuging", get_time_ms(), coder->id);
		action_usleep(coder->data->time_to_debug, coder);
	}
}
void	start_refactor(t_coder *coder)
{
	if (!check_simulation_status(coder))
	{
		printf("%lld %d is refactoring", get_time_ms(), coder->id);
		action_usleep(coder->data->time_to_refactor, coder);
	}
}
