#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define STREAM_TYPE       double
#define GRIDNX            3000
#define GRIDNY            GRIDNX
#define STREAM_ARRAY_SIZE (GRIDNX * GRIDNY)
#define NTIMES            10

int
main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);

  int mpi_rank, mpi_size;
  MPI_Comm mpi_comm = MPI_COMM_WORLD;
  MPI_Comm_rank(mpi_comm, &mpi_rank);
  MPI_Comm_size(mpi_comm, &mpi_size);

  static STREAM_TYPE a[STREAM_ARRAY_SIZE];
  static STREAM_TYPE b[STREAM_ARRAY_SIZE];

  // initialize
  STREAM_TYPE scalar = 3.0;
  for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
  {
    a[i] = 0.0;
    b[i] = 1.0;
  }

  double start, time1 = 0, time2 = 0;

  // 1D STREAM Scale kernel
  for (size_t t = 0; t < NTIMES; t++) {
    start = MPI_Wtime();
    for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
    {
      a[i] = scalar * b[i];
    }
    time1 += MPI_Wtime() - start;
  }

  // 2D STREAM Scale kernel
  for (size_t t = 0; t < NTIMES; t++) {
    start = MPI_Wtime();
    for (size_t y = 0; y < GRIDNY; y++)
    for (size_t x = 0; x < GRIDNX; x++)
    {
      size_t i = x + GRIDNX * y;
      a[i] = scalar * b[i];
    }
    time2 += MPI_Wtime() - start;
  }

  // calculate maximum time per kernel call
  time1 /= (1. * NTIMES);
  time2 /= (1. * NTIMES);
  double maxtime1, maxtime2;
  MPI_Reduce(&time1, &maxtime1, 1, MPI_DOUBLE, MPI_MAX, 0, mpi_comm);
  MPI_Reduce(&time2, &maxtime2, 1, MPI_DOUBLE, MPI_MAX, 0, mpi_comm);

  // print bandwidth
  size_t nops = 2;
  double bytes = sizeof(STREAM_TYPE) * STREAM_ARRAY_SIZE;
  double bw1 = 1e-9 * nops * mpi_size * bytes / maxtime1;
  double bw2 = 1e-9 * nops * mpi_size * bytes / maxtime2;
  if (0 == mpi_rank) {
    printf("scale 1D : %.2f GB/s, scale 2D : %.2f GB/s\n", bw1, bw2);
  }

  // verify result
   MPI_Barrier(mpi_comm);
  size_t errors = 0;
  for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
  {
    if (a[i] != scalar) {
      printf("error: found error in results\n");
      MPI_Abort(mpi_comm, 2);
      exit(1);
    }
  }

  return 0;
}
