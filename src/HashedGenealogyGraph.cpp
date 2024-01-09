#include "/home/salam/Documents/ds/Hashed-Genealogy-Graph/include/HashedGenealogyGraph.h"

#ifdef __cplusplus
extern "C"
{
#endif

    HashedGenealogyGraph *createHashedGenealogyGraph(const bool autoSave)
    {
        return new HashedGenealogyGraph(autoSave);
    }

    void addEdge(HashedGenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                 const char *n2, const char *ln2, const int id2, const tuple_str_int *children, size_t children_size)
    {
        string name1(n1);
        string lastName1(ln1);
        string name2(n2);
        string lastName2(ln2);
        vector<tuple<string, string, int>> children_vector;
        for (size_t i = 0; i < children_size; ++i)
        {
            children_vector.emplace_back(children[i].first, children[i].second, children[i].third);
        }

        obj->addEdge(name1, lastName1, id1, name2, lastName2, id2, children_vector);
    }

    int isAncestor(HashedGenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                   const char *n2, const char *ln2, const int id2)
    {
        string name1(n1);
        string lastName1(ln1);
        string name2(n2);
        string lastName2(ln2);

        return obj->isAncestor(name1, lastName1, id1, name2, lastName2, id2) ? 1 : 0;
    }

    int isSibling(HashedGenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                  const char *n2, const char *ln2, const int id2)
    {
        string name1(n1);
        string lastName1(ln1);
        string name2(n2);
        string lastName2(ln2);
        return obj->isSibling(n1, ln1, id1, n2, ln2, id2) ? 1 : 0;
    }

    int isDistantRelative(HashedGenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                          const char *n2, const char *ln2, const int id2)
    {
        string name1(n1);
        string lastName1(ln1);
        string name2(n2);
        string lastName2(ln2);
        return obj->isDistantRelative(n1, ln1, id1, n2, ln2, id2) ? 1 : 0;
    }

    const char *findCommonAncestor(HashedGenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                                   const char *n2, const char *ln2, const int id2)
    {
        string name1(n1);
        string lastName1(ln1);
        string name2(n2);
        string lastName2(ln2);

        string commonAncestor = obj->findCommonAncestor(name1, lastName1, id1, name2, lastName2, id2);

        char *res = new char[commonAncestor.length() + 1];

        strcpy(res, commonAncestor.c_str());

        return res;
    }

    int findFurthestDescendant(HashedGenealogyGraph *obj, const char *n1, const char *ln1, const int id1)
    {
        std::string name1(n1);
        std::string lastName1(ln1);
        return obj->findFurthestDescendant(name1, lastName1, id1);
    }

    void deleteHashedGenealogyGraph(HashedGenealogyGraph *obj)
    {
        delete obj;
    }

    HashedGenealogyGraph::HashedGenealogyGraph(const bool autoSave) : auto_save(autoSave)
    {
        if (auto_save)
            loadFromFile("genealogy_save.txt");
    }

    void HashedGenealogyGraph::addEdge(const string &n1, const string &ln1, const int &id1,
                                       const string &n2, const string &ln2, const int &id2,
                                       vector<tuple<string, string, int>> children)
    {
        string input1 = n1 + ln1 + to_string(id1);
        string input2 = n2 + ln2 + to_string(id2);

        string source = SHA256::getHashString(input1);
        string destination = SHA256::getHashString(input2);

        for (const auto &child : children)
        {
            string hashStr = get<0>(child) + get<1>(child) + to_string(get<2>(child));
            string childHash = SHA256::getHashString(hashStr);
            !source.empty() && adjacencyList[source].married[destination].insert(childHash).second;
            !destination.empty() && adjacencyList[destination].married[source].insert(childHash).second;
            !source.empty() ? adjacencyList[childHash].parent.emplace_back(source), void() : void();
            !destination.empty() ? adjacencyList[childHash].parent.emplace_back(destination), void() : void();
        }
        if (auto_save)
        {
            saveToFile("genealogy_save.txt");
        }
    }

    void HashedGenealogyGraph::saveToFile(const string &filename)
    {
        ofstream outFile(filename);

        if (outFile.is_open())
        {

            for (auto &entry : adjacencyList)
            {
                string parent1HashVal = entry.first;
                entry.second.visited = true;

                for (const auto &family : entry.second.married)
                {
                    string parent2HashVal = family.first;
                    if (entry.second.visited || adjacencyList[parent2HashVal].visited)
                    {
                        adjacencyList[parent2HashVal].visited = true;
                        outFile << parent1HashVal << " " << parent2HashVal;

                        for (const auto &childHashVal : family.second)
                        {
                            outFile << " " << childHashVal;
                        }

                        outFile << endl;
                    }
                }
            }
            outFile.close();
        }
        else
        {
            cerr << "File not found: " << filename << ". Creating a new file." << endl;

            ofstream outFile(filename);
            outFile.close();
        }
    }

    void HashedGenealogyGraph::loadFromFile(const string &filename)
    {
        ifstream inFile(filename);

        if (inFile.is_open())
        {
            adjacencyList.clear();

            string parent1HashVal, parent2HashVal, childHashVal;

            while (inFile >> parent1HashVal >> parent2HashVal)
            {

                while (inFile >> childHashVal)
                {
                    adjacencyList[parent1HashVal].married[parent2HashVal].insert(childHashVal);
                    adjacencyList[parent2HashVal].married[parent1HashVal].insert(childHashVal);
                    adjacencyList[childHashVal].parent.emplace_back(parent1HashVal);
                    adjacencyList[childHashVal].parent.emplace_back(parent2HashVal);
                }
            }

            inFile.close();
        }
        else
        {
            cerr << "File not found: " << filename << endl;
        }
    }

    bool HashedGenealogyGraph::isAncestor(const string &person1, const string &person2)
    {

        for (const auto &parent : adjacencyList[person2].parent)
        {
            if (parent == person1)
                return true;
            else
                return isAncestor(person1, parent);
        }
        return false;
    }
    bool HashedGenealogyGraph::isAncestor(const string &n1, const string &ln1, const int &id1,
                                          const string &n2, const string &ln2, const int &id2)
    {
        string input1 = n1 + ln1 + to_string(id1);
        string input2 = n2 + ln2 + to_string(id2);

        string person1 = SHA256::getHashString(input1);
        string person2 = SHA256::getHashString(input2);

        return isAncestor(person1, person2);
    }
    bool HashedGenealogyGraph::isSibling(const string &n1, const string &ln1, const int &id1,
                                         const string &n2, const string &ln2, const int &id2)
    {
        string input1 = n1 + ln1 + to_string(id1);
        string input2 = n2 + ln2 + to_string(id2);

        string person1 = SHA256::getHashString(input1);
        string person2 = SHA256::getHashString(input2);

        for (const auto &p : adjacencyList[person1].parent)
        {
            auto it = find(adjacencyList[person2].parent.begin(), adjacencyList[person2].parent.end(), p);
            if (it != adjacencyList[person2].parent.end())
                return true;
        }
        return false;
    }

    bool HashedGenealogyGraph::isDistantRelative(const string &n1, const string &ln1, const int &id1,
                                                 const string &n2, const string &ln2, const int &id2)
    {
        string input1 = n1 + ln1 + to_string(id1);
        string input2 = n2 + ln2 + to_string(id2);

        string person1 = SHA256::getHashString(input1);
        string person2 = SHA256::getHashString(input2);

        if (!isAncestor(person1, person2) && !isAncestor(person2, person1) && findCommonAncestor(person1, person2) != "")
            return true;

        return false;
    }
    void HashedGenealogyGraph::findAllAncestors(const string &person1, set<string> &ancestors1)
    {
        for (const auto &Parent : adjacencyList[person1].parent)
        {

            ancestors1.insert(Parent);
            findAllAncestors(Parent, ancestors1);
        }
    }
    string HashedGenealogyGraph::findCommonAncestor(const string &person1, const string &person2)
    {

        set<string> ancestors1;

        set<string> ancestors2;

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

    string HashedGenealogyGraph::findCommonAncestor(const string &n1, const string &ln1, const int &id1,
                                                    const string &n2, const string &ln2, const int &id2)
    {

        string input1 = n1 + ln1 + to_string(id1);
        string input2 = n2 + ln2 + to_string(id2);

        string person1 = SHA256::getHashString(input1);
        string person2 = SHA256::getHashString(input2);

        return findCommonAncestor(person1, person2);
    }
    int HashedGenealogyGraph::findFurthestDescendant(const string &n1, const string &ln1, const int &id1)
    {
        string input1 = n1 + ln1 + to_string(id1);
        string person = SHA256::getHashString(input1);

        return findFurthestDescendant(person);
    }

    int HashedGenealogyGraph::findFurthestDescendant(const string &person)
    {
        int maxDistance = 0;

        for (const auto &family : adjacencyList[person].married)
        {
            for (const auto &Child : family.second)
            {
                int distance = findFurthestDescendant(Child) + 1;
                maxDistance = max(maxDistance, distance);
            }
        }
        return maxDistance;
    }

#ifdef __cplusplus
}
#endif