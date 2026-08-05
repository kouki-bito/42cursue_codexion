
#include "src/codexion.h"
#include <time.h>

void	*routing(void *argv);


int	main(int argc, char *argv[])
{
	t_data	data;
	int i;
	i=0;
	printf("sisi\n");
	if (!ft_parse(argc, argv, &data))
	{
		printf("help");
		return (0);
	}
	int init_dongle_num = init_dongle(&data);
	int init_coder_num = init_coder(&data);

	if (!init_dongle(&data))
		destroy_dongle(data.dongle);
	if(!init_coder(&data))
		destroy_coder(data.coder);
	// printf("hehe");
	while(i < data.number_of_coders)
	{
		if(pthread_create(&(data.coder[i].thread), NULL, &coder_routine,
			&(data.coder[i])))
			destroy_all(&data);

		i++;
	}
	i=0;
	while (i < data.number_of_coders)
	{
		if(pthread_join(data.coder[i].thread, NULL))
			destroy_all(&data);
		i++;
	}
	
	destroy_all(&data);
	return 0;
}


// void test_monitor(t_data* data)
// {

// }