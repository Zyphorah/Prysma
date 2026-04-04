//===-- members_extractor_class.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef D3A3E436_A6F0_4651_9DE8_DD58645AE33C
#define D3A3E436_A6F0_4651_9DE8_DD58645AE33C

#include "compiler/visitor/visitor_base_generale.h"
#include <vector>
#include <string>

class NodeDeclarationFunction;
class NodeDeclarationVariable;
class NodeClass;

// Extracts members (methods and variables) from a class node
class MembersExtractorClass : public VisitorBaseGenerale {
private:
    std::vector<NodeDeclarationFunction*> methods;
    std::vector<NodeDeclarationVariable*> variables;
    std::string className;

public:
    [[nodiscard]] auto getMethods() const -> const std::vector<NodeDeclarationFunction*>& { return methods; }
    [[nodiscard]] auto getVariables() const -> const std::vector<NodeDeclarationVariable*>& { return variables; }
    [[nodiscard]] auto getClassName() const -> const std::string& { return className; }
    using VisitorBaseGenerale::visiter;
    void visiter(NodeDeclarationFunction* node) override;
    void visiter(NodeDeclarationVariable* node) override;
    void visiter(NodeClass* node) override;
};

#endif /* D3A3E436_A6F0_4651_9DE8_DD58645AE33C */
