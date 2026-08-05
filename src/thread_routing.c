#include "codexion.h"

long long		get_time_ms(void);
static int	check_simulation_status(t_coder *coder)
{
	int	ended;

	ended = is_simulation_ended(coder->data);
	return (ended);
}

static void	execute_coder_cycle(t_coder *coder)
{
	take_dongles(coder);
	start_compile(coder);
	leave_dongle(coder);
	start_debuging(coder);
	start_refactor(coder);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!check_simulation_status(coder))
	{
		execute_coder_cycle(coder);
	}
	return (NULL);
}

int	is_simulation_ended(t_data *data)
{
	int	end;

	pthread_mutex_lock(&data->data_mutex);
	end = data->is_simulation_ended;
	pthread_mutex_unlock(&data->data_mutex);
	return (end);
}

struct timespec	get_time(void)
{
	struct timespec	ts;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = tv.tv_usec * 1000;
	return (ts);
}

long long	get_time_ms(void)
{
	struct timeval	tv;
	long long		milliseconds;

	gettimeofday(&tv, NULL);
	milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
	return (milliseconds);
}
struct timespec	get_interval_time(long int time)
{
	struct timespec	ts;

	ts = get_time();
	ts.tv_sec += time / 1000;
	ts.tv_nsec += (time % 1000) * 1000000;
	if (ts.tv_nsec > 1000000000)
	{
		ts.tv_sec += ts.tv_nsec / 1000000000;
		ts.tv_nsec = ts.tv_nsec % 1000000000;
	}
	return (ts);
}
void	leave_dongle(t_coder *coder)
{
	pthread_mutex_lock(&(coder->left_dongle->mutex));
	coder->left_dongle->take_in_use = 0;
	pthread_mutex_unlock(&(coder->left_dongle->mutex));
	pthread_mutex_lock(&(coder->right_dongle->mutex));
	coder->right_dongle->take_in_use = 0;
	pthread_mutex_unlock(&(coder->right_dongle->mutex));
	pthread_cond_broadcast(&coder->right_dongle->cond);
	pthread_cond_broadcast(&coder->left_dongle->cond);
}
void	start_compile(t_coder *coder)
{
	if (!check_simulation_status(coder))
	{
		printf("%lld %d is compiling", get_time_ms(), coder->id);
		action_usleep(coder->data->time_to_compile, coder);
		pthread_mutex_lock(&coder->coder_mutex);
		coder->last_compile = get_time_ms();
		coder->count_compile += 1;
		pthread_mutex_unlock(&coder->coder_mutex);
	}
}
void	start_debuging(t_coder *coder)
{
	if (!check_simulation_status(coder))
	{
		printf("%lld %d is debuging", get_time_ms(), coder->id);
		action_usleep(coder->data->time_to_debug, coder);
	}
}
void	start_refactor(t_coder *coder)
{
	if (!check_simulation_status(coder))
	{
		printf("%lld %d is refactoring", get_time_ms(), coder->id);
		action_usleep(coder->data->time_to_refactor, coder);
	}
}

void	action_usleep(long long time, t_coder *coder)
{
	struct timespec ts;
	ts = get_interval_time(time);
	if (!check_simulation_status(coder))
	{
		pthread_mutex_lock(&coder->coder_task_mutex);
		pthread_cond_timedwait(&(coder->data->usleep_cond),
			&(coder->coder_task_mutex), &ts);
		pthread_mutex_unlock(&coder->coder_task_mutex);
	}
}