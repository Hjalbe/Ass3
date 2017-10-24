#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define NUM_THREADS 20

pthread_t tid[NUM_THREADS];
double times[NUM_THREADS];

void *BENCH_FUNC(void *OP);



int main(int argc, char* argv[]){

  int i = 0;
  int err;

  int threadIDS[NUM_THREADS];
  for (int k = 0; k < NUM_THREADS; k++) {
    threadIDS[k] = k+1;
  }


  clock_t start_TP = clock();

  while (i  < NUM_THREADS) {
    err = pthread_create(&(tid[i]), NULL, &BENCH_FUNC, &threadIDS[i]);
    if (err == 0) {
      //printf("Successfully created thread %d\n", i+1);
    } else {
      printf("Unable to create thread %d\n", i+1);
    }
    i++;
  }

  i = 0;
  while (i  < NUM_THREADS) {
    err = pthread_join(tid[i], NULL);
    if (err == 0) {
      //printf("Successfully joined thread %d\n", i+1);
    } else {
      printf("Unable to joined thread %d\n", i+1);
    }
    i++;
  }
  clock_t end_TP = clock();
  double elapsed_time_TP = (double) (end_TP - start_TP) / CLOCKS_PER_SEC;
  printf("Throughput nThreads %d: Elapsed time %f \n", NUM_THREADS, elapsed_time_TP );

  return 0;
}



void *BENCH_FUNC(void *OP) {


  clock_t start_Read = clock();
  int threadID = *(int*) OP;
  int maxSize = 1000000;
  char data[maxSize];

  FILE *fp;
  fp = fopen("movies.txt", "r");
  while(fgets(data, sizeof(data), fp) != NULL){
    size_t len = strlen(data);
    if (len > 0 && data[len-1] != '\n') {
      data[--len] = '\0';
    }
  }
  fclose(fp);
  clock_t end_Read = clock();
  double elapsed_time_Read = (double) (end_Read - start_Read) / CLOCKS_PER_SEC;
  printf("Read thread %d: Elapsed time %f \n", threadID, elapsed_time_Read );


  clock_t start_Write = clock();
  char filename[20];
  sprintf(filename, "%d", threadID);
  strcat(filename, ".txt");
  fp = fopen(filename, "w+");
  fputs(data, fp);
  fclose(fp);
  //times[threadID] = elapsed_time;

  clock_t end_Write = clock();
  double elapsed_time_Write = (double) (end_Write - start_Write) / CLOCKS_PER_SEC;
  printf("Write thread %d: Elapsed time %f \n", threadID, elapsed_time_Write );

  return NULL;
}
