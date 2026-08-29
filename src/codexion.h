/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 04:34:16 by kbito             #+#    #+#             */
/*   Updated: 2026/08/28 18:20:44 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

typedef struct s_data		t_data;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_heap		t_heap;
typedef struct s_request	t_request;

/* 2. 依存関係の順序に従って構造体を定義 */
struct						s_data
{
	long long				number_of_coders;
	long long				time_to_burnout;
	long long				time_to_compile;
	long long				time_to_debug;
	long long				time_to_refactor;
	long long				number_of_compiles_required;
	long long				dongle_cooldown;
	unsigned long			request_num;
	char					*scheduler;
	int						is_simulation_ended;
	int						is_finished;
	long long				start_time;
	int						start_flag;
	int						read_count;
	pthread_mutex_t			log_mutex;
	pthread_mutex_t			data_mutex;
	pthread_mutex_t			scheduler_mutex;
	pthread_cond_t			scheduler_cond;
	pthread_cond_t			state_cond;
	pthread_t				monitor;
	t_coder					*coder;
	t_coder					*burn_coder;
	t_dongle				*dongle;
};

struct						s_condtion
{
	int						think;
	int						complie;
	int						debuging;
	int						recatoring;
};
struct						s_request
{
	long long				deadline;
	t_coder					*coder;
	unsigned long			number;
};
struct						s_heap
{
	t_request				request[300];
	int						size;
};
struct						s_dongle
{
	pthread_mutex_t			mutex;
	pthread_mutex_t			cool_down_mutex;
	long long				cool_time;
	int						take_in_use;
	int						id;
	t_heap					heap;
};

struct						s_coder
{
	pthread_t				thread;
	int						id;
	long long				burn_out_time;
	int						count_compile;
	pthread_mutex_t			coder_mutex;
	pthread_mutex_t			action_sleep_mutex;
	pthread_cond_t			action_sleep_cond;
	t_data					*data;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
};

int							init_cond(t_data *data);
t_request					make_request(t_coder *coder);
void						wait_for_start(t_coder *coder);
void						has_finished(t_coder *coder);

void						cool_time_sleep(t_dongle *first, t_dongle *second,
								t_coder *coder);
void						take_dongles(t_coder *coder);
int							ft_parse(int argc, char *argv[], t_data *data);

int							is_simulation_ended(t_data *data);

int							check_simulation_status(t_coder *coder);
void						print_log(t_data *data, t_coder *coder,
								char *action, long long time);
// thread_routing.c
void						*coder_routine(void *arg);
int							check_count_compile(t_coder *coder);
void						execute_coder_cycle(t_coder *coder);

// monitor.c
int							check_dead(t_coder *coders);
void						*monitor(void *pointer);
void						broadcast_coders(t_coder *coders);

// init.c
int							init_coder(t_data *data);
int							init_dongle(t_data *data);
int							init_mutex_and_cond(t_data *data);
// getter.c
long long					get_burn_out(t_coder *coder);
long long					get_dongle_cool_time(t_dongle *dongle);

long long					max_cool_time(t_dongle *first, t_dongle *second);
// destroy.c
void						destroy_data(t_data *data);
void						destroy_all(t_data *data);
void						destroy_dongle(t_dongle *dongle, int num);
void						destroy_coder(t_coder *coder, int num);
// heap.c
void						heap_push(t_heap *managment, t_request *request);
void						heap_pop(t_heap *manegment, t_coder *coder);
int							heap_compare(t_request *curr, t_request *request);
int							heap_first(t_dongle *dongle, t_request *request);
// chreat_thread.c
int							init_thread(t_data *data);
int							join_thread(t_data *data);
// check_time.c
struct timespec				get_interval_time(long long time);
long long					get_time_ms(void);
struct timespec				mono_deadline_to_ts(long long mono_deadline);
void						action_usleep(long long time, t_coder *coder);

// setter.c
void						set_burn_out(t_coder *coder, long long time,
								long long now);
void						set_dongle_use(t_dongle *first, t_dongle *second,
								int use);
void						set_dongle_cool_time(t_dongle *first,
								t_dongle *second, long long cool_time,
								long long now);
