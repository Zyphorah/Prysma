# Official Documentation of the Prysma Language

## Introduction

Prysma is a compiled system language via LLVM, offering low-level control with a modern syntax. It supports procedural and object-oriented paradigms with deterministic memory management.

-----

## Syntax and Blocks

  * **End of Statement:** Each line of code must end with a semicolon (`;`).
  * **Delimitation:** Code blocks (functions, classes, loops) use curly braces `{ }`.
  * **Identifiers:** Case-sensitive.

-----

## Data Types

| Type | Description |
| :--- | :--- |
| `int32` | Signed 32-bit integer |
| `float` | Floating-point number 32/64 bits |
| `bool` | Logical value (`true`, `false`) |
| `string` | Native character string |
| `void` | Absence of type (function return only) |
| `T[]` | Array of type `T` (e.g., `int32[]`) |

-----

## Memory Management

Prysma uses **manual** memory management on the heap.

### Allocation and Release

  * **`new` :** Allocates memory for an object or an array.
  * **`delete` :** Frees previously allocated memory.

<!-- end list -->

```rust
// Allocating an object
dec MaClasse instance = new MaClasse();

// Release
delete instance;
```

-----

## Functions

Declared with `fn` and called via `call`.

  * **Syntax:** `fn returnType functionName(arg type argName)`
  * **Call:** `call functionName(arguments)`

### Recursion (Factorial Correction)

```rust
fn int32 factorielle(arg int32 n) {
    if (n <= 1) {
        return 1;
    }
    dec int32 suivant = n - 1;
    return n * call factorielle(suivant);
}
```

-----

## Control Structures

### Conditional `if...else`

```rust
if (a > b) {
    // true block
} else {
    // false block
}
```

### `while` Loop

The condition is evaluated before each iteration.

```rust
dec int32 i = 0;
while (i < 10) {
    aff i = i + 1;
}
```

-----

## Operators

| Category | Operators |
| :--- | :--- |
| **Arithmetic** | `+`, `-`, `*`, `/`, `%` |
| **Comparison** | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| **Logical** | `&&`, `||`, `!` |
| **Assignment** | `aff variable = value;` |
| **Reference** | `ref` (access to memory address) |

-----

## Reference Examples

### Creating and Manipulating a Dynamic Array

```rust
fn void test_array() {
    // Declaration of a 5-integer array on the stack
    dec int32[5] notes = [10, 15, 20, 12, 18];
    
    // Modification
    aff notes[0] = 20;
    
    call print(notes[0]);
}
```

### Using a Class and Polymorphism

```rust
fn void main() {
    // Heap allocation
    dec Chien mon_chien = new Chien();
    
    call mon_chien.set_nom("Rex");
    call mon_chien.aboyer();
    
    delete mon_chien;
}
```

-----

**Technical Note:** The Prysma compiler performs a two-pass analysis to resolve symbol dependencies in classes before generating LLVM IR code. Using `ref` is recommended when passing heavy structures or character strings to functions to avoid unnecessary copies.