#!/bin/sh

#SBATCH -p short

#SBATCH -n64

#SBATCH -C alpha

mpicc -o connectivity connectivity.c

mpirun connectivity -v