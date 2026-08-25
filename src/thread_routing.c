#include "codexion.h"
#include <pthread.h>

long long get_time_ms(void);
void wait_for_start(t_coder *coder);
void has_finished(t_coder *coder);
static void execute_coder_cycle(t_coder *coder) {
  take_dongles(coder);
  start_compile(coder);
  leave_dongle(coder);
  start_debuging(coder);
  start_refactor(coder);
}

void *coder_routine(void *arg) {
  t_coder *coder;

  coder = (t_coder *)arg;
  wait_for_start(coder);
  while (!check_simulation_status(coder)) {
    if (check_count_compile(coder))
      execute_coder_cycle(coder);
    else {
      has_finished(coder);
      break;
    };
  }
  return (NULL);
}

void has_finished(t_coder *coder) {
  pthread_mutex_lock(&coder->data->data_mutex);
  coder->data->is_finished++;
  pthread_cond_broadcast(&coder->data->state_cond);
  pthread_mutex_unlock(&coder->data->data_mutex);
  return;
}

int check_count_compile(t_coder *coder) {
  int count;

  pthread_mutex_lock(&coder->coder_mutex);
  count = coder->count_compile;
  pthread_mutex_unlock(&coder->coder_mutex);
  return (count < coder->data->number_of_compiles_required);
}

void wait_for_start(t_coder *coder) {
  pthread_mutex_lock(&coder->data->data_mutex);
  coder->data->read_count++;
  pthread_cond_broadcast(&coder->data->state_cond);
  while (!coder->data->start_flag && !coder->data->is_simulation_ended) {
    pthread_cond_wait(&coder->data->state_cond, &coder->data->data_mutex);
  }
  pthread_mutex_unlock(&coder->data->data_mutex);
}

void leave_dongle(t_coder *coder) {
  long long time;
  if (check_simulation_status(coder))
    return;
  pthread_mutex_lock(&coder->data->scheduler_mutex);
  time = get_time_ms();
  pthread_mutex_lock(&coder->right_dongle->mutex);
  pthread_mutex_lock(&coder->left_dongle->mutex);
  set_dongle_cool_time(coder->right_dongle, coder->left_dongle,
                       coder->data->dongle_cooldown, time);
  set_dongle_use(coder->right_dongle, coder->left_dongle, 0);
  pthread_mutex_unlock(&coder->right_dongle->mutex);
  pthread_mutex_unlock(&coder->left_dongle->mutex);
  pthread_cond_broadcast(&coder->data->scheduler_cond);
  pthread_mutex_unlock(&coder->data->scheduler_mutex);
}

void start_compile(t_coder *coder) {
  long long time;
  if (!check_simulation_status(coder)) {
    time = get_time_ms();
    set_burn_out(coder, coder->data->time_to_burnout, time);
    print_log(coder->data, coder, "compile", time);
    action_usleep(coder->data->time_to_compile, coder);
    if (!check_simulation_status(coder)) {
      pthread_mutex_lock(&coder->coder_mutex);
      coder->count_compile += 1;
      pthread_mutex_unlock(&coder->coder_mutex);
    }
  }
}

void start_debuging(t_coder *coder) {
  if (!check_simulation_status(coder)) {
    print_log(coder->data, coder, "debug", get_time_ms());
    action_usleep(coder->data->time_to_debug, coder);
  }
}

void start_refactor(t_coder *coder) {
  if (!check_simulation_status(coder)) {
    print_log(coder->data, coder, "refactor", get_time_ms());
    action_usleep(coder->data->time_to_refactor, coder);
  }
}
