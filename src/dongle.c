
#include "codexion.h"

int		first_deque_coder(t_dongle *dongle, t_coder *coder);
void	take_dongle(t_dongle *dongle, t_coder *coder)
{
	long long	cool_time;
	long long	time;

	cool_time = 0;
	pthread_mutex_lock(&(dongle->mutex));
	while (dongle->take_in_use != 0 || !(first_deque_coder(dongle, coder)))
	{
		pthread_cond_wait(&(dongle->cond), &(dongle->mutex));
	}
	time = get_time_ms();
	if ((dongle->last_compile + coder->data->dongle_cooldown) > time)
	{
		cool_time = dongle->last_compile + coder->data->dongle_cooldown - time;
		action_usleep(cool_time, coder);
	}
	dongle->take_in_use = 1;
	pthread_mutex_unlock($(dongle->mutex));
}

void	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 1)
	{
		take_dongle(&(coder->left_dongle), coder);
		take_dongle(&(coder->right_dongle), coder);
	}
	else
	{
		take_dongle(&(coder->right_dongle), coder);
		take_dongle(&(coder->left_dongle), coder);
	}
}
void	fifo(t_dongle *dongle, t_coder *coder)
{
	if (!ft_find_coder(dongle->head, coder))
		ft_lstadd_back(dongle->head, ft_lstnew(coder));
	return ;
}
void	edf(t_dongle *dongle, t_coder *coder)
{
	long long	time;
	long long	dead_time;
	deque		*current;
	deque		*prev;

	time = get_time_ms();
	if (!ft_find_coder(dongle->head, coder))
	{
		if (!dongle->head)
		{
			ft_lstadd_back(dongle->head, coder);
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
					ft_lstadd_front(dongle->head, ft_lstnew(coder));
					return ;
				}
				prev->next = ft_lstnew(coder);
				prev->next->next = current;
				return ;
			}
			prev = current;
			current = current->next;
		}
	}
	return ;
}

int	compare_last_compile(t_coder *coder1, t_coder *coder2, long long time)
{
	long long	coder1_time;
	long long	coder2_time;

	coder1_time = coder1->data->time_to_burnout - (time - coder1->last_compile);
	coder2_time = coder2->data->time_to_burnout - (time - coder2->last_compile);
	return (coder1_time >= coder2_time);
}

int	add_dongle_que(t_dongle *dongle, t_coder *coder)
{
	if (coder->data->scheduler == "fifo")
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
	if (*dongle->head->coder->id == coder->id)
	{
		ft_delete_list(dongle->head);
		return (1);
	}
	else
	{
		add_dongle_que(dongle, coder);
		return (0);
	}
}
