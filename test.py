from HashedGenealogyGraph import HGG, RelationType
import json



# children_data = [
#     ("Bob", "Johnson", 4),
#     ("Eve", "Taylor", 5)
# ]

# ds = {13 : ["Leslie",  "Liu", 13], 2: [ "Benjamin", "Arias", 2], 37:["Kyle","Davis", 37], 47:["Ronald","Vance", 47],
#       18 : [ "Valerie","Lewis", 18]}
# result_ancestor = hgg.isAncestor(graph, "Leslie",  "Liu", 13,  "Benjamin", "Arias", 2)
# print("Is Ancestor:", result_ancestor)
# result_ancestor = hgg.isAncestor(graph,  "Kyle","Davis", 37,   "Ronald","Vance", 47)
# print("Is Ancestor:", result_ancestor)
# result_ancestor = hgg.isAncestor(graph,  "Kyle","Davis", 37,   "Ronald","Vance", 47)
# print("Is Ancestor:", result_ancestor)
# result_ancestor = hgg.isAncestor(graph, "Leslie",  "Liu", 13, "Kyle","Davis", 37)
# print("Is Ancestor:", result_ancestor)
# result_ancestor = hgg.isAncestor(graph, "Valerie","Lewis", 18,  "Kyle","Davis", 37)
# print("Is Ancestor:", result_ancestor)
# result_distant_relative = hgg.isDistantRelative(graph, "John", "Doe", 1, "Eve", "Taylor", 5)
# result_sibling = hgg.isSibling(graph, "Eve", "Taylor", 5, "Bob", "Johnson", 4)

# result_common_ancestor = hgg.findCommonAncestor(graph, "Eve", "Taylor", 5, "Bob", "Johnson", 4)
# result_furthest_descendant = hgg.findFurthestDescendant(graph, "John", "Doe", 1)

# print("Is Ancestor:", result_ancestor)
# print("Is Sibling:", result_sibling)
# print("Is Distant Relative:", result_distant_relative)
# print("Common Ancestor:", result_common_ancestor)
# print("Furthest Descendant:", result_furthest_descendant)

def main():
    ds ={
        18: ['Valerie', 'Lewis', 18],
        12: ['Billy', 'Wilson', 12],
        39: ['Jonathon', 'Burke', 39],
        8: ['Daniel', 'Lawrence', 8],
        34: ['Craig', 'Davis', 34],
        13: ['Leslie', 'Liu', 13],
        7: ['Tyrone', 'Santos', 7],
        19: ['Erin', 'Austin', 19],
        37: ['Kyle', 'Davis', 37],
        2: ['Benjamin', 'Arias', 2],
        30: ['Karen', 'Williams', 30],
        6: ['Timothy', 'Thomas', 6],
        38: ['Robert', 'Garrison', 38],
        22: ['Joseph', 'Salas', 22],
        47: ['Ronald', 'Vance', 47],
        21: ['Adam', 'Rodriguez', 21],
     }

    hgg = HGG()

    graph = hgg.createHashedGenealogyGraph(auto_save=True)
    
    filename ="dataset/genealogy_dataset.json"
    with open(filename, 'r') as file:
        data = json.load(file)


    for marriage in data["marriages"]:
        spouse1 = marriage["spouse1"]
        spouse2 = marriage["spouse2"]
        children = marriage["children"]

        children_data = [(child["name"], child["last_name"], int(child["id"])) for child in children]

        hgg.addEdge(graph, spouse1["name"],spouse1["last_name"], spouse1["id"],
                    spouse2["name"],spouse2["last_name"], spouse2["id"], children_data)

    while True:
        print("\nGenealogy Program Menu:")
        print("1. Check Ancestor")
        print("2. Check Sibling")
        print("3. Check Distant Relative")
        print("4. Find Common Ancestor")
        print("5. Find Furthest Descendant")
        print("Type 'exit' to quit")

        user_input = input("Enter your choice: ")

        if user_input == "exit":
            print("Exiting the program.")
            break

        if user_input not in ["1", "2", "3", "4", "5"]:
            print("Invalid choice. Please enter a valid option.")
            continue

        id1 = int(input("Enter first ID 1: "))
        name1 = ds[id1][0]
        last_name1 = ds[id1][1]
        if user_input != "5":
            id2 = int(input("Enter second ID 2: "))
            name2 = ds[id2][0]
            last_name2 = ds[id2][1]

        if user_input == "1":
            result = hgg.isAncestor(graph, name1, last_name1, id1, name2, last_name2, id2)
        elif user_input == "2":
            result = hgg.isSibling(graph, name1, last_name1, id1, name2, last_name2, id2)
        elif user_input == "3":
            result = hgg.isDistantRelative(graph, name1, last_name1, id1, name2, last_name2, id2)
        elif user_input == "4":
            result = hgg.findCommonAncestor(graph, name1, last_name1, id1, name2, last_name2, id2)
        elif user_input == "5":
            result = hgg.findFurthestDescendant(graph, name1, last_name1, id1)

        print(f"\nResult: {result}")


if __name__ == "__main__":
    main()