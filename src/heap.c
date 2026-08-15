#include "codexion.h"

void		heap_swap(t_heap *manegment, int b, int a);
void		heap_pop(t_heap *manegment, t_coder *coder);
int			heap_compare(t_coder *curr, t_coder *coder);
static void	heap_down(t_heap *manegment, int index);
static void	heap_up(t_heap *manegment, int index);

void	heap_init(t_heap *manegment)
{
	manegment->size = 0;
	pthread_mutex_init(&manegment->lock, NULL);
	pthread_cond_init(&manegment->cond, NULL);
}
void	heap_push(t_heap *managment, t_coder *coder)
{
	int	index;
	int	parent;

	index = managment->size;
	managment->coders[index] = coder;
	managment->size++;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap_compare(managment->coders[index], managment->coders[parent]))
		{
			heap_swap(managment, parent, index);
			index = parent;
		}
		else
			break ;
	}
}

void	heap_swap(t_heap *manegment, int b, int a)
{
	t_coder	*temp;

	temp = manegment->coders[b];
	manegment->coders[b] = manegment->coders[a];
	manegment->coders[a] = temp;
	return ;
}
void	heap_pop(t_heap *manegment, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < manegment->size)
	{
		if (manegment->coders[i] == coder)
			break ;
		i++;
	}
	if (i == manegment->size)
		return ;
	manegment->size--;
	manegment->coders[i] = manegment->coders[manegment->size];
	heap_up(manegment, i);
	heap_down(manegment, i);
	return ;
}
static void	heap_down(t_heap *manegment, int index)
{
	int	right;
	int	left;
	int	smallest;

	while ((index * 2) + 1 < manegment->size)
	{
		right = index * 2 + 1;
		left = index * 2 + 2;
		smallest = right;
		if (heap_compare(manegment->coders[left], manegment->coders[right]))
			smallest = left;
		if (heap_compare(manegment->coders[smallest], manegment->coders[index]))
		{
			heap_swap(manegment, smallest, index);
			index = smallest;
		}
		else
			break ;
	}
}
void	heap_up(t_heap *manegment, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap_compare(manegment->coders[index], manegment->coders[parent]))
		{
			heap_swap(manegment, index, parent);
			index = parent;
		}
		else
			break ;
	}
}
int	heap_compare(t_coder *curr, t_coder *coder)
{
	return (get_burn_out(curr) < get_burn_out(coder));
}

int	heap_first(t_dongle *dongle, t_coder *coder)
{
	int	flag;

	pthread_mutex_lock(&dongle->scheduler_mutex);
	if (dongle->heap.size > 0)
		flag = dongle->heap.coders[0] == coder;
	pthread_mutex_unlock(&dongle->scheduler_mutex);
	return (flag);
}
