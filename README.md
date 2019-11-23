# MatMulMPI

MatMulMPI is a C-program implementing MPI to multiply two matrices with randomly-generated values.

## Compile

Within the MatMulMPI directory:

```bash
mpicc matmulmpi.c
```

## Run

```bash
mpirun -np processes a.out max seed size
```

### Example Run

```bash
mpirun -np 5 a.out 10 15 4
```
