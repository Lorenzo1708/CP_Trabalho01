#!/bin/bash

rm Output_1_Thread.csv
rm Output_2_Threads.csv
rm Output_4_Threads.csv
rm Output_8_Threads.csv

g++ -std=c++11 K_Means.cpp -o K_Means -fopenmp

./K_Means 1
./K_Means 2
./K_Means 4
./K_Means 8
