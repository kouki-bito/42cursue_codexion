/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-08 04:34:16 by kbito             #+#    #+#             */
/*   Updated: 2026-05-08 04:34:16 by kbito            ###   ########.jp       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_data
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	char				*scheduler;
}						t_data;

typedef struct codexion
{
	pthread_t			thread;
	int					last_compaile;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
}						t_coder;

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int					last_compaile;
	int					take_in_use;
	t_wait_list			**head;
}						t_dongle;

typedef struct s_wait_list
{
	t_coder				coder;
	struct s_wait_list	*next;
}						t_wait_list;