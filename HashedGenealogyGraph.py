from ctypes import CDLL, c_bool, c_char_p, c_int, c_void_p, POINTER, c_char

class RelationType:
    Child = 0
    Parent = 1
    Married = 2

class HGG:
    def __init__(self, library_path = "build/libHashedGenealogyGraph.so"):
        self.lib = CDLL(library_path)

     
        self.lib.createHashedGenealogyGraph.argtypes = [c_bool]
        self.lib.createHashedGenealogyGraph.restype = c_void_p
        self.lib.addEdge.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int, c_int]
        self.lib.isAncestor.argtypes = [c_void_p, c_char_p, c_char_p]
        self.lib.isAncestor.restype = c_int
        self.lib.isSibling.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int]
        self.lib.isSibling.restype = c_int
        self.lib.isDistantRelative.argtypes = [c_void_p, c_char_p, c_char_p, c_int, c_char_p, c_char_p, c_int]
        self.lib.isDistantRelative.restype = c_int
        self.lib.findAllAncestors.argtypes = [c_void_p, c_char_p, POINTER(c_char), c_int]
        self.lib.findCommonAncestor.argtypes = [c_void_p, c_char_p, c_char_p, POINTER(c_char), c_int]
        self.lib.findFurthestDescendant.argtypes = [c_void_p, c_char_p, c_char_p, c_int]
        self.lib.findFurthestDescendant.restype = c_int
    

    def createHashedGenealogyGraph(self, auto_save):
        return self.lib.createHashedGenealogyGraph(auto_save)

    def addEdge(self, obj, n1, ln1, id1, n2, ln2, id2, relation):
        return self.lib.addEdge(obj, self.create_char_p(n1), self.create_char_p(ln1)
                                , id1, self.create_char_p(n2), self.create_char_p(ln2), id2, relation)

    def isAncestor(self, obj, person1, person2):
        return bool(self.lib.isAncestor(obj, person1, person2))

    def isSibling(self, obj, n1, ln1, id1, n2, ln2, id2):
        return bool(self.lib.isSibling(obj, self.create_char_p(n1), self.create_char_p(ln1), id1,
                                        self.create_char_p(n2), self.create_char_p(ln2), id2))

    def isDistantRelative(self, obj, n1, ln1, id1, n2, ln2, id2):
        return bool(self.lib.isDistantRelative(obj, self.create_char_p(n1), self.create_char_p(ln1)
                                , id1, self.create_char_p(n2), self.create_char_p(ln2), id2))

    def findAllAncestors(self, obj, person):
        buffer_size = 1024 
        result_buffer = (c_char * buffer_size)()
        self.lib.findAllAncestors(obj, person, result_buffer, buffer_size)
        return result_buffer.value.decode()

    def findCommonAncestor(self, obj, person1, person2):
        buffer_size = 1024  
        result_buffer = (c_char * buffer_size)()
        self.lib.findCommonAncestor(obj, person1, person2, result_buffer, buffer_size)
        return result_buffer.value.decode()

    def findFurthestDescendant(self, obj, n1, ln1, id1):
        return self.lib.findFurthestDescendant(obj, self.create_char_p(n1), self.create_char_p(ln1), id1)
    
    def create_char_p(self, string):
        return c_char_p(string.encode('utf-8'))


