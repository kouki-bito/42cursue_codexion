
#include "src/codexion.h"
void* routing(void* argv);
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
	if(init_coder(&data)){
		printf("coder successd\n");
	}
	if(init_dongle(&data)){
		printf("dongle successd\n");
	}
	for(int i=0;i<data.number_of_coders;i++){
		pthread_create(&(data.coder[i].thread),NULL,&routing,&(data.coder[i]));
	}
	
	for(int i = 0; i < data.number_of_coders; i++){
		pthread_join(data.coder[i].thread, NULL);
	}
	printf("fifif\n");
	clean_up(&data);
}

void* routing(void* argv){
	t_coder* coder = (t_coder*)argv;
	for(int i=0;i<10;i++){
		printf("coder id:%d\n",coder->id);
	}
	return NULL;

}
