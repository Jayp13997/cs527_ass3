#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define COUNTER_VALUE (1UL << 24)

unsigned long counter = 0;
pthread_mutex_t counter_mutex;

void* update_counter(){
	for(unsigned long k = 0; k < COUNTER_VALUE; k++){
		//pthread_mutex_lock(&counter_mutex);
		__sync_lock_test_and_set(&(counter), counter++);
		//pthread_mutex_unlock(&counter_mutex);
	}
}

int main(int argc, char** argv) {

	if(argc < 2){
		printf("Please enter the number of threads\n");
		return 0;
	}

	int num_threads = atoi(argv[1]);
	if(num_threads < 1){
		printf("Please enter a valid number of threads\n");
		return 0;
	}
		
	pthread_t threads[num_threads];

	struct timeval start, end;

	gettimeofday(&start, NULL);

	for(int i = 0; i < num_threads; i++){
		pthread_create(&threads[i], NULL, update_counter, NULL); 
	}

	for(int j = 0; j < num_threads; j++){
		pthread_join(threads[j], NULL);
	}

	gettimeofday(&end, NULL);

	double seconds = (end.tv_sec - start.tv_sec);
	double microseconds = ((seconds * 1000000.0) + (end.tv_usec/1.0)) - (start.tv_usec/1.0);

	//double time_passed = (long)(end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;
	
	//printf("Counter finishe in %lf ms\n", time_passed);

	printf("Counter finish in %lf ms\n", (double)(microseconds/1000.0));
	printf("The value of counter should be %ld\n", (unsigned long)(COUNTER_VALUE * num_threads));
	printf("The value of counter is %ld\n", counter);

	return 0;
}
