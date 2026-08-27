/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:43:07 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 19:47:57 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_for_start(coder);
	while (!check_simulation_status(coder))
	{
		if (check_count_compile(coder))
			execute_coder_cycle(coder);
		else
		{
			has_finished(coder);
			break ;
		}
	}
	return (NULL);
}
