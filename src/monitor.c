#include "codexion.h"

int	check_start(t_data *data);
void	wait_coders(t_data *data);
void	*monitor(void *pointer)
{
	t_coder	*coders;

	coders = (t_coder *)pointer;
	wait_coders(coders[0].data);
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
return (void *)(1);

}
void	wait_coders(t_data *data)
{
	while (1)
	{
		if (check_start(data))
		{
			pthread_mutex_lock(&data->data_mutex);
			data->start_time = get_time_ms();
			printf("monitor %lld\n",data->start_time);
			pthread_mutex_unlock(&data->data_mutex);
			pthread_cond_broadcast(&data->usleep_cond);
			break ;
		}
	}
}
int	check_start(t_data *data)
{
	int	i;
	int flag;
	i = 0;

	pthread_mutex_lock(&data->data_mutex);
	while (i < data->number_of_coders)
	{
		flag = data->start_flag & (1 << i);
		if (flag == 0)
		{
			pthread_mutex_unlock(&data->data_mutex);

			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&data->data_mutex);
	return (1);
}
int	check_dead(t_coder *coders)
{
	int			i;
	long long	time;
	long long	now;


	i = 0;
	while (i < coders[0].data->number_of_coders)
	{
		time = get_burn_out(&coders[i]) - coders[0].data->start_time;

		pthread_mutex_lock(&(coders[i].coder_mutex));
		now = get_time_ms()-coders[0].data->start_time;
		if ((time >= now))
		{
			print_log(coders[0].data,&coders[i],"burn");


			pthread_mutex_unlock(&(coders[i].coder_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(coders[i].coder_mutex));
		i++;
	}
	return (0);
}
