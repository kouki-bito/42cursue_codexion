
#include "codexion.h"

void	init_thread(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_create(&data->monitor, NULL, &monitor, data->coder))
		destroy_all(data);
	while (i < data->number_of_coders)
	{
		if (pthread_create(&(data->coder[i].thread), NULL, &coder_routine,
				&(data->coder[i])))
		{
			destroy_all(data);
			return ;
		}
		i++;
	}
}
void	join_thread(t_data *data)
{
	int i;
	i = 0;
	if (pthread_join(data->monitor, NULL))
		destroy_all(data);
	while (i < data->number_of_coders)
	{
		if (pthread_join(data->coder[i].thread, NULL))
			destroy_all(data);
		i++;
	}
}
