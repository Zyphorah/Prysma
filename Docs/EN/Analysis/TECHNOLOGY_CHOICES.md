# Technical Analysis: Prysma Project

This document provides a detailed analysis of the development of the Prysma language, covering theoretical aspects, functional requirements, low-level challenges, and the compiler architecture.

## 1. Project Objectives

The main objective is to develop a language named "Prysma" replicating modern features. The compiler is written in C++. For code generation, three approaches were analyzed:

- **LLVM IR (Chosen):** Intermediate Representation in SSA (Single Static Assignment). Facilitates optimizations and multi-target machine code generation.
- **Manual x86-64 Backend:** Offers full control but requires complex management of registers and the ABI.
- **Transpilation to C:** Simple to implement but limits fine-grained control over specific optimizations.

Note: LLVM IR is an intermediate representation (IR) in SSA, target-independent, facilitating optimizations and code generation for different architectures. Using LLVM IR avoids having to manage all the complexities of machine code generation manually.

Example of target LLVM IR:

```llvm
%1 = load i32, i32* %ptr_x        ; load x from %ptr_x
%resultat = add i32 %1, 5         ; add 5 to the loaded value
```

## 2. Machine Code Generation Challenges

### Issues with a Custom Backend (x86-64 Linux)

1. **Register Management:** The x86-64 architecture offers 16 general-purpose registers. Efficient usage and allocation must be managed to avoid conflicts and unnecessary saves.
2. **Register Allocation:** Requires sophisticated algorithms (graph coloring, Linear Scan) or management of stack spilling.
3. **Symbol Table and Addressing:** Involves manual management of addresses and stack frame offsets.
4. **Calling Conventions (ABI):** Manual implementation of argument passing, stack alignment, and return value (System V AMD64).
5. **Encapsulation and Visibility:** Requires conventions (prefixes, sections) to control symbol visibility at link time.
6. **Complexity:** Writing and maintaining an assembler backend is more time-consuming and error-prone than using an established backend.

### Advantages of the LLVM Infrastructure

- **Abstraction and Portability:** The compiler can target multiple architectures without major modifications.
- **Optimizations:** LLVM integrates many advanced optimizations automatically applied to the intermediate code.
- **Ecosystem:** Rich in tools (static analysis, DWARF debug symbol generation).
- **Register Management:** LLVM uses virtual registers in SSA form, simplifying allocation.

## 3. Inspiring Similar Projects

- Rust: A systems programming language focused on safety and performance. It uses LLVM as a backend for compilation. It is a very solid language and has proven itself in the industry. Moreover, the language is at an advanced stage of maturity, and the compiler is written in Rust itself to benefit from its own memory safety features, demonstrating reliability. Building a large project shows its robustness.
GitHub project URL: https://github.com/rust-lang/rust.git

- Kaleidoscope: An educational programming language developed by the LLVM team to teach basic compilation concepts using LLVM as a backend. It is a good example that shows the steps to build a functional compiler. All steps are explained and documented, which makes understanding easier. Tutorial URL: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/

- GCC: A very popular open-source compiler, it allows compiling C, C++, Fortran, etc. It is widely used in the industry, is a mature and robust project. Interesting to study for understanding the construction of an advanced compiler, although its components are very complex. GitHub project URL: https://github.com/gcc-mirror/gcc.git
