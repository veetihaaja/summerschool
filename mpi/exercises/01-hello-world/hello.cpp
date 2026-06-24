// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm world = MPI_COMM_WORLD;
    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &size);

    // TODO: say hello! in parallel
    std::cout << "Hello!" << std::endl;
    std::cout << "I am process with rank " << rank << "\n";
    
    char name[MPI_MAX_PROCESSOR_NAME];
    int resultlen;
    MPI_Get_processor_name(name, &resultlen);

    std::cout << "Name of my processor: " << name << "\n"; 
    
    if (rank == 0) {
        std::cout << "Total size: " << size << "\n";
    }

    if (rank == size-1) {
        std::cout << "Im the last but not least\n";
    }

    if (rank == 42) {
        std::cout << "Im the Answer to the Ultimate Question of Life, the Universe, and Everything!\n";
    }


    MPI_Finalize();
}
