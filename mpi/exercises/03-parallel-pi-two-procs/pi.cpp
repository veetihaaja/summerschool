// SPDX-FileCopyrightText: 2019 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cmath>
#include <iostream>
#include <mpi.h>

constexpr int n = 840;

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);
    
    MPI_Comm world = MPI_COMM_WORLD;
    int rank, world_size;
    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &world_size);

    printf("Computing approximation to pi with N=%d\n", n);

    int istart, istop;

    if (rank==0) {
      istart = 1;
      istop = n/2;
    } else {
      istart = n/2+1;
      istop = n;
    }

    double pi = 0.0;
    for (int i=istart; i <= istop; i++) {
      double x = (i - 0.5) / n;
      pi += 1.0 / (1.0 + x*x);
    }

    if (rank == 0) {
      std::cout << "rank 0 partial sum: " << pi * 4.0/n << "\n";
    } else {
      std::cout << "rank 1 partial sum: " << pi * 4.0/n << "\n";
    }

    double piRcv;

    if (rank == 1) {
      MPI_Send(&pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
      MPI_Recv(&piRcv, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      pi += piRcv;
    }

    if (rank == 0) {
      pi *= 4.0 / n;
      printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
    }

    MPI_Finalize();
}
