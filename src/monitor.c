#include "codexion.h"

int	monitor(void *pointer)
{
	t_coder	*coders;

	coders = (t_coder *)pointer;
	while (1)
	{
		if (check_dead(coders))
		{
			pthread_mutex_lock(&(coders[0].data->data_mutex));
			coders[0].data->is_simulation_ended = 1;
			pthread_mutex_unlock(&(coders[0].data->data_mutex));
			return (1);
		}
	}
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
		pthread_mutex_lock(&(coders[i].coder_mutex));
		time = get_time_ms();
		if ((time - coders[i].last_compile) >= burn_out)
		{
			printf("%ld %d burned out", get_time_ms(), coder[i].id);
			pthread_mutex_unlock(&(coders[i].coder_mutex));
			return (1);
		}
		i++;
	}
	return (0);
}
