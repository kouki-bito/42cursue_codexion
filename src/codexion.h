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
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

typedef struct s_data			t_data;
typedef struct s_dongle			t_dongle;
typedef struct s_coder			t_coder;
typedef struct s_deque			deque;
typedef struct s_surveillance	t_surveillance;
typedef struct s_heap			t_heap;

/* 2. 依存関係の順序に従って構造体を定義 */
struct							s_data
{
	long long					number_of_coders;
	long long					time_to_burnout;
	long long					time_to_compile;
	long long					time_to_debug;
	long long					time_to_refactor;
	long long					number_of_compiles_required;
	long long					dongle_cooldown;
	char						*scheduler;
	int							is_simulation_ended;
	long long					start_time;
	pthread_mutex_t				log_mutex;
	pthread_mutex_t				data_mutex;
	pthread_mutex_t				usleep_mutex;
	pthread_cond_t				usleep_cond;
	pthread_t					monitor;
	t_coder						*coder;
	t_dongle					*dongle;
};

struct							s_condtion
{
	int							think;
	int							complie;
	int							debuging;
	int							recatoring;
};
struct							s_heap
{
	t_coder						*coders[300];
	int							size;
	pthread_mutex_t				lock;
	pthread_cond_t				cond;
};
struct							s_dongle
{
	pthread_mutex_t				mutex;
	pthread_mutex_t				scheduler_mutex;
	pthread_mutex_t				cool_down_mutex;
	pthread_cond_t				cond;
	long long					last_compile;
	long long					cool_time;
	int							take_in_use;
	int							id;
	t_heap						heap;
	deque						*head;
};

struct							s_coder
{
	pthread_t					thread;
	int							id;
	long int					last_compile;
	int							count_compile;
	pthread_mutex_t				coder_mutex;

	pthread_mutex_t				coder_task_mutex;

	t_data						*data;
	t_dongle					*left_dongle;
	t_dongle					*right_dongle;
};

struct							s_deque
{
	t_coder						*coder;
	struct s_deque				*next;
};


int								ft_parse(int, char *[], t_data *);
int								ft_is_num(char[]);
int								ft_check_format(char *[]);
int								init_coder(t_data *data);
int								init_dongle(t_data *data);
// int								clean_up(t_data *data);
void							*execute_safely(pthread_mutex_t *mutex,
									void *(*func)(t_coder *), t_coder *argv);
// void							*take(t_coder *argv);
void							take_dongles(t_coder *coder);
int								take_dongle(t_dongle *first, t_dongle *second);
long long						get_time_ms(void);
void							action_usleep(long long time, t_coder *coder);

void							ft_delete_list(deque **lst);
void							ft_lstadd_back(deque **lst, deque *new);
int								ft_lstsize(deque *lst);
void							destroy_all(t_data *data);
deque							*ft_lstnew(t_coder *coder);
int								ft_find_coder(deque *head, t_coder *coder);
void							ft_lstadd_front(deque **lst, deque *new);
void							*coder_routine(void *arg);
int								ft_atoi_safe(char *str);
int								init_mutex_and_cond(t_data *data);
int								compare_last_compile(t_coder *coder1,
									t_coder *coder2, long long time);
int								check_dead(t_coder *coders);
int								is_simulation_ended(t_data *data);
struct timespec					get_time(void);
long long						get_time_ms(void);
struct timespec					get_interval_time(long int time);
void							leave_dongle(t_coder *coder);
void							start_compile(t_coder *coder);
void							start_debuging(t_coder *coder);
void							start_refactor(t_coder *coder);
void							destroy_all(t_data *data);
void							destroy_coder(t_coder *coder);
void							destroy_dongle(t_dongle *dongle);
void							destroy_deque(deque **head);
long long						get_burn_out(t_coder *coder);
void							heap_push(t_heap *managment, t_coder *coder);
void							test_heap(t_data *data);
int								add_dongle_que(t_dongle *dongle,
									t_coder *coder);
void							heap_init(t_heap *manegment);
void							heap_pop(t_heap *manegment, t_coder *coder);
int								heap_compare(t_coder *curr, t_coder *coder);
int								check_simulation_status(t_coder *coder);
long long						get_dongle_cool_time(t_dongle *dongle);
void							set_dongle_use(t_dongle *first,
									t_dongle *second, int use);
void							set_dongle_cool_time(t_dongle *first,
									t_dongle *second, long long cool_time);
int								heap_first(t_dongle* dongle, t_coder *coder);
void*								monitor(void *pointer);
void	print_log(t_data *data, t_coder *coder, char *action);
void	init_thread(t_data *data);
void	join_thread(t_data *data);