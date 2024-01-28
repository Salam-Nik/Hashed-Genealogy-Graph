
#include "include/GenealogyGraph.h"

int main()
{
    GenealogyGraph hgg;

    std::ifstream jsonFile("dataset/genealogy_dataset.json");
    if (!jsonFile.is_open())
    {
        cerr << "Error opening JSON file" << endl;
        return 1;
    }

    std::ostringstream jsonContent;
    jsonContent << jsonFile.rdbuf();
    std::string jsonString = jsonContent.str();

    rapidjson::Document json_data;
    json_data.Parse(jsonString.c_str());

    if (json_data.HasParseError())
    {
        cerr << "Error parsing JSON file: " << endl;
        return 1;
    }

    hgg.addEdge(jsonString.c_str());

    while (true)
    {
        string result;
        int id1;
        int id2;
        int ch;
        cin >> ch;
        if (ch != 5 && ch != 6)
            cin >> id1 >> id2;
        else if (ch == 5)
            cin >> id1;
        switch (ch)
        {
        case 1:
        {
            result = "{\"result\": \"" + std::to_string(hgg.isAncestor(id1, id2)) + "\"}";

            break;
        }
        case 2:
        {
            string res = hgg.isSibling(id1, id2) ? "TRUE" : "FALSE";
            result = "{\"result\": \"" + res + "\"}";
            break;
        }
        case 3:
        {
            string res = hgg.isDistantRelative(id1, id2) ? "TRUE" : "FALSE";
            result = "{\"result\": \"" + res + "\"}";
            break;
        }
        case 4:
        {

            cout << "id1 " << endl;
            result = "{\"result\": \"" + hgg.findCommonAncestor(id1, id2) + "\"}";
            cout << result << endl;
            break;
        }
        case 5:
        {
            string res = to_string(hgg.findFurthestDescendant(id1));
            result = "{\"result\": \"" + res + "\"}";
            break;
        }

        case 6:
        {
            auto distantRelationship = hgg.findMostDistantRelationship();
            result = "{\"result\": \"" + distantRelationship.first + " and " + distantRelationship.second + "\"}";
            break;
        }

        case 'e':
            result = "Exiting the program.";
            break;

        default:
            result = "Invalid option. Please enter a valid option.";
            break;
        }
        cout << result << endl;
    }
}
