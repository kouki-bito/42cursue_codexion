
#include "src/codexion.h"
#include <time.h>

void	*routing(void *argv);


int	main(int argc, char *argv[])
{
	t_data	data;
	// int i;
	// i=0;
	printf("sisi");
	if (!ft_parse(argc, argv, &data))
	{
		printf("help");
		return (0);
	}
	init_dongle(&data);
	init_coder(&data);

	// if (init_dongle(&data)||init_coder(&data))
	// 	destroy_all(&data);
	// printf("hehe");
	// while(i < data.number_of_coders)
	// {
	// 	if(pthread_create(&(data.coder[i].thread), NULL, &coder_routine,
	// 		&(data.coder[i])))
	// 		destroy_all(&data);

	// 	i++;
	// }
	// i=0;
	// while (i < data.number_of_coders)
	// {
	// 	if(pthread_join(data.coder[i].thread, NULL))
	// 		destroy_all(&data);
	// 	i++;
	// }
	
	destroy_all(&data);
	return 0;
}

