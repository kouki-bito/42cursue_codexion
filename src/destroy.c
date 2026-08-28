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
	if (!data)
		return ;
	pthread_mutex_destroy(&(data->log_mutex));
	pthread_mutex_destroy(&(data->data_mutex));
	pthread_mutex_destroy(&(data->scheduler_mutex));
	pthread_cond_destroy(&(data->scheduler_cond));
	pthread_cond_destroy(&(data->state_cond));
	destroy_coder((data->coder), data->number_of_coders);
	destroy_dongle(data->dongle, data->number_of_coders);
	free(data->dongle);
	free(data->coder);
}

void	destroy_coder(t_coder *coder, int num)
{
	int	i;

	if (!coder)
		return ;
	i = 0;
	while (i < num)
	{
		if (!&coder[i])
			return ;
		pthread_mutex_destroy(&(coder[i].coder_mutex));
		pthread_mutex_destroy(&(coder[i].action_sleep_mutex));
		pthread_cond_destroy(&(coder[i].action_sleep_cond));
		i++;
	}
}

void	destroy_dongle(t_dongle *dongle, int num)
{
	int	i;

	i = 0;
	if (!dongle)
		return ;
	while (i < num)
	{
		pthread_mutex_destroy(&(dongle[i].mutex));
		pthread_mutex_destroy(&dongle[i].cool_down_mutex);
		i++;
	}
}
