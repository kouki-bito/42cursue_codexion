/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:01:20 by kbito             #+#    #+#             */
/*   Updated: 2026/08/28 18:20:29 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_cond(t_data *data)
{
	if (pthread_cond_init(&(data->scheduler_cond), NULL))
		return (0);
	if (pthread_cond_init(&(data->state_cond), NULL))
	{
		pthread_cond_destroy(&(data->scheduler_cond));
		return (0);
	}
	return (1);
}
