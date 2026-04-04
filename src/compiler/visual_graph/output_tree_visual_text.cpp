//===-- output_tree_visual_text.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visual_graph/output_visual_graph_text.h"
#include <fstream>
#include <stdexcept>
#include <string>

OutputVisualGraphText::OutputVisualGraphText(const std::string& outputPath)
    : _outputPath(outputPath), _idCounter(0) {}

auto OutputVisualGraphText::addNode(const std::string& label) -> int {
    int nodeId = getNewId();
    _graphContent << "    node" << nodeId 
                  << " [label=\"" << label << "\", shape=ellipse, style=filled, fillcolor=lightgreen];\n";
    return nodeId;
}

void OutputVisualGraphText::addEdge(int parentId, int childId) {
    _graphContent << "    node" << parentId << " -> node" << childId << ";\n";
}

void OutputVisualGraphText::generate() {
    std::ofstream file(_outputPath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + _outputPath);
    }
    
    file << "digraph AST {\n";
    file << "    rankdir=TB;\n";
    file << "    node [fontname=\"Arial\"];\n";
    file << _graphContent.str();
    file << "}\n";
    
    file.close();
}

auto OutputVisualGraphText::getNewId() -> int {
    return _idCounter++;
}
