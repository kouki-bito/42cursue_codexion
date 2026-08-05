#include "codexion.h"

void	destroy_all(t_data *data)
{
	int	i;

	i = 0;
	if(!data)
		return ;
	pthread_cond_destroy(&(data->usleep_cond));
	pthread_mutex_destroy(&(data->log_mutex));
	pthread_mutex_destroy(&(data->data_mutex));
	while (data->number_of_coders > i)
	{
		destroy_coder(&(data->coder[i]));
		destroy_dongle(&(data->dongle[i]));
		i++;
	}
	
	free(data->dongle);
	free(data->coder);
	free(data);


}

void	destroy_coder(t_coder *coder)
{
	if(!coder)
		pthread_mutex_destroy(&(coder->coder_mutex));
	if(!coder)
	pthread_mutex_destroy(&(coder->coder_task_mutex));
}

void	destroy_deque(deque **head)
{
	while (*head != NULL)
	{
		ft_delete_list(head);
	}
	return ;
}

void	destroy_dongle(t_dongle *dongle)
{
	pthread_cond_destroy(&(dongle->cond));
	pthread_mutex_destroy(&(dongle->mutex));
	if (ft_lstsize(dongle->head) > 0)
		destroy_deque(&dongle->head);
}
