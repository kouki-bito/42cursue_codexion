/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:29:13 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:12:23 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_simulation_status(t_coder *coder)
{
	int	ended;

	ended = is_simulation_ended(coder->data);
	return (ended);
}

void	has_finished(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->data_mutex);
	coder->data->is_finished++;
	pthread_cond_broadcast(&coder->data->state_cond);
	pthread_mutex_unlock(&coder->data->data_mutex);
	return ;
}

int	check_count_compile(t_coder *coder)
{
	int	count;

	pthread_mutex_lock(&coder->coder_mutex);
	count = coder->count_compile;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (count < coder->data->number_of_compiles_required);
}

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->data_mutex);
	coder->data->read_count++;
	pthread_cond_broadcast(&coder->data->state_cond);
	while (!coder->data->start_flag && !coder->data->is_simulation_ended)
	{
		pthread_cond_wait(&coder->data->state_cond, &coder->data->data_mutex);
	}
	pthread_mutex_unlock(&coder->data->data_mutex);
}

int	check_coder_compile_count(t_coder *coders)
{
	int	i;

	i = 0;
	while (i < coders[0].data->number_of_coders)
	{
		if (check_count_compile(&coders[i]))
			return (0);
		i++;
	}
	return (1);
}
