//===-- type_simple.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB
#define D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB

#include "i_type.h"
#include <llvm/IR/Type.h>

class TypeSimple : public IType {
private:
    llvm::Type::TypeID _typeID;
    unsigned int _integerBitWidth;
    unsigned int _pointerAddressSpace;

public:
    explicit TypeSimple(llvm::Type* llvmType);
    explicit TypeSimple(llvm::Type::TypeID typeID, unsigned int bitWidth = 0, unsigned int addressSpace = 0);
    ~TypeSimple() override = default;

    TypeSimple(const TypeSimple&) = delete;
    auto operator=(const TypeSimple&) -> TypeSimple& = delete;
    TypeSimple(TypeSimple&&) = delete;
    auto operator=(TypeSimple&&) -> TypeSimple& = delete;

    auto generateLLVMType(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto isFloating() const -> bool override;
    [[nodiscard]] auto isBoolean() const -> bool override;
    [[nodiscard]] auto isString() const -> bool override;
    [[nodiscard]] auto isArray() const -> bool override { return false; }
};

#endif /* D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB */
