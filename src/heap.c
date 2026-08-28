/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:07:35 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:43:07 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void		heap_swap(t_heap *manegment, int b, int a);
void		heap_pop(t_heap *manegment, t_coder *coder);
static void	heap_down(t_heap *manegment, int index);
static void	heap_up(t_heap *manegment, int index);

void	heap_push(t_heap *managment, t_request *request)
{
	int	index;
	int	parent;

	index = managment->size;
	managment->request[index] = *request;
	managment->size++;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap_compare(&managment->request[index],
				&managment->request[parent]))
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
	t_request	temp;

	temp = manegment->request[b];
	manegment->request[b] = manegment->request[a];
	manegment->request[a] = temp;
	return ;
}

void	heap_pop(t_heap *heap, t_coder *coder)
{
	int	i;
	int	parent;

	i = 0;
	while (i < heap->size)
	{
		if (heap->request[i].coder == coder)
			break ;
		i++;
	}
	if (i == heap->size)
		return ;
	heap->size--;
	if (i == heap->size)
		return ;
	heap->request[i] = heap->request[heap->size];
	parent = (i - 1) / 2;
	if (i > 0 && heap_compare(&heap->request[i], &heap->request[(parent)]))
		heap_up(heap, i);
	else
		heap_down(heap, i);
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
		if (left < manegment->size && heap_compare(&manegment->request[left],
				&manegment->request[right]))
			smallest = left;
		if (heap_compare(&manegment->request[smallest],
				&manegment->request[index]))
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
		if (heap_compare(&manegment->request[index],
				&manegment->request[parent]))
		{
			heap_swap(manegment, index, parent);
			index = parent;
		}
		else
			break ;
	}
}
