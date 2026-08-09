
#include "codexion.h"

int		first_deque_coder(t_dongle *dongle, t_coder *coder);

void	take_dongle(t_dongle *dongle, t_coder *coder)
{
	long long	cool_time;
	long long	time;

	cool_time = 0;
	pthread_mutex_lock(&(dongle->mutex));
	add_dongle_que(dongle, coder);
	while (dongle->take_in_use != 0 || !(first_deque_coder(dongle, coder)))
	{
		pthread_cond_wait(&(dongle->cond), &(dongle->mutex));
	}
	time = get_time_ms();
	if ((dongle->last_compile + coder->data->dongle_cooldown) >= time)
	{
		cool_time = dongle->last_compile + coder->data->dongle_cooldown - time;
		action_usleep(cool_time, coder);
	}
	ft_delete_list(&dongle->head);
	dongle->take_in_use = 1;
	pthread_mutex_unlock(&(dongle->mutex));
}
int		try_take_dongle(t_dongle* dongle)
{
	int n;
	long long time;
	long long cool_time;
	pthread_mutex_lock(&dongle->mutex);
	n = dongle->take_in_use;
	cool_time = dongle->cool_time;
	pthread_mutex_unlock(&dongle->mutex);
	time = get_time_ms();
	return !n && time>= cool_time;
}
void	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 1)
	{
		take_dongle(coder->left_dongle, coder);
		take_dongle(coder->right_dongle, coder);
	}
	else
	{
		take_dongle(coder->right_dongle, coder);
		take_dongle(coder->left_dongle, coder);
	}
}

void	fifo(t_dongle *dongle, t_coder *coder)
{
	if (!ft_find_coder(dongle->head, coder))
		ft_lstadd_back(&dongle->head, ft_lstnew(coder));
	return ;
}
void	edf(t_dongle *dongle, t_coder *coder)
{
	long long	time;
	deque		*current;
	deque		*prev;

	time = get_time_ms();
	if (!ft_find_coder(dongle->head, coder))
	{
		if (!dongle->head)
		{
			ft_lstadd_back(&(dongle->head), ft_lstnew(coder));
			return ;
		}
		current = dongle->head;
		prev = NULL;
		while (current)
		{
			if (!compare_last_compile(coder, current->coder, time))
			{
				if (prev == NULL)
				{
					ft_lstadd_front(&(dongle->head), ft_lstnew(coder));
					return ;
				}
				prev->next = ft_lstnew(coder);
				if (!prev->next)
					prev->next->next = current;
				return ;
			}
			prev = current;
			current = current->next;
		}
		ft_lstadd_back(&dongle->head, ft_lstnew(coder));
	}
	return ;
}

int	compare_last_compile(t_coder *coder1, t_coder *coder2, long long time)
{
	long long	coder1_time;
	long long	coder2_time;

	pthread_mutex_lock(&(coder1->coder_mutex));
	coder1_time = coder1->data->time_to_burnout - (time - coder1->last_compile);
	pthread_mutex_unlock(&(coder1->coder_mutex));
	pthread_mutex_lock(&(coder2->coder_mutex));
	coder2_time = coder2->data->time_to_burnout - (time - coder2->last_compile);
	pthread_mutex_unlock(&(coder2->coder_mutex));
	return (coder1_time >= coder2_time);
}

int	add_dongle_que(t_dongle *dongle, t_coder *coder)
{
	if (strcmp(coder->data->scheduler, "fifo") == 0)
	{
		fifo(dongle, coder);
	}
	else
	{
		edf(dongle, coder);
	}
	return (1);
}

int	first_deque_coder(t_dongle *dongle, t_coder *coder)
{
	if (dongle->head && (dongle->head)->coder->id == coder->id)
	{
		return (1);
	}
	return (0);
}
