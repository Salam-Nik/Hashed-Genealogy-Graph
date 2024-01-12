// HashedGenealogyGraph.h

#ifndef HASHEDGENEALOGYGRAPH_H
#define HASHEDGENEALOGYGRAPH_H

#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <unordered_map>
#include <memory>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Trie.h"
#include "sha256.h"

using namespace std;
struct tuple_str_int
{
    const char *first;
    const char *second;
    int third;
};

class GenealogyGraph
{
private:
    struct Person
    {   
        string hashValue;
        unordered_map<Person*, set<Person*>> married;
        vector<Person*> parent;

        bool visited;
    };
    bool auto_save;
    Trie<Person> adjacencyList;

public:
    GenealogyGraph(const bool autoSave = true);

    void insert(const string &key, const Person &value);

    void addEdge(const string &n1, const string &ln1, const int &id1, const string &n2, const string &ln2, const int &id2,
                 vector<tuple<string, string, int>> children);

    bool isAncestor(const Person* ancestor, const Person* person);

    bool isAncestor(const string &n1, const string &ln1, const int &id1,
                    const string &n2, const string &ln2, const int &id2);

    bool isDistantRelative(const string &n1, const string &ln1, const int &id1,
                           const string &n2, const string &ln2, const int &id2);

    bool isSibling(const string &n1, const string &ln1, const int &id1,
                   const string &n2, const string &ln2, const int &id2);

    void findAllAncestors(const Person* person1, set<Person*> &ancestors);

    string findCommonAncestor(const Person* person1, const Person* person2);

    string findCommonAncestor(const string &n1, const string &ln1, const int &id1,
                                   const string &n2, const string &ln2, const int &id2);

    int findFurthestDescendant(const string &n1, const string &ln1, const int &id1);

    int findFurthestDescendant(const Person* person);

    void saveToFile(const string &filename);

    void loadFromFile(const string &filename);
};

#endif // HASHEDGENEALOGYGRAPH_H
