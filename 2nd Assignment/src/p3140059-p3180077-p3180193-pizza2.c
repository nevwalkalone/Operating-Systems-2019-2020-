#include "p3140059-p3180077-p3180193-pizza2.h"


int main(int argc, char ** argv){

	//check for number of arguments

	if (argc!=3 ){
	
		printf("Incorrect number of arguments, must be 2! \n");
		
		exit (-1);
	
	}
	
	//check for positive number
	
	if(!isNumber(argv[1])  || !isNumber(argv[2])){
		
		printf("Both arguments must be positive numbers!\n");
		
		exit (-1);
	}
	
	//number of orders
	int Ncust=atoi(argv[1]);
	
	//table that will contain all times
	//for the orders
	
	F_times = (long*)malloc((Ncust+1) * sizeof(long));
	
	//checking for memory leaks
	memory_check(F_times);
	
	
	
	F_times[0] = 0;
	

	//table that will contain all times
	//for which the orders were cold

	Cold_times=(long*)malloc((Ncust+1) * sizeof(long));

	//checking for memory leaks
	memory_check(Cold_times);
	

	
	Cold_times[0]=0;
	
	//seed
	unsigned int seed=atoi(argv[2]);

	
	//table that will contain all threads
	pthread_t* threads=(pthread_t*)malloc(Ncust*sizeof(pthread_t));
	
	//checking for memory leaks
	memory_check(threads);
	
	
	//table with the orders ids
	int* id =(int*)malloc(Ncust*sizeof(int));
	
	//checking for memory leaks
	memory_check(id);
	
	
	//int variable for 
	//checking if pthread actions are ok

	int rc;
	

	
	//mutexes and conditions initializations
	//and response code checking for each
	
	rc=pthread_mutex_init(&oven_lock,NULL);
	
	rc_check(rc);

	rc=pthread_mutex_init(&cook_lock,NULL);
	
	rc_check(rc);

	rc=pthread_mutex_init(&deliverer_lock,NULL);
	
	rc_check(rc);

	rc=pthread_mutex_init(&time_lock,NULL);
	
	rc_check(rc);
	
	rc=pthread_mutex_init(&time2_lock,NULL);
	
	rc_check(rc);

	rc=pthread_mutex_init(&screen_lock,NULL);
	
	rc_check(rc);
	


	rc=pthread_cond_init(&oven_cond,NULL);
	
	rc_check(rc);

	rc=pthread_cond_init(&cook_cond,NULL);
	
	rc_check(rc);
	
	rc=pthread_cond_init(&deliverer_cond,NULL);
	
	rc_check(rc);
	


	
	//creating the threads
	
	for (int i=0; i<Ncust; i++){
		
		//ids and number of pizzas
		id[i]=i+1;
		pizzas_ids x;
		x.number_of_pizzas=rand_r(&seed) % Norderhigh + Norderlow;
		x.del_time=(rand_r(&seed) %(Thigh-Tlow+1))+Tlow;
		x.id=id[i];
		
		
		//changing the seed for
		//each thread
		//so it's more random
		seed=seed*x.number_of_pizzas+x.id;
		
		
		rc=pthread_create(&threads[i],NULL,order,(void*)&x);
		
		rc_check(rc);
		
		unsigned int y= rand_r(&seed) %(Torderhigh)+(Torderlow);
		//printf("%u\n",y);
		//sleeping until next order 
		sleep(y);
		
	}
	
	
	//waiting for all threads
	//to end their routine
	
	for (int i=0; i<Ncust; i++){
		
		rc=pthread_join(threads[i], NULL); 
		
		rc_check(rc);
	}




	
	//Destroying all mutexes and conditions
	
	rc=pthread_mutex_destroy(&oven_lock);
	
	rc_check(rc);


	rc=pthread_mutex_destroy(&deliverer_lock);
	
	rc_check(rc);



	rc=pthread_mutex_destroy(&cook_lock);
	
	rc_check(rc);

	rc=pthread_mutex_destroy(&time_lock);
	
	rc_check(rc);

	rc=pthread_mutex_destroy(&time2_lock);
	
	rc_check(rc);


	rc=pthread_mutex_destroy(&screen_lock);
	
	rc_check(rc);


	rc=pthread_cond_destroy(&oven_cond);
	
	rc_check(rc);




	rc=pthread_cond_destroy(&cook_cond);
	
	rc_check(rc);


	rc=pthread_cond_destroy(&deliverer_cond);
	
	rc_check(rc);



	
	//sums to calculate
	//average time it took
	//for the order to be completed
	//and for which the order was cold
	
	int sum = 0;
	int sum2=0;

	for (int i = 0;i<Ncust;i++) {

		sum += F_times[i+1];		
		sum2+=Cold_times[i+1];

		}
	
	double avg=(double)sum/Ncust;
	double avg2=(double)sum2/Ncust;
	
	
	time_t max = 1;	
	time_t max2=1;
	
	//calculating max time of an order
	//and max time of an order being cold
	
	for (int i = 1; i<=Ncust ; i++) {
	
		if (F_times[i] > max)   { max = F_times[i]; }
		if (Cold_times[i]>max2) { max2=Cold_times[i];}

	}
	


	//print messages for max and average time 
	//for both order time and the time the same order was cold

	
	printf("\nAverage time of an order: %.1f minutes \n", avg);
	
	printf("Maximum time of an order: %ld   minutes\n", max);
	
	printf("Average time of an order being cold: %.1f minutes\n",avg2);

	printf("Maximum time of an order being cold: %ld  minutes\n",max2);


	//freeing up
	//memory
	
	free(F_times);
	free(Cold_times);
	free(threads);
	free(id);

}






//routine for the threads
//to follow

void* order(void * x){

		
	//using the struct we 
	//have created to store
	//the id and the number of pizzas
	
	pizzas_ids* thread_arg;
	thread_arg = (pizzas_ids *)x;
	
	
	//initializing the variables
	
	int id = thread_arg->id;
	int pizzas = thread_arg->number_of_pizzas;
	int deliv_time=thread_arg->del_time;
	int rc;
	
	
	//locking the time_lock mutex
	
	rc=pthread_mutex_lock(&time_lock);
	
	rc_check(rc);
	
	//storing the starting time of the order
	
	clock_gettime(CLOCK_REALTIME , &TIME);
	
	F_times[id] = TIME.tv_sec;
	

	rc=pthread_mutex_unlock(&time_lock);	
	
	rc_check(rc);
	
	
	
	//locking the cook_lock mutex

	rc=pthread_mutex_lock(&cook_lock);
	
	rc_check(rc);

	//while there are no cooks available
	//the thread waits
	
	while(Ncook==0){
		
		rc=pthread_cond_wait(&cook_cond, &cook_lock);
		
		rc_check(rc);


	}
	
	//there is a cook to handle the specific order now

	Ncook--;
	
	
	//cook unlock

	rc=pthread_mutex_unlock(&cook_lock);
	
	rc_check(rc);

	//pizzas preparation time

	sleep(pizzas*Tprep);
	
	//now the order is ready to enter
	//the oven, so we lock it with 
	//the appropriate mutex and check if
	//there are any available ovens

	rc=pthread_mutex_lock(&oven_lock);

	rc_check(rc);

	
	//while there are no ovens available
	//the thread waits

	while(Noven==0){

		rc=pthread_cond_wait(&oven_cond,&oven_lock);

		rc_check(rc);


	}
	
	//there is an oven to enter now

	Noven--;

	
	//oven unlock

	rc=pthread_mutex_unlock(&oven_lock);

	rc_check(rc);


	//The cook has placed
	//the order in the oven
	//so he is now available

	rc=pthread_mutex_lock(&cook_lock);
	
	rc_check(rc);
	
	Ncook++;
	
	//signaling the threads that were
	//waiting for a cook to be available

	rc=pthread_cond_signal(&cook_cond);

	rc_check(rc);
	
	rc=pthread_mutex_unlock(&cook_lock);
	
	//bake time 

	sleep(Tbake);
	

	//starting time of an order being cold
	rc=pthread_mutex_lock(&time2_lock);
	
	rc_check(rc);
	
	clock_gettime(CLOCK_REALTIME, &TIME);
	
	Cold_times[id]=TIME.tv_sec;
	
	
	rc=pthread_mutex_unlock(&time2_lock);
	
	rc_check(rc);



	rc=pthread_mutex_lock(&deliverer_lock);
	
	rc_check(rc);
	

	//while there are no deliverers available
	//the thread waits
	while(Ndeliverer==0){
		
		rc=pthread_cond_wait(&deliverer_cond, &deliverer_lock);
		
		rc_check(rc);


	}
	
	//there is a deliverer to deliver the specific order now
	Ndeliverer--;

	rc=pthread_mutex_unlock(&deliverer_lock);
	
	rc_check(rc);


	//deliverer took the order from the specific oven
	//so the oven is now available 
	rc=pthread_mutex_lock(&oven_lock);
	
	rc_check(rc);

	Noven++;
	
	//signaling the threads that 
	//were waiting for an oven to enter
	rc=pthread_cond_signal(&oven_cond);

	rc_check(rc);
	
	rc=pthread_mutex_unlock(&oven_lock);
	
	rc_check(rc);
	
	
	
	//delivery_time
	sleep(deliv_time);
	

	//calculating order and cold time and storing it
	//in F_times table and Cold_times table
	//respectively
	
	rc=pthread_mutex_lock(&time2_lock);
		
	rc_check(rc);

	clock_gettime(CLOCK_REALTIME, &TIME);

	time_t finish_time = TIME.tv_sec;
	
	time_t cold_time=finish_time-Cold_times[id];
	
	Cold_times[id]=cold_time;
	
	rc=pthread_mutex_unlock(&time2_lock);
		
	rc_check(rc);
	


	rc=pthread_mutex_lock(&time_lock);
		
	rc_check(rc);
	
	clock_gettime(CLOCK_REALTIME, &TIME);

	finish_time = TIME.tv_sec;

	time_t f_time = finish_time - F_times[id];
	
	F_times[id] = f_time;
	
	
	rc=pthread_mutex_unlock(&time_lock);

	rc_check(rc);


	
	//screen lock to print order's messages

	rc=pthread_mutex_lock(&screen_lock);

	rc_check(rc);

	//order is delivered, so we print the appropriate message
	
	printf("Order with id %d and %d pizzas was served successfully in %ld minutes and was cold for %ld minutes \n", id,pizzas, F_times[id],Cold_times[id]);
	

	//screen unlock

	rc=pthread_mutex_unlock(&screen_lock);
	
	rc_check(rc);
	
	
	//time for the deliverer to return
	sleep(deliv_time);
	
	
	
	//the deliverer has returned and
	//is now available so we lock
	//the appropriate mutex

	rc=pthread_mutex_lock(&deliverer_lock);
	
	Ndeliverer++;
	
	//signaling the threads that
	//are sleeping 
	rc=pthread_cond_signal(&deliverer_cond);

	rc_check(rc);
	
	rc=pthread_mutex_unlock(&deliverer_lock);	
	
	rc_check(rc);


	//thread exits
	
	pthread_exit(NULL);
	

}


//function for checking
//memory leaks

void memory_check(void* pointer){

	if (pointer==NULL){
		
		printf("No memory available!\n");

		exit (-1);
		

	}

}

//function for checking
//if all pthread actions
//are ok

void rc_check(int rc){

	if (rc != 0) {	

		printf("ERROR: return code from the specific pthread action is %d\n", rc);

		pthread_exit(&rc);
	}


}



//function for checking
//if the argument is a 
//positive number

bool isNumber(char number[])
{
    
    //checking for negative numbers
    if(number[0]=='-'){

		return false;
    }
	
	for (int i=0; number[i] != 0; i++)
    {
       
        if (!isdigit(number[i])){

            return false;
		
	}	
    }

    return true;
}

