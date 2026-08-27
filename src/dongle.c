
#include "codexion.h"
#include <pthread.h>
#include <time.h>

int try_take_dongle(t_dongle *first, t_dongle *second);

int take_dongle(t_dongle *first, t_dongle *second, t_request *request);

void take_dongles(t_coder *coder) {
  t_dongle *first;
  t_dongle *second;
  t_request request;
  long long max_cooldown;
  long long time;
  struct timespec ts;

  pthread_mutex_lock(&(coder->data->scheduler_mutex));
  request.coder = coder;
  request.deadline = get_burn_out(coder);
  pthread_mutex_lock(&coder->data->data_mutex);
  request.number = coder->data->request_num;
  coder->data->request_num++;
  pthread_mutex_unlock(&coder->data->data_mutex);
  heap_push(&coder->left_dongle->heap, &request);
  heap_push(&coder->right_dongle->heap, &request);
  if (coder->id % 2 == 1) {
    first = coder->left_dongle;
    second = coder->right_dongle;
  } else {
    first = coder->right_dongle;
    second = coder->left_dongle;
  }
  while (!is_simulation_ended(coder->data)) {
    if (try_take_dongle(first, second)) {
      if (take_dongle(first, second, &request)) {
        pthread_cond_broadcast(&(coder->data->scheduler_cond));
        time = get_time_ms();
        print_log(coder->data, coder, "take", time);
        print_log(coder->data, coder, "take", time);
        heap_pop(&coder->left_dongle->heap, coder);
        heap_pop(&coder->right_dongle->heap, coder);
        break;
      } else {
        pthread_cond_wait(&coder->data->scheduler_cond,
                          &coder->data->scheduler_mutex);
      }
    } else {
      max_cooldown = max_cool_time(first, second);
      if (max_cooldown > get_time_ms()) {
        ts = mono_deadline_to_realtime_ts(max_cooldown);
        pthread_cond_timedwait(&(coder->data->scheduler_cond),
                               &(coder->data->scheduler_mutex), &ts);
      } else {
        pthread_cond_wait(&(coder->data->scheduler_cond),
                          &(coder->data->scheduler_mutex));
      }
    }
  }
  pthread_mutex_unlock(&coder->data->scheduler_mutex);
}

int try_take_dongle(t_dongle *first, t_dongle *second) {
  long long int time;
  int flag1;
  int flag2;

  pthread_mutex_lock(&(first->mutex));
  pthread_mutex_lock(&(second->mutex));
  time = get_time_ms();
  flag1 = first->cool_time <= time && first->take_in_use == 0;
  flag2 = second->cool_time <= time && second->take_in_use == 0;
  if (flag1 && flag2) {
    pthread_mutex_unlock(&(first->mutex));
    pthread_mutex_unlock(&(second->mutex));
    return (1);
  }
  pthread_mutex_unlock(&first->mutex);
  pthread_mutex_unlock(&second->mutex);
  return (0);
}

int take_dongle(t_dongle *first, t_dongle *second, t_request *request) {
  pthread_mutex_lock(&first->mutex);
  pthread_mutex_lock(&second->mutex);
  if (heap_first(first, request) && heap_first(second, request)) {
    set_dongle_use(first, second, 1);
    pthread_mutex_unlock(&first->mutex);
    pthread_mutex_unlock(&second->mutex);
    return (1);
  }
  pthread_mutex_unlock(&first->mutex);
  pthread_mutex_unlock(&second->mutex);
  return (0);
}

void cool_time_sleep(t_dongle *first, t_dongle *second, t_coder *coder) {
  long long first_cool_time;
  long long second_cool_time;
  long long set_time;

  first_cool_time = get_dongle_cool_time(first);
  second_cool_time = get_dongle_cool_time(second);
  set_time = first_cool_time;
  if (first_cool_time < second_cool_time)
    set_time = second_cool_time;
  set_time -= get_time_ms();
  if (set_time <= 0)
    set_time = 0;
  action_usleep(set_time, coder);
}
