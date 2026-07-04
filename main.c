
#include "src/codexion.h"
#include <time.h>

void	*routing(void *argv);


int	main(int argc, char *argv[])
{
	t_data	data;

	if (!ft_parse(argc, argv, &data))
	{
		return (0);
	}
	printf("number_of_coders: %lld\n", data.number_of_coders);
	printf("time_to_burnout: %lld\n", data.time_to_burnout);
	printf(" time_to_compile: %lld\n", data.time_to_compile);
	printf("time_to_debug: %lld\n", data.time_to_debug);
	printf("time_to_refactor: %lld\n", data.time_to_refactor);
	printf("number_of_compiles_required: %lld\n",
		data.number_of_compiles_required);
	printf("dongle_coolldown: %lld\n", data.dongle_cooldown);
	printf("scheduler: %s\n", data.scheduler);
	if (init_coder(&data))
	{
		printf("coder successd\n");
	}
	if (init_dongle(&data))
	{
		printf("dongle successd\n");
	}
	for (int i = 0; i < data.number_of_coders; i++)
	{
		pthread_create(&(data.coder[i].thread), NULL, &routing,
			&(data.coder[i]));
	}
	for (int i = 0; i < data.number_of_coders; i++)
	{
		pthread_join(data.coder[i].thread, NULL);
	}
	// printf("fifif\n");
	clean_up(&data);

}

void	*routing(void *argv)
{
	t_coder	*coder;
	pthread_cond_t cond;
	pthread_cond_init(&cond,NULL);
	// int		result;

	coder = (t_coder *)argv;
	execute_safely(&coder->coder_mutex, take,coder);
	// printf("coder:%d, take_lock_func:%d\n", coder->id, result);
	return (NULL);
}
