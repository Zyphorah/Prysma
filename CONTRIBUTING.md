# Contribution Guide: Prysma Compiler

The development of Prysma relies on strict architectural rigor. The main objective is to maintain a robust, deterministic, and high-performance compiler. Any contribution must align with these engineering standards.

This document defines the protocols for submitting changes.

## 1. Reporting Issues

Before submitting a report, check that the issue has not already been documented. All reports must be factual and reproducible.

Your report must include:
* **Minimal Prysma source code** that reproduces the error.
* **Execution environment** (LLVM version, OS).
* **Diagnostic traces:** If the crash occurs during compilation, provide the LLVM IR trace. If it is a memory error, provide the AddressSanitizer (ASAN) output.

## 2. Development Standards (C++)

The core of the compiler is written in C++17. The following rules are non-negotiable:

* **Separation of concerns:** Strictly respect the decoupling between the Frontend (Lexer/Parser) and the Backend (LLVM).
* **Metaprogramming:** Do not create AST nodes manually. Any new instruction or expression must be added via the Python/Jinja2 generation system (directory `/generation`).
* **Memory management:** Internal allocation in the compiler is handled by our Arena architecture (Bump Pointer Allocator). The use of standard smart pointers (`std::shared_ptr`) is prohibited in the AST critical path for performance reasons. No memory leaks are tolerated.
* **Design Patterns:** Modifications to code generation must respect the **Visitor** pattern contract.

## 3. Submission Protocol (Pull Requests)

1. Create a fork and work on a feature branch (`feat/feature-name` or `fix/bug-name`).
2. Write explicit and structured commit messages in English (e.g., `feat: Implement string concatenation in LLVM backend` or `fix: Resolve memory leak in VTable generation`).
3. The code must compile without any warnings.
4. **Mandatory validation:** You must run the entire test suite (unit and load tests) via the orchestrator before any submission.
   ```bash
   python3 build.py
   ```