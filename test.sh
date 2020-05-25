#!/bin/sh
SBATCH -n 64

mpicc -o merge merge.c
mpiexec -n 1 merge 500000
mpiexec -n 2 merge 5000000
mpiexec -n 4 merge 500000
mpiexec -n 8 merge 500000
mpiexec -n 16 merge 500000
mpiexec -n 32 merge 500000
mpiexec -n 64 merge 500000

mpiexec -n 1 merge 10000000
mpiexec -n 2 merge 10000000
mpiexec -n 4 merge 10000000
mpiexec -n 8 merge 10000000
mpiexec -n 16 merge 10000000
mpiexec -n 32 merge 10000000
mpiexec -n 64 merge 10000000

mpiexec -n 1 merge 70000000
mpiexec -n 2 merge 70000000
mpiexec -n 4 merge 70000000
mpiexec -n 8 merge 70000000
mpiexec -n 16 merge 70000000
mpiexec -n 32 merge 70000000
mpiexec -n 64 merge 70000000