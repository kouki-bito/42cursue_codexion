
#include "codexion.h"
#define ACT_COMPILE "%d"

void	scheduler_fifo(t_dongle *first, t_dongle *second, t_coder *coder);
void	scheduler_edf(t_dongle *first, t_dongle *second, t_coder *coder);
void	add_both_coder_heap(t_dongle *first, t_dongle *second, t_coder *coder);
void	delete_both_coder_heap(t_dongle *first, t_dongle *second,
			t_coder *coder);
void	cool_time_sleep(t_dongle *first, t_dongle *second, t_coder *coder);
void	add_deque_coder(t_dongle *dongle, t_coder *coder);
int		try_take_dongle(t_dongle *dongle);

void	add_both_deque_coder(t_dongle *first, t_dongle *second, t_coder *coder);
int		fisrt_deque_coder(t_coder *coder, t_dongle *dongle);
void	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id%2==1)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	else
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	if (strcmp(coder->data->scheduler, "fifo") == 0)
		scheduler_fifo(first, second, coder);
	else
		scheduler_edf(first, second, coder);
}
void	delete_stack(t_dongle *first, t_dongle *second, char *scheduler,
		t_coder *coder)
{
	pthread_mutex_lock(&first->scheduler_mutex);
	pthread_mutex_lock(&second->scheduler_mutex);
	if (strcmp(scheduler, "fifo") == 0)
	{
		ft_delete_list(&first->head);
		ft_delete_list(&second->head);
	}
	else
	{
		heap_pop(&first->heap, coder);
		heap_pop(&second->heap, coder);
	}
	pthread_mutex_unlock(&first->scheduler_mutex);
	pthread_mutex_unlock(&second->scheduler_mutex);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
}
void	scheduler_fifo(t_dongle *first, t_dongle *second, t_coder *coder)
{
	add_both_deque_coder(first, second, coder);
	while (!is_simulation_ended(coder->data))
	{
		if (fisrt_deque_coder(coder, first) && fisrt_deque_coder(coder, second))
		{
			if (take_dongle(first, second))
			{
				print_log(coder->data, coder, "take");
				print_log(coder->data, coder, "take");
				set_dongle_use(first, second, 1);
				delete_stack(first, second, coder->data->scheduler, coder);
				break ;
			}
			else
				cool_time_sleep(first, second, coder);
		}
		else
			action_usleep(0, coder);
	}
	return ;
}
void	scheduler_edf(t_dongle *first, t_dongle *second, t_coder *coder)
{
	add_both_coder_heap(first, second, coder);
	while (!is_simulation_ended(coder->data))
	{
		if (heap_first(first, coder) && heap_first(second, coder))
		{
			if (take_dongle(first, second))
			{
				print_log(coder->data, coder, "take");
				print_log(coder->data, coder, "take");
				set_dongle_use(first, second, 1);
				delete_stack(first, second, coder->data->scheduler, coder);
				break ;
			}
			else
				cool_time_sleep(first, second, coder);
		}
		else
			action_usleep(0, coder);
	}
	return ;
}
void	add_both_coder_heap(t_dongle *first, t_dongle *second, t_coder *coder)
{
	pthread_mutex_lock(&first->scheduler_mutex);
	pthread_mutex_lock(&second->scheduler_mutex);
	heap_push(&first->heap, coder);
	heap_push(&second->heap, coder);
	pthread_mutex_unlock(&first->scheduler_mutex);
	pthread_mutex_unlock(&second->scheduler_mutex);
}
void	delete_both_coder_heap(t_dongle *first, t_dongle *second,
		t_coder *coder)
{
	pthread_mutex_lock(&first->scheduler_mutex);
	pthread_mutex_lock(&second->scheduler_mutex);
	heap_pop(&first->heap, coder);
	heap_pop(&second->heap, coder);
	pthread_mutex_unlock(&first->scheduler_mutex);
	pthread_mutex_unlock(&second->scheduler_mutex);
}
void	cool_time_sleep(t_dongle *first, t_dongle *second, t_coder *coder)
{
	long long	first_cool_time;
	long long	second_cool_time;
	long long	set_time;

	first_cool_time = get_dongle_cool_time(first);
	second_cool_time = get_dongle_cool_time(second);
	set_time = first_cool_time;
	if (first_cool_time < second_cool_time)
		set_time = second_cool_time;
	set_time -= get_time_ms();
	if (set_time <= 0)
		set_time = 0;
	action_usleep(set_time, coder);
}

int	take_dongle(t_dongle *first, t_dongle *second)
{
	if (try_take_dongle(first))
	{
		if (try_take_dongle(second))
		{
			return (1);
		}
		else
		{
			pthread_mutex_unlock(&first->mutex);
			return (0);
		}
	}
	return (0);
}
int	try_take_dongle(t_dongle *dongle)
{
	long long int	time;

	time = get_time_ms();
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->cool_time <= time && dongle->take_in_use == 0)
		return (1);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

int	fisrt_deque_coder(t_coder *coder, t_dongle *dongle)
{
	int	flag;

	pthread_mutex_lock(&dongle->scheduler_mutex);
	flag = dongle->head != NULL && dongle->head->coder == coder;
	pthread_mutex_unlock(&dongle->scheduler_mutex);
	return (flag);
}
void	add_deque_coder(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->scheduler_mutex);
	if (!ft_find_coder(dongle->head, coder))
		ft_lstadd_back(&dongle->head, ft_lstnew(coder));
	pthread_mutex_unlock(&dongle->scheduler_mutex);
}

void	add_both_deque_coder(t_dongle *first, t_dongle *second, t_coder *coder)
{
	pthread_mutex_lock(&first->scheduler_mutex);
	pthread_mutex_lock(&second->scheduler_mutex);
	if (!ft_find_coder(first->head, coder))
		ft_lstadd_back(&first->head, ft_lstnew(coder));
	if (!ft_find_coder(second->head, coder))
		ft_lstadd_back(&second->head, ft_lstnew(coder));
	pthread_mutex_unlock(&first->scheduler_mutex);
	pthread_mutex_unlock(&second->scheduler_mutex);
}