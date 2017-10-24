/*
 * 5DV171: Assignment 3
 * Author: Niclas Nyström
 * Date:   2017-10-24
 * Version: 1.0
 *
 * Benchmark for comparing Linux IO Schedulers according
 * to read, write and throughput policies.
 */


#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string.h>


// Enter number of threads. 4/20 used in report.
#define NUM_THREADS 20

void *BENCH_FUNC(void *OP);

pthread_t tid[NUM_THREADS];

int main(int argc, char* argv[]){

  int i = 0;
  int err;

  int threadID[NUM_THREADS];
  for (int k = 0; k < NUM_THREADS; k++) {
    threadID[k] = k+1;
  }


  clock_t start_TP = clock();

  while (i  < NUM_THREADS) {
    err = pthread_create(&(tid[i]), NULL, &BENCH_FUNC, &threadID[i]);
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
  printf("\nThroughput with %d threads: %f seconds\n", NUM_THREADS, elapsed_time_TP );
  return 0;
}



void *BENCH_FUNC(void *OP) {

  // Read test
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
  printf("Read thread %d: %f seconds \n", threadID, elapsed_time_Read );

  // Write test
  clock_t start_Write = clock();
  char filename[20];
  sprintf(filename, "%d", threadID);
  strcat(filename, ".txt");
  fp = fopen(filename, "w+");
  fputs(data, fp);
  fclose(fp);

  clock_t end_Write = clock();
  double elapsed_time_Write = (double) (end_Write - start_Write) / CLOCKS_PER_SEC;
  printf("Write thread %d: %f seconds\n", threadID, elapsed_time_Write );
  return NULL;
}

