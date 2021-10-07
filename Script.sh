#!/bin/bash

rm Output/Output_1_Thread.csv
rm Output/Output_2_Threads.csv
rm Output/Output_4_Threads.csv
rm Output/Output_8_Threads.csv
rm Output/Output_Sequencial.csv

g++ -std=c++11 K_Means_Paralelo.cpp -o K_Means_Paralelo -fopenmp

for i in 1 2 4 8;
do
    ./K_Means_Paralelo $i
done

g++ -std=c++11 K_Means_Sequencial.cpp -o K_Means_Sequencial -fopenmp

./K_Means_Sequencial
