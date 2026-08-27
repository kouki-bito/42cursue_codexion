/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:31:12 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:20:57 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_burn_out(t_coder *coder)
{
	long long	bourn_out;

	pthread_mutex_lock(&coder->coder_mutex);
	bourn_out = coder->burn_out_time;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (bourn_out);
}

long long	get_min_burnout(t_coder *coder)
{
	long long	temp;
	long long	min;
	int			i;

	i = 0;
	min = LLONG_MAX;
	while (i < coder[0].data->number_of_coders)
	{
		if (check_count_compile(&coder[i]))
		{
			temp = get_burn_out(&coder[i]);
			if (temp < min)
				min = temp;
		}
		i++;
	}
	return (min);
}

long long	get_dongle_cool_time(t_dongle *dongle)
{
	long long	time;

	pthread_mutex_lock(&dongle->cool_down_mutex);
	time = dongle->cool_time;
	pthread_mutex_unlock(&dongle->cool_down_mutex);
	return (time);
}

long long	max_cool_time(t_dongle *first, t_dongle *second)
{
	long long	first_cooldown;
	long long	second_cooldown;

	first_cooldown = get_dongle_cool_time(first);
	second_cooldown = get_dongle_cool_time(second);
	if (first_cooldown >= second_cooldown)
		return (first_cooldown);
	else
		return (second_cooldown);
}

int	is_simulation_ended(t_data *data)
{
	int	end;

	pthread_mutex_lock(&data->data_mutex);
	end = data->is_simulation_ended;
	pthread_mutex_unlock(&data->data_mutex);
	return (end);
}
