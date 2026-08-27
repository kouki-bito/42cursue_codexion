/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbito <kbito@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 21:02:18 by kbito             #+#    #+#             */
/*   Updated: 2026/08/27 21:02:40 by kbito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (!init_dongle(&data))
		destroy_dongle(data.dongle);
	if (!init_coder(&data))
		destroy_coder(data.coder);
	init_thread(&data);
	join_thread(&data);
	destroy_all(&data);
	return (0);
}
