// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

// Main solver routines for heat equation solver

#include <mpi.h>
#include <vector>

#include "heat.hpp"

// Exchange the boundary values
std::vector<MPI_Request> exchange(Field& field, const ParallelData parallel)
{

    double* sbuf;
    double* rbuf;

    // TODO start: implement halo exchange

    // You can utilize the data() method of the Matrix class to obtain pointer
    // to element, e.g. field.temperature.data(i, j)

    // // Send to up, receive from down
    // sbuf = field.temperature.data(1, 0);
    // rbuf = field.temperature.data(field.nx+1, 0);

    // MPI_Send(sbuf, field.ny+2, MPI_DOUBLE, parallel.nup, 0, MPI_COMM_WORLD);
    // MPI_Recv(rbuf, field.ny+2, MPI_DOUBLE, parallel.ndown, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    // // Send to down, receive from up
    // rbuf = field.temperature.data(0, 0);
    // sbuf = field.temperature.data(field.nx, 0);

    // MPI_Send(sbuf, field.ny+2, MPI_DOUBLE, parallel.ndown, 0, MPI_COMM_WORLD);
    // MPI_Recv(rbuf, field.ny+2, MPI_DOUBLE, parallel.nup, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // // TODO end

    // sbuf = field.temperature.data(1, 0);
    // rbuf = field.temperature.data(field.nx+1, 0);

    // MPI_Sendrecv(sbuf, field.ny+2, MPI_DOUBLE, parallel.nup, 0, 
    //              rbuf, field.ny+2, MPI_DOUBLE, parallel.ndown, 0,
    //              MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // rbuf = field.temperature.data(0, 0);
    // sbuf = field.temperature.data(field.nx, 0);

    // MPI_Sendrecv(sbuf, field.ny+2, MPI_DOUBLE, parallel.ndown, 0, 
    //              rbuf, field.ny+2, MPI_DOUBLE, parallel.nup, 0,
    //              MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Send to up, receive from down
    MPI_Request sendToUp, sendToDown, ReceiveFromUp, receiveFromDown;
    sbuf = field.temperature.data(1, 0);
    rbuf = field.temperature.data(field.nx+1, 0);    
    MPI_Irecv(rbuf, field.ny+2, MPI_DOUBLE, parallel.ndown, 0, MPI_COMM_WORLD, &receiveFromDown);
    MPI_Isend(sbuf, field.ny+2, MPI_DOUBLE, parallel.nup, 0, MPI_COMM_WORLD, &sendToUp);

    // Send to down, receive from up
    rbuf = field.temperature.data(0, 0);
    sbuf = field.temperature.data(field.nx, 0);
    MPI_Irecv(rbuf, field.ny+2, MPI_DOUBLE, parallel.nup, 0, MPI_COMM_WORLD, &ReceiveFromUp);
    MPI_Isend(sbuf, field.ny+2, MPI_DOUBLE, parallel.ndown, 0, MPI_COMM_WORLD, &sendToDown);

    std::vector<MPI_Request> returnVec = {receiveFromDown, ReceiveFromUp};

    return returnVec;
}

// Update the temperature values using five-point stencil */
void evolve(Field& curr, const Field& prev, const double a, const double dt, bool edges)
{

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.

  if (!edges) { // calculate middle and left and right, dont need mpi comm to finish
    for (int i = 2; i < curr.nx; i++) {
      for (int j = 1; j < curr.ny + 1 ; j++) {
              curr(i, j) = prev(i, j) + a * dt * (
          ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
          ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
                );
      }
    }
  } else { 
    int edgesVals[2] = {1, curr.nx};
    for (int i : edgesVals) {
      for (int j = 1; j < curr.ny + 1; j++) {

          curr(i, j) = prev(i, j) + a * dt * (
          ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
          ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
                );
      }
    }
  }
}
