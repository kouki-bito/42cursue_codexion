/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:26:24 by kbito             #+#    #+#             */
/*   Updated: 2026/08/28 00:42:12 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

static void	take_single_dongle(t_coder *coder);
static int	try_take_dongle(t_dongle *first, t_dongle *second);
static int	take_dongle(t_coder *coder, t_request *request);

static void	complete_dongle_acquisition(t_coder *coder);

void	take_dongles(t_coder *coder)
{
	t_request	request;

	pthread_mutex_lock(&coder->data->scheduler_mutex);
	if (coder->data->number_of_coders == 1)
	{
		take_single_dongle(coder);
		pthread_mutex_unlock((&coder->data->scheduler_mutex));
		return ;
	}
	request = make_request(coder);
	heap_push(&coder->left_dongle->heap, &request);
	heap_push(&coder->right_dongle->heap, &request);
	while (!is_simulation_ended(coder->data))
	{
		if (try_take_dongle(coder->right_dongle, coder->left_dongle)
			&& take_dongle(coder, &request))
		{
			complete_dongle_acquisition(coder);
			break ;
		}
		else
			cool_time_sleep(coder->right_dongle, coder->left_dongle, coder);
	}
	pthread_mutex_unlock(&coder->data->scheduler_mutex);
}

static void	take_single_dongle(t_coder *coder)
{
	long long	time;

	time = get_time_ms();
	pthread_mutex_lock(&coder->right_dongle->mutex);
	coder->right_dongle->take_in_use = 1;
	pthread_mutex_unlock(&coder->right_dongle->mutex);
	print_log(coder->data, coder, "take", time);
}

static void	complete_dongle_acquisition(t_coder *coder)
{
	long long	time;

	pthread_cond_broadcast(&(coder->data->scheduler_cond));
	time = get_time_ms();
	print_log(coder->data, coder, "take", time);
	print_log(coder->data, coder, "take", time);
	heap_pop(&coder->left_dongle->heap, coder);
	heap_pop(&coder->right_dongle->heap, coder);
}

static int	try_take_dongle(t_dongle *first, t_dongle *second)
{
	long long int	time;
	int				flag1;
	int				flag2;

	pthread_mutex_lock(&(first->mutex));
	pthread_mutex_lock(&(second->mutex));
	time = get_time_ms();
	flag1 = first->cool_time <= time && first->take_in_use == 0;
	flag2 = second->cool_time <= time && second->take_in_use == 0;
	if (flag1 && flag2)
	{
		pthread_mutex_unlock(&(first->mutex));
		pthread_mutex_unlock(&(second->mutex));
		return (1);
	}
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
	return (0);
}

static int	take_dongle(t_coder *coder, t_request *request)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id % 2 == 1)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	if (heap_first(first, request) && heap_first(second, request))
	{
		set_dongle_use(first, second, 1);
		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);
		return (1);
	}
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
	return (0);
}
