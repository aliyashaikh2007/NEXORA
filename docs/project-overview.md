# NEXORA
## Smart File Directory Management System

**Tagline:** Structure. Organize. Navigate.

## 1. Project Overview

NEXORA is a C-based Smart File Directory Management System
that demonstrates hierarchical file and directory management
using a General Tree data structure.

The system uses the First Child-Next Sibling representation
to maintain parent-child relationships and manage nested
directories and files dynamically.

## 2. Problem Statement

Traditional directory structures organize files and folders
hierarchically. Managing such structures programmatically
requires an efficient way to represent parent-child relationships,
navigate nested directories, and perform operations such as
insertion, deletion, searching, renaming, and moving.

NEXORA addresses this problem by implementing a hierarchical
directory management system using a General Tree in C. It
maintains structural integrity and prevents invalid operations
such as circular directory movement.

## 3. Project Objectives

1. Design and implement a hierarchical directory management
   system using C.
2. Represent directory structures using a General Tree.
3. Implement First Child-Next Sibling representation.
4. Support dynamic creation and management of files and directories.
5. Implement insertion, searching, deletion, renaming, and moving.
6. Maintain directory hierarchy and prevent circular relationships.
7. Ensure proper dynamic memory allocation and deallocation.
8. Validate operations and handle invalid inputs.
9. Perform functional testing and AddressSanitizer testing.

## 4. Project Scope

### Current Implementation

- Console-based interface.
- In-memory directory tree.
- General Tree data structure.
- First Child-Next Sibling representation.
- File and directory management operations.
- Input validation and memory management.
- Functional and memory-safety testing.

### Future Expansion

- Graphical user interface.
- Persistent directory data storage.
- Database integration.
- Extended file and directory metadata.
- Automated application testing.

## 5. Functional Features

| Feature | Description |
|---|---|
| Create | Create files and directories |
| Display | Display the hierarchical directory structure |
| Search | Search for files and directories |
| Path Search | Locate items using directory paths |
| Rename | Rename existing items |
| Move | Move items between directories |
| Delete | Delete files or directory subtrees |
| Validation | Reject invalid operations |
| Memory Management | Allocate and release tree nodes |

## 6. Non-Functional Requirements

- Reliability: Maintain a valid directory hierarchy.
- Memory Safety: Avoid memory errors during tested operations.
- Maintainability: Keep the source code modular.
- Usability: Provide a clear console interface.
- Efficiency: Use appropriate tree traversal and management algorithms.
- Portability: Use standard C features where possible.

## 7. Technology Stack

| Component | Technology |
|---|---|
| Programming Language | C |
| Core Data Structure | General Tree |
| Tree Representation | First Child-Next Sibling |
| Compiler | GCC |
| Testing | C test programs and AddressSanitizer |
| Development Environment | VS Code and WSL Ubuntu |

## 8. Current System Limitation

The current implementation manages a simulated directory
structure in memory. It does not yet manipulate actual
operating-system files or provide persistent storage.
