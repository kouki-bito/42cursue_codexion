

#include "codexion.h"

struct timespec get_time(void) {
  struct timespec ts;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  ts.tv_sec = tv.tv_sec;
  ts.tv_nsec = tv.tv_usec * 1000;
  return (ts);
}

long long get_time_ms(void) {
  struct timeval tv;
  long long milliseconds;

  gettimeofday(&tv, NULL);
  milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
  return (milliseconds);
}
struct timespec get_interval_time(long int time) {
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
void action_usleep(long long time, t_coder *coder) {
  struct timespec ts;
  long long stime;

  if (time != 0) {
    ts = get_interval_time(time + get_time_ms());
    stime = get_time_ms() + time;
    while (!check_simulation_status(coder) && get_time_ms() < stime) {
      pthread_mutex_lock(&coder->action_sleep_mutex);
      pthread_cond_timedwait(&(coder->action_sleep_cond),
                             &coder->action_sleep_mutex, &ts);
      pthread_mutex_unlock(&coder->action_sleep_mutex);
    }
  } else {
    if (!check_simulation_status(coder)) {
      pthread_mutex_lock(&coder->action_sleep_mutex);
      pthread_cond_timedwait(&(coder->action_sleep_cond),
                             &coder->action_sleep_mutex, &ts);
      pthread_mutex_unlock(&coder->action_sleep_mutex);
    }
  }
}
