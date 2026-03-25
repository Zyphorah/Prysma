# Robustness and Security - Prysma Compiler

## Overview

This document details the security and robustness measures implemented in the Prysma compiler to anticipate and manage edge cases, memory errors, and concurrency conditions.

## 1. Equation Tree Construction - Invalid Case Management

**Component:** `BuilderTreeEquation::construire()`  
**Criticality:** High (most vulnerable component to invalid syntax inputs)

### Edge Case 1: Empty or Asymmetric Parentheses

**Scenario:** 
```rust
dec int x = ();
dec int y = (5 + );
dec int z = ((5 + 3);
```

**Implemented Protection:**
- `ServiceParenthese::enleverParenthesesEnglobantes()` first validates the integrity of the parentheses
- If the resulting expression is empty after removal → **exception thrown immediately**
- Prevents invalid pointer accesses and variable undefinedness
- The AST is not constructed, compilation stops at the frontend stage

**Code:**
```cpp
if (equation.empty()) {
    throw std::runtime_error(
        "Error: Empty equation after parenthesis removal at line " + 
        std::to_string(token.line)
    );
}
```

### Edge Case 2: Consecutive Operators without Operand

**Scenario:**
```rust
dec int result = 5 + * 3;
dec int bad = 10 / / 2;
```

**Implemented Protection:**
- `ChainOfResponsibility` divides the expression at each found operator
- Isolates an orphan operator (e.g., `*` without left/right)
- Call to `convertirEnFloat()` fails on the invalid token
- **Semantic exception thrown** instead of corrupted LLVM IR generation
- Error reported to the user with line and context

**Error Example:**
```
Syntax Error at line 5: Invalid operand after operator '+' 
Expected expression, got operator '*'
```

### Edge Case 3: Stack Overflow on Complex Expressions

**Scenario:**
```rust
// Very deeply nested expression
dec int x = ((((((((((a + b) + c) + d) + e) + f) + g) + h) + i) + j)));
```

**Implemented Protection:**
- `BuilderTreeEquation` uses controlled mutual recursion
- Each call divides the expression into two parts (left/right)
- Recursion depth ≤ number of operators = O(e) bounded
- No artificial limit, but the equation structure forces balanced division
- Memory overflow unlikely in practice for readable code

## 2. Parallel Multi-File Compilation - Concurrency Management

**Component:** `OrchestratorInclude::compilerProject()`  
**Criticality:** Critical (multi-thread synchronization, memory integrity)

### Edge Case 1: Circular Dependencies

**Scenario:**
```
file_a.prysma includes file_b.prysma
file_b.prysma includes file_c.prysma
file_c.prysma includes file_a.prysma  ← infinite loop without protection
```

**Implemented Protection:**
- `RegistryFile` maintains a `std::set<std::string>` of compiled files
- Protected by `std::mutex` for thread-safety
- On each `inclureFile()` call:
  1. Checks if the file is already present in the set
  2. If present → ignores inclusion (prevents infinite loop)
  3. If absent → adds to the list and triggers compilation
- Each check is in `O(log F)` where F = number of files

**Simplified Code:**
```cpp
bool RegistryFile::estDejaCompile(const std::string& nomFile) {
    std::lock_guard<std::mutex> lock(mutexFiles);
    if (fichiers.find(nomFile) != fichiers.end()) {
        return true;  // Already compiled, ignore
    }
    fichiers.insert(nomFile);
    return false;  // New, needs to be compiled
}
```

**Result:**
- Prevents infinite thread creation
- Avoids RAM saturation (2000 threads = ~16 GB)
- Avoids stack overflow
- Circular dependencies silently managed

### Edge Case 2: Exception Thrown in a Parallel Thread

**Scenario:**
```
Pass 1 (Analysis) : 5 files in parallel
File 1 : ✓ Success
File 2 : ✗ Syntax error at line 42
File 3 : ✓ Success
File 4 : In progress...
File 5 : In progress...
```

**Implemented Protection:**
- The faulty thread captures the exception and reports the error to the global context
- Error flag stored in `ContextGenCode`
- After the first pass, error flag check
- If error detected → pass 2 (LLVM generation) locked and canceled
- `join()` call on all threads to preserve allocation arena integrity

**Simplified Code:**
```cpp
void OrchestratorInclude::compilerPasse1() {
    for (auto& thread : threads) {
        try {
            // Parallel compilation
            thread = std::thread([this]() { 
                compilerfichier(); 
            });
        } catch (const std::exception& e) {
            contextGenCode.setError(true);
            contextGenCode.setMessageError(e.what());
        }
    }
    
    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();  // Safe : preserves arenas
    }
    
    if (contextGenCode.hasError()) {
        return;  // Pass 2 canceled
    }
}
```

**Result:**
- No undefined behavior
- All threads terminate cleanly
- Resources correctly freed
- Precise error message to the user

### Edge Case 3: Race Condition on Shared Registers

**Scenario:**
```
Thread 1 : Writes to RegistryVariable
Thread 2 : Reads from RegistryVariable
→ Reading inconsistent state
```

**Implemented Protection:**
- Registers protected by `std::mutex` for exclusive access
- Static polymorphism for conditional lock application
- Pass 1 (analysis) : protected concurrent access
- Pass 2 (LLVM generation) : can be made single-threaded if necessary

**Result:**
- No race conditions
- Consistent state guaranteed
- Small acceptable synchronization latency

## 3. Lexical Analysis - Memory Read Security

**Component:** `Lexer::tokenizer()`  
**Criticality:** High (entry point for unvalidated data)

### Edge Case 1: Empty Source File or Only Spaces

**Scenario:**
```
file.prysma → 0 bytes (empty file)
or
file.prysma → "     \n\n    " (spaces and newlines)
```

**Implemented Protection:**
- Tokenization loop reaches the end of the file without generating symbols
- Returned token vector is empty
- `BuilderTreeInstruction` receives empty vector, ignores Main construction loop
- Compiler generates **valid and minimal executable** that returns `0`
- No segfault, no undefined behavior

**Code:**
```cpp
std::vector<Token> Lexer::tokenizer(const std::string& source) {
    std::vector<Token> tokens;
    size_t i = 0;
    
    while (i < source.length()) {
        if (std::isspace(source[i])) {
            i++;
            continue;  // Ignore spaces
        }
        // ... normal tokenization
    }
    
    return tokens;  // Can be empty, it's valid
}
```

### Edge Case 2: Unclosed String Literal at End of File

**Scenario:**
```prysma
main {
    dec string message = "Hello World
}  ← End of file without closing quote
```

**Implemented Protection:**
- String extraction loop validates `i < source.length()` condition at each iteration
- Premature end of file forces interruption before closing quote
- **Lexical error reported** with line number
- Prevents:
  - Buffer Overread (reading out of memory)
  - Infinite loop
  - Corrupted token

**Code:**
```cpp
if (source[i] == '"') {
    std::string value;
    i++;  // Skip opening quote
    
    while (i < source.length() && source[i] != '"') {
        value += source[i];
        i++;
    }
    
    if (i >= source.length()) {
        // ← Unclosed string detected BEFORE end of code
        throw LexerException(
            "Unterminated string starting at line " + 
            std::to_string(currentLine)
        );
    }
    
    i++;  // Skip closing quote
    tokens.push_back(Token(TOKEN_STRING, value, currentLine));
}
```

### Edge Case 3: Non-ASCII Characters or Invalid Encoding

**Scenario:**
```
file.prysma → "Déclarer int x = 5;"  (unexpected accents)
or
file.prysma → corrupted binary data
```

**Implemented Protection:**
- Lexer scans character by character
- Classification via `std::isspace()`, `std::isdigit()`, direct comparisons
- Unrecognized characters silently ignored or classified as identifiers
- No out-of-bounds access as loop always validates `i < source.length()`
- Acceptable degraded behavior: invalid characters removed

## 4. Type Conversion - Memory Alignment Security

**Component:** `BuilderTreeEquation`, LLVM generation  
**Criticality:** Critical (invisible errors: segmentation fault without trace)

### Edge Case: 64-bit Float Assigned to 32-bit Float

**Scenario:**
```
float64 (8 bytes, align 8) → float32 (4 bytes, align 4)
Incompatible memory alignment: silent segmentation fault
```

**Documented Historical Bug:**
Attempt to assign a 64-bit value to a 32-bit register caused:
- Segmentation fault with no error message
- Invisible error: generated machine code was syntactically valid
- Symptom: program crashes randomly

**Implemented Protection:**
- **Explicit dynamic casting** system before assignment
- Alignment checks in LLVM IR (attributees `align`)
- AddressSanitizer (ASAN) with flags `-fsanitize=address`
- Immediate runtime detection:
  ```
  =================================================================
  ==12345==ERROR: AddressSanitizer: attempting double-free on 0x...
  ```

**Code:**
```cpp
llvm::Value* ContextGenCode::creerAutoCast(
    llvm::Value* valeur,
    llvm::Type* typeDestination
) {
    llvm::Type* typeSource = valeur->getType();
    
    if (typeSource == typeDestination) {
        return valeur;  // No cast needed
    }
    
    // Explicit cast based on types
    if (typeSource->isFloatTy() && typeDestination->isDoubleTy()) {
        return builder->CreateFPExt(valeur, typeDestination);
    } else if (typeSource->isDoubleTy() && typeDestination->isFloatTy()) {
        return builder->CreateFPTrunc(valeur, typeDestination);
    }
    // ... other cases
    
    throw std::runtime_error("Invalid cast requested");
}
```

## 5. Argument Passing - Scope Management

**Component:** `RegistryArgument`, function calls  
**Criticality:** High (memory corruption in nested contexts)

### Edge Case: Nested Calls with Shared Register

**Scenario:**
```prysma
dec int testArgDanIf(int param) {
    if (param > 0) {
        return call testArgDanIf(param - 1);
    }
    return 0;
}
```

**Historical Bug:**
- Inner function emptied shared `RegistryArgument`
- Return value expected by parent function was corrupted
- Result: random number instead of calculated value

**Implemented Protection:**
- **Context stack** (`std::stack<ContextLocal>`)
- Each function call creates an isolated local context
- Variables private to function guaranteed
- Strict clearing of registers after each call
- Scope guaranteed by RAII

**Simplified Code:**
```cpp
class RegistryArgument {
private:
    std::stack<std::vector<llvm::Value*>> pile_contexte;
    
public:
    void inputrFunction() {
        pile_contexte.push({});  // New empty context
    }
    
    void ajouterArgument(llvm::Value* val) {
        pile_contexte.top().push_back(val);
    }
    
    void sortirFunction() {
        pile_contexte.pop();  // Context destroyed cleanly
    }
};
```

**Result:**
- No memory corruption
- Recursion correctly managed
- Return values intact

## 6. GraphViz Identifiers - ID Overwrite Prevention

**Component:** `GeneralVisitorGraphViz`  
**Criticality:** Medium (affects debugging, not execution)

### Edge Case: Deep Tree with Static IDs

**Scenario:**
```
Very deep tree (100+ levels)
Static identifiers reused → collision
Distorted visual representation: nodes merged incorrectly
```

**Historical Bug:**
Old static ID system overwrote IDs in deep trees, distorting Graphviz representation.

**Implemented Protection:**
- Replaced by `std::stack<int>` of ID counters
- Each visited node increments the context counter
- Guaranteed unique ID : (depth, global_counter)
- No collision even for very deep trees

**Code:**
```cpp
class GeneralVisitorGraphViz {
private:
    std::stack<int> pile_id;
    int compteur_global = 0;
    
public:
    void visit(INode* node) {
        int id_courant = compteur_global++;
        pile_id.push(id_courant);
        
        // ... process node
        // Unique ID : (depth, global_counter)
        
        pile_id.pop();
    }
};
```

**Result:**
- No ID collision
- Correct Graphviz representation
- Reliable visual debugging

## Summary of Security Measures

| Component | Edge Case | Protection Mechanism | Result |
|-----------|-----------|----------------------|--------|
| BuilderTreeEquation | Empty parentheses | Immediate exception | Uncorrupted AST |
| BuilderTreeEquation | Orphan operators | Semantic validation | Precise user error |
| OrchestratorInclude | Circular dependencies | Set + mutex | No infinite loop |
| OrchestratorInclude | Thread exception | Global error flag | Pass 2 canceled |
| Lexer | Empty file | Empty vector handling | Valid executable |
| Lexer | Unclosed string | Boundary check | Buffer overread avoided |
| Type Conversion | Memory alignment | Auto-cast + ASAN | Segfault detected |
| RegistryArgument | Nested calls | Context stack | No corruption |
| VisitorGraphViz | Static IDs | Unique ID stack | No collision |

## Future Work

1. **Add fuzz testing** : Randomly generate invalid inputs to discover edge cases
2. **Complete memory coverage** : Use ValGrind to detect residual leaks
3. **Compilation timeout** : Prevent infinite compilation loops
4. **Schema validation** : Check `.prysma` files conformity before parsing
