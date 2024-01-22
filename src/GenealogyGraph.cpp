#include "../include/GenealogyGraph.h"

// #ifdef __cplusplus
// extern "C"
// {
// #endif

//     string cStringToString(char const *cStr)
//     {
//         return (cStr != nullptr) ? string(cStr) : string();
//     }

//     pair<char *, char *> findMostDistantRelationship(GenealogyGraph *obj)
//     {
//         auto vec = obj->findMostDistantRelationship();
//         char *res1 = new char[vec.first.length() + 1];

//         char *res2 = new char[vec.second.length() + 1];
//         strcpy(res2, vec.second.c_str());

//         return pair<char *, char *>(res1, res2);
//     }
//     GenealogyGraph *createHashedGenealogyGraph(const bool autoSave)
//     {
//         return new GenealogyGraph(autoSave);
//     }

//     void addEdge(GenealogyGraph *obj, const char *json_data)
//     {
//         obj->addEdge(json_data);
//     }

//     int isAncestor(GenealogyGraph *obj, int const id1, int const id2)
//     {
//         string hashedId1 = SHA256::getHashString(to_string(id1));
//         string hashedId2 = SHA256::getHashString(to_string(id2));

//         return obj->isAncestor(hashedId1, hashedId2) ? 1 : 0;
//     }

//     int isSibling(GenealogyGraph *obj, int const id1, int const id2)
//     {
//         string hashedId1 = SHA256::getHashString(to_string(id1));
//         string hashedId2 = SHA256::getHashString(to_string(id2));

//         return obj->isSibling(hashedId1, hashedId2) ? 1 : 0;
//     }

//     int isDistantRelative(GenealogyGraph *obj, int const id1, int const id2)
//     {
//         string hashedId1 = SHA256::getHashString(to_string(id1));
//         string hashedId2 = SHA256::getHashString(to_string(id2));

//         return obj->isDistantRelative(hashedId1, hashedId2) ? 1 : 0;
//     }

//     const char *findCommonAncestor(GenealogyGraph *obj, int const id1, int const id2)
//     {
//         string hashedId1 = SHA256::getHashString(to_string(id1));
//         string hashedId2 = SHA256::getHashString(to_string(id2));

//         string commonAncestor = obj->findCommonAncestor(hashedId1, hashedId2);

//         char *res = new char[commonAncestor.length() + 1];
//         strcpy(res, commonAncestor.c_str());

//         return res;
//     }

//     int findFurthestDescendant(GenealogyGraph *obj, int const id1)
//     {
//         string hashedId1 = SHA256::getHashString(to_string(id1));

//         return obj->findFurthestDescendant(hashedId1);
//     }

//     void deleteHashedGenealogyGraph(GenealogyGraph *obj)
//     {
//         delete obj;
//     }

// #ifdef __cplusplus
// }
// #endif

GenealogyGraph::GenealogyGraph(bool const autoSave) : auto_save(autoSave)
{

    allPeople.clear();
    if (auto_save)
        loadFromFile("genealogy_save.json");
}

void GenealogyGraph::addFamily(rapidjson::Value const &marriageItem)
{
    try
    {
        string input1 = to_string(marriageItem["spouse1"]["id"].GetInt());
        string input2 = to_string(marriageItem["spouse2"]["id"].GetInt());
        string p1 = SHA256::getHashString(input1);
        string p2 = SHA256::getHashString(input2);

        const rapidjson::Value &children = marriageItem["children"];
        int n;
        !allPeople.empty() ? n = allPeople.size() : n = 0;
        if (children.IsArray())
        {

            for (rapidjson::SizeType i = 0; i < children.Size(); ++i)
            {
                const rapidjson::Value &childItems = children[i];

                string hashStr = to_string(childItems["id"].GetInt());

                string childHash = SHA256::getHashString(hashStr);

                auto child = adjacencyList[childHash];

                auto parent1 = adjacencyList[p1];

                auto parent2 = adjacencyList[p2];

                allPeople.push_back(parent1);

                parent1->id = n++;
                allPeople.push_back(parent2);

                parent2->id = n++;
                allPeople.push_back(child);
                child->id = n;
                if (child->parent.empty())
                {
                    if (!p1.empty())
                        parent2->married[parent1].insert(child);
                    if (!p2.empty())
                        parent1->married[parent2].insert(child);
                    child->parent.emplace_back(parent1);
                    child->parent.emplace_back(parent2);
                    child->hashValue = childHash;
                    parent1->hashValue = p1;
                    parent2->hashValue = p2;
                }
            }
        }
        else
            cerr << "no array " << endl;
    }
    catch (const std::exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    if (auto_save)
        saveToFile("genealogy_save.json");
}

void GenealogyGraph::addEdge(char const *json_data)
{
    rapidjson::Document document;
    document.Parse(json_data);
    rapidjson::Value &data = document.GetObject();
    if (!document.IsObject())
    {
        cerr << "Error parsing JSON data." << endl;
        return;
    }

    const rapidjson::Value &marriagesArray = document["marriages"];

    if (marriagesArray.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < marriagesArray.Size(); ++i)
        {
            const rapidjson::Value &marriageItem = marriagesArray[i];
            addFamily(marriageItem);
        }
    }
    if (auto_save)
        saveToFile("genealogy_save.json");
}

void GenealogyGraph::loadFromFile(string const &filename)
{
    ifstream inFile(filename, ios::in | ios::binary);
    if (!inFile.is_open())
    {
        cerr << "Error: File not found - " << filename << endl;
        return;
    }
    int n;
    !allPeople.empty() ? n = allPeople.size() : n = 0;
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

            auto parent1 = adjacencyList[parent1HashVal];
            auto parent2 = adjacencyList[parent2HashVal];
            parent1->married[parent2];
            parent2->married[parent1];

            allPeople.push_back(parent1);

            parent1->id = n++;
            allPeople.push_back(parent2);

            parent2->id = n++;

            const auto &children = family["children"];

            for (const auto &childHash : children.GetArray())
            {
                if (childHash.IsString())
                {

                    string childHashVal = childHash.GetString();
                    if (!childHashVal.empty())
                    {
                        auto child = adjacencyList[childHashVal];
                        allPeople.push_back(child);
                        child->id = n++;
                        if (child->parent.empty())
                        {
                            parent1->married[parent2].insert(child);
                            parent2->married[parent1].insert(child);
                            child->parent.emplace_back(parent1);
                            child->parent.emplace_back(parent2);
                        }
                    }
                }
            }
        }
    }
}

void GenealogyGraph::saveToFile(string const &filename)
{
    rapidjson::Document document;
    document.SetObject();

    auto &allocator = document.GetAllocator();
    rapidjson::Value families(rapidjson::kArrayType);

    for (const auto entry : adjacencyList.values())
    {
        string parent1HashVal = entry.first;

        for (const auto &family : entry.second->married)
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

bool GenealogyGraph::isAncestor(int id1, int id2)
{
    string ancestor = SHA256::getHashString(to_string(id1));
    string person = SHA256::getHashString(to_string(id2));
    return isAncestor(adjacencyList[ancestor], adjacencyList[person]);
}

bool GenealogyGraph::isAncestor(Person const *ancestor, Person const *person)
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

bool GenealogyGraph::isSibling(int const id1, int const id2)
{
    string person1 = SHA256::getHashString(to_string(id1));
    string person2 = SHA256::getHashString(to_string(id2));
    for (const auto &p : adjacencyList[person1]->parent)
    {
        auto it = find(adjacencyList[person2]->parent.begin(), adjacencyList[person2]->parent.end(), p);
        if (it != adjacencyList[person2]->parent.end())
            return true;
    }
    return false;
}

bool GenealogyGraph::isDistantRelative(int const id1, int const id2)
{
    string person1 = SHA256::getHashString(to_string(id1));
    string person2 = SHA256::getHashString(to_string(id2));
    auto p1 = adjacencyList[person1];
    auto p2 = adjacencyList[person2];
    if (!isAncestor(p1, p2) && !isAncestor(p2, p1) && findCommonAncestor(p1, p2) != "")
        return true;

    return false;
}

string GenealogyGraph::findCommonAncestor(int const id1, int const id2)
{
    string person1 = SHA256::getHashString(to_string(id1));
    string person2 = SHA256::getHashString(to_string(id2));
    return findCommonAncestor(adjacencyList[person1], adjacencyList[person2]);
}

string GenealogyGraph::findCommonAncestor(Person *person1, Person *person2)
{

    findAllAncestorsBFS(person1);
    Person *Found = findAllAncestorsBFS(person2);

    if (Found)
        return Found->hashValue;
    return "";
}

Person *GenealogyGraph::findAllAncestorsBFS(Person *person)
{
    queue<Person *> bfsQueue;
    bfsQueue.push(person);

    while (!bfsQueue.empty())
    {
        Person *current = bfsQueue.front();
        bfsQueue.pop();

        for (Person *Parent : current->parent)
        {
            if (Parent->state == 'r')
                return Parent;
            Parent->state = 'r';
            bfsQueue.push(Parent);
        }
    }
    return nullptr;
}
int GenealogyGraph::findFurthestDescendant(int const id)
{
    string person = SHA256::getHashString(to_string(id));
    return findFurthestDescendant(adjacencyList[person]);
}

int GenealogyGraph::findFurthestDescendant(Person const *person)
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
pair<string, string> GenealogyGraph::findMostDistantRelationship()
{
    int n = allPeople.size();
    vector<vector<int>> shortestPaths(n, vector<int>(n, -1));
    pair<string, string> mostDistantPair;
    int diameter = 0;

    for (int s = 0; s < n; ++s)
    {
        vector<bool> visited(n, false);
        queue<int> q;

        visited[s] = true;
        q.push(s);
        shortestPaths[s][s] = 0;
        while (!q.empty())
        {

            int v = q.front();
            q.pop();
            if (!allPeople[v]->parent.empty())
            {
                for (Person *parent : allPeople[v]->parent)
                {
                    int id = parent->id;
                    if (!visited[id])
                    {
                        visited[id] = true;
                        shortestPaths[s][id] = shortestPaths[s][v] + 1;
                        if (shortestPaths[s][id] > diameter)
                        {
                            diameter = shortestPaths[s][id];
                            mostDistantPair = {allPeople[s]->hashValue, allPeople[id]->hashValue};
                        }

                        q.push(id);
                    }
                }
            }
            for (const auto &family : allPeople[v]->married)
            {
                for (Person *child : family.second)
                {
                    int id = child->id;
                    if (!visited[id])
                    {
                        visited[id] = true;
                        shortestPaths[s][id] = shortestPaths[s][v] + 1;
                        if (shortestPaths[s][id] > diameter)
                        {
                            diameter = shortestPaths[s][id];
                            mostDistantPair = {allPeople[s]->hashValue, allPeople[id]->hashValue};
                        }

                        q.push(id);
                    }
                }
            }
        }
    }
    return mostDistantPair;
}
