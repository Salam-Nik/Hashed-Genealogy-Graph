#include "../include/GenealogyGraph.h"


#ifdef __cplusplus
extern "C"
{
#endif

    // Helper function to convert C-style strings to C++ std::string
    std::string cStringToString(const char *cStr)
    {
        return (cStr != nullptr) ? std::string(cStr) : std::string();
    }

    std::unique_ptr<GenealogyGraph> createHashedGenealogyGraph(const bool autoSave)
    {
        return std::make_unique<GenealogyGraph>(autoSave);
    }

    void addEdge(GenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                 const char *n2, const char *ln2, const int id2, const tuple_str_int *children, size_t children_size)
    {
        std::string name1 = cStringToString(n1);
        std::string lastName1 = cStringToString(ln1);
        std::string name2 = cStringToString(n2);
        std::string lastName2 = cStringToString(ln2);

        std::vector<std::tuple<std::string, std::string, int>> children_vector;
        for (size_t i = 0; i < children_size; ++i)
        {
            children_vector.emplace_back(cStringToString(children[i].first), cStringToString(children[i].second), children[i].third);
        }

        obj->addEdge(name1, lastName1, id1, name2, lastName2, id2, children_vector);
    }

    int isAncestor(GenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                   const char *n2, const char *ln2, const int id2)
    {
        std::string name1 = cStringToString(n1);
        std::string lastName1 = cStringToString(ln1);
        std::string name2 = cStringToString(n2);
        std::string lastName2 = cStringToString(ln2);

        return obj->isAncestor(name1, lastName1, id1, name2, lastName2, id2) ? 1 : 0;
    }

    int isSibling(GenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                  const char *n2, const char *ln2, const int id2)
    {
        std::string name1 = cStringToString(n1);
        std::string lastName1 = cStringToString(ln1);
        std::string name2 = cStringToString(n2);
        std::string lastName2 = cStringToString(ln2);

        return obj->isSibling(name1, lastName1, id1, name2, lastName2, id2) ? 1 : 0;
    }

    int isDistantRelative(GenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                          const char *n2, const char *ln2, const int id2)
    {
        std::string name1 = cStringToString(n1);
        std::string lastName1 = cStringToString(ln1);
        std::string name2 = cStringToString(n2);
        std::string lastName2 = cStringToString(ln2);

        return obj->isDistantRelative(name1, lastName1, id1, name2, lastName2, id2) ? 1 : 0;
    }

    const char *findCommonAncestor(GenealogyGraph *obj, const char *n1, const char *ln1, const int id1,
                                   const char *n2, const char *ln2, const int id2)
    {
        std::string name1 = cStringToString(n1);
        std::string lastName1 = cStringToString(ln1);
        std::string name2 = cStringToString(n2);
        std::string lastName2 = cStringToString(ln2);

        std::string commonAncestor = obj->findCommonAncestor(name1, lastName1, id1, name2, lastName2, id2);

        char *res = new char[commonAncestor.length() + 1];
        std::strcpy(res, commonAncestor.c_str());

        return res;
    }

    int findFurthestDescendant(GenealogyGraph *obj, const char *n1, const char *ln1, const int id1)
    {
        std::string name1 = cStringToString(n1);
        std::string lastName1 = cStringToString(ln1);
        return obj->findFurthestDescendant(name1, lastName1, id1);
    }

    void deleteHashedGenealogyGraph(GenealogyGraph *obj)
    {
        delete obj;
    }

#ifdef __cplusplus
}
#endif

GenealogyGraph::GenealogyGraph(const bool autoSave) : auto_save(autoSave), adjacencyList()
{
    if (auto_save)
        loadFromFile("genealogy_save.json");
}

void GenealogyGraph::addEdge(const string &n1, const string &ln1, const int &id1,
                             const string &n2, const string &ln2, const int &id2,
                             vector<tuple<string, string, int>> children)
{
    string input1 = n1 + ln1 + to_string(id1);
    string input2 = n2 + ln2 + to_string(id2);

    string p1 = SHA256::getHashString(input1);
    string p2 = SHA256::getHashString(input2);

    for (auto &childVec : children)
    {
        string hashStr = get<0>(childVec) + get<1>(childVec) + to_string(get<2>(childVec));
        string childHash = SHA256::getHashString(hashStr);
        auto child = adjacencyList[childHash];
        auto parent1 = adjacencyList[p1];
        auto parent2 = adjacencyList[p2];

        if (child->parent.empty())
        {
            if (!p1.empty())
                parent2->married[parent1].insert(child);
            if (!p2.empty())
                parent1->married[parent2].insert(child);
            child->parent.emplace_back(parent1);
            child->parent.emplace_back(parent2);
            child->hashValue = hashStr;
            parent1->hashValue = p1;
            parent1->hashValue = p2;
        }
    }
    if (auto_save)
    {
        saveToFile("genealogy_save.json");
    }
}

void GenealogyGraph::loadFromFile(const string &filename)
{
    ifstream inFile(filename, ios::in | ios::binary);
    if (!inFile.is_open())
    {
        cerr << "Error: File not found - " << filename << endl;
        return;
    }

    // adjacencyList.clear();

    string line;
    while (getline(inFile, line))
    {

        rapidjson::Document document;
        document.Parse(line.c_str());

        if (document.HasParseError())
        {
            cerr << "Error parsing JSON: " << rapidjson::GetParseErrorFunc(document.GetParseError())(document.GetParseError()) << " (Code " << document.GetParseError() << ")" << endl;
            return;
        }

        const auto &families = document["families"];
        for (const auto &family : families.GetArray())
        {
            string parent1HashVal = family["parent1HashVal"].GetString();
            string parent2HashVal = family["parent2HashVal"].GetString();

            auto p1Relations = adjacencyList[parent1HashVal];
            auto p2Relations = adjacencyList[parent2HashVal];
            p1Relations->married[p2Relations];
            p2Relations->married[p1Relations];

            const auto &children = family["children"];
            for (const auto &childHash : children.GetArray())
            {
                if (childHash.IsString())
                {
                    string childHashVal = childHash.GetString();
                    if (!childHashVal.empty())
                    {
                        auto child = adjacencyList[childHashVal];
                        if (child->parent.empty())
                        {
                            p1Relations->married[p2Relations].insert(child);
                            p2Relations->married[p1Relations].insert(child);
                            child->parent.emplace_back(p1Relations);
                            child->parent.emplace_back(p2Relations);
                        }
                    }
                }
            }
        }
    }
}

void GenealogyGraph::saveToFile(const string &filename)
    {
        rapidjson::Document document;
        document.SetObject();

        auto &allocator = document.GetAllocator();
        rapidjson::Value families(rapidjson::kArrayType);

        for (const auto entry : adjacencyList.values())
        {
            string parent1HashVal = entry->hashValue;

            for (const auto &family : entry->married)
            {
                string parent2HashVal = family.first->hashValue;

                rapidjson::Value familyData(rapidjson::kObjectType);
                familyData.AddMember("parent1HashVal", rapidjson::Value(parent1HashVal.c_str(), allocator).Move(), allocator);
                familyData.AddMember("parent2HashVal", rapidjson::Value(parent2HashVal.c_str(), allocator).Move(), allocator);

                rapidjson::Value children(rapidjson::kArrayType);
                for (const auto &child : family.second)
                {
                    children.PushBack(rapidjson::Value(child->hashValue.c_str(), allocator).Move(), allocator);
                }

                familyData.AddMember("children", children, allocator);
                families.PushBack(familyData, allocator);
            }
        }

        document.AddMember("families", families, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);


        ofstream outFile(filename, ios::out | ios::binary);
        if (outFile.is_open())
        {
            outFile << buffer.GetString() << endl;
            outFile.close();
        }
        else
        {
            cerr << "Error: Unable to open file for writing - " << filename << endl;
        }
    }

bool GenealogyGraph::isAncestor(const Person* ancestor, const Person* person)
{
    for (const auto &p : person->parent)
    {
        if (p == ancestor || isAncestor(ancestor, p))
        {
            return true;
        }
    }
    return false;
}

bool GenealogyGraph::isAncestor(const string &n1, const string &ln1, const int &id1,
                                const string &n2, const string &ln2, const int &id2)
{
    string input1 = n1 + ln1 + to_string(id1);
    string input2 = n2 + ln2 + to_string(id2);

    string Ancestor = SHA256::getHashString(input1);
    string person = SHA256::getHashString(input2);

    return isAncestor(adjacencyList[Ancestor], adjacencyList[person]);
}

bool GenealogyGraph::isSibling(const string &n1, const string &ln1, const int &id1,
                               const string &n2, const string &ln2, const int &id2)
{
    string input1 = n1 + ln1 + to_string(id1);
    string input2 = n2 + ln2 + to_string(id2);

    string person1 = SHA256::getHashString(input1);
    string person2 = SHA256::getHashString(input2);

    for (const auto &p : adjacencyList[person1]->parent)
    {
        auto it = find(adjacencyList[person2]->parent.begin(), adjacencyList[person2]->parent.end(), p);
        if (it != adjacencyList[person2]->parent.end())
            return true;
    }
    return false;
}

bool GenealogyGraph::isDistantRelative(const string &n1, const string &ln1, const int &id1,
                                       const string &n2, const string &ln2, const int &id2)
{
    string input1 = n1 + ln1 + to_string(id1);
    string input2 = n2 + ln2 + to_string(id2);

    string person1 = SHA256::getHashString(input1);
    string person2 = SHA256::getHashString(input2);
    auto p1 = adjacencyList[person1];
    auto p2 = adjacencyList[person2];
    if (!isAncestor(p1, p2) && !isAncestor(p2, p1) && findCommonAncestor(p1, p2) != "")
        return true;

    return false;
}

void GenealogyGraph::findAllAncestors(const Person* person1, set<Person*> &ancestors)
{
    for (const auto &p : person1->parent)
    {
        ancestors.insert(p);
        findAllAncestors(p, ancestors);
    }
}

string GenealogyGraph::findCommonAncestor(const Person* person1, const Person* person2)
{
    set<Person*> ancestors1;
    set<Person*> ancestors2;

    findAllAncestors(person1, ancestors1);
    findAllAncestors(person2, ancestors2);

    for (auto it1 = ancestors1.rbegin(); it1 != ancestors1.rend(); ++it1)
    {
        for (auto it2 = ancestors2.rbegin(); it2 != ancestors2.rend(); ++it2)
        {
        if (*it1 == *it2)// Dereference the iterators to access Person objects
        {
            return (*it1)->hashValue;  // Dereference the iterator to access Person object's member
        }
        }
    }

    return "";
}

string GenealogyGraph::findCommonAncestor(const string &n1, const string &ln1, const int &id1,
                                               const string &n2, const string &ln2, const int &id2)
{
    string input1 = n1 + ln1 + to_string(id1);
    string input2 = n2 + ln2 + to_string(id2);

    string person1 = SHA256::getHashString(input1);
    string person2 = SHA256::getHashString(input2);

    auto p1 = adjacencyList[person1];
    auto p2 = adjacencyList[person2];

    return findCommonAncestor(p1, p2);
}

int GenealogyGraph::findFurthestDescendant(const string &n1, const string &ln1, const int &id1)
{
    string input1 = n1 + ln1 + to_string(id1);
    string person = SHA256::getHashString(input1);

    auto p= adjacencyList[person];
    return findFurthestDescendant(p);
}

int GenealogyGraph::findFurthestDescendant(const Person* person)
{
    int maxDistance = 0;

    for (const auto &family : person->married)
    {
        for (const auto &Child : family.second)
        {
            int distance = findFurthestDescendant(Child) + 1;
            maxDistance = max(maxDistance, distance);
        }
    }
    return maxDistance;
}
