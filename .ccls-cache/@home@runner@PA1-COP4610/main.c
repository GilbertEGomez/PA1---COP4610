/*
        PA1-Temp
*/
#include "pa1.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define frand() (rand() / (double)RAND_MAX)
#define nrand() (sqrt(-2 * log(frand())) * cos(2 * M_PI * frand()))
heap generate_arrival_times(int n, int k) {
  heap h = create_heap(n);
  for (int i = 0; i < n; i++)
    min_insert(&h, frand() * k);
  return h;
}
void display(lnklst_queue *queue) {
  printf("\n\nDISPLAY SIZE=%d\n\n", (*queue).size);
  node *point = queue->head;
  while (point != NULL) {
    printf("\ndisplay process starting at %d time with burst time %d and "
           "remaining time %d\n",
           point->value.arrival_time, point->value.burst_time,
           point->value.remaining_time);
    point = point->next;
  }
}
// Function to remove an item from queue with the shortest remaining time.
process dequeue2_srtf(lnklst_queue *queue) {
  if (is_empty2(*queue)) {
    process p;
    p.arrival_time = -1; // dummy process
    return p;
  }

  node *point = queue->head;
  node *prev_min = point;
  point = point->next;
  while (point->next != NULL) {
    if ((prev_min->next->value.remaining_time >
         point->next->value.remaining_time) ||
        (prev_min->next->value.remaining_time ==
             point->next->value.remaining_time &&
         prev_min->next->value.arrival_time > point->next->value.arrival_time))
      prev_min = point;
    point = point->next;
  }
  node *deleted_min = prev_min->next;
  process item = deleted_min->value;
  prev_min->next = deleted_min->next;
  queue->size--;
  if (deleted_min->next == NULL)
    queue->tail = prev_min;
  free(deleted_min);
  return item;
}

int debug = 0;
int main(int argc, char **argv) {
  srand(time(NULL));
  int n = 100;
  int k = 1000;
  int d = k / n;
  while (*++argv) {
    if (**argv == '-' && (*argv)[1] == 'n') //-n
      n = atoi(*++argv);
    else if (**argv == '-' && (*argv)[1] == 'k') //-k
      k = atoi(*++argv);
    else if (**argv == '-' && (*argv)[1] == 'd') //-d
      d = atoi(*++argv);
    else
      continue;
  }

  /* print flags with their values */
  /*
printf("nflag = %d\n", nflag);
printf("nval = %d\n", nval);
printf("kflag = %d\n", kflag);
  printf("kval = %d\n", kval);
printf("dflag = %d\n", dflag);
  printf("dval = %d\n", dval);
*/

  /*FIFO*/
  double v = d / 4.0;
  heap arrival_times = generate_arrival_times(n, d);
  lnklst_queue queue = create_queue2();
  int t = 0;
  double att = 0.0;
  process *current = NULL;
  while (!current || t < k || !is_empty2(queue)) {
    while (t == get_min(arrival_times)) {
      process p;
      p.arrival_time = t;
      p.remaining_time = p.burst_time = (int)round(nrand() * v + d);
      p.tt = 0;
      p.priority_level = rand() % 10 + 1;
      enqueue2(&queue, p);
      min_delete(&arrival_times);
    }
    if (current == NULL && !is_empty2(queue)) {
      current = (process *)malloc(sizeof(process));
      *current = dequeue2(&queue);
    }
    if (current != NULL) {
      current->remaining_time--;
      if (current->remaining_time == 0) {
        current->tt = (t + 1) - current->arrival_time;
        att += current->tt;
        free(current);
        current = NULL;
      }
    }
    t++;
  }
  printf("FIFO Algorithm for (n,k)=(%d,%d): ATT= %.3f, d= %d, d/ATT= %.4f\n", n,
         k, att / n, d, d * n / att);
  /*END FIFO*/

  /*SJF*/
  double sjf_v = d / 4.0;
  heap sjf_arrival_times = generate_arrival_times(n, k);
  lnklst_queue sjf_queue = create_queue2();
  int curr_at, next_at, curr_bt, next_bt;
  int sjf_t = 0;
  double sjf_att = 0.0; // sum of all turnaround times
  process *sjf_current = NULL;
  while (!sjf_current || sjf_t < k || !is_empty2(sjf_queue)) {
    while (sjf_t == get_min(sjf_arrival_times)) { // get min arrival time
      process p;
      curr_at = sjf_t;
      min_delete(&sjf_arrival_times);
      next_at = get_min(sjf_arrival_times);

      if (curr_at == next_at) { // curr and next arrive simultaneously, give
                                // each bt and compare
        curr_bt = (int)round(nrand() * sjf_v + d);
        next_bt = (int)round(nrand() * sjf_v + d);
        if (curr_bt <= next_bt) {
          // admit curr_at
          p.remaining_time = p.burst_time = curr_bt;
          p.arrival_time = sjf_t;
          p.tt = 0;
          p.priority_level = rand() % 10 + 1;
          enqueue2(&sjf_queue, p);
          min_delete(&sjf_arrival_times);
          // then admit next_at
          p.remaining_time = p.burst_time = next_bt;
          p.arrival_time = sjf_t;
          p.tt = 0;
          p.priority_level = rand() % 10 + 1;
          enqueue2(&sjf_queue, p);
          min_delete(&sjf_arrival_times);
        } else {
          // admit next_at
          p.remaining_time = p.burst_time = next_bt;
          p.arrival_time = sjf_t;
          p.tt = 0;
          p.priority_level = rand() % 10 + 1;
          enqueue2(&sjf_queue, p);
          min_delete(&sjf_arrival_times);
          // then admit curr_at
          p.remaining_time = p.burst_time = curr_bt;
          p.arrival_time = sjf_t;
          p.tt = 0;
          p.priority_level = rand() % 10 + 1;
          enqueue2(&sjf_queue, p);
          min_delete(&sjf_arrival_times);
        }
      } else { // curr arrives alone, give bt, admit
        // admit curr_at
        p.remaining_time = p.burst_time = (int)round(nrand() * sjf_v + d);
        p.arrival_time = sjf_t;
        p.tt = 0;
        p.priority_level = rand() % 10 + 1;
        enqueue2(&sjf_queue, p);
        min_delete(&sjf_arrival_times);
      }
    }
    if (sjf_current == NULL && !is_empty2(sjf_queue)) {
      sjf_current = (process *)malloc(sizeof(process));
      *sjf_current = dequeue2(&sjf_queue);
    }
    if (sjf_current != NULL) {
      sjf_current->remaining_time--;
      if (sjf_current->remaining_time == 0) {
        sjf_current->tt = (sjf_t + 1) - sjf_current->arrival_time;
        sjf_att = sjf_att + sjf_current->tt;
        free(sjf_current);
        sjf_current = NULL;
      }
    }
    sjf_t++;
  }
  printf("SJF  Algorithm for (n,k)=(%d,%d): ATT= %.3f,  d= %d, d/ATT= %.4f\n",
         n, k, sjf_att / n, d, d * n / sjf_att);
  /*END SJF*/
  /*SRTF*/
  double srtf_v = d / 4.0;
  heap srtf_arrival_times = generate_arrival_times(n, d);
  lnklst_queue srtf_queue = create_queue2();
  int srtf_t = 0;
  double srtf_att = 0.0;
  process *srtf_current = NULL;
  while (srtf_current != NULL || srtf_t < k || !is_empty2(srtf_queue)) {
    while (srtf_t == get_min(srtf_arrival_times)) {
      process p;
      p.arrival_time = srtf_t;
      p.remaining_time = p.burst_time = (int)round(nrand() * srtf_v + d);
      p.tt = 0;
      p.priority_level = rand() % 10 + 1;
      enqueue2(&srtf_queue, p);
      min_delete(&srtf_arrival_times);
    }
    if (srtf_current == NULL && !is_empty2(srtf_queue)) {
      srtf_current = (process *)malloc(sizeof(process));
      *srtf_current = dequeue2_srtf(&srtf_queue);
    }
    if (srtf_current != NULL) {
      srtf_current->remaining_time--;
      if (srtf_current->remaining_time == 0) {
        srtf_current->tt = (srtf_t + 1) - srtf_current->arrival_time;
        srtf_att += srtf_current->tt;
      } else
        enqueue2(&srtf_queue, *srtf_current);
      free(srtf_current);
      srtf_current = NULL;
    }
    srtf_t++;
  }
  printf("SRTF Algorithm for (n,k)=(%d,%d): ATT= %.3f, d= %d, d/ATT= %.4f\n", n,
         k, srtf_att / n, d, d * n / srtf_att);
  /*END SRTF*/
  exit(0);
}