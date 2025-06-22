#!/bin/bash

mkdir -p build || exit 1
cd build || exit 1

echo "Compiling and running client"
if g++ ../src/client.cpp -o client -lboost_system -pthread -O3 -Wall; then
  ./server
else
  echo "Compilation failed!" >&2
  exit 1
fi
