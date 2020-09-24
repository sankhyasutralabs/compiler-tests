#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define STREAM_TYPE       double
#define GRIDNX            3000
#define GRIDNY            GRIDNX
#define STREAM_ARRAY_SIZE (GRIDNX * GRIDNY)
#define NTIMES            10

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

  double start, time1 = 0, time2 = 0;

  // 1D STREAM Scale kernel
  for (size_t t = 0; t < NTIMES; t++) {
    start = wtime();
    for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
    {
      a[i] = scalar * b[i];
    }
    time1 += wtime() - start;
  }

  // 2D STREAM Scale kernel
  for (size_t t = 0; t < NTIMES; t++) {
    start = wtime();
    for (size_t y = 0; y < GRIDNY; y++)
    for (size_t x = 0; x < GRIDNX; x++)
    {
      size_t i = x + GRIDNX * y;
      a[i] = scalar * b[i];
    }
    time2 += wtime() - start;
  }

  // calculate maximum time per kernel call
  time1 /= (1. * NTIMES);
  time2 /= (1. * NTIMES);

  // print bandwidth
  size_t nops = 2;
  double bytes = sizeof(STREAM_TYPE) * STREAM_ARRAY_SIZE;
  double bw1 = 1e-9 * nops * bytes / time1;
  double bw2 = 1e-9 * nops * bytes / time2;
  printf("scale 1D : %.2f GB/s, scale 2D : %.2f GB/s\n", bw1, bw2);

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
