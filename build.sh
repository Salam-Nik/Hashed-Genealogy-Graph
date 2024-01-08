#!/bin/bash

g++ -c -fPIC src/sha256.cpp -o build/sha256.o

g++ -c -fPIC src/HashedGenealogyGraph.cpp -o build/HashedGenealogyGraph.o

g++ -shared -o build/libHashedGenealogyGraph.so build/HashedGenealogyGraph.o build/sha256.o

echo "Compilation completed successfully."
