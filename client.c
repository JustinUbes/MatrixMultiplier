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

struct data
{
    // information needed for one thread to process
    int row1;
    int row2;
    int col1;
    int col2;
    int row_index;
    int col_index;
    int **mat1;
    int **mat2;
    int **product;
};

datum find_rows_and_columns(const char *filename)
{
    int symbol, rows = 0, columns = 0;
    datum returnStruct;
    if (filename != NULL)
    {
        FILE *f = fopen(filename, "r");
        if (f == NULL)
        {
            printf("Error opening file.\n");
            return returnStruct;
        }
        do
        {
            symbol = fgetc(f);

            if (rows == 0 && symbol != '\n' && symbol != EOF)
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
        returnStruct.rows = rows;
        returnStruct.columns = columns;
        fclose(f);
        return returnStruct;
    }
    else
    {
        perror("ERROR: Could not open the file correctly");
    }
}

// int **populate_matrix(int rows, int columns, char *filename)
// {
//     // allocate mem for general matrix
//     int **mat = (int **)malloc(rows * sizeof(int *));
//     for (int x = 0; x < rows; x++)
//     {
//         mat[x] = malloc(columns * sizeof(int));
//     }

//     // populate matrix
//     FILE *f = fopen(filename, "r");
//     for (int a = 0; a < rows; a++)
//     {
//         for (int b = 0; b < columns; b++)
//         {
//             if (fscanf(f, "%d ", &mat[a][b]) > 1)
//             {
//                 printf("Error with fscanf\n");
//                 fclose(f);
//                 return -1;
//             }
//         }
//     }
//     fclose(f);
//     return mat;
// }

void compute_partial_product(void *param)
{
    struct data *temp;
    temp = (struct data *)param;
    // define this

    int jawn = 0; // I just set the product matrix = 0, dont need to do it here anymore
    for (int k = 0; k < temp->row2; k++)
    {
        jawn += temp->mat1[temp->row_index][k] * temp->mat2[k][temp->col_index];
    }
    temp->product[temp->row_index][temp->col_index] = jawn;
    printf("%d\n", temp->product[temp->row_index][temp->col_index]);
}

int main(int argc, char *argv[])
{

    //  create some work to do
    int **m1, **m2, **p;
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

    printf("Matrix A: rows=%d, columns=%d\n", ameta.rows, ameta.columns);
    printf("Matrix B: rows=%d, columns=%d\n", bmeta.rows, bmeta.columns);
    // a printf statement here will not execute, but the previous two do.
    struct data *work = (struct data *)malloc(sizeof(struct data));

    work->mat1 = (int **)malloc(sizeof(int) * ameta.rows);
    for (int l = 0; l < ameta.rows; ++l)
    {
        work->mat1[l] = (int *)malloc(sizeof(int) * ameta.columns);
    }

    work->mat2 = (int **)malloc(sizeof(int) * bmeta.rows);
    for (int l = 0; l < bmeta.rows; ++l)
    {
        work->mat2[l] = (int *)malloc(sizeof(int) * bmeta.columns);
    }

    work->product = (int **)malloc(sizeof(int) * ameta.rows);
    for (int l = 0; l < ameta.rows; ++l)
    {
        work->product[l] = (int *)malloc(bmeta.columns * sizeof(int));
    }

    // m1 = populate_matrix(ameta.rows, ameta.columns, afilename);
    // m2 = populate_matrix(bmeta.rows, bmeta.columns, bfilename);
    m1 = (int **)malloc(ameta.rows * sizeof(int *));
    for (int a = 0; a < ameta.rows; ++a)
    {
        m1[a] = (int *)malloc(ameta.columns * sizeof(int));
    }

    m2 = (int **)malloc(bmeta.rows * sizeof(int *));
    for (int a = 0; a < bmeta.rows; a++)
    {
        m2[a] = (int *)malloc(bmeta.columns * sizeof(int));
    }
    // populate matrix a
    FILE *f = fopen(afilename, "r");
    for (int a = 0; a < ameta.rows; a++)
    {
        for (int b = 0; b < ameta.columns; b++)
        {
            // printf("%d\n", m1[a][b]);
            if (fscanf(f, "%d ", &m1[a][b]) > 1)
            {
                printf("Error with fscanf\n");
                fclose(f);
                return -1;
            }
        }
    }
    fclose(f);

    // populate matrix b
    FILE *g = fopen(bfilename, "r");
    for (int a = 0; a < bmeta.rows; a++)
    {
        for (int b = 0; b < bmeta.columns; b++)
        {
            if (fscanf(g, "%d ", &m2[a][b]) > 1)
            {
                printf("Error with fscanf\n");
                fclose(g);
                return -1;
            }
        }
    }
    fclose(f);

    for (int a = 0; a < ameta.rows; ++a)
    {
        for (int b = 0; b < ameta.columns; ++b)
        {
            work->mat1[a][b] = m1[a][b];
        }
    }

    for (int a = 0; a < bmeta.rows; ++a)
    {
        for (int b = 0; b < bmeta.columns; ++b)
        {
            work->mat2[a][b] = m2[a][b];
        }
    }

    for (int a = 0; a < bmeta.rows; ++a)
    {
        for (int b = 0; b < bmeta.columns; ++b)
        {
            work->product[a][b] = 0;
        }
    }

    // int matrix1[3][4] = {
    //     {1, 4, 20, 2},
    //     {3, 6, 10, 1},
    //     {5, 10, 15, 13}};

    // int matrix2[4][3] = {
    //     {1, 2, 3},
    //     {4, 5, 6},
    //     {7, 8, 9},
    //     {3, 5, 7}};
    // struct data *work = (struct data *)malloc(sizeof(struct data));

    // for (int a = 0; a < 3; a++)
    // {
    //     for (int b = 0; b < 4; b++)
    //     {
    //         work->mat1[a][b] = matrix1[a][b];
    //     }
    // }

    // for (int c = 0; c < 3; c++)
    // {
    //     for (int d = 0; d < 4; d++)
    //     {
    //         work->mat2[c][d] = matrix1[c][d];
    //     }
    // }
    // // multiplying matrices yields matrix a row by matrix b col matrix
    // int matrix3[3][3] = {
    //     {0, 0, 0},
    //     {0, 0, 0},
    //     {0, 0, 0}};
    // for (int g = 0; g < 3; g++)
    // {
    //     for (int h = 0; h < 3; h++)
    //     {
    //         work->product[g][h] = matrix3[g][h];
    //     }
    // }
    work->row1 = ameta.rows;
    work->col1 = ameta.columns;
    work->row2 = bmeta.rows;
    work->col2 = bmeta.columns;
    work->row_index = ameta.rows;
    work->col_index = bmeta.columns;
    pool_init();
    for (int e = 0; e < work->row1; e++)
    {
        for (int f = 0; f < work->col2; f++)
        {
            work->row_index = e;
            work->col_index = f;
            pool_submit(&compute_partial_product, work);
        }
    }

    // initialize the thread pool

    // // submit the work to the queue
    // for (i; i < ameta.rows; i++)
    // {
    //     for (j; j < bmeta.columns; j++)
    //     {
    //         // work[count].row_index = i;
    //         // work[count].column_index = j;
    //         pool_submit(&compute_partial_product, &work);
    //         // count++;
    //     }
    // }
    // // may be helpful
    // // sleep(3);

    pool_shutdown();
    // for (int w = 0; w < 3; w++)
    // {
    //     for (int v = 0; v < 3; v++)
    //     {
    //         printf("%d ", work->product[w][v]);
    //     }
    //     printf("\n");
    // }

    // free(mat1);
    // free(mat2);
    // free(prod);

    return 0;
}
