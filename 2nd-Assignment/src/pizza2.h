#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// for bool types
#include <stdbool.h>

// to avoid implicit declaration warnings
#include <unistd.h>
#include <ctype.h>

// constants
#define Torderlow 1
#define Torderhigh 5
#define Norderlow 1
#define Norderhigh 5
#define Tprep 1
#define Tbake 10
#define Tlow 5
#define Thigh 15

// initial number of cooks, ovens and deliverers
int Ncook = 2;
int Noven = 5;
int Ndeliverer = 10;

// variable initialization for gettime function
struct timespec TIME;

// tables that will contain
// appropriate times
long *F_times;
long *Cold_times;

// routine declaration
void *order(void *x);

// function declaration for checking if an
// argument is a positive number
bool isNumber(char number[]);

// function for checking if pthread actions are ok
void rc_check(int rc);

// function for checking any possible memory leaks
void memory_check(void *pointer);

// mutexes declarations
pthread_mutex_t oven_lock;
pthread_mutex_t cook_lock;
pthread_mutex_t deliverer_lock;
pthread_mutex_t time_lock;
pthread_mutex_t screen_lock;
pthread_mutex_t time2_lock;

// conditions declarations
pthread_cond_t oven_cond;
pthread_cond_t cook_cond;
pthread_cond_t deliverer_cond;

// struct declaration the order's arguments
typedef struct order_args
{
	int number_of_pizzas;
	int id;
	int del_time;
} pizzas_ids;