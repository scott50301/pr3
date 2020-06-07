#!/bin/bash
#
#SBATCH --job-name=sort
#SBATCH --nodes=3
#SBATCH --partition=gpu
#SBATCH --cpus-per-task=1
#SBATCH --ntasks=84
#SBATCH --mem-per-cpu=16G
#SBATCH --output=sort-%j.out 
#SBATCH --time=01:00:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=sjhou@scu.edu #

current_time=$(date "+%Y.%m.%d-%H.%M.%S")
log_file_name=sort-$current_time.log
file_path=/WAVE/users/unix/sjhou/pr3

./mergesort 10000000