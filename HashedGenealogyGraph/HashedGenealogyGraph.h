#pragma once
#include <map>
#include <set>
#include <fstream>
#include "SHA256.h" 
#include <algorithm>


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
    std::map<std::string, std::vector<std::pair<std::string, RelationType>>> adjacencyList;
    std::map<std::string, std::string> nodeMap;
    

public:
    HashedGenealogyGraph(const bool autoSave = true);

    void addEdge(const std::string &n1, const std::string &ln1, const int &id1,
                 const std::string &n2, const std::string &ln2, const int &id2, RelationType relation);

    bool isAncestor(const std::string &person1, const std::string &person2);

    bool isAncestor(const std::string &n1, const std::string &ln1, const int &id1,
                    const std::string &n2, const std::string &ln2, const int &id2);

    bool isDistantRelative(const std::string &n1, const std::string &ln1, const int &id1,
                           const std::string &n2, const std::string &ln2, const int &id2);

    bool isSibling(const std::string &n1, const std::string &ln1, const int &id1,
                   const std::string &n2, const std::string &ln2, const int &id2);

    void findAllAncestors(const std::string &person1, std::set<std::string> &ancestors1);

    std::string findCommonAncestor(const std::string &person1, const std::string &person2);

    std::string findCommonAncestor(const std::string &n1, const std::string &ln1, const int &id1,
                                   const std::string &n2, const std::string &ln2, const int &id2);

    int findFurthestDescendant(const std::string &n1, const std::string &ln1, const int &id1);

    int findFurthestDescendant(const std::string &person);

    void saveToFile(const std::string &filename) const;

    void loadFromFile(const std::string &filename);
};

