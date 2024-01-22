// #include "../include/PyCppInterface.h" 
// #ifdef __cplusplus
// extern "C"
// {
// #endif


//     string cStringToString(char const *cStr)
//     {
//         return (cStr != nullptr) ? string(cStr) : string();
//     }

//     StringPair findMostDistantRelationship(GenealogyGraph *obj)
//     {
//         auto vec = obj->findMostDistantRelationship();
//         const char *res1 = vec.first.c_str();
//         const char *res2 = vec.second.c_str();

//         return {res1, res2};
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