#include "codexion.h"

void	*monitor(void *pointer)
{
	t_coder	*coders;

	coders = (t_coder *)pointer;
	while (!is_simulation_ended(coders[0].data))
	{
		if (check_dead(coders))
		{
			pthread_mutex_lock(&(coders[0].data->data_mutex));
			coders[0].data->is_simulation_ended = 1;
			pthread_mutex_unlock(&(coders[0].data->data_mutex));
			pthread_cond_broadcast(&coders[0].data->usleep_cond);
			return (void *)(1);
		}
	}
}
void	wait_coders(t_data *data)
{
	while (1)
	{
		if (check_start(data))
		{
			pthread_mutex_lock(&data->data_mutex);
			data->start_time = get_time_ms();
			pthread_mutex_lock(&data->data_mutex);
			pthread_cond_broadcast(&data->usleep_cond);
			break ;
		}
	}
}
int	check_start(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->data_mutex);
	while (i < data->number_of_coders)
	{
		if (data->start_flag & 1 << i == 0)
		{
			pthread_mutex_unlock(&data->data_mutex);
			return (0);
		}
	}
	pthread_mutex_unlock(&data->data_mutex);
	return (1);
}
int	check_dead(t_coder *coders)
{
	int			i;
	long long	time;
	long long	burn_out;

	i = 0;
	burn_out = coders[i].data->time_to_burnout;
	while (i < coders[0].data->number_of_coders)
	{
		time = get_burn_out(&coders[i]);
		pthread_mutex_lock(&(coders[i].coder_mutex));
		if ((time >= burn_out))
		{
			pthread_mutex_lock(&coders[0].data->log_mutex);
			printf("%lld %d burned out", get_time_ms(), coders[i].id);
			pthread_mutex_unlock(&coders[0].data->log_mutex);
			pthread_mutex_unlock(&(coders[i].coder_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(coders[i].coder_mutex));
		i++;
	}
	return (0);
}
