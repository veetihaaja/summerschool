/*
 * SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank;
    int array[8][8];

    // Declare a variable storing the MPI datatype
    // TODO

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize arrays
    if (rank == 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                array[i][j] = (i + 1) * 10 + j + 1;
            }
        }
    } else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                array[i][j] = 0;
            }
        }
    }

    // Print data on rank 0
    if (rank == 0) {
        printf("Data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    // Create datatype
    // TODO

    // MPI_Datatype column;
    // MPI_Type_vector(8, 1, 8, MPI_INT, &column);
    // MPI_Type_commit(&column);

    // // Send data from rank 0 to rank 1
    // // TODO

    // if (rank == 0) {
    //     MPI_Send(&(array[0][1]), 1, column, 1, 0, MPI_COMM_WORLD);
    // } else {
    //     MPI_Recv(&(array[0][1]), 1, column, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }

    // // Free datatype
    // MPI_Type_free(&column);

    MPI_Datatype colV;
    const int displ[4] = {0, 2*8+1, 4*8+2, 6*8+3};
    const int bSizes[4] = {1, 2, 3, 4};
    MPI_Type_indexed(4, bSizes, displ, MPI_INT, &colV);
    MPI_Type_commit(&colV);

    if (rank == 0) {
        MPI_Send(&(array[0][0]), 1, colV, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&(array[0][0]), 1, colV, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // // Free datatype
    MPI_Type_free(&colV);


    // Print received data
    if (rank == 1) {
        printf("Received data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}
