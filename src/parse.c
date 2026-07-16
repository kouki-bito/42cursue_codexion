#include "codexion.h"

int	ft_parse(int argc, char *argv[], t_data *data)
{
	if (argc != 9)
		return (write(2, "Error: Invalid number of arguments.\n", 36), 0);
	if (!ft_check_format(argv))
		return (write(2, "Error: Invalid format.\n", 24), 0);
	(*data).number_of_coders = ft_atoi_safe(argv[1]);
	(*data).time_to_burnout = ft_atoi_safe(argv[2]);
	(*data).time_to_compile = ft_atoi_safe(argv[3]);
	(*data).time_to_debug = ft_atoi_safe(argv[4]);
	(*data).time_to_refactor = ft_atoi_safe(argv[5]);
	(*data).number_of_compiles_required = ft_atoi_safe(argv[6]);
	(*data).dongle_cooldown = ft_atoi_safe(argv[7]);
	(*data).scheduler = argv[8];
	if ((*data).number_of_coders == -1 || (*data).time_to_burnout == -1
		|| (*data).time_to_compile == -1 || (*data).time_to_debug == -1
		|| (*data).time_to_refactor == -1
		|| (*data).number_of_compiles_required == -1
		|| (*data).dongle_cooldown == -1)
		return (0);
	if (init_mutex_and_cond(data))
		return (0);
	return (1);
}
int	init_mutex_and_cond(t_data *data)
{
	if (pthread_mutex_init(&(*data).data_mutex, NULL)
		&& pthread_mutex_init((&(*data).log_mutex), NULL)
		&& pthread_cond_init(&(data->usleep_cond), NULL))
		return (0);
	return (1);
}
int	ft_atoi_safe(char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > 214748364 || (res == 214748364 && (str[i] - '0') > 7))
			return (-1);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return ((int)res);
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
