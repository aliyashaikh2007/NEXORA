# NEXORA
### Smart File Directory Management System

**Structure. Organize. Navigate.**

NEXORA is a C-based console application that demonstrates
how a hierarchical file and directory management system
can be implemented using a General Tree data structure.

It uses the **First Child-Next Sibling (FCNS)**
representation to organize files and directories dynamically.

---

## 1. Project Overview

NEXORA demonstrates a hierarchical directory system
through a General Tree implemented in C.

Each node represents either a file or a directory.
Pointers maintain child and sibling relationships.

The project focuses on Data Structures, dynamic memory
allocation, recursion, tree traversal and validation.

## 2. Objectives

- Implement a General Tree in C.
- Represent multiple children using FCNS pointers.
- Perform directory searching and traversal.
- Implement insertion, deletion, renaming and movement.
- Practice dynamic memory allocation.
- Prevent invalid input and circular tree structures.

## 3. Technology Stack

| Component | Technology |
|---|---|
| Language | C |
| Standard | C11 |
| Data Structure | General Tree |
| Representation | First Child-Next Sibling |
| Compiler | GCC |
| Testing | C test programs and AddressSanitizer |
| Environment | Windows / WSL |

---

## 4. Core Features

- Create file and directory nodes.
- Insert nodes into the directory hierarchy.
- Display the directory tree recursively.
- Search nodes by name and path.
- Rename existing nodes.
- Move nodes between directories.
- Delete nodes and their descendants.
- Prevent circular movement.
- Validate invalid input.
- Release dynamically allocated memory.

## 5. Data Structure: FCNS

Each node contains five fields:

| Field | Purpose |
|---|---|
| name | File or directory name |
| isFile | Identifies file or directory |
| fileSize | Stores file size |
| firstChild | Points to the first child |
| nextSibling | Points to the next sibling |

This representation supports a variable number of
children without requiring a fixed-size child array.

### Example Directory Structure

```text
NEXORA/
├── Documents/
├── Projects/
│   └── ThreatLens/
│       └── src/
│           └── main.py
└── README.md
```

## 6. Project Structure

```text
NEXORA/
├── src/
│   ├── directory.c
│   ├── directory.h
│   └── main.c
├── tests/
│   ├── test_cycle.c
│   ├── test_delete.c
│   ├── test_invalid.c
│   ├── test_memory.c
│   ├── test_move.c
│   └── test_rename.c
├── build/
├── docs/
│   ├── archive/
│   ├── architecture/
│   ├── diagrams/
│   ├── testing/
│   ├── function-reference.md
│   └── project-overview.md
├── .gitignore
└── README.md
```

## 7. Compilation and Execution

### Prerequisites
- GCC compiler
- Terminal or WSL environment

### Compile

```bash
mkdir -p build

gcc -Wall -Wextra -Wpedantic -std=c11 \
-I src src/main.c src/directory.c \
-o build/nexora
```

---

## 8. Testing and Validation

| Test | Result |
|---|---|
| Circular movement prevention | Passed |
| Delete operation | 15/15 passed |
| Invalid input validation | 47/47 passed |
| Memory management | 12/12 passed |
| Move operation | Passed |
| Rename operation | Passed |

### AddressSanitizer

The recorded AddressSanitizer test executions reported
no memory errors.

This applies to the tested execution paths and does not
guarantee the absence of every possible memory error.

Detailed results: `docs/testing/test-report.md`

## 9. Documentation

- `docs/project-overview.md` — Project objectives and scope.
- `docs/architecture/tree-architecture.md` — FCNS architecture.
- `docs/diagrams/system-architecture.md` — System workflow.
- `docs/diagrams/fcns-tree.md` — Tree representation.
- `docs/function-reference.md` — Function descriptions.
- `docs/testing/test-report.md` — Testing results.

## 10. Current Scope and Limitations

The current version is a console-based educational
simulation.

- Directory structures are maintained in memory.
- Actual operating-system files and folders are not modified.
- Data is not persisted after the application exits.
- The interface is command-line based.

## 11. Future Enhancements

- Graphical user interface.
- Persistent directory storage.
- Integration with actual file-system operations.
- File metadata management.
- Database integration.
- Improved search and navigation.

## 12. Academic Relevance

NEXORA demonstrates practical applications of:

- General Trees.
- First Child-Next Sibling representation.
- Pointers and structures.
- Dynamic memory allocation.
- Recursion and tree traversal.
- Searching and modification operations.
- Memory management and input validation.

---

**NEXORA — Structure. Organize. Navigate.**