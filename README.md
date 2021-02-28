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
scale 1D : 22.21 GB/s, scale 2D : 21.81 GB/s, scale 3D : 21.82 GB/s
scale 1D : 22.00 GB/s, scale 2D : 21.67 GB/s, scale 3D : 21.70 GB/s
scale 1D : 22.16 GB/s, scale 2D : 21.64 GB/s, scale 3D : 21.69 GB/s
scale 1D : 22.07 GB/s, scale 2D : 21.74 GB/s, scale 3D : 21.76 GB/s
scale 1D : 22.32 GB/s, scale 2D : 21.80 GB/s, scale 3D : 21.70 GB/s
scale 1D : 22.14 GB/s, scale 2D : 21.59 GB/s, scale 3D : 21.65 GB/s
scale 1D : 22.13 GB/s, scale 2D : 21.61 GB/s, scale 3D : 21.58 GB/s
scale 1D : 21.56 GB/s, scale 2D : 21.61 GB/s, scale 3D : 23.51 GB/s
scale 1D : 22.07 GB/s, scale 2D : 21.59 GB/s, scale 3D : 21.58 GB/s
scale 1D : 21.63 GB/s, scale 2D : 21.71 GB/s, scale 3D : 23.27 GB/s
scale 1D : 21.48 GB/s, scale 2D : 21.52 GB/s, scale 3D : 23.19 GB/s
scale 1D : 21.64 GB/s, scale 2D : 21.72 GB/s, scale 3D : 23.65 GB/s
scale 1D : 21.74 GB/s, scale 2D : 21.72 GB/s, scale 3D : 23.35 GB/s
scale 1D : 21.66 GB/s, scale 2D : 21.73 GB/s, scale 3D : 23.25 GB/s
scale 1D : 21.57 GB/s, scale 2D : 21.62 GB/s, scale 3D : 23.65 GB/s
scale 1D : 21.57 GB/s, scale 2D : 21.60 GB/s, scale 3D : 23.42 GB/s
```
