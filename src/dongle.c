
#include "codexion.h"
#define ACT_COMPILE "%d"

void	scheduler_fifo(t_dongle *first, t_dongle *second, t_coder *coder);
void	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id % 2 == 1)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (strcmp(coder->data->scheduler, "fifo") == 0)
		scheduler_fifo(first, second, coder);
	else
		scheduler_edf(first,second,coder);
}

void	scheduler_fifo(t_dongle *first, t_dongle *second, t_coder *coder)
{
	add_deque_coder(first, coder);
	add_deque_coder(second, coder);
	while (!is_simulation_ended(coder->data))
	{
		if (fisrt_deque_coder(coder, first) && fisrt_deque_coder(coder, second))
		{
			if (take_dongle(first, second, coder))
			{
				print_log(coder->data,coder,"take",first);
				print_log(coder->data,coder,"take",second);
				break ;
			}
			else
				usleep(1000);
		}
		else
		{
			pthread_mutex_lock(&coder->data->usleep_mutex);
			pthread_cond_wait(&coder->data->usleep_cond,
				&coder->data->usleep_mutex);
			pthread_mutex_unlock(&coder->data->usleep_mutex);
		}
	}
	return ;
}
void	print_log(t_data *data, t_coder *coder, char *action, t_dongle *dongle)
{
	long long	time;

	pthread_mutex_lock(&data->log_mutex);
	time = get_time_ms() - data->start_time;
	if (strcmp(action, "take")==0)
		printf("%lld %d has taken a dongle", time, coder->id);
	else if (strcmp(action, "compile")==)
		printf("%lld %d is compiling", time, coder->id);
	else if (strcmp(action, "debug")==0)
		printf("%lld %d is debugging", time, coder->id);
	else if (strcmp(action, "refactor")==0)
		printf("%lld %d is refactoring", time, coder->id);
	else if (strcmp(action, "burn")==0)
		printf("&lld %d burned out", time, coder->id);
	pthread_mutex_unlock(&data->log_mutex);
}
int	take_dongle(t_dongle *first, t_dongle *second, t_coder *coder)
{
	if (try_take_dongle(first))
	{
		if (try_take_dongle(second))
		{
			print_log();
			return (1);
		}
		else
		{
			pthread_mutex_unlock(&first->mutex);
			return (0);
		}
	}
}
int	try_take_dongle(t_dongle *dongle)
{
	long long int	time;

	pthread_mutex_lock(&dongle->mutex);
	if (dongle->cool_time <= time)
		return (1);
	pthread_mutex_unlock(&dongle->mutex);
}

int	fisrt_deque_coder(t_coder *coder, t_dongle *dongle)
{
	int	flag;

	pthread_mutex_lock(&dongle->scheduler_mutex);
	flag = dongle->head->coder == coder;
	pthread_mutex_unlock(&dongle->scheduler_mutex);
	return (flag);
}
void	add_deque_coder(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->scheduler_mutex);
	if (!ft_find_coder(dongle, coder))
		ft_lstadd_back(&dongle->head, ft_lstnew(coder));
	pthread_mutex_unlock(&dongle->scheduler_mutex);
}