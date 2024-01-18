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
#include <queue>
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
        unordered_map<Person *, set<Person *>> married;
        vector<Person *> parent;

        bool visited;

        int id;

        char state = 'w';
    };
    bool auto_save;
    Trie<Person> adjacencyList = Trie<Person>();

    vector<Person *> allPeople;

    bool isAncestor(Person const *ancestor, Person const *person);

    void findAllAncestors(Person const *person1, set<Person *> &ancestors);

    string findCommonAncestor(Person *person1, Person *person2);

    int findFurthestDescendant(Person const *person);

    void saveToFile(string const &filename);

    void loadFromFile(string const &filename);

    pair<Person *, int> dfs(Person *current);

    void addFamily(rapidjson::Value const &marriageItem);

public:
    GenealogyGraph(bool const autoSave = true);

    void insert(string const &key, Person const &value);

    void addEdge(char const *json_data);

    bool isAncestor(string const &ancestor, string const &person2);

    bool isDistantRelative(string const &person1, string const &person2);

    bool isSibling(string const &person1, string const &person2);

    string findCommonAncestor(string const &person1, string const &person2);

    int findFurthestDescendant(string const &person);

    Person *findAllAncestorsBFS(Person *person);

    // pair<Person *, Person *> findMostDistantRelationship();

    pair<string, string> findMostDistantRelationship();
};

#endif // HASHEDGENEALOGYGRAPH_H
