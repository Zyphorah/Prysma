# Prysma Programming Language - Development Roadmap

This document details the current capabilities of the Prysma language as well as future development directions. The project is structured around a modular architecture using LLVM as the compilation backend.

## 1. Current State of the Project (Stable)

### Core Language & Syntax
* **Primitive Types :** Full support for integers (`int32`), floats (`float`), and booleans (`bool`).
* **Arithmetic :** Implementation of basic operations (`+`, `-`, `*`, `/`) with management of operator precedence and parenthesization.
* **Control Structures :**
    * `if-else` conditional statements with complex boolean expressions.
    * `while` loops with stack management via `alloca`.
* **Functions :** Support for recursive functions, passing arguments by value, and managing return types.
* **Identifier Management :** Variable declaration, assignment, and scope resolution.

### Object-Oriented Programming (OOP)
* **Class Model :** Definition of data structures and behaviors.
* **Encapsulation :** Management of access levels `public`, `private`, and `protected`.
* **Inheritance :** Class hierarchies and interfaces.
* **Polymorphism :** Implementation of a dynamic **VTable** for resolving virtual methods at runtime.

### Memory Management
* **Dynamic Allocation :** Integration of `prysma_malloc` and `free` from the standard C library.
* **System Operators :** Implementation of the `new` and `delete` keywords for manual heap control.
* **Internal Optimization :** Use of a **Bump Pointer Allocator** (Arena) for managing AST nodes, reducing fragmentation, and increasing compilation performance.

## 2. Infrastructure & Tools

* **Multi-threaded Compilation :** Orchestration of compilation via `llvm::ThreadPool` for parallel processing of source files.
* **Dependency Management :** File inclusion system (`#include`) for code modularity.
* **Diagnostics :** Precise syntax analyzer reporting errors by line and column.
* **Visualization :** Export of the Abstract Syntax Tree (AST) in text and graphic formats (Graphviz/DOT).
* **IDE Ecosystem :** VS Code extension for native Prysma syntax highlighting.

## 3. Development Roadmap (Planned Features)

### Short Term (v1.1)
* **Standard Library (StdLib) :** Development of a library of native data structures (linked lists, stacks, queues).
* **C Interoperability :** Automatic import system to link external C libraries without manual configuration.
* **Built-in Functions :** Integration of advanced mathematical functions (power, GCD) at the runtime level.

### Medium Term (v1.2)
* **Advanced Memory Management :** Introduction of pointers for granular low-level control.
* **Namespaces & Modules :** Organization of code into isolated compilation units to avoid name conflicts.
* **Generics :** Support for generic programming (templates) for functions and classes.
* **Extended Semantic Analysis :** Strengthening of type checking to prevent logic errors at compile time.

### Long Term (Vision)
* **JIT Compilation (Just-In-Time) :** Immediate execution mode for scripting.
* **SSA Optimizations :** Custom optimization passes on LLVM IR.
* **Inline Assembly :** `asm` block for direct injection of processor instructions.
* **Meta-programming :** Macros and code execution at compile time (Compile-time execution).