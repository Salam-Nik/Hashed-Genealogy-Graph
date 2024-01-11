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

void GenealogyGraph::addEdge(const std::string &n1, const std::string &ln1, const int &id1,
                             const std::string &n2, const std::string &ln2, const int &id2,
                             std::vector<std::tuple<std::string, std::string, int>> children)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string p1 = SHA256::getHashString(input1);
    std::string p2 = SHA256::getHashString(input2);

    for (auto &child : children)
    {
        std::string hashStr = std::get<0>(child) + std::get<1>(child) + std::to_string(std::get<2>(child));
        std::string childHash = SHA256::getHashString(hashStr);
        auto &tempRelations = adjacencyList[childHash];

        if (tempRelations.parent.empty())
        {
            if (!p1.empty())
                tempRelations.married[p2].insert(childHash);
            if (!p2.empty())
                tempRelations.married[p1].insert(childHash);
            tempRelations.parent.emplace_back(p1);
            tempRelations.parent.emplace_back(p2);
        }
    }
    if (auto_save)
    {
        saveToFile("genealogy_save.json");
    }
}

void GenealogyGraph::loadFromFile(const std::string &filename)
{
    std::ifstream inFile(filename, std::ios::in | std::ios::binary);
    if (!inFile.is_open())
    {
        std::cerr << "Error: File not found - " << filename << std::endl;
        return;
    }

    // adjacencyList.clear();

    std::string line;
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

            auto &p1Relations = adjacencyList[parent1HashVal];
            auto &p2Relations = adjacencyList[parent2HashVal];
            p1Relations.married[parent2HashVal];
            p2Relations.married[parent1HashVal];

            const auto &children = family["children"];
            for (const auto &child : children.GetArray())
            {
                if (child.IsString())
                {
                    std::string childHashVal = child.GetString();
                    if (!childHashVal.empty())
                    {
                        auto &childRelations = adjacencyList[childHashVal];
                        if (childRelations.parent.empty())
                        {
                            p1Relations.married[parent2HashVal].insert(childHashVal);
                            p2Relations.married[parent1HashVal].insert(childHashVal);
                            childRelations.parent.emplace_back(parent1HashVal);
                            childRelations.parent.emplace_back(parent2HashVal);
                        }
                    }
                }
            }
        }
    }
}

void GenealogyGraph::saveToFile(const std::string &filename)
    {
        rapidjson::Document document;
        document.SetObject();

        auto &allocator = document.GetAllocator();
        rapidjson::Value families(rapidjson::kArrayType);

        for (const auto &entry : adjacencyList.values())
        {
            std::string parent1HashVal = adjacencyList.getKey(entry);

            for (const auto &family : entry.married)
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

bool GenealogyGraph::isAncestor(const std::string &ancestor, const std::string &person)
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

bool GenealogyGraph::isAncestor(const std::string &n1, const std::string &ln1, const int &id1,
                                const std::string &n2, const std::string &ln2, const int &id2)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string Ancestor = SHA256::getHashString(input1);
    std::string person = SHA256::getHashString(input2);

    return isAncestor(Ancestor, person);
}

bool GenealogyGraph::isSibling(const std::string &n1, const std::string &ln1, const int &id1,
                               const std::string &n2, const std::string &ln2, const int &id2)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string person1 = SHA256::getHashString(input1);
    std::string person2 = SHA256::getHashString(input2);

    for (const auto &p : adjacencyList[person1].parent)
    {
        auto it = std::find(adjacencyList[person2].parent.begin(), adjacencyList[person2].parent.end(), p);
        if (it != adjacencyList[person2].parent.end())
            return true;
    }
    return false;
}

bool GenealogyGraph::isDistantRelative(const std::string &n1, const std::string &ln1, const int &id1,
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

void GenealogyGraph::findAllAncestors(const std::string &person1, std::set<std::string> &ancestors)
{
    for (const auto &p : adjacencyList[person1].parent)
    {
        ancestors.insert(p);
        findAllAncestors(p, ancestors);
    }
}

std::string GenealogyGraph::findCommonAncestor(const std::string &person1, const std::string &person2)
{
    std::set<std::string> ancestors1;
    std::set<std::string> ancestors2;

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

std::string GenealogyGraph::findCommonAncestor(const std::string &n1, const std::string &ln1, const int &id1,
                                               const std::string &n2, const std::string &ln2, const int &id2)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string input2 = n2 + ln2 + std::to_string(id2);

    std::string person1 = SHA256::getHashString(input1);
    std::string person2 = SHA256::getHashString(input2);

    return findCommonAncestor(person1, person2);
}

int GenealogyGraph::findFurthestDescendant(const std::string &n1, const std::string &ln1, const int &id1)
{
    std::string input1 = n1 + ln1 + std::to_string(id1);
    std::string person = SHA256::getHashString(input1);

    return findFurthestDescendant(person);
}

int GenealogyGraph::findFurthestDescendant(const std::string &person)
{
    int maxDistance = 0;

    for (const auto &family : adjacencyList[person].married)
    {
        for (const auto &Child : family.second)
        {
            int distance = findFurthestDescendant(Child) + 1;
            maxDistance = std::max(maxDistance, distance);
        }
    }
    return maxDistance;
}
