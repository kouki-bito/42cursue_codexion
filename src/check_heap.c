/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:07:06 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 20:07:10 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	has_ready_higher_priority_request(t_dongle *dongle,
				t_request *request);

int	heap_compare(t_request *curr, t_request *request)
{
	if (strcmp(curr->coder->data->scheduler, "edf") == 0)
	{
		if (curr->deadline == request->deadline)
			return (curr->number < request->number);
		else
			return (curr->deadline < request->deadline);
	}
	else
		return (curr->number < request->number);
}

static int	has_ready_higher_priority_request(t_dongle *dongle,
		t_request *request)
{
	int			i;
	long long	now;
	t_request	*candinate;

	i = 0;
	now = get_time_ms();
	while (i < dongle->heap.size)
	{
		candinate = &dongle->heap.request[i];
		if (candinate->coder != request->coder && heap_compare(candinate,
				request))
		{
			if (candinate->coder->right_dongle->take_in_use == 0
				&& candinate->coder->left_dongle->take_in_use == 0
				&& get_dongle_cool_time(candinate->coder->right_dongle) <= now
				&& get_dongle_cool_time(candinate->coder->left_dongle) <= now)
				return (1);
		}
		i++;
	}
	return (0);
}

int	heap_first(t_dongle *dongle, t_request *request)
{
	int	flag;

	flag = 0;
	if (dongle->heap.size == 0)
		return (0);
	if (strcmp(request->coder->data->scheduler, "fifo") == 0)
	{
		flag = dongle->heap.request[0].coder == request->coder;
	}
	else
	{
		if (dongle->heap.request[0].coder == request->coder)
			return (1);
		if (has_ready_higher_priority_request(dongle, request))
			return (0);
		return (1);
	}
	return (flag);
}
