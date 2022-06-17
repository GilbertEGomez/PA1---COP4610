/*
	PA1-COP4610
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
  for(int i = 0; i < n;i++)
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
	int nflag = 0, kflag = 0;
  int nval, kval, dval;
  
	static char usage[] = "usage: %s -n nval -k kval\n";

	while ((c = getopt(argc, argv, "n:k:")) != -1)
		switch (c) {
		case 'n':
			nflag = 1;
      nval = atoi(optarg);
			break;
		case 'k':
			kflag = 1;
      kval = atoi(optarg);
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
  
	/* print flags and there values */
	printf("nflag = %d\n", nflag);
  printf("nval = %d\n", nval);
  printf("kflag = %d\n", kflag);
	printf("kval = %d\n", kval);

  /* do something useful with the cmdline arguments */
  for (int dcount = 1; dcount < 26; dcount++){
  dval = dcount*kval/nval;
  /* add FIFO, SJF, SRT */
  double v = dval/4.0;
  heap arrival_times = generate_arrival_times(nval,kval);
    
  /*FIFO START*/
  lnklst_queue queue = create_queue2();
  int t = 0;
  double att = 0.0;
  process * current = NULL;
  while(!current || t < kval || !is_empty2(queue)){
    while(t == get_min(arrival_times)){
      process p;
      p.arrival_time = t;
      p.remaining_time = p.burst_time = 
        (int)round(nrand()*v + dval);
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
  printf("FIFO Algorithm for (n,k)=(%d,%d): ATT= %.3f,   d= %d, d/ATT= %.3f\n", nval, kval, att/nval, dval, dval*nval/att);
  }
  /*END FIFO*/

  /*SJF START*/

  //SJF

  /*END SFJ*/
  
	exit(0);
}
