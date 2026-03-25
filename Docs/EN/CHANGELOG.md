# Development Journal - Architecture and Technical Decisions (Prysma)

## 1. Redesign of the Abstract Syntax Tree (AST) and Visitors
* **Dependency Injection (General Visitor) :** The constructor-based context passing required modifications in over 30 distinct places for a single feature, generating bugs related to missing or uninitialized objects. Directly injecting `ContextGenCode` into the visitor centralizes the toolkit. This architecture eliminates the tedious transmission by parsers and reduces debugging time.
* **Mutual Recursion :** The old equation extraction algorithm, based on manual scanning of commas and parentheses, was inefficient and caused crashes (e.g., integrating a function call into an addition). Replacing it with a mutual recursion system between `BuilderTreeEquation` and `BuilderTreeInstruction` naturally delegates the stop logic and resolution of nested expressions.
* **Typing and Abstraction :** The use of LLVM and reflection to guess types has been removed. The register system now encapsulates `llvm::Value*` with an `IType*` abstraction. The compiler knows exactly the type and memory address, eliminating the rigidity of typing.
* **Dynamic Node Generation :** Replacing manual nodes with an automatic factory using an abstract YAML file speeds up node addition. Label deduction by types has been automated in the Python script, cleaning up `ast.yaml`.
* **Replacement of Dynamic Casts :** `dynamic_cast` has been removed in favor of dynamic AST code generation via automatically generated enums, increasing node identification speed.

## 2. Memory Management and LLVM
* **Arena Allocator (Bump Pointer) :** Memory management has migrated from `std::shared_ptr` to an arena allocation system provided by LLVM. The conversion to raw pointers (`INode*`) makes memory contiguous. L1/L2 cache performance is optimized, fragmentation is reduced, and the risk of memory leaks is limited. 
* **Redundant Memory Jumps :** Fixing an architecture error on pointer loading resolved segmentation faults. When passing values as function arguments, the system now only copies the pointer (8 bytes) instead of the entire structure.
* **Composition-based Arrays :** The architecture of arrays has been changed to use composition instead of inheritance. Using dynamic sizes rather than hard-coded values (e.g., `constValue`) offers total flexibility for variable storage.
* **LLVM Decompilation :** Analyzing intermediate LLVM machine code via decompilation served as the main documentary basis for implementing complex structures (e.g., `if/else` branching logic, byte alignment, array behavior in memory).

## 3. Optimizations and Workflow
* **CMake and Clang Compiler :** Abandoning outdated `Makefile` (which required two `cmake` then `make` passes causing conflicts) and exclusive configuration under Clang improved error accuracy. Integrating the `glob` function into `CMakeLists.txt` eliminated the manual addition of include paths, allowing for direct time savings.
* **Multithreading and LLVM::ThreadPool :** The initial use of `std::thread` caused uncontrolled thread creation (e.g., 2000 threads for 2000 files). Migrating to `llvm::ThreadPool` delegates instantiation management to the pool, stabilizing mass compilation. Separating the orchestrator into two passes prevents access conflicts.
* **Thread Safety of Registers :** Extracting LLVM objects (`llvm::Value`, `llvm::Function`, `llvm::AllocaInst`) from register construction resolved *thread safety* issues. Static polymorphism has been implemented for the conditional application of `std::mutex`.
* **Sanitizer (ASAN) :** Implementing AddressSanitizer adds allocation flags allowing immediate display of memory leaks at *runtime*. 
* **VTable and Classes :** The *vtable* constructor has been moved to pass 2 in `builderEnvironnementFunction.cpp`. During the first pass, function pointers do not yet exist in memory, leading to the filling of an empty register.

## 4. Major Documented Bug Fixes
* **Float Assignment Bug :** The segmentation fault when assigning a 64-bit float to a 32-bit float was identified thanks to LLVM IR's alignment attributees. A dynamic *casting* system has been implemented.
* **Argument Scope Bug (If) :** The nested call `call testArgDanIf(param)` caused memory corruption. The internal function emptied the shared `RegistryArgument` and corrupted the return value expected by the parent function. Fixed by strict management of register flushing.
* **Variable Synchronization Bug :** Reads have been moved down to the node level rather than in the parser to resolve desynchronizations when searching in the `RegistryVariable`.
* **Identifiers Bug (GraphViz) :** Replacing static node identifiers (ID) with a stack (`std::stack`) in the GraphViz visitor fixed a bug where the old system overwrote IDs in deep trees, distorting the visual representation.

## 5. Algorithmic and Design 
* **Keywords and Constexpr :** Type checking in the parser and keyword definition of the lexer have migrated from a `std::unordered_set` to static `constexpr` arrays, optimizing compilation time.
* **Templates Matcher :** The variadic *template matcher* system has been initiated to automate the generation of repetitive code.
* **Technical Debt :** Pragmatic decisions have been made, such as postponing the implementation of linked lists. Realistic algorithms have been prioritized over the instability of pointer management in the compiler under development. Hard-coded values (e.g., return types) have been gradually eliminated thanks to the `RegistryType` and introspection.

## 6. Debugging Strategy with LLVM

* **LLVM IR Decompilation for documentation :** I adopted a technique of compiling C++ code to extract the LLVM code generated by version 18 exactly. This allows me to better understand the subtleties of LLVM and avoid hallucinations from AIs that often generalize with examples from earlier versions (LLVM 14 for example) that do not work with my current version.

* **Sanitizers for invisible debugging :** Segmentation faults caused by incorrect memory alignments (float 64-bit vs 32-bit) were invisible. Implementing AddressSanitizer (--sanitize from clang) immediately identified memory leaks and corruptions at runtime.

## 7. Next priorities for the following iterations

* **Allow storage of function returns :** I need to implement the ability to store the result of a function call in a variable. This is a major missing feature for the next iteration.

* **Passing raw numbers as arguments :** The bug on `call(1,1)` calls remains to be fixed. Currently, only variables pass correctly; numeric constants are blocked.

* **Object-Oriented Programming :** The class system with polymorphism via vtable and the standard library (linked lists, binary trees, stacks, queues) are the major challenges of future iterations.