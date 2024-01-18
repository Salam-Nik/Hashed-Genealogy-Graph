#include <iostream>
#include "include/GenealogyGraph.h"
#include <unordered_map>
#include <vector>

using namespace std;


int main() {
    std::unordered_map<int, std::vector<std::string>> ds = {
        {18, {"Valerie", "Lewis", "18"}},
        {12, {"Billy", "Wilson", "12"}},
        {39, {"Jonathon", "Burke", "39"}},
        {8, {"Daniel", "Lawrence", "8"}},
        {34, {"Craig", "Davis", "34"}},
        {13, {"Leslie", "Liu", "13"}},
        {7, {"Tyrone", "Santos", "7"}},
        {19, {"Erin", "Austin", "19"}},
        {37, {"Kyle", "Davis", "37"}},
        {2, {"Benjamin", "Arias", "2"}},
        {30, {"Karen", "Williams", "30"}},
        {6, {"Timothy", "Thomas", "6"}},
        {38, {"Robert", "Garrison", "38"}},
        {22, {"Joseph", "Salas", "22"}},
        {47, {"Ronald", "Vance", "47"}},
        {21, {"Adam", "Rodriguez", "21"}}
    };

     GenealogyGraph graph(true);

    //  vector<int> children;
    //  children.emplace_back(21);

    // const auto& spouse1 = ds[6];
    // const auto& spouse2 = ds[0];




    // graph.addEdge(
    //     6,
    //      88,
    //     children);

    //  cout << "hi " << endl;
    // cout << graph.findFurthestDescendant(21);
    // while (true) {
    //     std::cout << "\nGenealogy Program Menu:" << std::endl;
    //     std::cout << "1. Check Ancestor" << std::endl;
    //     std::cout << "2. Check Sibling" << std::endl;
    //     std::cout << "3. Check Distant Relative" << std::endl;
    //     std::cout << "4. Find Common Ancestor" << std::endl;
    //     std::cout << "5. Find Furthest Descendant" << std::endl;
    //     std::cout << "Type 'exit' to quit" << std::endl;

    //     std::string user_input;
    //     std::cout << "Enter your choice: ";
    //     std::cin >> user_input;

    //     if (user_input == "exit") {
    //         std::cout << "Exiting the program." << std::endl;
    //         break;
    //     }

    //     if (user_input != "1" && user_input != "2" && user_input != "3" && user_input != "4" && user_input != "5") {
    //         std::cout << "Invalid choice. Please enter a valid option." << std::endl;
    //         continue;
    //     }

    //     int id1;
    //     std::cout << "Enter first ID 1: ";
    //     std::cin >> id1;
    //     const auto& info1 = ds[id1];
    //     const std::string& name1 = info1[0];
    //     const std::string& last_name1 = info1[1];

    //     int id2;
    //     std::string name2;
    //     std::string last_name2;

    //     if (user_input != "5") {
    //         std::cout << "Enter second ID 2: ";
    //         std::cin >> id2;
    //         const auto& info2 = ds[id2];
    //         name2 = info2[0];
    //         last_name2 = info2[1];
    //     }

    //     bool result;
    //     if (user_input == "1") {
    //         result = hgg.isAncestor(graph, name1, last_name1, id1, name2, last_name2, id2);
    //     } else if (user_input == "2") {
    //         result = hgg.isSibling(graph, name1, last_name1, id1, name2, last_name2, id2);
    //     } else if (user_input == "3") {
    //         result = hgg.isDistantRelative(graph, name1, last_name1, id1, name2, last_name2, id2);
    //     } else if (user_input == "4") {
    //         result = hgg.findCommonAncestor(graph, name1, last_name1, id1, name2, last_name2, id2);
    //     } else if (user_input == "5") {
    //         result = hgg.findFurthestDescendant(graph, name1, last_name1, id1);
    //     }

    //     std::cout << "\nResult: " << std::boolalpha << result << std::endl;
    // }

    return 0;
}
