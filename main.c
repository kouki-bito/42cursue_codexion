
#include "src/codexion.h"
#include <time.h>

void	*routing(void *argv);

void print_coder_dongle(t_coder* coder,int size);
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
	
	print_coder_dongle(data.coder,data.number_of_coders);
	
	destroy_all(&data);
	return 0;
}
void print_coder_dongle(t_coder* coder,int size)
{
	int i;
	i = 0;
	while (i < size)
	{
		printf("oder id:%d has left dongle:%d and right dongle:%d\n",coder[i].id,coder[i].left_dongle->id,coder[i].right_dongle->id);
		i++;
	}
	
}

// void test_monitor(t_data* data)
// {

// }