/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"
#define Total_no_of_partial_product 5 //for make to work I have defined this, you should comupte this value from the dimension of the input matrices
struct data
{
    //information needed for one thread to process 
};

void compute_partial_product(void *param)
{
    struct data *temp;
    temp = (struct data*)param;
    //define this
}

int main(void)
{
    //read the files with input  martices an create the data
    // create some work to do
    struct data *work;
    int i;
    //dynamically allocate the array depending on the number of partial products

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for(i=0;i<Total_no_of_partial_product;i++)
    	pool_submit(&compute_partial_product,&work[i]);

    // may be helpful 
    //sleep(3);

    pool_shutdown();

    return 0;
}
