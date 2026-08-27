#include "codexion.h"
#include <bits/time.h>
#include <bits/types/clockid_t.h>
#include <pthread.h>
#include <time.h>

struct timespec get_time(void) {
  struct timespec ts;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  ts.tv_sec = tv.tv_sec;
  ts.tv_nsec = tv.tv_usec * 1000;
  return (ts);
}

long long get_time_ms(void) {
  struct timespec ts;
  long long milliseconds;

  clock_gettime(CLOCK_MONOTONIC, &ts);
  milliseconds = (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
  return (milliseconds);
}
struct timespec get_interval_time(long long time) {

  struct timespec ts;

  ts.tv_nsec = 0;
  ts.tv_sec = 0;
  ts.tv_sec += time / 1000;
  ts.tv_nsec += (time % 1000) * 1000000;
  if (ts.tv_nsec >= 1000000000) {
    ts.tv_sec += ts.tv_nsec / 1000000000;
    ts.tv_nsec = ts.tv_nsec % 1000000000;
  }
  return (ts);
}
long long get_real_time(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
struct timespec mono_deadline_to_realtime_ts(long long mono_deadline) {
  long long remaining;
  long long realtime_deadline;

  remaining = mono_deadline - get_time_ms();
  if (remaining < 0)
    remaining = 0;
  realtime_deadline = get_real_time() + remaining;
  return (get_interval_time(realtime_deadline));
}

void action_usleep(long long time, t_coder *coder) {
  struct timespec ts;
  long long mono_deadline;

  if (time != 0) {
    mono_deadline = get_time_ms() + time;
    pthread_mutex_lock(&coder->action_sleep_mutex);
    while (!check_simulation_status(coder) && get_time_ms() < mono_deadline) {
      ts = mono_deadline_to_realtime_ts(mono_deadline);
      pthread_cond_timedwait(&(coder->action_sleep_cond),
                             &coder->action_sleep_mutex, &ts);
    }
    pthread_mutex_unlock(&coder->action_sleep_mutex);
  }
}
