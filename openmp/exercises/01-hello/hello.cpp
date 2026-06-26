// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>

int main()
{
    printf("Hello world!\n");

    #pragma omp parallel
    {
        printf("Hello from thread!\n");
        printf("My tid is %d \n", omp_get_thread_num());
        printf("Total number of threads is %d \n", omp_get_num_threads());
    }

    return 0;
}
