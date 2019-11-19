#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

// num rows and columns
#define MAX 4

int matrixOne[MAX][MAX];
int matrixTwo[MAX][MAX];
int matrixThree[MAX][MAX];


int main()
{

    MPI_Status status;

    int my_rank;
    int world_size;
    int workers;
    int dest, source, rows, offset;
    int i, j, k;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    workers = world_size -1;

    if(my_rank == 0)
    {
        for(i = 0; i < MAX; i++)
            for(j = 0; j < MAX; j++)
                matrixOne[i][j] = i + j;
        for(i = 0; i < MAX; i++)
            for(j = 0; j < MAX; j++)
                matrixTwo[i][j] = i * j;

    //rows = MAX/workers;
    rows = MAX;
    offset=0;
  for(dest = 1; dest <=  workers; dest++)
    {
        MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&matrixOne[offset][0], rows*MAX, MPI_INT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&matrixTwo, MAX*MAX, MPI_INT, dest, 1, MPI_COMM_WORLD);
        offset = offset + rows;
    }

    for(i = 1; i <=  workers; i++)
    {
        source = i;
        MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixThree[offset][0], rows*MAX, MPI_INT, source, 2, MPI_COMM_WORLD, &status);

    }



    // print matrixThree
    printf("Multiplied Matrix Result: \n");
    for(i = 0; i < MAX; i++)
    {
        for(j = 0; j < MAX; j++)
        {
            printf("%d ", matrixThree[i][j]);
        }
        printf("\n");
    }
    }

 if(my_rank > 0)
    {

        MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixOne, rows*MAX, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixTwo, MAX*MAX, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        for(k = 0; k < MAX; k++)
            for(i = 0; i < rows; i++)
            {
                matrixThree[i][k] = 0;
                for(j = 0; j < MAX; j++)
                {
                    matrixThree[i][k] = matrixThree[i][k] + matrixOne[i][j] * matrixTwo[j][k];
                }
            }

        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&matrixThree, rows*MAX, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }


    MPI_Finalize();
}
