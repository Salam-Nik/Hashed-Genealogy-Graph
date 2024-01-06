# hashed_genealogy_graph.pyx
from libcpp.string cimport string

cdef extern from "HashedGenealogyGraph.h":
    cdef cppclass HashedGenealogyGraph:
        HashedGenealogyGraph(bint autoSave)
        void addEdge(string n1, string ln1, int id1, string n2, string ln2, int id2, int relation)

# Include SHA256.h
cdef extern from "SHA256.h":
    # Add declarations from SHA256.h as needed

cdef class PyHashedGenealogyGraph:
    cdef HashedGenealogyGraph* _cpp_object

    def __cinit__(self, bint autoSave):
        self._cpp_object = new HashedGenealogyGraph(autoSave)

    def __dealloc__(self):
        del self._cpp_object

    def addEdge(self, str n1, str ln1, int id1, str n2, str ln2, int id2, int relation):
        self._cpp_object.addEdge(n1, ln1, id1, n2, ln2, id2, relation)
