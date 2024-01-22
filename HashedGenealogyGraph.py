import json
from ctypes import CDLL, c_bool, c_char_p, c_int, c_void_p, c_char, Structure

class TupleStrInt(Structure):
    _fields_ = [("first", c_char_p), ("second", c_char_p), ("third", c_int)]

class TupleStrStr(Structure):
    _fields_ = [("first", c_char_p), ("second", c_char_p)]

class HGG:

    LIBRARY_PATH = "build/PyCppInterface.so"

    def __init__(self, library_path=LIBRARY_PATH):
        try:
            self.lib = CDLL(library_path)
        except OSError as e:
             print(f"Error loading library: {e}")
        self.setup_function_signatures()

    def setup_function_signatures(self):
        
        self.lib.findMostDistantRelationship.argtypes = [c_void_p]
        self.lib.findMostDistantRelationship.restype = TupleStrStr

        self.lib.createHashedGenealogyGraph.argtypes = [c_bool]
        self.lib.createHashedGenealogyGraph.restype = c_void_p

        self.lib.isAncestor.argtypes = [c_void_p, c_int, c_int]
        self.lib.isAncestor.restype = c_void_p

        self.lib.isAncestor.argtypes = [c_void_p, c_int, c_int]
        self.lib.isAncestor.restype = c_int

        self.lib.isSibling.argtypes = [c_void_p, c_int, c_int]
        self.lib.isSibling.restype = c_int

        self.lib.isDistantRelative.argtypes = [c_void_p, c_int, c_int]
        self.lib.isDistantRelative.restype = c_int

        self.lib.findCommonAncestor.argtypes = [c_void_p, c_int, c_int]
        self.lib.findCommonAncestor.restype = c_char_p

        self.lib.findFurthestDescendant.argtypes = [c_void_p, c_int]
        self.lib.findFurthestDescendant.restype = c_int

    def findMostDistantRelationship(self, obj):
        result = self.lib.findMostDistantRelationship(obj)
        return (result.first.decode('utf-8'), result.second.decode('utf-8'))

    def createHashedGenealogyGraph(self, autoSave=True):
        return self.lib.createHashedGenealogyGraph(c_bool(autoSave))

    def addEdge(self, obj,json_data):
        try:
            # Convert Python dictionary to JSON string
            json_str = json.dumps(json_data)

            # Convert JSON string to C-style string (char*)
            c_json_str = c_char_p(json_str.encode('utf-8'))

            # Call the C++ addEdge function with the C-style string
            return self.lib.addEdge(obj, c_json_str)
        except OSError as e:
            print(f"Error loading library: {e}")



    def isAncestor(self, obj, id1, id2):
        return bool(self.lib.isAncestor(obj, id1, id2))

    def isSibling(self, obj, id1, id2):
        return bool(self.lib.isSibling(obj, id1, id2))

    def isDistantRelative(self, obj, id1, id2):
        return bool(self.lib.isDistantRelative(obj, id1, id2))

    def findCommonAncestor(self, obj, id1, id2):
        res = self.lib.findCommonAncestor(obj, id1, id2)
        return res.decode('utf-8')

    def findFurthestDescendant(self, obj, id1):
        return self.lib.findFurthestDescendant(obj, id1)

    # def create_char_p(self, string):
    #     return c_char_p(string.encode('utf-8'))

