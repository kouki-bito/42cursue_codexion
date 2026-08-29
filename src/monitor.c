/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:03:36 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 21:31:16 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int			check_start(t_data *data);
void		wait_coders(t_data *data);
void		set_all_coder_time(t_data *data);
int			check_coder_compile_count(t_coder *coder);

long long	get_min_burnout(t_coder *coder);
int			check_is_finished(t_data *data);
static void	wait_monitor_event(t_data *data, t_coder *coders);

void	broadcast_coders(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders[0].data->number_of_coders)
	{
		pthread_mutex_lock(&coders[i].action_sleep_mutex);
		pthread_cond_broadcast(&coders[i].action_sleep_cond);
		pthread_mutex_unlock(&coders[i].action_sleep_mutex);
		i++;
	}
}

void	*monitor(void *pointer)
{
	t_coder	*coders;
	t_data	*data;

	coders = (t_coder *)pointer;
	data = coders[0].data;
	wait_coders(coders[0].data);
	pthread_mutex_lock(&data->data_mutex);
	while (!data->is_simulation_ended
		&& data->is_finished != data->number_of_coders && !check_dead(coders))
	{
		wait_monitor_event(data, coders);
	}
	data->is_simulation_ended = 1;
	pthread_mutex_unlock(&data->data_mutex);
	if (data->burn_coder)
		print_log(data, data->burn_coder, "burn", get_time_ms());
	pthread_mutex_lock(&data->scheduler_mutex);
	broadcast_coders(coders);
	pthread_cond_broadcast(&coders[0].data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
	return ((void *)(1));
}

void	wait_coders(t_data *data)
{
	pthread_mutex_lock(&data->data_mutex);
	while (data->read_count < data->number_of_coders
		&& !data->is_simulation_ended)
	{
		pthread_cond_wait(&data->state_cond, &data->data_mutex);
	}
	if (!data->is_simulation_ended)
	{
		data->start_time = get_time_ms();
		set_all_coder_time(data);
		data->start_flag = 1;
	}
	pthread_cond_broadcast(&data->state_cond);
	pthread_mutex_unlock(&data->data_mutex);
}

static void	wait_monitor_event(t_data *data, t_coder *coders)
{
	long long		min_burnout;
	struct timespec	ts;

	min_burnout = get_min_burnout(coders);
	if (min_burnout == LLONG_MAX)
		pthread_cond_wait(&data->state_cond, &data->data_mutex);
	else
	{
		ts = mono_deadline_to_ts(min_burnout);
		pthread_cond_timedwait(&data->state_cond, &data->data_mutex, &ts);
	}
}

int	check_dead(t_coder *coders)
{
	int			i;
	long long	time;
	long long	now;
	t_data		*data;

	i = 0;
	data = coders[0].data;
	while (i < coders[0].data->number_of_coders)
	{
		pthread_mutex_lock(&(coders[i].coder_mutex));
		if (coders[i].count_compile < data->number_of_compiles_required)
		{
			time = coders[i].burn_out_time - coders[0].data->start_time;
			now = get_time_ms() - coders[0].data->start_time;
			if ((time <= now))
			{
				coders[0].data->burn_coder = &coders[i];
				pthread_mutex_unlock(&(coders[i].coder_mutex));
				return (1);
			}
		}
		pthread_mutex_unlock(&(coders[i].coder_mutex));
		i++;
	}
	return (0);
}
