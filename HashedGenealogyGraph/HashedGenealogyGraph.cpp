#include "HashedGenealogyGraph.h"
#include <algorithm>


Person::Person(const std::string &n, const std::string &ln, int i)
{
    std::string input = n + ln + std::to_string(i);
    hashValue = SHA256::getHashString(input);
}

HashedGenealogyGraph::HashedGenealogyGraph(const bool autoSave) : auto_save(autoSave) {}

void HashedGenealogyGraph::addEdge(const Person &source, const Person &destination, RelationType relation)
{
    adjacencyList[source].emplace_back(destination, relation);

    if (relation == Married)
        adjacencyList[destination].emplace_back(source, relation);
    else if (relation == Child)
        adjacencyList[destination].emplace_back(source, Parent);
    else
        adjacencyList[destination].emplace_back(source, Child);
}

void HashedGenealogyGraph::addPerson(const Person &newPerson)
{
    if (adjacencyList.find(newPerson) == adjacencyList.end())
    {
        adjacencyList[newPerson] = std::vector<std::pair<Person, RelationType>>();
        nodeMap[newPerson.hashValue] = newPerson;
    }
}

bool HashedGenealogyGraph::isAncestor(const Person &person1, const Person &person2)
{
    for (const auto &pair : adjacencyList[person2])
    {
        if (pair.second == Parent && pair.first.hashValue == person1.hashValue)
        {
            return true;
        }
    }
    return false;
}

bool HashedGenealogyGraph::isSibling(const Person &person1, const Person &person2)
{
    for (const auto &pair : adjacencyList[person1])
    {
        if (pair.second == Parent)
        {
            for (const auto &sibling : adjacencyList[pair.first])
            {
                if (sibling.first.hashValue == person2.hashValue && sibling.second == Parent)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

std::string HashedGenealogyGraph::findCommonAncestor(const Person &person1, const Person &person2)
{
    std::set<std::string> ancestors1;

    for (const auto &pair : adjacencyList[person1])
    {
        if (pair.second == Parent)
        {
            ancestors1.insert(pair.first.hashValue);
        }
    }

    for (const auto &pair : adjacencyList[person2])
    {
        if (pair.second == Parent && ancestors1.count(pair.first.hashValue))
        {
            return pair.first.hashValue;
        }
    }

    return ""; // No common ancestor found
}

int HashedGenealogyGraph::findFurthestDescendant(const Person &person)
{
    int maxDistance = 0;

    for (const auto &pair : adjacencyList[person])
    {
        if (pair.second == Child)
        {
            int distance = findFurthestDescendant(pair.first) + 1;
            maxDistance = std::max(maxDistance, distance);
        }
    }

    return maxDistance;
}
