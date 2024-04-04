/**
 * Implementation of thread pool.
 */
// Adding for justin branch
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 100
#define NUMBER_OF_THREADS 5

#define TRUE 1

// this represents work that has to be
// completed by a thread in the pool
typedef struct
{
    void (*function)(void *p);
    void *data;
} task;

// the work queue
task worktodo;

task workqueue[QUEUE_SIZE];
int front = -1;
int back = -1;
int size = 0;

// the worker bee
pthread_t bee;

// insert a task into the queue
// returns 0 if successful or 1 otherwise,
int enqueue(task t)
{
    if (size < QUEUE_SIZE)
    {
        if (back == QUEUE_SIZE - 1)
        {
            workqueue[0] = t;
            back = 0;
            ++size;
        }
        else
        {
            workqueue[back + 1] = t;
            ++size;
            ++back;
        }
    }
    else
    {
        printf("Queue is full.\n");
    }
}

// remove a task from the queue
task dequeue()
{
    if (size == 0)
    {
        printf("Queue is empty.\n");
    }
    else
    {
        worktodo = workqueue[front];
        --size;
        front = (front + 1) % QUEUE_SIZE;
    }
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    return 0;
}

// initialize the thread pool
int pool_init(void)
{
    int create_val;
    create_val = pthread_create(&bee, NULL, worker, NULL); // create_val should have the value "0" if the thread is created successfully
    if (create_val != 0)
    {
        printf("\nERROR: Failed to create thread\n");
        return 1;
    }
    else
    {
        return create_val;
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee, NULL);
}
