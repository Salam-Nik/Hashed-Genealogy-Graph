#include "crow.h"
#include "crow/middlewares/cors.h"

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

    crow::App<crow::CORSHandler> app;

    CROW_ROUTE(app, "/")
    ([]()
     {
    auto page = crow::mustache::load_text("home.html");
    return page; });

    auto &cors = app.get_middleware<crow::CORSHandler>();

    CROW_ROUTE(app, "/menu")
        .methods("POST"_method)([&hgg](crow::request req)
                                {
        rapidjson::Document json_data;
        const char* json_str = req.body.c_str();
        
        rapidjson::ParseResult parseResult = json_data.Parse<0>(json_str);
        cout << "pars" <<json_str << endl;
        cout << "req" << req.body << endl;
        if (!parseResult) {
            return crow::response(400, "Bad Request - JSON parsing error");
        }

        string user_input = json_data["user_input"].GetString();
        string result;
        int id1 = stoi(json_data["data"]["id1"].GetString());
        int id2 = user_input[0] != '5' ? stoi(json_data["data"]["id2"].GetString()) : 0;
         cout << user_input[0] << endl;
        switch (user_input[0]) {
            case '1':{
                result = "{\"result\": \"" + std::to_string(hgg.isAncestor(id1, id2)) + "\"}";


                break;
            }
            case '2':{
                string res =  hgg.isSibling(id1, id2) ? "TRUE" : "FALSE" ;
                result = "{\"result\": \""+res+ "\"}";
                break;
            }
            case '3':{
                string res =   hgg.isDistantRelative(id1, id2) ? "TRUE" : "FALSE" ;
                result = "{\"result\": \""+res+ "\"}";
                break;
            }
            case '4':{
    
                cout << "id1 "<< endl;
                result = "{\"result\": \""+ hgg.findCommonAncestor(id1,id2)+ "\"}";
                cout <<result<< endl;
                break;
            }
            case 5: {
                string res =  to_string(hgg.findFurthestDescendant(id1));
                result = "{\"result\": \""+res+ "\"}";
                break;
            }

            case '6': {
                auto distantRelationship = hgg.findMostDistantRelationship();
                result =  "{\"result\": \""+distantRelationship.first + " and " + distantRelationship.second+ "\"}";
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
        return  crow::response{result}; });
    app.bindaddr("127.0.0.1").port(18080).run();
}
