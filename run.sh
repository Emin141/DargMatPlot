#!/bin/bash

g++ generator.cpp -o generator ;
./generator test.csv ;
g++ main.cpp -g -std=c++17 -o main ;
./main test.csv ;