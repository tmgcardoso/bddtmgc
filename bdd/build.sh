#!/bin/bash

# you must run this script within 'bdd' as current work directory
mkdir ./build &&
cd ./build &&
cmake ../ &&
make
