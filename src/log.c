
#include "codexion.h"

void	print_log(t_data *data, t_coder *coder, char *action)
{
	long long	time;

	pthread_mutex_lock(&data->log_mutex);
	time = get_time_ms() - data->start_time;
	if (strcmp(action, "take") == 0)
		printf("%lld %d has taken a dongle", time, coder->id);
	else if (strcmp(action, "compile") == 0)
		printf("%lld %d is compiling", time, coder->id);
	else if (strcmp(action, "debug") == 0)
		printf("%lld %d is debugging", time, coder->id);
	else if (strcmp(action, "refactor") == 0)
		printf("%lld %d is refactoring", time, coder->id);
	else if (strcmp(action, "burn") == 0)
		printf("%lld %d burned out", time, coder->id);
	pthread_mutex_unlock(&data->log_mutex);
}
