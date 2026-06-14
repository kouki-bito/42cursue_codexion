#include "codexion.h"

int	ft_parse(int argc, char *argv[], t_data *data)
{
	if (argc != 9)
	{
		write(2, "Error: Invalid number of arguments.\n", 36);
		return (0);
	}
	if (!ft_check_format(argv))
	{
		write(2, "Error: Invalid format.\n", 24);
		return (0);
	}
	(*data).number_of_coders = atoi(argv[1]);
	(*data).time_to_burnout = atoi(argv[2]);
	(*data).time_to_compile = atoi(argv[3]);
	(*data).time_to_debug = atoi(argv[4]);
	(*data).time_to_refactor = atoi(argv[5]);
	(*data).number_of_compiles_required = atoi(argv[6]);
	(*data).dongle_cooldown = atoi(argv[7]);
	(*data).scheduler = argv[8];
	if (pthread_mutex_init(&(*data).data_mutex, NULL)
		&& pthread_mutex_init((&(*data).log_mutex), NULL))
		return (0);
	return (1);
}

int	ft_check_format(char *argv[])
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!ft_is_num(argv[i]))
			return (0);
		if (strlen(argv[i]))
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (0);
	return (1);
}

int	ft_is_num(char argv[])
{
	int	i;

	i = 0;
	if (argv[i] == '\0')
		return (0);
	while (argv[i] != '\0')
	{
		if (argv[i] < '0' || '9' < argv[i])
			return (0);
		i++;
	}
	return (1);
}
