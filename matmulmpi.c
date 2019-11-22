#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{
    int max, seed, size;
    int my_rank, world_size, workers;
    int rows, rowsDiv, rowsEx;
    int balance;
    int target, origin;
    int i, j, k;
    MPI_Status status;

    if (argc != 4)
    {
        printf("Please include the max, seed, and size values in the parameters.\n");
        printf("Exiting.\n");
        return 0;
    }

    max = atoi(argv[1]);
    seed = atoi(argv[2]);
    size = atoi(argv[3]);

    srand(seed);

    //int **matrixOne = (int **) malloc(sizeof(int *) * size);
    //int **matrixTwo = (int **) malloc(sizeof(int *) * size);
    //int **matrixThree = (int **) malloc(sizeof(int *) * size);

    int matrixOne[size][size];
    int matrixTwo[size][size];
    int matrixThree[size][size];

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    workers = world_size - 1;

    if (my_rank == 0)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                matrixOne[i][j] = rand() % (max + 1);
            }
        }

        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                matrixTwo[i][j] = rand() % (max + 1);
            }
        }

        rowsDiv = size / workers;   // To divide work among rows
        rowsEx = size % workers;    // Excess row work
        balance = 0;

        for (target = 1; target <= workers; target++)
        {
            // Divide work amongst rows
            if(target <= rowsEx)
            {
                rows = rowsDiv + 1;
            }
            else
            {
                rows = rowsDiv;
            }

            MPI_Send(&balance, 1, MPI_INT, target, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, target, 1, MPI_COMM_WORLD);
            MPI_Send(&matrixOne[balance][0], rows * size, MPI_INT, target, 1, MPI_COMM_WORLD);
            MPI_Send(&matrixTwo, size * size, MPI_INT, target, 1, MPI_COMM_WORLD);
            balance = balance + rows;
        }

        for (i = 1; i <= workers; i++)
        {
            origin = i;
            MPI_Recv(&balance, 1, MPI_INT, origin, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, origin, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&matrixThree[balance][0], rows * size, MPI_INT, origin, 2, MPI_COMM_WORLD, &status);

        }

        //print matrixOne
        printf("Matrix One: \n");
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%4d ", matrixOne[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        //print matrixTwo
        printf("Matrix Two: \n");
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%4d ", matrixTwo[i][j]);
            }
            printf("\n");
        }
        printf("\n");


        // print matrixThree
        printf("Multiplied Matrix Result: \n");
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%4d ", matrixThree[i][j]);
            }
            printf("\n");
        }
    }

    if (my_rank > 0)
    {
        MPI_Recv(&balance, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixOne, rows * size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixTwo, size * size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        for (k = 0; k < size; k++)
        {
            for (i = 0; i < rows; i++)
            {
                matrixThree[i][k] = 0;
                for (j = 0; j < size; j++)
                {
                    matrixThree[i][k] = matrixThree[i][k] + matrixOne[i][j] * matrixTwo[j][k];
                }
            }
        }

        MPI_Send(&balance, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&matrixThree, rows * size, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
