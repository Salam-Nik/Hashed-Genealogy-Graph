# hashed_genealogy_graph.pyx

# Include any necessary Cython and C++ headers
cdef extern from "HashedGenealogyGraph.h":
    cppclass HashedGenealogyGraph:
        HashedGenealogyGraph(bool autoSave)
        void addEdge(string n1, string ln1, int id1, string n2, string ln2, int id2, int relation)
        # Add other function declarations as needed

# Cython code here
cdef class PyHashedGenealogyGraph:
    cdef HashedGenealogyGraph* _cpp_object

    def __cinit__(self, bool autoSave):
        self._cpp_object = new HashedGenealogyGraph(autoSave)

    def __dealloc__(self):
        del self._cpp_object

    def addEdge(self, string n1, string ln1, int id1, string n2, string ln2, int id2, int relation):
        self._cpp_object.addEdge(n1, ln1, id1, n2, ln2, id2, relation)
        # Add other wrapper functions as needed
