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
            loadFromFile("genealogy_save.json");
    }

    void HashedGenealogyGraph::addEdge(const string &n1, const string &ln1, const int &id1,
                                       const string &n2, const string &ln2, const int &id2,
                                       vector<tuple<string, string, int>> children)
    {
        string input1 = n1 + ln1 + to_string(id1);
        string input2 = n2 + ln2 + to_string(id2);

        string p1 = SHA256::getHashString(input1);
        string p2 = SHA256::getHashString(input2);

        for (const auto &child : children)
        {
            string hashStr = get<0>(child) + get<1>(child) + to_string(get<2>(child));
            string childHash = SHA256::getHashString(hashStr);
            if (adjacencyList[childHash].parent.empty())
            {
                !p1.empty() && adjacencyList[p1].married[p2].insert(childHash).second;
                !p2.empty() && adjacencyList[p2].married[p1].insert(childHash).second;
                adjacencyList[childHash].parent.emplace_back(p1);
                adjacencyList[childHash].parent.emplace_back(p2);
            }
        }
        if (auto_save)
        {
            saveToFile("genealogy_save.json");
        }
    }

    void HashedGenealogyGraph::loadFromFile(const std::string &filename)
    {
        std::ifstream inFile(filename, std::ios::in | std::ios::binary);
        if (!inFile.is_open())
        {
            std::cerr << "Error: File not found - " << filename << std::endl;
            return;
        }

        adjacencyList.clear();

        string line;
        while (std::getline(inFile, line))
        {

            rapidjson::Document document;
            document.Parse(line.c_str());

            if (document.HasParseError())
            {
                std::cerr << "Error parsing JSON: " << rapidjson::GetParseErrorFunc(document.GetParseError())(document.GetParseError()) << " (Code " << document.GetParseError() << ")" << std::endl;
                return;
            }

            const auto &families = document["families"];
            for (const auto &family : families.GetArray())
            {
                std::string parent1HashVal = family["parent1HashVal"].GetString();
                std::string parent2HashVal = family["parent2HashVal"].GetString();

                // Ensure entries exist in the married map
                adjacencyList[parent1HashVal].married[parent2HashVal];
                adjacencyList[parent2HashVal].married[parent1HashVal];

                const auto &children = family["children"];
                for (const auto &child : children.GetArray())
                {
                    if (child.IsString()) // Check if it's a valid string
                    {
                        std::string childHashVal = child.GetString();
                        if (!childHashVal.empty())
                        {
                            if (adjacencyList[childHashVal].parent.empty())
                            {
                                adjacencyList[parent1HashVal].married[parent2HashVal].insert(childHashVal);
                                adjacencyList[parent2HashVal].married[parent1HashVal].insert(childHashVal);
                                adjacencyList[childHashVal].parent.emplace_back(parent1HashVal);
                                adjacencyList[childHashVal].parent.emplace_back(parent2HashVal);
                            }
                        }
                    }
                }
            }
        }
    }

    void HashedGenealogyGraph::saveToFile(const std::string &filename)
    {
        rapidjson::Document document;
        document.SetObject();

        auto &allocator = document.GetAllocator();
        rapidjson::Value families(rapidjson::kArrayType);

        for (const auto &entry : adjacencyList)
        {
            std::string parent1HashVal = entry.first;

            for (const auto &family : entry.second.married)
            {
                std::string parent2HashVal = family.first;

                rapidjson::Value familyData(rapidjson::kObjectType);
                familyData.AddMember("parent1HashVal", rapidjson::Value(parent1HashVal.c_str(), allocator).Move(), allocator);
                familyData.AddMember("parent2HashVal", rapidjson::Value(parent2HashVal.c_str(), allocator).Move(), allocator);

                rapidjson::Value children(rapidjson::kArrayType);
                for (const auto &childHashVal : family.second)
                {
                    children.PushBack(rapidjson::Value(childHashVal.c_str(), allocator).Move(), allocator);
                }

                familyData.AddMember("children", children, allocator);
                families.PushBack(familyData, allocator);
            }
        }

        document.AddMember("families", families, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        // Save the string to the file with each item in a new line
        std::ofstream outFile(filename, std::ios::out | std::ios::binary);
        if (outFile.is_open())
        {
            outFile << buffer.GetString() << std::endl;
            outFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open file for writing - " << filename << std::endl;
        }
    }

    bool HashedGenealogyGraph::isAncestor(const string &ancestor, const string &person)
    {
        for (const auto &p : adjacencyList[person].parent)
        {
            if (p == ancestor || isAncestor(ancestor, p))
            {
                return true;
            }
        }
        return false;
    }

    bool HashedGenealogyGraph::isAncestor(const string &n1, const string &ln1, const int &id1,
                                          const string &n2, const string &ln2, const int &id2)
    {
        string input1 = n1 + ln1 + to_string(id1);
        string input2 = n2 + ln2 + to_string(id2);

        string Ancestor = SHA256::getHashString(input1);
        string person = SHA256::getHashString(input2);

        return isAncestor(Ancestor, person);
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
    void HashedGenealogyGraph::findAllAncestors(const string &person1, set<string> &ancestors)
    {
        for (const auto &p : adjacencyList[person1].parent)
        {
            ancestors.insert(p);
            findAllAncestors(p, ancestors);
        }
    }
    string HashedGenealogyGraph::findCommonAncestor(const string &person1, const string &person2)
    {

        set<string> ancestors1;

        set<string> ancestors2;

        findAllAncestors(person1, ancestors1);

        findAllAncestors(person2, ancestors2);

        for (auto it1 = ancestors1.rbegin(); it1 != ancestors1.rend(); ++it1)
        {
            for (auto it2 = ancestors2.rbegin(); it2 != ancestors2.rend(); ++it2)
            {
                if (*it1 == *it2)
                {
                    return *it1;
                }
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