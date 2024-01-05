#ifndef HASHEDGENEALOGYGRAPH_H
#define HASHEDGENEALOGYGRAPH_H

#include <map>
#include <set>
#include "sha256/sha256.h" 

enum RelationType
{
    Married,
    Child,
    Parent
};

struct Person
{
    std::string hashValue;

    Person(const std::string &n, const std::string &ln, int i);
};

class HashedGenealogyGraph
{
private:
    bool auto_save;
    std::map<Person, std::vector<std::pair<Person, RelationType>>> adjacencyList;
    std::map<std::string, Person> nodeMap;

public:
    HashedGenealogyGraph(const bool autoSave = true);

    void addEdge(const Person &source, const Person &destination, RelationType relation);

    void addPerson(const Person &newPerson);

    bool isAncestor(const Person &person1, const Person &person2);

    bool isDistantRelative(const Person& person1, const Person& person2);

    bool isSibling(const Person &person1, const Person &person2);

    void findAllAncestors(const Person &person1, std::set<std::string>& ancestors1);

    std::string findCommonAncestor(const Person &person1, const Person &person2);

    int findFurthestDescendant(const Person &person);

    
};

#endif
