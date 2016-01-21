/*
Solution by Jenna and Andrea! :)

The matrix is partitioned by rows.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include "Lab2IO.h"

int thread_count_;
int **dp_;  //pointer to the distance array
int city_count_;

pthread_mutex_t *muts;

void* thread_subcal (void* rank);

int main (int argc, char* argv[])
{
    long thread_i;
    pthread_t* thread_handle_pt;
    double start, end;
    
    if (argc < 2) {
        printf("Please indicate the number of threads!\n");
        return 1;
    }
    thread_count_ = strtol(argv[1], NULL, 10);
    
    Lab2_loadinput(&dp_, &city_count_);

    thread_handle_pt=malloc(thread_count_*sizeof(pthread_t));

    muts = malloc(city_count_ * sizeof(pthread_mutex_t));

    int z;
    for (z = 0; z < city_count_; z++) {
      pthread_mutex_init(&muts[z], NULL);
    }

    GET_TIME(start);
    for (thread_i = 0; thread_i < thread_count_; ++thread_i)
        pthread_create(&thread_handle_pt[thread_i], NULL, thread_subcal, (void*)thread_i);
    for (thread_i = 0; thread_i < thread_count_; ++thread_i)
        pthread_join(thread_handle_pt[thread_i], NULL);
    GET_TIME(end);

    printf("The elapsed time is %lfs.\n", end-start);

    for (z = 0; z < city_count_; z++) {
      pthread_mutex_destroy(&muts[z]);
    }
    free(thread_handle_pt);
    Lab2_saveoutput(dp_, city_count_, end-start);

    DestroyMat(dp_, city_count_);
    return 0;
}

void* thread_subcal(void* rank){
    long myrank = (long)rank;
    int i, j, k, temp;
    
    for (k = 0; k < city_count_; ++k) {
      pthread_mutex_lock(&muts[k]);
      for (i = myrank * city_count_ / thread_count_; i < (myrank + 1) * city_count_ / thread_count_; ++i) {
	if (i != k) {
	  pthread_mutex_lock(&muts[i]);
	}
	for (j = 0; j < city_count_; ++j) {
                if ((temp = dp_[i][k]+dp_[k][j]) < dp_[i][j])
                    dp_[i][j] = temp; 
	}
	if (i != k) {
	  pthread_mutex_unlock(&muts[i]);
	}
      }
      pthread_mutex_unlock(&muts[k]);
    }
    return 0;
}
