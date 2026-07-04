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
#include <string.h>
#include <unistd.h>

typedef struct s_data t_data;
typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;
typedef struct s_deque deque;
typedef struct s_surveillance t_surveillance;

/* 2. 依存関係の順序に従って構造体を定義 */
struct s_data
{
	long long number_of_coders;
	long long time_to_burnout;
	long long time_to_compile;
	long long time_to_debug;
	long long time_to_refactor;
	long long number_of_compiles_required;
	long long dongle_cooldown;
	char *scheduler;
	int is_simulation_ended;
	long long start_time;
	pthread_mutex_t log_mutex;
	pthread_mutex_t data_mutex;
	t_coder *coder;
	t_dongle *dongle;
};

struct s_condtion
{
	int think;
	int complie;
	int debuging;
	int recatoring;
};

struct s_dongle
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int last_compile;
	int take_in_use;
	deque **head;
};

struct s_coder
{
	pthread_t thread;
	int id;
	int last_compile;
	pthread_mutex_t coder_mutex;
	t_data *data;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
};

struct s_deque
{
	t_coder *coder;
	struct s_deque *next;
};

int ft_parse(int, char *[], t_data *);
int ft_is_num(char[]);
int ft_check_format(char *[]);
int init_coder(t_data *data);
int init_dongle(t_data *data);
int clean_up(t_data *data);
void *execute_safely(pthread_mutex_t *mutex,
					 void *(*func)(t_coder *), t_coder *argv);
void *take(t_coder *argv);
int take_dongle(t_coder* coder,t_dongle* dongle);