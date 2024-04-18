/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"
#define Total_no_of_partial_product 5 //for make to work I have defined this, you should comupte this value from the dimension of the input matrices

static int i = 0;
static int j = 0;

struct data
{
    //information needed for one thread to process 
    int row1;
    int row2;
    int col1;
    int col2;
    int row3;
    int col3;
    int** matrix1;
    int** matrix2;
    int** product;
};

void compute_partial_product(void *param)
{
    struct data *temp;
    temp = (struct data*)param;
    //define this
    
    temp->product[i][j] = 0;
    for (int k = 0; k < temp->row2; k++)
    {
        temp->product[i][j] += temp->matrix1[i][k] * temp->matrix2[k][j];
    }

}

int main(void)
{
    //read the files with input  martices an create the data
    // create some work to do
    struct data *work;
    int x, rows, columns, **mat1, **mat2, **prod;
    //dynamically allocate the array depending on the number of partial products

    // some code to read matrix 1
    
    
    work->row1 = rows;
    work->col1 = columns;
    mat1 = malloc(rows*sizeof(int*));
    for(x = 0; x<rows; x++)
    {
        mat1[x] = malloc(columns*sizeof(int));
    }
    work->matrix1 = mat1;

    // some code to read matrix 1


    work->row2 = rows;
    work->col2 = columns;
    mat2 = malloc(rows*sizeof(int*));
    for(x = 0; x<rows; x++)
    {
        mat2[x] = malloc(columns*sizeof(int));
    }
    work->matrix2 = mat2;


    work->row3 = work->col2;
    work->col3 = columns;
    prod = malloc(rows*sizeof(int*));
    for(x = 0; x<rows; x++)
    {
        prod[x] = malloc(columns*sizeof(int));
    }
    work->product = prod;


    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for(x=0;x<Total_no_of_partial_product;x++)
    {
    	pool_submit(&compute_partial_product,&work[x]);
    
    }
    // may be helpful 
    //sleep(3);

    pool_shutdown();


    free(mat1);
    free(mat2);
    free(prod);

    return 0;
}
