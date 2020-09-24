# Compiler Tests
Tests for performance across different compilers

## Compilation

### Intel ICC

```
icc -std=c99 -O3 -march=core-avx2 -qopt-streaming-stores always scale.c -o scale.icc
```

### AMD AOCC

```
clang -std=c99 -O3 -mavx2 -march=znver2 -fnt-store=aggressive scale.c -o scale.aocc
```

## Run

On 2S AMD 7742, 16 processes mapped to the 16 memory channels saturate memory bandwidth.
This can be achieved by launching 16 copies of the above serial job using MPI and binding
each process to every eighth core.

Using OpenMPI
```
mpirun -n 16 --map-by numa:pe=8 ./scale.icc
mpirun -n 16 --map-by numa:pe=8 ./scale.aocc
```

## Sample Output for `scale.icc`

```
scale 1D : 23.46 GB/s, scale 2D : 21.63 GB/s
scale 1D : 24.27 GB/s, scale 2D : 21.65 GB/s
scale 1D : 23.48 GB/s, scale 2D : 21.70 GB/s
scale 1D : 23.39 GB/s, scale 2D : 21.69 GB/s
scale 1D : 21.61 GB/s, scale 2D : 25.47 GB/s
scale 1D : 22.29 GB/s, scale 2D : 21.60 GB/s
scale 1D : 22.78 GB/s, scale 2D : 21.69 GB/s
scale 1D : 21.51 GB/s, scale 2D : 26.18 GB/s
scale 1D : 22.71 GB/s, scale 2D : 21.57 GB/s
scale 1D : 21.51 GB/s, scale 2D : 25.36 GB/s
scale 1D : 21.53 GB/s, scale 2D : 25.29 GB/s
scale 1D : 22.23 GB/s, scale 2D : 21.59 GB/s
scale 1D : 21.53 GB/s, scale 2D : 24.08 GB/s
scale 1D : 21.55 GB/s, scale 2D : 24.51 GB/s
scale 1D : 21.48 GB/s, scale 2D : 24.50 GB/s
scale 1D : 21.57 GB/s, scale 2D : 23.99 GB/s
```
