// #include "../include/FileManager.h" 

// void loadFromFile(const string& filename, vector<Person *> allPeople, Trie<Person> adjacencyList)
// {

//     ifstream inFile(filename, ios::in | ios::binary);
//     if (!inFile.is_open())
//     {
//         cerr << "Error: File not found - " << filename << endl;
//         return;
//     }
//     int n;
//     !allPeople.empty() ? n = allPeople.size() : n = 0;
//     string line;
//     while (getline(inFile, line))
//     {
//         rapidjson::Document document;
//         document.Parse(line.c_str());

//         if (document.HasParseError())
//         {
//             cerr << "Error parsing JSON: " << rapidjson::GetParseErrorFunc(document.GetParseError())(document.GetParseError()) << " (Code " << document.GetParseError() << ")" << endl;
//             return;
//         }

//         const auto& families = document["families"];
//         for (const auto& family : families.GetArray())
//         {
//             string parent1HashVal = family["parent1HashVal"].GetString();
//             string parent2HashVal = family["parent2HashVal"].GetString();

//             auto parent1 = adjacencyList[parent1HashVal];
//             auto parent2 = adjacencyList[parent2HashVal];
//             parent1->married[parent2];
//             parent2->married[parent1];

//             allPeople.push_back(parent1);

//             parent1->id = n++;
//             allPeople.push_back(parent2);

//             parent2->id = n++;

//             const auto& children = family["children"];

//             for (const auto& childHash : children.GetArray())
//             {
//                 if (childHash.IsString())
//                 {
//                     string childHashVal = childHash.GetString();
//                     if (!childHashVal.empty())
//                     {
//                         auto child = adjacencyList[childHashVal];
//                         allPeople.push_back(child);
//                         child->id = n++;
//                         if (child->parent.empty())
//                         {
//                             parent1->married[parent2].insert(child);
//                             parent2->married[parent1].insert(child);
//                             child->parent.emplace_back(parent1);
//                             child->parent.emplace_back(parent2);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

// void saveToFile(const std::string& filename,  Trie<Person> adjacencyList)
// {

//     rapidjson::Document document;
//     document.SetObject();

//     auto& allocator = document.GetAllocator();
//     rapidjson::Value families(rapidjson::kArrayType);

//     for (const auto entry : adjacencyList.values())
//     {
//         string parent1HashVal = entry.first;

//         for (const auto& family : entry.second->married)
//         {
//             string parent2HashVal = family.first->hashValue;

//             rapidjson::Value familyData(rapidjson::kObjectType);
//             familyData.AddMember("parent1HashVal", rapidjson::Value(parent1HashVal.c_str(), allocator).Move(), allocator);
//             familyData.AddMember("parent2HashVal", rapidjson::Value(parent2HashVal.c_str(), allocator).Move(), allocator);

//             rapidjson::Value children(rapidjson::kArrayType);
//             for (const auto& child : family.second)
//             {
//                 children.PushBack(rapidjson::Value(child->hashValue.c_str(), allocator).Move(), allocator);
//             }

//             familyData.AddMember("children", children, allocator);
//             families.PushBack(familyData, allocator);
//         }
//     }

//     document.AddMember("families", families, allocator);

//     rapidjson::StringBuffer buffer;
//     rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//     document.Accept(writer);

//     ofstream outFile(filename, ios::out | ios::binary);
//     if (outFile.is_open())
//     {
//         outFile << buffer.GetString() << endl;
//         outFile.close();
//     }
//     else
//     {
//         cerr << "Error: Unable to open file for writing - " << filename << endl;
//     }
// }
