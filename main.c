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

int main(int argc, char *argv[]) {
  t_data data;

  if (!ft_parse(argc, argv, &data))
    return (0);
  if (!init_dongle(&data))
    return (destroy_dongle(data.dongle, data.number_of_coders), 0);
  if (!init_coder(&data))
    return (destroy_coder(data.coder, data.number_of_coders), 0);
  if (!init_thread(&data))
    return (0);
  if (!join_thread(&data))
    return (0);
  destroy_all(&data);
  return (0);
}
