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
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int nflag = 0, kflag = 0, dflag = 0;
  int nval, kval, dval=1;
  
	static char usage[] = "usage: %s -n nval -k kval [-d] dval\n";

	while ((c = getopt(argc, argv, "n:k:d:")) != -1)
		switch (c) {
		case 'n':
			nflag = 1;
      nval = atoi(optarg);
			break;
		case 'k':
			kflag = 1;
      kval = atoi(optarg);
			break;
    case 'd':
			dflag = 1;
      dval = atoi(optarg);
			break;
		case '?':
			err = 1;
			break;
		}
  /* check for for mandatory flags */
	if (nflag == 0 || kflag == 0) {	
		fprintf(stderr, "%s: missing -n or -k option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
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

  int d_val = dval*kval/nval;//d...25d
  
  /*FIFO*/
  double v = d_val/4.0;
  heap arrival_times = generate_arrival_times(nval,kval);
  lnklst_queue queue = create_queue2();
  int t = 0;
  double att = 0.0;
  process * current = NULL;
  while(!current || t < kval || !is_empty2(queue)){
    while(t == get_min(arrival_times)){
      process p;
      p.arrival_time = t;
      p.remaining_time = p.burst_time = 
        (int)round(nrand()*v + d_val);
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
  printf("FIFO Algorithm for (n,k)=(%d,%d): ATT= %.3f, d= %d, d/ATT= %.4f\n", nval, kval, att/nval, d_val, d_val*nval/att);
  /*END FIFO*/

  /*SJF*/
  double sjf_v = d_val/4.0;
  heap sjf_arrival_times = generate_arrival_times(nval,kval);
  lnklst_queue sjf_queue = create_queue2();
  int curr_at, next_at, curr_bt, next_bt;
  int sjf_t = 0;
  double sjf_att = 0.0;//sum of all turnaround times
  process * sjf_current = NULL;
  while(!sjf_current || sjf_t < kval || !is_empty2(sjf_queue)){
    while(sjf_t == get_min(sjf_arrival_times)){ //get min arrival time
      process p;
      curr_at = sjf_t;
      min_delete(&sjf_arrival_times);
      next_at = get_min(sjf_arrival_times);
      
        if(curr_at == next_at){//curr and next arrive simultaneously, give each bt and compare
          curr_bt = (int)round(nrand()*sjf_v + d_val);
          next_bt = (int)round(nrand()*sjf_v + d_val);
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
          p.remaining_time = p.burst_time = (int)round(nrand()*sjf_v + d_val);
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
  printf("SJF  Algorithm for (n,k)=(%d,%d): ATT= %.3f,  d= %d, d/ATT= %.4f\n", nval, kval, sjf_att/nval, d_val, d_val*nval/sjf_att);
  /*END SJF*/
  
  exit(0);
  }