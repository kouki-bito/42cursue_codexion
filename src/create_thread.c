/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:11:10 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:11:25 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_thread(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_create(&data->monitor, NULL, &monitor, data->coder))
	{
		destroy_all(data);
		return (0);
	}
	while (i < data->number_of_coders)
	{
		if (pthread_create(&(data->coder[i].thread), NULL, &coder_routine,
				&(data->coder[i])))
		{
			destroy_all(data);
			return (0);
		}
		i++;
	}
	return (1);
}

int	join_thread(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_join(data->monitor, NULL))
	{
		destroy_all(data);
		return (0);
	}
	while (i < data->number_of_coders)
	{
		if (pthread_join(data->coder[i].thread, NULL))
		{
			destroy_all(data);
			return (0);
		}
		i++;
	}
	return (1);
}
