#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include "HashedGenealogyGraph.h"

namespace py = pybind11;


void init_genealogy_graph(pybind11::module& m) {
    py::class_<HashedGenealogyGraph>(m, "HashedGenealogyGraph")
        .def(py::init<bool>())
        .def("addEdge", &HashedGenealogyGraph::addEdge)
        .def("isAncestor", py::overload_cast<const std::string&, const std::string&>(&HashedGenealogyGraph::isAncestor))
        .def("isAncestor", py::overload_cast<const std::string&, const std::string&, const int&,
                                             const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::isAncestor))
        .def("findCommonAncestor", py::overload_cast<const std::string&, const std::string&>(&HashedGenealogyGraph::findCommonAncestor))
        .def("findCommonAncestor", py::overload_cast<const std::string&, const std::string&, const int&,
                                                   const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::findCommonAncestor))
        .def("isSibling", py::overload_cast<const std::string&, const std::string&, const int&,
                                            const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::isSibling))
        .def("isDistantRelative", py::overload_cast<const std::string&, const std::string&, const int&,
                                                  const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::isDistantRelative))
        .def("findAllAncestors", &HashedGenealogyGraph::findAllAncestors)
        .def("findFurthestDescendant", py::overload_cast<const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::findFurthestDescendant))
        .def("findFurthestDescendant", py::overload_cast<const std::string&>(&HashedGenealogyGraph::findFurthestDescendant));
}


PYBIND11_MODULE(HashedGenealogyGraph, m) {
    init_genealogy_graph(m);
}

HashedGenealogyGraph::HashedGenealogyGraph(const bool autoSave) : auto_save(autoSave)
{
    if (auto_save)
        loadFromFile("genealogy_save.txt");
}

void HashedGenealogyGraph::addEdge(const std::string &n1, const std::string &ln1, const int &id1,
                                   const std::string &n2, const std::string &ln2, const int &id2, RelationType relation)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string source = SHA256::getHashString(input1);
    std::string destination = SHA256::getHashString(input2);

    adjacencyList[source].emplace_back(destination, relation);

    if (relation == Married)
        adjacencyList[destination].emplace_back(source, relation);
    else if (relation == Child)
        adjacencyList[destination].emplace_back(source, Parent);
    else
        adjacencyList[destination].emplace_back(source, Child);

    if (auto_save)
    {
        saveToFile("genealogy_save.txt");
    }
}

void HashedGenealogyGraph::saveToFile(const std::string &filename) const
{
    std::ofstream outFile(filename);

    if (outFile.is_open())
    {
        for (const auto &entry : adjacencyList)
        {
            std::string source = entry.first;
            for (const auto &pair : entry.second)
            {
                std::string destination = pair.first;
                RelationType relation = pair.second;

                outFile << source << " " << destination << " " << static_cast<int>(relation) << std::endl;
            }
        }
        outFile.close();
    }
    else
    {
        std::cerr << "Unable to open file for saving: " << filename << std::endl;
    }
}

void HashedGenealogyGraph::loadFromFile(const std::string &filename)
{
    std::ifstream inFile(filename);

    if (inFile.is_open())
    {
        adjacencyList.clear(); // Clear existing data before loading

        std::string source, destination;
        int relationInt;

        while (inFile >> source >> destination >> relationInt)
        {
            RelationType relation = static_cast<RelationType>(relationInt);
            adjacencyList[source].emplace_back(destination, relation);

            if (relation == Married)
            {
                adjacencyList[destination].emplace_back(source, relation);
            }
            else if (relation == Child)
            {
                adjacencyList[destination].emplace_back(source, Parent);
            }
            else
            {
                adjacencyList[destination].emplace_back(source, Child);
            }
        }

        inFile.close();
    }
    else
    {
        std::cerr << "Save file not found. Creating a new one: " << filename << std::endl;
        saveToFile(filename); 
    }
}
bool HashedGenealogyGraph::isAncestor(const std::string &person1, const std::string &person2)
{

    for (const auto &pair : adjacencyList[person2])
    {
        if (pair.second == Parent && pair.first == person1)
            return true;
        else if (pair.second == Parent)
            return isAncestor(person1, pair.first);
    }
    return false;
}
bool HashedGenealogyGraph::isAncestor(const std::string &n1, const std::string &ln1, const int &id1,
                                      const std::string &n2, const std::string &ln2, const int &id2)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string person1 = SHA256::SHA256::getHashString(input1);
    std::string person2 = SHA256::getHashString(input2);

    return isAncestor(person1, person2);
}
bool HashedGenealogyGraph::isSibling(const std::string &n1, const std::string &ln1, const int &id1,
                                     const std::string &n2, const std::string &ln2, const int &id2)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string person1 = SHA256::getHashString(input1);
    std::string person2 = SHA256::getHashString(input2);

    for (const auto &pair : adjacencyList[person1])
    {
        if (pair.second == Parent)
        {
            for (const auto &sibling : adjacencyList[pair.first])
            {
                if (sibling.first == person2 && sibling.second == Parent)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool HashedGenealogyGraph::isDistantRelative(const std::string &n1, const std::string &ln1, const int &id1,
                                             const std::string &n2, const std::string &ln2, const int &id2)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string person1 = SHA256::getHashString(input1);
    std::string person2 = SHA256::getHashString(input2);

    if (!isAncestor(person1, person2) && !isAncestor(person2, person1) && findCommonAncestor(person1, person2) != "")
        return true;

    return false;
}
void HashedGenealogyGraph::findAllAncestors(const std::string &person1, std::set<std::string> &ancestors1)
{
    for (const auto &pair : adjacencyList[person1])
    {
        if (pair.second == Parent)
        {
            ancestors1.insert(pair.first);
            findAllAncestors(pair.first, ancestors1);
        }
    }
}
std::string HashedGenealogyGraph::findCommonAncestor(const std::string &person1, const std::string &person2)
{

    std::set<std::string> ancestors1;

    std::set<std::string> ancestors2;

    findAllAncestors(person1, ancestors1);

    findAllAncestors(person2, ancestors2);

    for (const auto &pair : ancestors2)
    {
        if (ancestors1.count(pair))
        {
            return pair;
        }
    }

    return "";
}

std::string HashedGenealogyGraph::findCommonAncestor(const std::string &n1, const std::string &ln1, const int &id1,
                                                     const std::string &n2, const std::string &ln2, const int &id2)
{

     std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string person1 = SHA256::getHashString(input1);
    std::string person2 = SHA256::getHashString(input2);

    return findCommonAncestor(person1, person2);
}
int HashedGenealogyGraph::findFurthestDescendant(const std::string &n1, const std::string &ln1, const int &id1)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string person = SHA256::getHashString(input1);


    return findFurthestDescendant(person);
}

int HashedGenealogyGraph::findFurthestDescendant(const std::string &person)
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