#!/bin/bash

g++ -c -fPIC src/sha256.cpp -o build/sha256.o

g++ -c -fPIC src/Trie.cpp -o build/Trie.o

g++ -c -fPIC src/GenealogyGraph.cpp -o build/GenealogyGraph.o

g++ -shared -o build/GenealogyGraph.so build/GenealogyGraph.o build/Trie.o build/sha256.o

echo "Compilation completed successfully."
