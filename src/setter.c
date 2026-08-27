/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:36:24 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:16:33 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

void	set_dongle_cool_time(t_dongle *first, t_dongle *second,
		long long cool_time, long long now)
{
	pthread_mutex_lock(&first->cool_down_mutex);
	pthread_mutex_lock(&second->cool_down_mutex);
	first->cool_time = now + cool_time;
	second->cool_time = now + cool_time;
	pthread_mutex_unlock(&first->cool_down_mutex);
	pthread_mutex_unlock(&second->cool_down_mutex);
}

void	set_dongle_use(t_dongle *first, t_dongle *second, int use)
{
	first->take_in_use = use;
	second->take_in_use = use;
}

void	set_burn_out(t_coder *coder, long long time, long long now)
{
	pthread_mutex_lock(&coder->coder_mutex);
	coder->burn_out_time = now + time;
	pthread_mutex_unlock(&coder->coder_mutex);
}

void	set_all_coder_time(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coder[i].coder_mutex);
		data->coder[i].burn_out_time = data->time_to_burnout + data->start_time;
		pthread_mutex_unlock(&data->coder[i].coder_mutex);
		i++;
	}
}
