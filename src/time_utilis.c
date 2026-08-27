/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utilis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:01:20 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:01:21 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*execute_safely(pthread_mutex_t *mutex, void *(*func)(t_coder *),
		t_coder *arg)
{
	void	*res;

	pthread_mutex_lock(mutex);
	res = (void *)func(arg);
	pthread_mutex_unlock(mutex);
	return (res);
}
