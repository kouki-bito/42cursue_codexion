
#include "codexion.h"

long long	get_burn_out(t_coder *coder)
{
	long long	bourn_out;

	pthread_mutex_lock(&coder->coder_mutex);
	bourn_out = coder->last_compile + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (bourn_out);
}

t_coder	*get_heap_first(t_heap *heap)
{
	t_coder	*coder;

	pthread_mutex_lock(&heap->lock);
	coder = heap->coders[0];
	pthread_mutex_unlock(&heap->lock);
	return (coder);
}
int	is_simulation_ended(t_data *data)
{
	int end;

	pthread_mutex_lock(&data->data_mutex);
	end = data->is_simulation_ended;
	pthread_mutex_unlock(&data->data_mutex);
	return (end);
}