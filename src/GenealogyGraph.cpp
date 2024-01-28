#include "../include/GenealogyGraph.h"


GenealogyGraph::GenealogyGraph(bool const autoSave) : auto_save(autoSave)
{

    allPeople.clear();
    // if (auto_save)
    //     loadFromFile("genealogy_save.json");
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

                bool check1 = adjacencyList.contains(p1) ? true : false;

                bool check2 = adjacencyList.contains(p2) ? true : false;

                bool check3 = adjacencyList.contains(childHash) ? true : false;

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
                    parent1->gender = marriageItem["spouse1"]["gender"].GetInt();
                     parent2->gender = marriageItem["spouse2"]["gender"].GetInt();
                    child->gender = childItems["gender"].GetInt();
                    child->hashValue = childHash;
                    parent1->hashValue = p1;
                    parent2->hashValue = p2;
                }
                if (!check1)
                {
                    allPeople.push_back(parent1);
                    parent1->id = n++;
                }
                if (!check2)
                {
                    allPeople.push_back(parent2);
                    parent2->id = n++;
                }
                if (!check3)
                {
                    allPeople.push_back(child);
                    child->id = n;
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
            cerr << "Error parsing JSON(L201): " << rapidjson::GetParseErrorFunc(document.GetParseError())(document.GetParseError()) << " (Code " << document.GetParseError() << ")" << endl;
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

void GenealogyGraph::saveToFile(const std::string &filename)
{
    cerr << "F1" << endl;
    rapidjson::Document document;
    document.SetObject();

    auto &allocator = document.GetAllocator();
    rapidjson::Value nodes(rapidjson::kArrayType);
    rapidjson::Value dict(rapidjson::kObjectType);

    for (int i = 0; i < allPeople.size(); i++)
    {
        auto person = allPeople[i];

        rapidjson::Value node(rapidjson::kObjectType);
        node.AddMember("id", i, allocator);
        //!   cerr<< "F1" << endl;
        string gender = person->gender ? "female" : "male";
        node.AddMember("hash", rapidjson::Value(person->hashValue.c_str(), allocator).Move(), allocator);
        node.AddMember("gender", rapidjson::Value(gender.c_str(), allocator).Move(), allocator);
        if (!person->married.empty())
        {

            rapidjson::Value pids(rapidjson::kArrayType);
            for (const auto &marriedPerson : person->married)
            {
                rapidjson::Value marriedPersonId(rapidjson::kNumberType);
                marriedPersonId.SetInt(marriedPerson.first->id);
                pids.PushBack(marriedPersonId, allocator);
            }
            node.AddMember("pids", pids, allocator);
            //!    cerr<< "F2" << endl;
        }

        if (!person->parent.empty())
        {
            rapidjson::Value p1(rapidjson::kNumberType);
            p1.SetInt(person->parent[0]->id);
            rapidjson::Value p2(rapidjson::kNumberType);
            p2.SetInt(person->parent[1]->id);

            node.AddMember("mid", p1, allocator);
            node.AddMember("fid", p2, allocator);
            //!   cerr<< "F3" << endl;
        }

        // Add other fields like name and gender if needed
        // node.AddMember("name", rapidjson::Value(person->name.c_str(), allocator).Move(), allocator);
        // node.AddMember("gender", rapidjson::Value(person->gender.c_str(), allocator).Move(), allocator);

        nodes.PushBack(node, allocator);
        //!   cerr<< "F4" << endl;

        // Add the person's hashValue to the dictionary
        dict.AddMember(rapidjson::Value(std::to_string(i).c_str(), allocator).Move(), rapidjson::Value(person->hashValue.c_str(), allocator).Move(), allocator);

        //!   cerr<< "F5" << endl;
    }

    document.AddMember("nodes", nodes, allocator);
    document.AddMember("dict", dict, allocator);

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
    cerr << "F5" << endl;
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

void GenealogyGraph::deletePerson(int const id)
{
    string personHash = SHA256::getHashString(to_string(id));
    auto personToDelete = adjacencyList[personHash];

    for (auto parent : personToDelete->parent)
    {
        auto it = find(parent->married[personToDelete].begin(), parent->married[personToDelete].end(), personToDelete);
        if (it != parent->married[personToDelete].end())
        {
            parent->married[personToDelete].erase(it);
        }
    }

    for (auto family : personToDelete->married)
    {
        for (auto child : family.second)
        {
            auto it = find(child->parent.begin(), child->parent.end(), personToDelete);
            if (it != child->parent.end())
            {
                child->parent.erase(it);
            }
        }
    }

    adjacencyList.erase(personHash);

    auto it = find(allPeople.begin(), allPeople.end(), personToDelete);
    if (it != allPeople.end())
    {
        delete *it;
        allPeople.erase(it);
    }

    if (auto_save)
        saveToFile("genealogy_save.json");
}
