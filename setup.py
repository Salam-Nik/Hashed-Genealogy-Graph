# setup.py
from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    ext_modules = cythonize(Extension(
        "hashed_genealogy_graph",
        sources=["SHA256.cpp", "HashedGenealogyGraph.cpp", "hashed_genealogy_graph.pyx"],  
        language="c++",
        extra_compile_args=["-std=c++11"],
    ))
)
