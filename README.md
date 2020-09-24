# compiler-tests
Tests for performance across different compilers

## Intel ICC

```
export cc_flags="-std=c99 -O3 -march=core-avx2 -qopt-streaming-stores always"
mpiicc $cc_flags scale.c -o scale.icc
mpirun -n 16 -env I_MPI_PIN_DOMAIN 8:platform ./scale.icc
```

## AMD AOCC

```
export cc_flags="-std=c99 -O3 -mavx2 -march=znver2 -fnt-store=aggressive"
MPICH_CC=clang mpicc $cc_flags scale.c -o scale.aocc
mpirun -n 16 -bind-to core:8 ./scale.aocc
```
