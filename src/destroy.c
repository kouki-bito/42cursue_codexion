/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:29:36 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 19:29:37 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

void	destroy_all(t_data *data)
{
	int	i;

	i = 0;
	if (!data)
		return ;
	pthread_mutex_destroy(&(data->log_mutex));
	pthread_mutex_destroy(&(data->data_mutex));
	pthread_mutex_destroy(&(data->scheduler_mutex));
	pthread_cond_destroy(&(data->scheduler_cond));
	pthread_cond_destroy(&(data->state_cond));
	while (data->number_of_coders > i)
	{
		destroy_coder(&(data->coder[i]));
		destroy_dongle(&(data->dongle[i]));
		i++;
	}
	free(data->dongle);
	free(data->coder);
}

void	destroy_coder(t_coder *coder)
{
	if (!coder)
		return ;
	pthread_mutex_destroy(&(coder->coder_mutex));
	pthread_mutex_destroy(&(coder->action_sleep_mutex));
	pthread_cond_destroy(&(coder->action_sleep_cond));
}

void	destroy_dongle(t_dongle *dongle)
{
	pthread_cond_destroy(&(dongle->cond));
	pthread_mutex_destroy(&(dongle->mutex));
	pthread_cond_destroy(&(dongle->heap.cond));
	pthread_mutex_destroy(&(dongle->heap.lock));
	pthread_mutex_destroy(&dongle->scheduler_mutex);
	pthread_mutex_destroy(&dongle->cool_down_mutex);
}
