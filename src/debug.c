
#include "codexion.h"
#include <stdlib.h>
static void	push_tester(t_heap *heap, int size, t_coder *coder);
static void	print_burn_out(t_heap *heap);
static void pop_tester(t_heap* heap,int size,int index);
static void	print_coder(t_coder *coder, int size);

void	test_heap(t_data *data)
{

	t_heap	heap;
    heap_init(&heap);


	push_tester(&heap, data->number_of_coders, data->coder);
    pop_tester(&heap,data->number_of_coders,2);
}

static void pop_tester(t_heap* heap,int size,int index)
{
    t_coder* coder;
    coder = heap->coders[index];
    printf("pop test\n");
    print_burn_out(heap);
    printf("remove %d coder id:%d\n",index,coder->id);
    heap_pop(heap,coder);
    print_burn_out(heap);
}
static void	push_tester(t_heap *heap, int size, t_coder *coder)
{

	int		i;


	i = 0;
    
	while (i < size)
	{
		coder[i].last_compile = rand() % size*2;
		i++;
	}
	print_coder(coder, size);
	i = 0;
    printf("heap size :%d",heap->size);
	while (i < size)
	{
        heap_push(heap, &coder[i]);
        i++;
	}
    print_burn_out(heap);
}

static void	print_burn_out(t_heap *heap)
{
	int	i;

	i = 0;
	printf("------------------------------\n");
	while (i < heap->size)
	{
		printf("coder id: %d was burnout:%ld\n", heap->coders[i]->id,
			heap->coders[i]->last_compile);
		i++;
	}
	printf("------------------------------\n");
}
static void	print_coder(t_coder *coder, int size)
{
	int i;
	i = 0;
	printf("------------------------------\n");
	printf("coder print\n");
	while (i < size)
	{
		printf("coder id: %d was burnout:%ld\n", coder[i].id,
			coder[i].last_compile);
		i++;
	}
	printf("------------------------------\n");
}