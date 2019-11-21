all: matmulmpi

clean: 
	rm -f matmulmpi

matmulmpi: matmulmpi.c 
	mpicc matmulmpi.c -o matmulmpi