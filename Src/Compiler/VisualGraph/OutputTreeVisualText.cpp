#include "Compiler/VisualGraph/OutputVisualGraphText.h"
#include <fstream>
#include <stdexcept>
#include <string>

OutputVisualGraphText::OutputVisualGraphText(const std::string& cheminOutput)
    : _cheminOutput(cheminOutput), _compteurId(0) {}

auto OutputVisualGraphText::ajouterNode(const std::string& label) -> int {
    int nodeId = obtenirNouvelId();
    _contenuGraph << "    node" << nodeId 
                   << " [label=\"" << label << "\", shape=ellipse, style=filled, fillcolor=lightgreen];\n";
    return nodeId;
}

void OutputVisualGraphText::ajouterArete(int parentId, int childId) {
    _contenuGraph << "    node" << parentId << " -> node" << childId << ";\n";
}

void OutputVisualGraphText::generatedr() {
    std::ofstream fichier(_cheminOutput);
    
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + _cheminOutput);
    }
    
    fichier << "digraph AST {\n";
    fichier << "    rankdir=TB;\n";
    fichier << "    node [fontname=\"Arial\"];\n";
    fichier << _contenuGraph.str();
    fichier << "}\n";
    
    fichier.close();
}

auto OutputVisualGraphText::obtenirNouvelId() -> int {
    return _compteurId++;
}
