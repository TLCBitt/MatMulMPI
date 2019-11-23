# MatMulMPI

MatMulMPI is a C-program implementing MPI to multiply two matrices with randomly-generated values.

## Compile

Within the MatMulMPI directory:

```bash
make
```

## Run

```bash
mpirun -np processes matmulmpi max seed size
```

### Example Run

```bash
mpirun -np 5 matmulmpi 10 15 4
```
