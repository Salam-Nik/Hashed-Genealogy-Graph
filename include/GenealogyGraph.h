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

struct tuple_str_int
{
    const char *first;
    const char *second;
    int third;
};

class GenealogyGraph
{
private:
    struct Relations
    {
        std::unordered_map<std::string, std::set<std::string>> married;
        std::vector<std::string> parent;
        bool visited;
    };
    bool auto_save;
    Trie<Relations> adjacencyList;

public:
    GenealogyGraph(const bool autoSave = true);

    void insert(const std::string &key, const Relations &value);

    void addEdge(const std::string &n1, const std::string &ln1, const int &id1, const std::string &n2, const std::string &ln2, const int &id2,
                 std::vector<std::tuple<std::string, std::string, int>> children);

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

    void saveToFile(const std::string &filename);

    void loadFromFile(const std::string &filename);
};

#endif // HASHEDGENEALOGYGRAPH_H
