#!/bin/bash

make -C ./Problem_1/

echo "1M" 
cd ./Problem_1/ && mpirun -n 2 ./pingpong_MPI 1000000 time_1M_diff.csv; cd -

echo "2M" 
cd ./Problem_1/ && mpirun -n 2 ./pingpong_MPI 2000000 time_3M_diff.csv; cd -

echo "4M" 
cd ./Problem_1/ && mpirun -n 2 ./pingpong_MPI 4000000 time_4M_diff.csv; cd -

echo "8M" 
cd ./Problem_1/ && mpirun -n 2 ./pingpong_MPI 8000000 time_8M_diff.csv; cd -