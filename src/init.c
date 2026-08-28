/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:02:21 by kbito             #+#    #+#             */
/*   Updated: 2026/08/28 15:47:31 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

int	init_dongle_mutex_and_cond(t_data *data);
int	init_coder_mutex_and_cond(t_data *data);

int	init_coder(t_data *data)
{
	int	i;

	i = 0;
	if (data == NULL)
		return (0);
	data->coder = (t_coder *)malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coder)
		return (0);
	while (i < data->number_of_coders)
	{
		data->coder[i].burn_out_time = 0;
		data->coder[i].id = i + 1;
		data->coder[i].left_dongle = &data->dongle[i];
		data->coder[i].count_compile = 0;
		data->coder[i].right_dongle = &data->dongle[(i + 1)
			% data->number_of_coders];
		data->coder[i].data = data;
		i++;
	}
	if (!init_coder_mutex_and_cond(data))
		return (0);
	return (1);
}

int	init_coder_mutex_and_cond(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_mutex_init(&(data->coder[i].coder_mutex), NULL))
		{
			destroy_coder(data->coder, i);
		}
		if (pthread_mutex_init(&(data->coder[i].action_sleep_mutex), NULL))
		{
			pthread_mutex_destroy(&(data->coder[i].coder_mutex));
			destroy_coder(data->coder, i);
			return (0);
		}
		if (pthread_cond_init(&(data->coder[i].action_sleep_cond), NULL))
		{
			pthread_mutex_destroy(&(data->coder[i].coder_mutex));
			pthread_mutex_destroy(&(data->coder[i].action_sleep_mutex));
			destroy_coder(data->coder, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_dongle(t_data *data)
{
	int	i;

	i = 0;
	if (!data)
		return (0);
	data->dongle = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongle)
		return (0);
	while (i < data->number_of_coders)
	{
		data->dongle[i].take_in_use = 0;
		data->dongle[i].cool_time = 0;
		data->dongle[i].heap.size = 0;
		data->dongle[i].id = i;
		data->dongle[i].heap.size = 0;
		i++;
	}
	if (!init_dongle_mutex_and_cond(data))
		return (0);
	return (1);
}

int	init_dongle_mutex_and_cond(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_mutex_init(&(data->dongle[i].mutex), NULL))
		{
			destroy_dongle(data->dongle, i);
			return (0);
		}
		if (pthread_mutex_init(&(data->dongle[i].cool_down_mutex), NULL))
		{
			pthread_mutex_destroy(&(data->dongle[i].mutex));
			destroy_dongle(data->dongle, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_mutex_and_cond(t_data *data)
{
	if (!pthread_cond_init(&(data->scheduler_cond), NULL)
		&& !pthread_cond_init(&(data->state_cond), NULL)
		&& !pthread_mutex_init(&(data->data_mutex), NULL)
		&& !pthread_mutex_init(&(data->log_mutex), NULL)
		&& !pthread_mutex_init(&(data->scheduler_mutex), NULL))
		return (1);
	return (0);
}
