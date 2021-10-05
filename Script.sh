#!/bin/bash

echo "Starting RM"

rm Output_1_Thread.csv
rm Output_2_Threads.csv
rm Output_4_Threads.csv
rm Output_8_Threads.csv

echo "Starting G++"

g++ -std=c++11 K_Means.cpp -o K_Means -fopenmp

echo "Starting ./"

./K_Means
