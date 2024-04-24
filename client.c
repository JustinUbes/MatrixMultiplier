/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"
// #define Total_no_of_partial_product 5 // for make to work I have defined this, you should comupte this value from the dimension of the input matrices

static int Total_no_of_partial_product = 5;
static int i = 0;
static int j = 0;

typedef struct matrixData
{
    int rows;
    int columns;
} datum;

// struct data
// {
//     int row_index;
//     int col_index;
// };

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

int **populate_matrix(int rows, int columns, char *filename)
{
    // allocate mem for general matrix
    int **mat = (int **)malloc(rows * sizeof(int));
    for (int x = 0; x < rows; x++)
    {
        mat[x] = malloc(columns * sizeof(int));
    }

    // populate matrix
    FILE *f = fopen(filename, "r");
    for (int a = 0; a < rows; a++)
    {
        for (int b = 0; b < columns; b++)
        {
            if (fscanf(f, "%d ", &mat[a][b]) == 0)
            {
                printf("Error with fscanf\n");
                fclose(f);
                return -1;
            }
        }
    }
    fclose(f);
    return mat;
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

    //  create some work to do
    struct data *work = malloc(sizeof(struct data)); // Changed to allocate memory
    int x, **mat1, **mat2;
    // Changed to allocate memory
    int **mat1, **mat2, **prod, symbol;
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

    // meta = metadata, data about data, just a quick way to say rows and columns for matrix a or b
    ameta = find_rows_and_columns(afilename);
    bmeta = find_rows_and_columns(bfilename);

    Total_no_of_partial_product = ameta.rows * bmeta.columns; // Im about 90% sure this is how this works
    struct data *work = malloc(sizeof(struct data) * Total_no_of_partial_product);

    mat1 = populate_matrix(ameta.rows, ameta.columns, afilename);
    mat2 = populate_matrix(bmeta.rows, bmeta.columns, bfilename);

    work->matrix1 = mat1;
    work->matrix2 = mat2;

    work->row3 = ameta.rows; // seg fault happens here because row1 and col2 are nothing
    work->col3 = bmeta.columns;
    prod = malloc(ameta.rows * sizeof(int *));
    for (int x = 0; x < ameta.rows; x++)
    {
        prod[x] = malloc(bmeta.columns * sizeof(int));
    }
    work->product = prod;

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for (i; i < ameta.rows; i++)
    {
        for (j; j < bmeta.columns; j++)
        {
            // work[count].row_index = i;
            // work[count].column_index = j;
            pool_submit(&compute_partial_product, &work);
            // count++;
        }
    }
    // may be helpful
    // sleep(3);

    pool_shutdown();

    free(mat1);
    free(mat2);

    return 0;
}
