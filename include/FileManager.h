#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <fstream>
#include "../include/GenealogyGraph.h"  

class FileManager {
public:

    void saveToFile(const string& filename,  Trie<Person> adjacencyList);
    void loadFromFile(const string& filename, vector<Person *> allPeople,  Trie<Person> adjacencyList);
};

#endif // FILE_MANAGER_H
