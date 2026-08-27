#include "codexion.h"

int check_start(t_data *data);
void wait_coders(t_data *data);
void set_all_coder_time(t_data *data);
int check_coder_compile_count(t_coder *coder);

long long get_min_burnout(t_coder *coder);
int check_is_finished(t_data *data);

void broadcast_coders(t_coder *coders) {
  int i;

  i = 0;
  while (i < coders[0].data->number_of_coders) {
    pthread_mutex_lock(&coders[i].action_sleep_mutex);
    pthread_cond_broadcast(&coders[i].action_sleep_cond);
    pthread_mutex_unlock(&coders[i].action_sleep_mutex);
    i++;
  }
}

void *monitor(void *pointer) {
  t_coder *coders;
  t_data *data;
  struct timespec ts;

  coders = (t_coder *)pointer;
  data = coders[0].data;
  wait_coders(coders[0].data);
  pthread_mutex_lock(&data->data_mutex);
  while (!data->is_simulation_ended &&
         data->is_finished != data->number_of_coders && !check_dead(coders)) {
    ts = mono_deadline_to_realtime_ts(get_min_burnout(coders));
    pthread_cond_timedwait(&data->state_cond, &data->data_mutex, &ts);
  }
  data->is_simulation_ended = 1;
  pthread_mutex_unlock(&data->data_mutex);
  if (data->burn_coder)
    print_log(data, data->burn_coder, "burn", get_time_ms());
  pthread_mutex_lock(&data->scheduler_mutex);
  broadcast_coders(coders);
  pthread_cond_broadcast(&coders[0].data->scheduler_cond);
  pthread_mutex_unlock(&data->scheduler_mutex);
  return (void *)(1);
}

long long get_min_burnout(t_coder *coder) {
  long long temp;
  long long min;
  int i;

  i = 0;
  min = LLONG_MAX;
  while (i < coder[0].data->number_of_coders) {
    if (check_count_compile(&coder[i])) {
      temp = get_burn_out(&coder[i]);
      if (temp < min)
        min = temp;
    }
    i++;
  }
  return (min);
}

int check_coder_compile_count(t_coder *coders) {
  int i;

  i = 0;
  while (i < coders[0].data->number_of_coders) {
    if (check_count_compile(&coders[i]))
      return (0);
    i++;
  }
  return (1);
}

void wait_coders(t_data *data) {
  pthread_mutex_lock(&data->data_mutex);
  while (data->read_count < data->number_of_coders) {
    pthread_cond_wait(&data->state_cond, &data->data_mutex);
  }
  data->start_time = get_time_ms();
  set_all_coder_time(data);
  data->start_flag = 1;
  pthread_cond_broadcast(&data->state_cond);
  pthread_mutex_unlock(&data->data_mutex);
}

void set_all_coder_time(t_data *data) {
  int i;

  i = 0;
  while (i < data->number_of_coders) {
    pthread_mutex_lock(&data->coder[i].coder_mutex);
    data->coder[i].burn_out_time = data->time_to_burnout + data->start_time;
    pthread_mutex_unlock(&data->coder[i].coder_mutex);
    i++;
  }
}

int check_start(t_data *data) {
  int i;
  int flag;

  i = 0;
  pthread_mutex_lock(&data->data_mutex);
  while (i < data->number_of_coders) {
    flag = data->start_flag & (1 << i);
    if (flag == 0) {
      pthread_mutex_unlock(&data->data_mutex);
      return (0);
    }
    i++;
  }
  pthread_mutex_unlock(&data->data_mutex);
  return (1);
}

// int check_is_finished(t_data *data) { return (1); }

int check_dead(t_coder *coders) {
  int i;
  long long time;
  long long now;

  i = 0;
  while (i < coders[0].data->number_of_coders) {
    pthread_mutex_lock(&(coders[i].coder_mutex));
    if (coders[i].count_compile < coders[0].data->number_of_compiles_required) {
      time = coders[i].burn_out_time - coders[0].data->start_time;
      now = get_time_ms() - coders[0].data->start_time;
      if ((time <= now)) {
        coders[0].data->burn_coder = &coders[i];
        pthread_mutex_unlock(&(coders[i].coder_mutex));
        return (1);
      }
    }
    pthread_mutex_unlock(&(coders[i].coder_mutex));
    i++;
  }
  return (0);
}
