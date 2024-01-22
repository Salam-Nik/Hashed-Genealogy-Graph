#!/bin/bash

for file in src/*.cpp; do
  g++ -c "$file" -o "build/$(basename "$file" .cpp).o"
done

g++ -o main.exe app.cpp build/*.o

echo "Compilation completed successfully."
