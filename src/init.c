#include "codexion.h"
#include <pthread.h>

// int		clean_dongle(t_data *data);
// void	free_que(deque **head);
int init_coder(t_data *data) {
  int i;

  i = 0;
  if (data == NULL)
    return (0);
  data->coder = (t_coder *)malloc(sizeof(t_coder) * data->number_of_coders);
  if (!data->coder)
    return (0);
  while (i < data->number_of_coders) {
    data->coder[i].burn_out_time = 0;
    data->coder[i].id = i + 1;
    data->coder[i].left_dongle = &data->dongle[i];
    data->coder[i].count_compile = 0;
    data->coder[i].right_dongle =
        &data->dongle[(i + 1) % data->number_of_coders];
    if (pthread_mutex_init(&(data->coder[i].coder_mutex), NULL) ||
        pthread_mutex_init(&(data->coder[i].action_sleep_mutex), NULL) ||
        pthread_cond_init(&(data->coder[i].action_sleep_cond), NULL)) {
      return (0);
    }
    data->coder[i].data = data;
    i++;
  }
  return (1);
}

int init_dongle(t_data *data) {
  int i;

  i = 0;
  if (!data)
    return (0);
  data->dongle = malloc(sizeof(t_dongle) * data->number_of_coders);
  while (i < data->number_of_coders) {
    if (pthread_mutex_init(&(data->dongle[i].mutex), NULL))
      return (0);
    if (pthread_mutex_init(&(data->dongle[i].scheduler_mutex), NULL))
      return (0);
    if (pthread_mutex_init(&(data->dongle[i].cool_down_mutex), NULL))
      return (0);
    if (pthread_cond_init(&(data->dongle[i].cond), NULL))
      return (0);
    data->dongle[i].take_in_use = 0;
    data->dongle[i].cool_time = 0;
    data->dongle[i].heap.size = 0;
    data->dongle[i].id = i;
    heap_init(&(data->dongle[i].heap));
    i++;
  }
  return (1);
}
