/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:00:21 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:01:59 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cool_time_sleep(t_dongle *first, t_dongle *second, t_coder *coder)
{
	long long		max_cooldown;
	struct timespec	ts;

	max_cooldown = max_cool_time(first, second);
	if (max_cooldown > get_time_ms())
	{
		ts = mono_deadline_to_ts(max_cooldown);
		pthread_cond_timedwait(&(coder->data->scheduler_cond),
			&(coder->data->scheduler_mutex), &ts);
	}
	else
	{
		pthread_cond_wait(&(coder->data->scheduler_cond),
			&(coder->data->scheduler_mutex));
	}
}

void	action_usleep(long long time, t_coder *coder)
{
	struct timespec	ts;
	long long		mono_deadline;

	if (time != 0)
	{
		mono_deadline = get_time_ms() + time;
		pthread_mutex_lock(&coder->action_sleep_mutex);
		while (!check_simulation_status(coder) && get_time_ms() < mono_deadline)
		{
			ts = mono_deadline_to_ts(mono_deadline);
			pthread_cond_timedwait(&(coder->action_sleep_cond),
				&coder->action_sleep_mutex, &ts);
		}
		pthread_mutex_unlock(&coder->action_sleep_mutex);
	}
}
