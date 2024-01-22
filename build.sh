#!/bin/bash

# g++ -c -g -fPIC src/sha256.cpp -o build/sha256.o
# g++ -c -g -fPIC src/Trie.cpp -o build/Trie.o
# g++ -c -g -fPIC src/GenealogyGraph.cpp -o build/GenealogyGraph.o
# g++ -g -shared -o build/GenealogyGraph.so build/GenealogyGraph.o build/Trie.o build/sha256.o
# g++ -o app app.cpp build/*.o

g++ -c src/*.cpp
g++ -o main.exe app.cpp *.o
# main.exe

echo "Compilation completed successfully."
