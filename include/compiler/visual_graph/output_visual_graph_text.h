//===-- output_visual_graph_text.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef ECD975BF_C219_4D88_AC4A_B12DE1902F2B
#define ECD975BF_C219_4D88_AC4A_B12DE1902F2B

#include <string>
#include <sstream>

class OutputVisualGraphText
{
private:    
    std::string _outputPath;
    std::stringstream _graphContent;
    int _idCounter;
    auto getNewId() -> int;

public:
    explicit OutputVisualGraphText(const std::string& outputPath);
    auto addNode(const std::string& label) -> int;
    void addEdge(int parentId, int childId);
    void generate();
};

#endif /* ECD975BF_C219_4D88_AC4A_B12DE1902F2B */
