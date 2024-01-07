#include <pybind11/pybind11.h>
#include "HashedGenealogyGraph.h"
#include "sha256.h"  // Include if needed
#include <string>    // Include if needed

namespace py = pybind11;

PYBIND11_MODULE(HashedGenealogyGraph, m) {
    py::class_<HashedGenealogyGraph>(m, "HashedGenealogyGraph")
        .def(py::init<bool>())
        .def("addEdge", &HashedGenealogyGraph::addEdge)
        .def("isAncestor", py::overload_cast<const std::string&, const std::string&>(&HashedGenealogyGraph::isAncestor))
        .def("isAncestor", py::overload_cast<const std::string&, const std::string&, const int&,
                                             const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::isAncestor))
        .def("findCommonAncestor", py::overload_cast<const std::string&, const std::string&>(&HashedGenealogyGraph::findCommonAncestor))
        .def("findCommonAncestor", py::overload_cast<const std::string&, const std::string&, const int&,
                                                   const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::findCommonAncestor))
        .def("isSibling", py::overload_cast<const std::string&, const std::string&, const int&,
                                            const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::isSibling))
        .def("isDistantRelative", py::overload_cast<const std::string&, const std::string&, const int&,
                                                  const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::isDistantRelative))
        .def("findAllAncestors", &HashedGenealogyGraph::findAllAncestors)
        .def("findFurthestDescendant", py::overload_cast<const std::string&, const std::string&, const int&>(&HashedGenealogyGraph::findFurthestDescendant))
        .def("findFurthestDescendant", py::overload_cast<const std::string&>(&HashedGenealogyGraph::findFurthestDescendant));
}
