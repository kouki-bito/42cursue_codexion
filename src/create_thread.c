/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:11:10 by kbito             #+#    #+#             */
/*   Updated: 2026/08/28 18:21:01 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <unistd.h>

void	finish_threads(t_data *data);
void	join_created_threads(t_data *data, int num);

int	init_thread(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_create(&data->monitor, NULL, &monitor, data->coder))
	{
		return (0);
	}
	while (i < data->number_of_coders)
	{
		if (pthread_create(&(data->coder[i].thread), NULL, &coder_routine,
				&(data->coder[i])))
		{
			finish_threads(data);
			join_created_threads(data, i);
			return (0);
		}
		i++;
	}
	return (1);
}

void	finish_threads(t_data *data)
{
	pthread_mutex_lock(&data->data_mutex);
	data->is_simulation_ended = 1;
	pthread_mutex_unlock(&data->data_mutex);
	pthread_mutex_lock(&data->scheduler_mutex);
	broadcast_coders(data->coder);
	pthread_cond_broadcast(&data->coder[0].data->scheduler_cond);
	pthread_cond_broadcast(&data->coder[0].data->state_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
}

void	join_created_threads(t_data *data, int num)
{
	int	i;

	i = 0;
	pthread_join(data->monitor, NULL);
	while (i < num)
	{
		pthread_join(data->coder[i].thread, NULL);
		i++;
	}
}

int	join_thread(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_join(data->monitor, NULL))
	{
		return (0);
	}
	while (i < data->number_of_coders)
	{
		if (pthread_join(data->coder[i].thread, NULL))
		{
			return (0);
		}
		i++;
	}
	return (1);
}
