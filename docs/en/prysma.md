# Official Documentation of the Prysma Language

## Introduction

Prysma is a compiled system language via LLVM, offering low-level control with a modern syntax. It supports procedural and object-oriented paradigms with deterministic memory management.

-----

## Syntax and Blocks

  * **End of Statement:** Each line of code must end with a semicolon (`;`).
  * **Delimitation:** Code blocks (functions, classes, loops) use curly braces `{ }`.
  * **Identifiers:** Case-sensitive.
  * **Please note there is no floating point types currently, nor unsigned types, everything is a signed interger.
-----

## Data Types

| Type | Description |
| :--- | :--- |
| `int32` | Signed 32-bit integer** |
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
dec MaClasse instance = new MaClasse(); // This 'dec' is equavilent of a 'let' or similar in other languages. 

// Release
delete instance;
```

-----

## Functions

Declared with `fn` and called via `call`.

  * **Syntax:** `fn returnType functionName(arg type argName)`
  * **Call:** `call functionName(arguments)`
  * A quick tip, it follows this syntex: `fn <type of data> <function name> { <opening brace, insert code here, assuming like rust style ish> <closing brace> }`
  * The end of a function has no semi-colon (;)

### Recursion (Factorial Correction)

```rust
fn int32 factorielle(arg int32 n) {
    if (n <= 1) {
        return 1;
    }
    dec int32 suivant = n - 1; // This 'dec' is equavilent of a 'let' or similar in other languages. 
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
dec int32 i = 0; // This 'dec' is equavilent of a 'let' or similar in other languages. 
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
    dec int32[5] notes = [10, 15, 20, 12, 18]; // This 'dec' is equavilent of a 'let' or similar in other languages. 
    
    // Modification
    aff notes[0] = 20;
    
    call print(notes[0]);
}
```

### Using a Class and Polymorphism

```rust
fn void main() {
    // Heap allocation
    dec Chien mon_chien = new Chien(); // This 'dec' is equavilent of a 'let' or similar in other languages. 
    
    call mon_chien.set_nom("Rex");
    call mon_chien.aboyer();
    
    delete mon_chien;
}
```

-----

**Technical Note:** The Prysma compiler performs a two-pass analysis to resolve symbol dependencies in classes before generating LLVM IR code. Using `ref` is recommended when passing heavy structures or character strings to functions to avoid unnecessary copies.
