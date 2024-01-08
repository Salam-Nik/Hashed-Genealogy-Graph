#include "/home/salam/Documents/ds/Hashed-Genealogy-Graph/include/HashedGenealogyGraph.h"

#ifdef __cplusplus
extern "C" {
#endif

HashedGenealogyGraph* createHashedGenealogyGraph(const bool autoSave)
{
    return new HashedGenealogyGraph(autoSave);
}

void addEdge(HashedGenealogyGraph* obj, const char* n1, const char* ln1, const int id1,
             const char* n2, const char* ln2, const int id2, RelationType relation)
{
    std::string name1(n1);
    std::string lastName1(ln1);
    std::string name2(n2);
    std::string lastName2(ln2);

    obj->addEdge(name1, lastName1, id1, name2, lastName2, id2, relation);
}
// Check if a person is an ancestor of another person
int isAncestor(HashedGenealogyGraph* obj, const char* person1, const char* person2)
{
    return obj->isAncestor(person1, person2) ? 1 : 0;
}

int isSibling(HashedGenealogyGraph* obj, const char* n1, const char* ln1, const int id1,
              const char* n2, const char* ln2, const int id2)
{
    return obj->isSibling(n1, ln1, id1, n2, ln2, id2) ? 1 : 0;
}

int isDistantRelative(HashedGenealogyGraph* obj, const char* n1, const char* ln1, const int id1,
                      const char* n2, const char* ln2, const int id2)
{
    return obj->isDistantRelative(n1, ln1, id1, n2, ln2, id2) ? 1 : 0;
}

const char* findCommonAncestor(HashedGenealogyGraph* obj, const char* n1, const char* ln1, const int id1,
                               const char* n2, const char* ln2, const int id2)
{
    string name1(n1);
    string lastName1(ln1);
    string name2(n2);
    string lastName2(ln2);
    
    std::string commonAncestor = obj->findCommonAncestor(name1, lastName1, id1, name2, lastName2, id2);

    char* res = new char[commonAncestor.length() + 1];

    strcpy(res, commonAncestor.c_str());

    return res;
}

int findFurthestDescendant(HashedGenealogyGraph* obj, const char* n1, const char* ln1, const int id1)
{
    std::string name1(n1);
    std::string lastName1(ln1);
    return obj->findFurthestDescendant(name1, lastName1, id1);
}

void deleteHashedGenealogyGraph(HashedGenealogyGraph* obj)
{
    delete obj;
}

HashedGenealogyGraph::HashedGenealogyGraph(const bool autoSave) : auto_save(autoSave)
{
    if (auto_save)
        loadFromFile("genealogy_save.txt");
}

void HashedGenealogyGraph::addEdge(const string &n1, const string &ln1, const int &id1,
                                   const string &n2, const string &ln2, const int &id2, RelationType relation)
{
    string input1 = n1 + ln1 + to_string(id1);
    string input2 = n2 + ln2 + to_string(id2);

    string source = SHA256::getHashString(input1);
    string destination = SHA256::getHashString(input2);

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

void HashedGenealogyGraph::saveToFile(const string &filename) const
{
    ofstream outFile(filename);

    if (outFile.is_open())
    {
        for (const auto &entry : adjacencyList)
        {
            string source = entry.first;
            for (const auto &pair : entry.second)
            {
                string destination = pair.first;
                RelationType relation = pair.second;

                outFile << source << " " << destination << " " << static_cast<int>(relation) << endl;
            }
        }
        outFile.close();
    }
    else
    {
        cerr << "Unable to open file for saving: " << filename << endl;
    }
}

void HashedGenealogyGraph::loadFromFile(const string &filename)
{
    ifstream inFile(filename);

    if (inFile.is_open())
    {
        adjacencyList.clear(); 

        string source, destination;
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
        cerr << "Save file not found. Creating a new one: " << filename << endl;
        saveToFile(filename); 
    }
}
bool HashedGenealogyGraph::isAncestor(const string &person1, const string &person2)
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
    for (const auto &pair : adjacencyList[person1])
    {
        if (pair.second == Parent)
        {
            ancestors1.insert(pair.first);
            findAllAncestors(pair.first, ancestors1);
        }
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

    for (const auto &pair : adjacencyList[person])
    {
        if (pair.second == Child)
        {
            int distance = findFurthestDescendant(pair.first) + 1;
            maxDistance = max(maxDistance, distance);
        }
    }

    return maxDistance;
}

#ifdef __cplusplus
}
#endif