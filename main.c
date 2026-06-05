
#include "src/codexion.h"

int	main(int argc, char *argv[])
{
	t_data	data;

	if (!ft_parse(argc, argv, &data))
	{
		return (0);
	}
	printf("number_of_coders: %d\n", data.number_of_coders);
	printf("time_to_burnout: %d\n", data.time_to_burnout);
	printf(" time_to_compile: %d\n", data.time_to_compile);
	printf("time_to_debug: %d\n", data.time_to_debug);
	printf("time_to_refactor: %d\n", data.time_to_refactor);
	printf("number_of_compiles_required: %d\n",
		data.number_of_compiles_required);
	printf("dongle_cooldown: %d\n", data.dongle_cooldown);
	printf("scheduler: %s\n", data.scheduler);
}

int	init_coder(t_data *data)
{
	int	i;

	i = 0;
	if (data == NULL)
		return (0);
	data->coder = (t_data *)malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coder)
		return (0);
	while (i < data->number_of_coders)
	{
		data->coder[i].last_compile = 0;
		data->coder[i].last_compile = 0;
		data->coder[i].id = i + 1;
		data->coder[i].left_dongle = &data->dongle[i];
		data->coder[i].right_dongle = &data->dongle[(i + 1)
			% data->number_of_coders];
		if (pthread_mutex_init(&(data->coder[i].coder_mutex), NULL))
			return (0);
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
	while (i < data->number_of_coders)
	{
		if (pthread_mutex_init(&(data->dongle[i].mutex), NULL)
			&& pthread_cond_init(&(data->dongle[i].cond), NULL))
			;
			return (0);
		data->dongle[i].last_compile = 0;
		data->dongle[i].take_in_use = 0;
		data->dongle[i].head = NULL;
		i++;
	}
	return (1);
}

int	clean_up(t_data *data)
{
	int	i;

	if (!data->dongle)
		return (0);
	pthread_mutex_destroy(&data->data_mutex);
	pthread_mutex_destroy(&data->log_mutex);
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->coder[i].coder_mutex);
		i++;
	}
	free(data->coder);
	clean_dongle(data);
	return (1);
}

int	clean_dongle(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&(data->dongle[i].mutex));
		pthread_cond_destroy(&(data->dongle[i].cond));
		free_que((data->dongle[i].head));
		i++;
	}
	free(data->dongle);
	return 1;
}

void	free_que(t_wait_list *head)
{
	t_wait_list *tmp;
	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}
