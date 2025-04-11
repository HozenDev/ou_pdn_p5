#!/bin/bash
#SBATCH --partition=oucspdn_cpu
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=1
#
#SBATCH --time=00:15:00
#SBATCH --job-name=hw5prob3
#
# --------------------- Change the items below here! ---------------------
#
#SBATCH --mail-user=Enzo.B.Durel-1@ou.edu
#SBATCH --mail-type=ALL
#
#SBATCH --chdir=/home/oucspdn025/ou_pdn_p5/code/Durel_Enzo_Project_5/Problem_3
#SBATCH --output=/home/oucspdn025/ou_pdn_p5/code/Durel_Enzo_Project_5/Problem_3/results/diff_%J_stdout.txt
#SBATCH --error=/home/oucspdn025/ou_pdn_p5/code/Durel_Enzo_Project_5/Problem_3/results/diff_%J_stderr.txt

#################################################

echo "Working directory:"
pwd
echo ""

VEC_1_18_INPUT="../../test_data/Vectors/vec1_2^18.csv"
VEC_1_19_INPUT="../../test_data/Vectors/vec1_2^19.csv"
VEC_1_20_INPUT="../../test_data/Vectors/vec1_2^20.csv"

module load intel/2022.2
module load OpenMPI/4.1.4-GCC-11.3.0

make

mpirun -n 4 ./merge_sort_MPI 262144 $VEC_1_18_INPUT result_3_diff_18.csv time_3_diff_18.csv
mpirun -n 4 ./merge_sort_MPI 524288 $VEC_1_19_INPUT result_3_diff_19.csv time_3_diff_19.csv
mpirun -n 4 ./merge_sort_MPI 1048576 $VEC_1_20_INPUT result_3_diff_20.csv time_3_diff_20.csv
