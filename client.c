/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"
#define Total_no_of_partial_product 5 // for make to work I have defined this, you should comupte this value from the dimension of the input matrices

static int i = 0;
static int j = 0;

typedef struct matrixData
{
    int rows;
    int columns;
} datum;

struct data
{
    // information needed for one thread to process
    int row1;
    int row2;
    int col1;
    int col2;
    int row3;
    int col3;
    int **matrix1;
    int **matrix2;
    int **product;
};

datum find_rows_and_columns(const char *filename)
{
    int symbol, rows = 0, columns = 0;
    if (filename != NULL)
    {
        FILE *f = fopen(filename, "r");
        do
        {
            symbol = fgetc(f);

            if (rows == 0 && symbol != "\n" && symbol != EOF)
            {
                columns++;
            }
            if (symbol == '\n' || feof(f))
            {
                rows++;
            }
        } while (symbol != EOF);

        if (ferror(f))
            printf("ERROR: Could not read the file correctly.\n");
        else
            printf("\nThis matrix has...\nNumber of rows: %d\nNumber of columns: %d\n", rows, columns);
        fclose(f);
        datum returnStruct;
        returnStruct.rows = rows;
        returnStruct.columns = columns;
        return returnStruct;
    }
    else
    {
        perror("ERROR: Could not open the file correctly");
    }
}

void compute_partial_product(void *param)
{
    struct data *temp;
    temp = (struct data *)param;
    // define this

    temp->product[i][j] = 0;
    for (int k = 0; k < temp->row2; k++)
    {
        temp->product[i][j] += temp->matrix1[i][k] * temp->matrix2[k][j];
    }
}

int main(int argc, char *argv[])
{

    // read the files with input  martices an create the data
    //  create some work to do
    struct data *work;
    int i, x, rows, columns, **mat1, **mat2, **prod, symbol;
    char *afilename, *bfilename;
    datum ameta, bmeta;

    if (argc != 3)
    {
        printf("You must add two file names.\n");
        return 1;
    }
    // We set a and b filename to the files we got from the command line
    afilename = argv[1];
    bfilename = argv[2];

    // open file A

    // finds the value of rows and columns for Matrix A
    // meta = metadata, data about data, just a quick way to say rows and columns for matrix a or b
    ameta = find_rows_and_columns(afilename);
    bmeta = find_rows_and_columns(bfilename);
    // dynamically allocate the array depending on the number of rows and columns for Matrix A
    // dynamically allocate the array depending on the number of partial products

    // some code to read matrix 1

    work->row1 = ameta.rows;
    work->col1 = ameta.columns;
    mat1 = malloc(ameta.rows * sizeof(int *));
    for (x = 0; x < ameta.rows; x++)
    {
        mat1[x] = malloc(ameta.columns * sizeof(int));
    }
    work->matrix1 = mat1;

    // dynamically allocate the array depending on the number of rows and columns for Matrix B
    work->row2 = bmeta.rows;
    work->col2 = bmeta.columns;
    mat2 = malloc(bmeta.rows * sizeof(int *));
    for (x = 0; x < bmeta.rows; x++)
    {
        mat2[x] = malloc(bmeta.columns * sizeof(int));
    }
    work->matrix2 = mat2;

    work->row3 = work->row1;
    work->col3 = work->col2;
    prod = malloc(rows * sizeof(int *));
    for (x = 0; x < rows; x++)
    {
        prod[x] = malloc(columns * sizeof(int));
    }
    work->product = prod;

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for (x = 0; x < Total_no_of_partial_product; x++)
    {
        pool_submit(&compute_partial_product, &work[x]);
    }
    // may be helpful
    // sleep(3);

    pool_shutdown();

    free(mat1);
    free(mat2);
    free(prod);

    return 0;
}
