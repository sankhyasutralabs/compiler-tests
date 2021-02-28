#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NTIMES            10
#define STREAM_TYPE       double
#define NA                16
#define GRIDNX3D          (NA*NA)
#define GRIDNY3D          GRIDNX3D
#define GRIDNZ3D          GRIDNX3D
#define GRIDNX2D          (NA*NA*NA)
#define GRIDNY2D          GRIDNX2D
#define GRIDNX1D          (NA*NA*NA*NA*NA*NA)
#define STREAM_ARRAY_SIZE GRIDNX1D

double
wtime()
{
  struct timeval tp;
  int i = gettimeofday(&tp, NULL);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1e-6);
}

int
main(int argc, char* argv[])
{
  static STREAM_TYPE a[STREAM_ARRAY_SIZE];
  static STREAM_TYPE b[STREAM_ARRAY_SIZE];

  // initialize
  STREAM_TYPE scalar = 3.0;
  for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
  {
    a[i] = 0.0;
    b[i] = 1.0;
  }

  double start, time1 = 0, time2 = 0, time3 = 0;

  // 1D STREAM Scale kernel
  for (size_t t = 0; t < NTIMES; t++) {
    start = wtime();
    for (size_t x = 0; x < GRIDNX1D; x++)
    {
      size_t i = x;
      a[i] = scalar * b[i];
    }
    time1 += wtime() - start;
  }

  // 2D STREAM Scale kernel
  for (size_t t = 0; t < NTIMES; t++) {
    start = wtime();
    for (size_t y = 0; y < GRIDNY2D; y++)
    for (size_t x = 0; x < GRIDNX2D; x++)
    {
      size_t i = x + GRIDNX2D * y;
      a[i] = scalar * b[i];
    }
    time2 += wtime() - start;
  }

  // 3D STREAM Scale kernel
  for (size_t t = 0; t < NTIMES; t++) {
    start = wtime();
    for (size_t z = 0; z < GRIDNZ3D; z++)
    for (size_t y = 0; y < GRIDNY3D; y++)
    for (size_t x = 0; x < GRIDNX3D; x++)
    {
      size_t i = x + GRIDNX3D * y + GRIDNX3D * GRIDNY3D * z;
      a[i] = scalar * b[i];
    }
    time3 += wtime() - start;
  }

  // calculate maximum time per kernel call
  time1 /= (1. * NTIMES);
  time2 /= (1. * NTIMES);
  time3 /= (1. * NTIMES);

  // print bandwidth
  size_t nops = 2;
  double bytes = sizeof(STREAM_TYPE) * STREAM_ARRAY_SIZE;
  double bw1 = 1e-9 * nops * bytes / time1;
  double bw2 = 1e-9 * nops * bytes / time2;
  double bw3 = 1e-9 * nops * bytes / time3;
  printf("scale 1D : %.2f GB/s, scale 2D : %.2f GB/s, scale 3D : %.2f GB/s\n", bw1, bw2, bw3);

  // verify result
  size_t errors = 0;
  for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
  {
    if (a[i] != scalar) {
      printf("error: found error in results\n");
      exit(1);
    }
  }

  return 0;
}
~
