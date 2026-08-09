
#include "src/codexion.h"
#include <time.h>

void	*routing(void *argv);


int	main(int argc, char *argv[])
{
	t_data	data;
	printf("sisi\n");
	if (!ft_parse(argc, argv, &data))
	{
		printf("help");
		return (0);
	}
	if (!init_dongle(&data))
		destroy_dongle(data.dongle);
	if(!init_coder(&data))
		destroy_coder(data.coder);
	// printf("hehe");
	test_heap(&data);
	
	destroy_all(&data);
	return 0;
}


// void test_monitor(t_data* data)
// {

// }