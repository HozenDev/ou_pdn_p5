#!/bin/bash

make -C ./Problem_2/

cd ./Problem_2/ && mpirun -n 2 ./dotprod_MPI 262144  ../../../test_data/vec1_2^18.csv  ../../../test_data/vec2_2^18.csv result_2p_18.csv time_2p_18.txt; cd -
cd ./Problem_2/ && mpirun -n 4 ./dotprod_MPI 262144  ../../../test_data/vec1_2^18.csv  ../../../test_data/vec2_2^18.csv result_4p_18.csv time_4p_18.txt; cd -
cd ./Problem_2/ && mpirun -n 8 ./dotprod_MPI 262144  ../../../test_data/vec1_2^18.csv  ../../../test_data/vec2_2^18.csv result_8p_18.csv time_8p_18.txt; cd -

cd ./Problem_2/ && mpirun -n 2 ./dotprod_MPI 524288  ../../../test_data/vec1_2^19.csv  ../../../test_data/vec2_2^19.csv result_2p_19.csv time_2p_19.txt; cd -
cd ./Problem_2/ && mpirun -n 4 ./dotprod_MPI 524288  ../../../test_data/vec1_2^19.csv  ../../../test_data/vec2_2^19.csv result_4p_19.csv time_4p_19.txt; cd -
cd ./Problem_2/ && mpirun -n 8 ./dotprod_MPI 524288  ../../../test_data/vec1_2^19.csv  ../../../test_data/vec2_2^19.csv result_8p_19.csv time_8p_19.txt; cd -

cd ./Problem_2/ && mpirun -n 2 ./dotprod_MPI 1048576 ../../../test_data/vec1_2^20.csv  ../../../test_data/vec2_2^20.csv result_2p_20.csv time_2p_20.txt; cd -
cd ./Problem_2/ && mpirun -n 4 ./dotprod_MPI 1048576 ../../../test_data/vec1_2^20.csv  ../../../test_data/vec2_2^20.csv result_4p_20.csv time_4p_20.txt; cd -
cd ./Problem_2/ && mpirun -n 8 ./dotprod_MPI 1048576 ../../../test_data/vec1_2^20.csv  ../../../test_data/vec2_2^20.csv result_8p_20.csv time_8p_20.txt; cd -
