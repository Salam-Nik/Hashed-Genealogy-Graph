import os
import ctypes

# Get the current script directory
script_dir = os.path.dirname(os.path.abspath(__file__))

# Get the library path
lib_path = os.path.join(script_dir, "HashedGenealogyGraph.so")

# Load the shared library
HGG = ctypes.CDLL(lib_path)

# Create an instance of HashedGenealogyGraph
graph = HGG.HashedGenealogyGraph(True)  # Pass the required parameters to the constructor

# Now you can use the 'graph' object as intended
