
#include "codexion.h"
#include <pthread.h>

void	print_log(t_data *data, t_coder *coder, char *action, long long time)
{
	time -= data->start_time;
	pthread_mutex_lock(&data->log_mutex);
	if (strcmp(action, "burn") == 0)
		printf("%lld %d burned out\n", time, coder->id);
	if (is_simulation_ended(coder->data))
	{
		pthread_mutex_unlock(&data->log_mutex);
		return ;
	}
	if (strcmp(action, "take") == 0)
		printf("%lld %d has taken a dongle\n", time, coder->id);
	if (strcmp(action, "compile") == 0)
		printf("%lld %d is compiling\n", time, coder->id);
	else if (strcmp(action, "debug") == 0)
		printf("%lld %d is debugging\n", time, coder->id);
	else if (strcmp(action, "refactor") == 0)
		printf("%lld %d is refactoring\n", time, coder->id);
	pthread_mutex_unlock(&data->log_mutex);
}
