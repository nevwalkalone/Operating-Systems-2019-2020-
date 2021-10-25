#include "pizza1.h"


int main(int argc, char ** argv){

	// check for number of arguments
	if (argc!=3 ){
	
		printf("Incorrect number of arguments, must be 2! \n");
		exit (-1);
	}
	
	// check for positive number
	if(!isNumber(argv[1])  || !isNumber(argv[2])){
	
		printf("Both arguments must be positive numbers!\n");
		exit (-1);
	}
	
	// number of orders
	int Ncust = atoi(argv[1]);
	
	// table that will contain all times for the orders
	F_times = (long*)malloc((Ncust+1) * sizeof(long));
	
	// checking for memory leaks
	memory_check(F_times);
	F_times[0] = 0;
	
	// seed
	unsigned int seed=atoi(argv[2]);

	//table that will contain all threads
	pthread_t* threads=(pthread_t*)malloc(Ncust*sizeof(pthread_t));
	
	// checking for memory leaks
	memory_check(threads);
	
	// table with the orders ids
	int* id = (int*)malloc(Ncust*sizeof(int));
	
	// checking for memory leaks
	memory_check(id);
	
	
	// int variable for checking  if pthread actions are ok
	int rc;
	
	// mutexes and conditions initializations and response
	// code checking for each
	rc=pthread_mutex_init(&oven_lock,NULL);
	rc_check(rc);

	rc=pthread_mutex_init(&cook_lock,NULL);
	rc_check(rc);

	rc=pthread_cond_init(&oven_cond,NULL);
	rc_check(rc);

	rc=pthread_cond_init(&cook_cond,NULL);
	rc_check(rc);
	
	rc=pthread_mutex_init(&time_lock,NULL);
	rc_check(rc);
	
	rc=pthread_mutex_init(&screen_lock,NULL);
	rc_check(rc);

	
	// creating the threads	
	for (int i=0; i<Ncust; i++){
		
		// ids and number of pizzas
		id[i]=i+1;
		pizzas_ids x;
		x.number_of_pizzas=rand_r(&seed) % Norderhigh + Norderlow;
		x.id=id[i];

		// changing the seed for each thread so it's
		// more random
		seed=seed+id[i];
		
		rc=pthread_create(&threads[i],NULL,order,(void*)&x);
		rc_check(rc);
		
		unsigned int y= rand_r(&seed) %(Torderhigh)+(Torderlow);

		// sleeping until next order 
		sleep(y);	
	}
	
	
	// waiting for all threads to end their routine
	for (int i=0; i<Ncust; i++){
		
		rc=pthread_join(threads[i], NULL); 
		rc_check(rc);
	}
	
	// Destroying all mutexes and conditions
	rc=pthread_mutex_destroy(&oven_lock);
	rc_check(rc);

	rc=pthread_mutex_destroy(&cook_lock);
	rc_check(rc);

	rc=pthread_cond_destroy(&oven_cond);
	rc_check(rc);

	rc=pthread_cond_destroy(&cook_cond);
	rc_check(rc);

	rc=pthread_mutex_destroy(&time_lock);
	rc_check(rc);

	rc=pthread_mutex_destroy(&screen_lock);
	rc_check(rc);


	
	// sum to calculate average time
	int sum = 0;

	for (int i = 0;i<Ncust;i++) {

		sum += F_times[i+1];		
	}
	
	double avg = (double) sum/Ncust;	
	time_t max = 1;	
	
	// calculating max time
	for (int i = 1; i<=Ncust ; i++) {
	
		if(F_times[i] > max) { max = F_times[i]; }
	}

	// print messages for max and average time
	
	printf("\nAverage time: %.1f minutes \n", avg);
	printf("Maximum time: %ld   minutes\n", max);
	
	
	// freeing up memory
	free(F_times);
	free(threads);
	free(id);
}

// check pizza1.h 
void* order(void * x){

	// using the struct we have created to 
	// to store the id and the number of pizzas
	pizzas_ids* thread_arg;
	thread_arg = (pizzas_ids *)x;
	
	
	// initializing the variables
	int id = thread_arg->id;
	int pizzas = thread_arg->number_of_pizzas;
	int rc;
	
	
	// locking the time_lock mutex
	rc=pthread_mutex_lock(&time_lock);
	rc_check(rc);
	
	// storing the starting time of the order
	
	clock_gettime(CLOCK_REALTIME , &start);
	F_times[id] = start.tv_sec;
	
	rc=pthread_mutex_unlock(&time_lock);	
	rc_check(rc);
		
	// locking the cook_lock mutex

	rc=pthread_mutex_lock(&cook_lock);
	rc_check(rc);

	// while there are no cooks available the thread
	// waits
	while(Ncook==0){
		
		rc=pthread_cond_wait(&cook_cond, &cook_lock);
		rc_check(rc);
	}
	
	// there is a cook to handle the specific order now
	Ncook--;
	
	// cook unlock
	rc=pthread_mutex_unlock(&cook_lock);
	rc_check(rc);

	// pizzas preparation time
	sleep(pizzas*Tprep);
	
	// now the order is ready to enter the oven so we
	// so we lock it with the appropriate mutex and check
	// if there are any available ovens
	rc=pthread_mutex_lock(&oven_lock);
	rc_check(rc);

	
	// while there are no ovens available the thread
	// waits
	while(Noven==0){

		rc=pthread_cond_wait(&oven_cond,&oven_lock);
		rc_check(rc);
	}
	
	// there is an oven to enter now
	Noven--;

	// oven unlock
	rc=pthread_mutex_unlock(&oven_lock);
	rc_check(rc);

	// bake time 
	sleep(Tbake);
	
	// locking the oven_lock again to make changes, now
	// the order is done
	rc=pthread_mutex_lock(&oven_lock);
	rc_check(rc);

	// the oven that was busy with the specific order is now
	// available
	Noven++;

	// we signal the threads that are sleeping
	rc=pthread_cond_signal(&oven_cond);
	rc_check(rc);

	// oven unlock
	rc=pthread_mutex_unlock(&oven_lock);
	rc_check(rc);

	// cook lock
	rc=pthread_mutex_lock(&cook_lock);
	rc_check(rc);
	
	// same logic as above
	Ncook++;


	// we signal again the threads that are sleeping 
	rc=pthread_cond_signal(&cook_cond);
	rc_check(rc);

	// cook unlock
	rc=pthread_mutex_unlock(&cook_lock);
	rc_check(rc);

	// time_lock to calculate the time with 
	// gettime function
	rc=pthread_mutex_lock(&time_lock);
	rc_check(rc);
	
	// calculating time and storing it in F_times
	// table
	clock_gettime(CLOCK_REALTIME, &finish);
	time_t finish_time = finish.tv_sec;
	time_t f_time = finish_time - F_times[id];
	F_times[id] = f_time;

	// time unlock
	rc=pthread_mutex_unlock(&time_lock);
	rc_check(rc);

	// screen lock to print order's messages
	rc=pthread_mutex_lock(&screen_lock);
	rc_check(rc);

	// order is finished, so we print the appropriate message
	printf("Order with id %d and %d pizzas was served successfully in %ld minutes \n", id,pizzas, F_times[id]);

	// screen unlock
	rc=pthread_mutex_unlock(&screen_lock);
	rc_check(rc);

	// thread exits	
	pthread_exit(NULL);	
}

// check pizza1.h 
void memory_check(void* pointer){

	if (pointer==NULL){
		
		printf("No memory available!\n");
		exit (-1);	
	}
}

// check pizza1.h 
void rc_check(int rc){

	if (rc != 0) {

		printf("ERROR: return code from the specific pthread action is %d\n", rc);
		pthread_exit(&rc);
	}
}

// check pizza1.h 
bool isNumber(char number[])
{
    // checking for negative numbers
    if(number[0]=='-'){
		return false;
    }
	
	for (int i=0; number[i] != 0; i++)
    {	
		// if it is not a digit return false
        if (!isdigit(number[i])){
            return false;	
		}	
    }
    return true;
}

