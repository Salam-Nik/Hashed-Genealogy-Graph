from ctypes import CDLL, c_bool, c_char_p, c_int, c_void_p, POINTER, c_char, Structure

class RelationType:
    Married = 0
    Child = 1
    Parent = 2

class TupleStrInt(Structure):
    _fields_ = [("first", c_char_p), ("second", c_char_p), ("third", c_int)]

class HGG:

    LIBRARY_PATH = "build/GenealogyGraph.so"

    def __init__(self, library_path=LIBRARY_PATH):
        self.lib = CDLL(library_path)
        self.RelationType = RelationType 
        self.setup_function_signatures()

    def setup_function_signatures(self):
        self.lib.createHashedGenealogyGraph.argtypes = [c_bool]
        self.lib.createHashedGenealogyGraph.restype = c_void_p

        self.lib.addEdge.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int, POINTER(TupleStrInt), c_int]
        
        self.lib.isAncestor.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int]
        self.lib.isAncestor.restype = c_int

        self.lib.isSibling.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int]
        self.lib.isSibling.restype = c_int

        self.lib.isDistantRelative.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int]
        self.lib.isDistantRelative.restype = c_int

        self.lib.findCommonAncestor.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int]
        self.lib.findCommonAncestor.restype = c_char_p

        self.lib.findFurthestDescendant.argtypes = [c_void_p, c_char_p, c_char_p, c_int]
        self.lib.findFurthestDescendant.restype = c_int

    def createHashedGenealogyGraph(self, auto_save):
        return self.lib.createHashedGenealogyGraph(auto_save)

    def addEdge(self, obj, n1, ln1, id1, n2, ln2, id2, children):
        children_array = (TupleStrInt * len(children))(*[
            TupleStrInt(
                self.create_char_p(child[0]), 
                self.create_char_p(child[1]), 
                child[2] 
            ) for child in children
        ])
        
        return self.lib.addEdge(
            obj,
            self.create_char_p(n1),
            self.create_char_p(ln1),
            id1,
            self.create_char_p(n2),
            self.create_char_p(ln2),
            id2,
            children_array,
            len(children)
        )


    def isAncestor(self, obj, n1, ln1, id1, n2, ln2, id2):
        return bool(self.lib.isAncestor(obj, self.create_char_p(n1), self.create_char_p(ln1),
                                       id1, self.create_char_p(n2), self.create_char_p(ln2), id2))

    def isSibling(self, obj, n1, ln1, id1, n2, ln2, id2):
        return bool(self.lib.isSibling(obj, self.create_char_p(n1), self.create_char_p(ln1),
                                       id1, self.create_char_p(n2), self.create_char_p(ln2), id2))

    def isDistantRelative(self, obj, n1, ln1, id1, n2, ln2, id2):
        return bool(self.lib.isDistantRelative(obj, self.create_char_p(n1), self.create_char_p(ln1),
                                              id1, self.create_char_p(n2), self.create_char_p(ln2), id2))

    def findCommonAncestor(self, obj, n1, ln1, id1, n2, ln2, id2):
        res = self.lib.findCommonAncestor(obj, self.create_char_p(n1), self.create_char_p(ln1), id1,
                                          self.create_char_p(n2), self.create_char_p(ln2), id2)
        return res.decode('utf-8')

    def findFurthestDescendant(self, obj, n1, ln1, id1):
        return self.lib.findFurthestDescendant(obj, self.create_char_p(n1), self.create_char_p(ln1), id1)

    def create_char_p(self, string):
        return c_char_p(string.encode('utf-8'))
