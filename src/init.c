#include "codexion.h"

// int		clean_dongle(t_data *data);
// void	free_que(deque **head);
int	init_coder(t_data *data)
{
	int	i;

	i = 0;
	if (data == NULL)
		return (0);
	data->coder = (t_coder *)malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coder)
		return (0);
	while (i < data->number_of_coders)
	{
		data->coder[i].last_compile = 0;

		data->coder[i].id = i + 1;
		data->coder[i].left_dongle = &data->dongle[i];
		data->coder[i].right_dongle = &data->dongle[(i + 1)
			% data->number_of_coders];
		if (pthread_mutex_init(&(data->coder[i].coder_mutex), NULL)
			|| pthread_mutex_init(&(data->coder[i].coder_task_mutex), NULL))
		{
			return (0);
		}
		data->coder[i].data = data;
		i++;
	}
	return (1);
}

int	init_dongle(t_data *data)
{
	int	i;

	i = 0;
	if (!data)
		return (0);
	data->dongle = malloc(sizeof(t_dongle) * data->number_of_coders);
	while (i < data->number_of_coders)
	{
		if (pthread_mutex_init(&(data->dongle[i].mutex), NULL))
			return (0);
		// printf("kbito\n");
		if (pthread_cond_init(&(data->dongle[i].cond), NULL))
			return (0);
		data->dongle[i].last_compile = 0;
		data->dongle[i].take_in_use = 0;
		data->dongle[i].head = NULL;
		i++;
	}
	return (1);
}

// int	clean_up(t_data *data)
// {
// 	int	i;

// 	if (!data->dongle)
// 		return (0);
// 	pthread_mutex_destroy(&data->data_mutex);
// 	pthread_mutex_destroy(&data->log_mutex);
// 	i = 0;
// 	while (i < data->number_of_coders)
// 	{
// 		pthread_mutex_destroy(&data->coder[i].coder_mutex);
// 		i++;
// 	}
// 	free(data->coder);
// 	clean_dongle(data);
// 	return (1);
// }

// int	clean_dongle(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->number_of_coders)
// 	{
// 		pthread_mutex_destroy(&(data->dongle[i].mutex));
// 		pthread_cond_destroy(&(data->dongle[i].cond));
// 		if (data->dongle[i].head)
// 			free_que((data->dongle[i].head));
// 		i++;
// 	}
// 	free(data->dongle);
// 	return (1);
// }

// // void	free_que(deque **head)
// // {
// // 	deque	*tmp;
// // 	deque	*current;

// // 	current = *head;
// // 	while (current)
// // 	{
// // 		tmp = current->next;
// // 		free(current);
// // 		current = tmp;
// // 	}
// // }
