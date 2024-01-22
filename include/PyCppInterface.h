#ifndef PY_CPP_INTERFACE
#define PY_CPP_INTERFACE

#include "../include/GenealogyGraph.h"

#ifdef __cplusplus
extern "C"
{
#endif

    string cStringToString(char const *cStr);

    struct StringPair
    {
        const char *first;
        const char *second;
    };

    StringPair findMostDistantRelationship(GenealogyGraph *obj);

    GenealogyGraph *createHashedGenealogyGraph(const bool autoSave);

    void addEdge(GenealogyGraph *obj, const char *json_data);

    int isAncestor(GenealogyGraph *obj, int const id1, int const id2);

    int isSibling(GenealogyGraph *obj, int const id1, int const id2);

    int isDistantRelative(GenealogyGraph *obj, int const id1, int const id2);

    const char *findCommonAncestor(GenealogyGraph *obj, int const id1, int const id2);

    int findFurthestDescendant(GenealogyGraph *obj, int const id1);

    void deleteHashedGenealogyGraph(GenealogyGraph *obj);

#ifdef __cplusplus
}
#endif

#endif