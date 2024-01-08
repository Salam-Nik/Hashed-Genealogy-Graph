#pragma once
#include <map>
#include <set>
#include <fstream>
#include "sha256.h" 
#include <algorithm>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

enum RelationType
{
    Married,
    Child,
    Parent
};

class HashedGenealogyGraph
{

private:
    bool auto_save;
    map<string, vector<pair<string, RelationType>>> adjacencyList;
    map<string, string> nodeMap;

public:
    HashedGenealogyGraph(const bool autoSave = true);

    void addEdge(const string &n1, const string &ln1, const int &id1,
                 const string &n2, const string &ln2, const int &id2, RelationType relation);

    bool isAncestor(const string &person1, const string &person2);

    bool isAncestor(const string &n1, const string &ln1, const int &id1,
                    const string &n2, const string &ln2, const int &id2);

    bool isDistantRelative(const string &n1, const string &ln1, const int &id1,
                           const string &n2, const string &ln2, const int &id2);

    bool isSibling(const string &n1, const string &ln1, const int &id1,
                   const string &n2, const string &ln2, const int &id2);

    void findAllAncestors(const string &person1, set<string> &ancestors1);

    string findCommonAncestor(const string &person1, const string &person2);

    string findCommonAncestor(const string &n1, const string &ln1, const int &id1,
                                   const string &n2, const string &ln2, const int &id2);

    int findFurthestDescendant(const string &n1, const string &ln1, const int &id1);

    int findFurthestDescendant(const string &person);

    void saveToFile(const string &filename) const;

    void loadFromFile(const string &filename);
};

#ifdef __cplusplus
}
#endif