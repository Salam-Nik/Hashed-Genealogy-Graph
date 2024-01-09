from HashedGenealogyGraph import HGG, RelationType

hgg = HGG()

graph = hgg.createHashedGenealogyGraph(auto_save=True)

children_data = [
    ("Bob", "Johnson", 4),
    ("Eve", "Taylor", 5)
]

hgg.addEdge(graph, "John", "Doe", 1, "Jane", "Doe", 2, children_data)

result_ancestor = hgg.isAncestor(graph, "John", "Doe", 1, "Jane", "Doe", 2)
result_sibling = hgg.isSibling(graph, "Eve", "Taylor", 5, "Bob", "Johnson", 4)
result_distant_relative = hgg.isDistantRelative(graph, "John", "Doe", 1, "Eve", "Taylor", 5)
result_common_ancestor = hgg.findCommonAncestor(graph, "Eve", "Taylor", 5, "Bob", "Johnson", 4)
result_furthest_descendant = hgg.findFurthestDescendant(graph, "John", "Doe", 1)

print("Is Ancestor:", result_ancestor)
print("Is Sibling:", result_sibling)
print("Is Distant Relative:", result_distant_relative)
print("Common Ancestor:", result_common_ancestor)
print("Furthest Descendant:", result_furthest_descendant)