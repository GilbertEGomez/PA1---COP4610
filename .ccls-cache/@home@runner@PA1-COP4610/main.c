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
  
  /*FIFO*/
  double v = d/4.0;
  heap arrival_times = generate_arrival_times(n,d);
  lnklst_queue queue = create_queue2();
  int t = 0;
  double att = 0.0;
  process * current = NULL;
  while(!current || t < k || !is_empty2(queue)){
    while(t == get_min(arrival_times)){
      process p;
      p.arrival_time = t;
      p.remaining_time = p.burst_time = 
        (int)round(nrand()*v + d);
      p.tt = 0;
      p.priority_level = rand()%10 + 1;
      enqueue2(&queue, p);
      min_delete(&arrival_times);
    }
    if(current == NULL && !is_empty2(queue)){
      current = (process*)malloc(sizeof(process));
      *current = dequeue2(&queue);
    }
    if(current != NULL){
      current->remaining_time--;
      if(current->remaining_time == 0){
        current->tt = (t+1) - current->arrival_time;
        att += current->tt;        
        free(current);
        current = NULL;
      }
    }
    t++;
  }
<<<<<<< HEAD
  printf("FIFO Algorithm for (n,k)=(%d,%d): ATT= %.3f,   d= %d, d/ATT= %.3f\n", nval, kval, att/nval, dval, dval*nval/att);
  
  /*END FIFO*/

  /*SJF START*/

  //SJF

  /*END SFJ*/

  /* MLF START */
  heap mlf_arrival_times = generate_arrival_times(nval,kval);
  heap mlf_priorities = create_heap(10); // Used to keep track of current priorities in MLF
  lnklst_queue n1 = create_queue2();
  lnklst_queue n2 = create_queue2();
  lnklst_queue n3 = create_queue2();
  lnklst_queue n4 = create_queue2();
  lnklst_queue n5 = create_queue2();
  lnklst_queue n6 = create_queue2();
  lnklst_queue n7 = create_queue2();
  lnklst_queue n8 = create_queue2();
  lnklst_queue n9 = create_queue2();
  lnklst_queue n10 = create_queue2();
  lnklst_queue levels[10] = {n1, n2, n3, n4, n5, n6, n7, n8, n9, n10}; //Preparing heaps for multi-level processing.
  

  int t2 = 0;
  double att2 = 0;
  process *current2 = NULL;

  // Checking if the priority level queues are not empty, and that a pointer exists to use for a process. Additonally, checks if t2 has finished its interval for kval.
  while (!current2 || t2 < kval || !is_empty2(n1) || !is_empty2(n2)
    || !is_empty2(n3) || !is_empty2(n4) || !is_empty2(n5) || !is_empty2(n6)
    || !is_empty2(n7) || !is_empty2(n8) || !is_empty2(n9) || !is_empty2(n10)) {
    
    // initalizing process information and enqueuing it to the correct priority Queue.
    while (t2 == get_min(mlf_arrival_times))
    {
      process p;
      p.arrival_time = t2;
      p.remaining_time = p.burst_time =
        (int)round(nrand()*v +dval);
      p.tt = 0;
      p.priority_level = rand()%10+1;
      max_insert(&mlf_priorities, p.priority_level);
      // Enqueue process at the correct priority level.
      enqueue2(&levels[p.priority_level], p);
      // Remove its arrival time from the possible set.
      min_delete(&mlf_arrival_times);
      printf("t=%d: a new process admitted, bt= %d\n", t2, p.burst_time);
    }
    // TODO...
   if (current2 == NULL && !is_empty(mlf_priorities))
    {
      current2 = (process*)malloc(sizeof(process));
      *current2 = dequeue2(&levels[get_max(mlf_priorities)]);
      printf("t=%d: a process is running, bt= %d\n", t2, current2->burst_time);
      max_delete(&mlf_priorities);
    }

   if (current2 != NULL) {
     free(current2);
     printf("Test");
     current2 = NULL;
   }
    
    
  }

  


  /* MLF END */
    }
	exit(0);
}
=======
  printf("FIFO Algorithm for (n,k)=(%d,%d): ATT= %.3f, d= %d, d/ATT= %.4f\n", n, k, att/n, d, d*n/att);
  /*END FIFO*/

  /*SJF*/
  double sjf_v = d/4.0;
  heap sjf_arrival_times = generate_arrival_times(n,k);
  lnklst_queue sjf_queue = create_queue2();
  int curr_at, next_at, curr_bt, next_bt;
  int sjf_t = 0;
  double sjf_att = 0.0;//sum of all turnaround times
  process * sjf_current = NULL;
  while(!sjf_current || sjf_t < k || !is_empty2(sjf_queue)){
    while(sjf_t == get_min(sjf_arrival_times)){ //get min arrival time
      process p;
      curr_at = sjf_t;
      min_delete(&sjf_arrival_times);
      next_at = get_min(sjf_arrival_times);
      
        if(curr_at == next_at){//curr and next arrive simultaneously, give each bt and compare
          curr_bt = (int)round(nrand()*sjf_v + d);
          next_bt = (int)round(nrand()*sjf_v + d);
          if(curr_bt <= next_bt){
            //admit curr_at
            p.remaining_time = p.burst_time = curr_bt;
            p.arrival_time = sjf_t;
            p.tt = 0;
            p.priority_level = rand()%10 + 1;
            enqueue2(&sjf_queue, p);
            min_delete(&sjf_arrival_times);
            //then admit next_at
            p.remaining_time = p.burst_time = next_bt;
            p.arrival_time = sjf_t;
            p.tt = 0;
            p.priority_level = rand()%10 + 1;
            enqueue2(&sjf_queue, p);
            min_delete(&sjf_arrival_times);
          }
          else{
            //admit next_at
            p.remaining_time = p.burst_time = next_bt;
            p.arrival_time = sjf_t;
            p.tt = 0;
            p.priority_level = rand()%10 + 1;
            enqueue2(&sjf_queue, p);
            min_delete(&sjf_arrival_times);
            //then admit curr_at
            p.remaining_time = p.burst_time = curr_bt;
            p.arrival_time = sjf_t;
            p.tt = 0;
            p.priority_level = rand()%10 + 1;
            enqueue2(&sjf_queue, p);
            min_delete(&sjf_arrival_times);
          }
        }
        else{//curr arrives alone, give bt, admit
          //admit curr_at
          p.remaining_time = p.burst_time = (int)round(nrand()*sjf_v + d);
          p.arrival_time = sjf_t;
          p.tt = 0;
          p.priority_level = rand()%10 + 1;
          enqueue2(&sjf_queue, p);
          min_delete(&sjf_arrival_times);
        }
      }
    if(sjf_current == NULL && !is_empty2(sjf_queue)){
      sjf_current = (process*)malloc(sizeof(process));
      *sjf_current = dequeue2(&sjf_queue);
    }
    if(sjf_current != NULL){
      sjf_current->remaining_time--;
      if(sjf_current->remaining_time == 0){
        sjf_current->tt = (sjf_t+1) - sjf_current->arrival_time;
        sjf_att = sjf_att + sjf_current->tt;   
        free(sjf_current);
        sjf_current = NULL;
      }
    }
    sjf_t++;
  }
  printf("SJF  Algorithm for (n,k)=(%d,%d): ATT= %.3f,  d= %d, d/ATT= %.4f\n", n, k, sjf_att/n, d, d*n/sjf_att);
  /*END SJF*/
  
  exit(0);
  }
>>>>>>> origin/main
