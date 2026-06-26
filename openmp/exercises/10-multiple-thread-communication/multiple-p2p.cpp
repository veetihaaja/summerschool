// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <vector>
#include <omp.h>


int main(int argc, char *argv[])
{
    int rank, ntasks;

    int provided, required=MPI_THREAD_MULTIPLE;
    MPI_Init_thread(&argc, &argv, required, &provided);

    if (provided < required) {
        printf("Womp womp\n");
        return 1;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    std::vector<MPI_Comm> mpi_comm_thread(omp_get_max_threads());
    for (auto& comm : mpi_comm_thread) {
        MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    }

    int tid, msg, tag=123;

    #pragma omp parallel
    {
    if (rank == 0) {
            tid = omp_get_thread_num();
            msg = tid;
            for (int i = 1; i < ntasks; i++) {
                MPI_Send(&msg, 1, MPI_INT, i, tag, mpi_comm_thread.at(omp_get_thread_num()));
            }
        } else {
            tid = omp_get_thread_num();
            MPI_Recv(&msg, 1, MPI_INT, 0, tag, mpi_comm_thread.at(omp_get_thread_num()), MPI_STATUS_IGNORE);
            printf("Rank %d thread %d received %d\n", rank, tid, msg);
        }
    }

    // Free communicators  
    for (auto& comm : mpi_comm_thread) {
        MPI_Comm_free(&comm);
    }

    MPI_Finalize();
    return 0;
}
