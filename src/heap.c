#include "codexion.h"
#include <string.h>

void heap_swap(t_heap *manegment, int b, int a);
void heap_pop(t_heap *manegment, t_coder *coder);
static void heap_down(t_heap *manegment, int index);
static void heap_up(t_heap *manegment, int index);

void heap_init(t_heap *manegment) {
  manegment->size = 0;
  pthread_mutex_init(&manegment->lock, NULL);
  pthread_cond_init(&manegment->cond, NULL);
}

void heap_push(t_heap *managment, t_request *request) {
  int index;
  int parent;

  index = managment->size;
  managment->request[index] = *request;
  managment->size++;
  while (index > 0) {
    parent = (index - 1) / 2;
    if (heap_compare(&managment->request[index], &managment->request[parent])) {
      heap_swap(managment, parent, index);
      index = parent;
    } else
      break;
  }
}

void heap_swap(t_heap *manegment, int b, int a) {
  t_request temp;

  temp = manegment->request[b];
  manegment->request[b] = manegment->request[a];
  manegment->request[a] = temp;
  return;
}
void heap_pop(t_heap *manegment, t_coder *coder) {
  int i;

  i = 0;
  while (i < manegment->size) {
    if (manegment->request[i].coder == coder)
      break;
    i++;
  }
  if (i == manegment->size)
    return;
  manegment->size--;
  if (i == manegment->size)
    return;
  manegment->request[i] = manegment->request[manegment->size];
  if (i > 0 &&
      heap_compare(&manegment->request[i], &manegment->request[(i - 1) / 2]))
    heap_up(manegment, i);
  else
    heap_down(manegment, i);
  return;
}
static void heap_down(t_heap *manegment, int index) {
  int right;
  int left;
  int smallest;

  while ((index * 2) + 1 < manegment->size) {
    right = index * 2 + 1;
    left = index * 2 + 2;
    smallest = right;
    if (left < manegment->size &&
        heap_compare(&manegment->request[left], &manegment->request[right]))
      smallest = left;
    if (heap_compare(&manegment->request[smallest],
                     &manegment->request[index])) {
      heap_swap(manegment, smallest, index);
      index = smallest;
    } else
      break;
  }
}
void heap_up(t_heap *manegment, int index) {
  int parent;

  while (index > 0) {
    parent = (index - 1) / 2;
    if (heap_compare(&manegment->request[index], &manegment->request[parent])) {
      heap_swap(manegment, index, parent);
      index = parent;
    } else
      break;
  }
}
int heap_compare(t_request *curr, t_request *request) {
  if (strcmp(curr->coder->data->scheduler, "edf") == 0) {
    if (curr->deadline == request->deadline)
      return (curr->number < request->number);
    else
      return (curr->deadline < request->deadline);
  } else
    return (curr->number < request->number);
}
int has_ready_higher_priority_request(t_dongle *dongle, t_request *request) {
  int i;
  i = 0;
  long long now;
  t_request *candinate;
  now = get_time_ms();
  while (i < dongle->heap.size) {
    candinate = &dongle->heap.request[i];
    if (candinate->coder != request->coder &&
        heap_compare(candinate, request)) {
      printf("req = %d candidate = %d now = %lld req_deadline=%lu candidate_deadline=%lu Luse = %d Ruse = %d Lcool = %lld Rcool =%lld\n ",
                                                          request->coder->id,
             candinate->coder->id, now, request->deadline, candinate->deadline,
             candinate->coder->left_dongle->take_in_use,
             candinate->coder->right_dongle->take_in_use,
             get_dongle_cool_time(candinate->coder->left_dongle),
             get_dongle_cool_time(candinate->coder->right_dongle));
      if (candinate->coder->right_dongle->take_in_use == 0 &&
          candinate->coder->left_dongle->take_in_use == 0 &&
          get_dongle_cool_time(candinate->coder->right_dongle) <= now &&
          get_dongle_cool_time(candinate->coder->left_dongle) <= now)
        return 0;
    }
    i++;
  }
  return 1;
}
int heap_first(t_dongle *dongle, t_request *request) {
  int flag;
  flag = 0;
  if (dongle->heap.size == 0)
    return 0;
  if (strcmp(request->coder->data->scheduler, "fifo") == 0) {
    flag = dongle->heap.request[0].coder == request->coder;
  } else {
    flag = dongle->heap.request[0].deadline == request->deadline;
  }
  return (flag);
}

// int heap_first(t_dongle *dongle, t_request *request) {
//   int flag;
//   flag = 0;
//   if (dongle->heap.size == 0)
//     return (0);
//   if (strcmp(request->coder->data->scheduler, "fifo") == 0) {
//     flag = dongle->heap.request[0].coder == request->coder;
//   } else {
//     if (dongle->heap.request[0].coder == request->coder)
//       flag = 1;
//     else
//       flag = has_ready_higher_priority_request(dongle, request);
//   }
//   return (flag);
// }
