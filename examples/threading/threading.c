#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data * thread_args = (struct thread_data *)thread_param;
    usleep(thread_args->wait_to_obtain_ms * 1000);

    DEBUG_LOG("Locking");
    int rc = pthread_mutex_lock(thread_args->mutex);

    if (rc != 0)
        ERROR_LOG("Failed to lock");

    usleep(thread_args->wait_to_release_ms * 1000);

    DEBUG_LOG("Unlocking");
    rc = pthread_mutex_unlock(thread_args->mutex);

        if (rc != 0)
        ERROR_LOG("Failed to unlock");

    pthread_exit((void *)thread_args);
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
    struct thread_data* thread_params = (struct thread_data*)malloc(sizeof(thread_params));

    thread_params->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_params->wait_to_release_ms = wait_to_release_ms;
    thread_params->mutex = mutex;

    void * ret_val;

    DEBUG_LOG("Initializing mutex");
    int rc = pthread_mutex_init(mutex, NULL);

    if (rc != 0)
        ERROR_LOG("Intializing mutex failed");

    DEBUG_LOG("Creating Thread");
    rc = pthread_create(thread, NULL, threadfunc, thread_params);

    if (rc != 0)
        ERROR_LOG("Creating thread failed");

    DEBUG_LOG("Joining Thread");
    rc = pthread_join((pthread_t)*thread, &ret_val);

    if (rc != 0)
        ERROR_LOG("Joining thread failed");

    struct thread_data* thread_params_ret = (struct thread_data*)ret_val;
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    DEBUG_LOG("Freeing Pointer");

    free(thread_params);

    if (thread_params_ret->thread_complete_success)
        return true;
    
    return false;
}

