#!/bin/bash

rm Output/Output_1_Thread.csv
rm Output/Output_2_Threads.csv
rm Output/Output_4_Threads.csv
rm Output/Output_8_Threads.csv

g++ -std=c++11 K_Means.cpp -o K_Means -fopenmp

for i in 1 2 4 8;
do
    ./K_Means $i
done
