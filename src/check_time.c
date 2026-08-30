/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:48:25 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:00:06 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long	get_real_time(void);

long long	get_time_ms(void)
{
	struct timespec	ts;
	long long		milliseconds;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	milliseconds = (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
	return (milliseconds);
}

struct timespec	get_interval_time(long long time)
{
	struct timespec	ts;

	ts.tv_nsec = 0;
	ts.tv_sec = 0;
	ts.tv_sec += time / 1000;
	ts.tv_nsec += (time % 1000) * 1000000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec += ts.tv_nsec / 1000000000;
		ts.tv_nsec = ts.tv_nsec % 1000000000;
	}
	return (ts);
}

static long long	get_real_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

struct timespec	mono_deadline_to_ts(long long mono_deadline)
{
	long long	remaining;
	long long	realtime_deadline;

	remaining = mono_deadline - get_time_ms();
	if (remaining < 0)
		remaining = 0;
	realtime_deadline = get_real_time() + remaining;
	return (get_interval_time(realtime_deadline));
}
