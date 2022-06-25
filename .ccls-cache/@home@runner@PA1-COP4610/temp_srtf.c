/*
	PA1-Temp
*/
#include "pa1.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))
heap generate_arrival_times(int n, int k){
  heap h = create_heap(n);
  for(int i = 0; i < n; i++)
    min_insert(&h, frand()*k);
  return h;
}

// Function to remove an item from queue with the shortest remaining time.
process dequeue2_srtf(lnklst_queue* queue)
{
  if (is_empty2(*queue))
  {
    process p;
    p.arrival_time = -1;//dummy process
    return p;
  }

  node* point = queue->head;
  node* prev_min = point;
  point = point->next;
  while (point->next != NULL) 
  {
    if ((prev_min->next->value.remaining_time > point->next->value.remaining_time) || (prev_min->next->value.remaining_time == point->next->value.remaining_time && prev_min->next->value.arrival_time > point->next->value.arrival_time))
      prev_min = point;
    point = point->next;
  }
  node* deleted_min = prev_min->next;
  process item = deleted_min->value;
  prev_min->next = deleted_min->next;
  queue->size--;
  if(queue->tail == deleted_min)
      queue->tail = queue->head;
  free(deleted_min);
  return item;
}

int debug = 0;
int main(int argc, char **argv)
{
  srand(time(NULL));
  int n = 100;
  int k = 1000;
  int d = k/n;
  while(*++argv){
    if(**argv == '-' && (*argv)[1] == 'n')//-n
      n = atoi(*++argv);
    else if(**argv == '-' && (*argv)[1] == 'k')//-k
      k = atoi(*++argv);
    else if(**argv == '-' && (*argv)[1] == 'd')//-d
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
  
  
  /*SRTF*/
  double srtf_v = d/4.0;
  heap srtf_arrival_times = generate_arrival_times(n,d);
  lnklst_queue srtf_queue = create_queue2();
  int srtf_t = 0;
  double srtf_att = 0.0;
  process * srtf_current = NULL;
  while(!srtf_current || srtf_t < k || !is_empty2(srtf_queue))
  {
    while(srtf_t == get_min(srtf_arrival_times))
    {
      process p;
      p.arrival_time = srtf_t;
      p.remaining_time = p.burst_time = 
        (int)round(nrand()*srtf_v + d);
      p.tt = 0;
      p.priority_level = rand()%10 + 1;
      enqueue2(&srtf_queue, p);
      min_delete(&srtf_arrival_times);
    }
    if(srtf_current == NULL && !is_empty2(srtf_queue))
    {
      srtf_current = (process*)malloc(sizeof(process));
      *srtf_current = dequeue2_srtf(&srtf_queue);
    }
    if(srtf_current != NULL)
    {
      srtf_current->remaining_time--;
      if(srtf_current->remaining_time == 0)
      {
        srtf_current->tt = (srtf_t+1) - srtf_current->arrival_time;
        srtf_att += srtf_current->tt;
      }
      else
        enqueue2(&srtf_queue, *srtf_current);        
      free(srtf_current);
      srtf_current = NULL;
    }
    srtf_t++;
  }
  printf("SRTF Algorithm for (n,k)=(%d,%d): ATT= %.3f, d= %d, d/ATT= %.4f\n", n, k, srtf_att/n, d, d*n/srtf_att);
  /*END SRTF*/
  exit(0);
}