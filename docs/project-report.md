# NEXORA
## Smart File Directory Management System

**A Data Structures Mini Project**

**Technology:** C Programming
**Core Data Structure:** General Tree
**Tree Representation:** First Child-Next Sibling (FCNS)

---

# ABSTRACT

NEXORA is a Smart File Directory Management System developed using the C programming language. The project demonstrates how hierarchical file and directory structures can be represented and managed using a General Tree data structure.

The system uses the First Child-Next Sibling (FCNS) representation, in which each node maintains pointers to its first child and next sibling. This approach allows a node to have multiple children without requiring a fixed-size array.

The application supports essential directory management operations such as insertion, searching, traversal, renaming, moving and deletion of nodes. It also incorporates input validation, circular movement prevention and dynamic memory management.

The primary objective of NEXORA is to provide practical understanding of tree-based data structures, pointers, structures, recursion and memory allocation through a working console-based application.

The current implementation maintains the directory hierarchy in memory and serves as an educational simulation. It does not directly modify the operating system's actual files or folders.

---

# 1. INTRODUCTION

File and directory management is an important concept in computer systems. Files are generally organized into directories and subdirectories, forming a hierarchical structure.

A General Tree is suitable for representing such hierarchies because each node can have multiple child nodes. In this project, the directory structure is implemented using a General Tree with the First Child-Next Sibling (FCNS) representation.

NEXORA provides a console-based environment to demonstrate how directory nodes can be created, searched, displayed, renamed, moved and deleted using tree operations.

The project emphasizes the practical implementation of Data Structures concepts using the C programming language.

## 1.1 Problem Statement

Managing hierarchical directory structures requires an efficient way to represent relationships between directories and files.

A simple linear data structure cannot naturally represent multiple levels of parent-child relationships.

Therefore, this project aims to develop a C-based directory management simulation using a General Tree and FCNS representation to perform basic directory operations while maintaining structural consistency and safe memory management.

## 1.2 Motivation

The motivation behind developing NEXORA is to understand how tree data structures are applied in real-world hierarchical systems.

The project connects theoretical concepts such as pointers, recursion, dynamic memory allocation and tree traversal with practical programming operations.

---

# 2. OBJECTIVES

The main objectives of the NEXORA project are:

1. To implement a General Tree using the C programming language.
2. To represent hierarchical relationships using First Child-Next Sibling (FCNS).
3. To understand structures and pointers in C.
4. To implement insertion and searching operations.
5. To display the directory hierarchy using tree traversal.
6. To implement renaming, moving and deletion of nodes.
7. To prevent invalid operations such as circular movement.
8. To manage dynamically allocated memory safely.
9. To apply theoretical Data Structures concepts in a practical project.

# 3. PROJECT SCOPE

## 3.1 Current Scope

The current version of NEXORA provides:

- Creation of file and directory nodes.
- Hierarchical organization of nodes.
- Display of the directory tree.
- Searching by node name and path.
- Renaming existing nodes.
- Moving nodes between directories.
- Deleting nodes and their descendants.
- Input validation and circular movement prevention.
- Dynamic memory allocation and deallocation.

## 3.2 Limitations

- The application is console-based.
- Directory structures are maintained in memory.
- Actual operating-system files and directories are not modified.
- Data is not permanently stored after program termination.
- The application does not currently provide a graphical interface.

---

# 4. TECHNOLOGY STACK

The following technologies and concepts are used in NEXORA:

| Component | Technology |
|---|---|
| Programming Language | C |
| Language Standard | C11 |
| Compiler | GCC |
| Data Structure | General Tree |
| Tree Representation | First Child-Next Sibling |
| Memory Management | Dynamic Memory Allocation |
| Development Environment | Visual Studio Code |
| Execution Environment | Windows / WSL |
| Testing | C Test Programs |
| Memory Testing | AddressSanitizer |

## 4.1 Programming Language: C

C is used because it provides direct control over memory through pointers and dynamic memory allocation.

Structures allow related node information to be grouped into a single data type.

## 4.2 Development Tools

- **Visual Studio Code:** Used for writing and editing source code.
- **GCC:** Used to compile the C source files.
- **WSL:** Used as the Linux-based development and testing environment.
- **AddressSanitizer:** Used to detect memory-related errors during tested executions.

# 5. SYSTEM REQUIREMENTS

## 5.1 Hardware Requirements

- Computer or laptop.
- Minimum 4 GB RAM recommended.
- Keyboard and display.
- Sufficient storage for source files and compiler tools.

## 5.2 Software Requirements

- Windows operating system.
- Windows Subsystem for Linux (WSL), if using the Linux environment.
- GCC compiler.
- Visual Studio Code or another C-compatible code editor.

## 5.3 Compilation Requirements

The application uses the C11 standard and can be compiled using GCC with warning flags.

Example compilation command:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -I src src/main.c src/core/directory.c -o build/nexora
```

# 6. DATA STRUCTURE USED

## 6.1 General Tree

A General Tree is a non-linear hierarchical data structure in which each node can have zero or more child nodes.

Unlike a binary tree, which allows at most two children per node, a General Tree supports multiple children.

In NEXORA:

- The root node represents the main directory.
- Child nodes represent files or subdirectories.
- Parent-child relationships represent directory hierarchy.
- Leaf nodes represent files or empty directories.

## 6.2 First Child-Next Sibling (FCNS) Representation

NEXORA uses the First Child-Next Sibling representation to implement a General Tree.

In this representation, each node maintains two important pointers:

1. **firstChild:** Points to the first child of the current node.
2. **nextSibling:** Points to the next sibling of the current node.

This representation allows a General Tree to be implemented using two pointers instead of maintaining an array of child pointers.

## 6.3 Node Structure

The following structure is used in NEXORA:

```c
typedef struct Node {
    char name[50];
    int isFile;
    int fileSize;
    struct Node *firstChild;
    struct Node *nextSibling;
} Node;
```

## 6.4 Advantages of FCNS
- Supports multiple children.
- Uses a consistent two-pointer representation.
- Allows dynamic creation of nodes.
- Avoids fixed-size child arrays.
- Supports hierarchical traversal and modification.

## 6.5 Application in NEXORA

The FCNS representation is used throughout NEXORA to insert, search, display, rename, move and delete nodes in the directory hierarchy.

It forms the foundation of the complete directory management system.

# 7. SYSTEM ARCHITECTURE

NEXORA follows a modular architecture in which the user interacts with the system through a console-based interface.

The application is divided into the following major components:

## 7.1 User Interface Layer

The user interacts with NEXORA through a menu-driven console interface.

The interface accepts user input and displays the results of directory operations.

## 7.2 Main Control Layer

The main function in `main.c` controls the program execution.

It accepts user choices, processes menu operations and calls the appropriate functions.

## 7.3 Directory Management Layer

The files `directory.c` and `directory.h` contain the directory management implementation and function declarations.

This layer handles:
- Node creation.
- Insertion and searching.
- Tree traversal and display.
- Renaming and moving nodes.
- Deletion and memory cleanup.

## 7.4 Data Structure Layer

The General Tree with FCNS representation maintains the hierarchical directory structure.

Each node is dynamically allocated and connected through pointers.

## 7.5 Overall Workflow

1. The application starts.
2. The user selects an operation.
3. The main control module processes the selection.
4. The corresponding directory function is called.
5. The General Tree is updated or searched.
6. The result is displayed to the user.
7. The application continues until the user exits.

---

# 8. ALGORITHMS AND OPERATIONS

NEXORA uses different tree operations to manage its hierarchical directory structure.

## 8.1 Node Creation

The `createNode()` function dynamically allocates memory for a new node.

It initializes the node's name, file or directory status, file size and pointer fields.

**Main steps:**
1. Allocate memory using `malloc()`.
2. Check whether memory allocation was successful.
3. Initialize the node information.
4. Set `firstChild` and `nextSibling` to NULL.
5. Return the newly created node.

## 8.2 Insertion Operation

The `insertChild()` function inserts a new node into a parent's child list.

**Main steps:**
1. Validate the parent and child pointers.
2. Check whether the insertion is valid.
3. If the parent has no child, assign the new node to `firstChild`.
4. Otherwise, traverse the sibling list.
5. Attach the new node at the end of the sibling list.

## 8.3 Searching Operation

NEXORA provides searching by node name and path.

- `searchNode()` searches for a node using its name.
- `searchPath()` searches for a node using a hierarchical path.

The search process traverses the tree and checks the relevant nodes.

## 8.4 Display Operation

The `displayTree()` function displays the directory hierarchy recursively.

It uses indentation to represent different directory levels.

## 8.5 Rename Operation

The `renameChild()` function changes the name of an existing child node.

It validates the requested names and checks for conflicts before performing the rename.

## 8.6 Move Operation

The `moveChild()` function moves a node from one parent directory to another.

It updates the sibling relationships while maintaining the tree structure.

The implementation also prevents circular movement, ensuring that a node cannot be moved into its own descendant.

## 8.7 Delete Operation

The `deleteChild()` function removes a selected child node from its parent's sibling list.

The deleted node and its descendants are released from dynamically allocated memory.

## 8.8 Memory Cleanup

The `freeTree()` function recursively releases dynamically allocated nodes.

This helps prevent memory leaks when the directory structure is no longer required.

---

# 9. TESTING AND VALIDATION

Testing was performed to verify the correctness, reliability and memory safety of the NEXORA application.

Individual C test programs were created to validate different operations and edge cases.

## 9.1 Functional Testing

The following tests were performed:

| Test Case | Expected Behaviour | Result |
|---|---|---|
| Node creation | Allocate and initialize nodes | Passed |
| Insertion | Insert nodes into the hierarchy | Passed |
| Searching | Find nodes by name or path | Passed |
| Rename | Change an existing node's name | Passed |
| Move | Move a node between directories | Passed |
| Circular movement | Reject invalid circular movement | Passed |
| Deletion | Delete a node and its descendants | 15/15 passed |
| Invalid input | Reject invalid inputs | 47/47 passed |
| Memory management | Allocate and release memory safely | 12/12 passed |

## 9.2 Memory Safety Testing

AddressSanitizer was used during testing to detect memory-related errors.

The recorded test executions completed without AddressSanitizer diagnostics.

The following areas were tested:

- Memory allocation.
- Memory deallocation.
- Null pointer handling.
- Node deletion.
- Circular movement prevention.
- Tree integrity after operations.

These results apply to the tested execution paths and do not guarantee the absence of every possible memory error.

## 9.3 Compilation Testing

The application was compiled using GCC with the following warning flags:

- `-Wall`
- `-Wextra`
- `-Wpedantic`
- `-std=c11`

The application was also executed successfully in the WSL environment.

## 9.4 Testing Conclusion

The performed tests demonstrate that the implemented directory operations work for the tested scenarios.

The project also demonstrates the use of input validation, structural checks and dynamic memory management.

Detailed test results are available in `docs/testing/test-report.md`.

---

# 10. RESULTS AND DISCUSSION

The NEXORA project successfully demonstrates the implementation of a hierarchical directory management simulation using a General Tree and First Child-Next Sibling representation.

The application supports the following operations:

- Creating and inserting file and directory nodes.
- Displaying the hierarchical tree.
- Searching nodes by name and path.
- Renaming existing nodes.
- Moving nodes between directories.
- Deleting nodes and their descendants.
- Preventing circular movement.
- Managing dynamically allocated memory.

The test programs verified the implemented operations across the tested scenarios.

The project provides practical experience in applying non-linear data structures and memory management concepts using C.

# 11. LIMITATIONS

The current version has the following limitations:

1. It provides a console-based interface only.
2. The directory structure exists only in memory.
3. Actual operating-system files and directories are not modified.
4. Directory information is not retained after program termination.
5. Advanced file permissions and metadata management are not implemented.

# 12. FUTURE SCOPE

NEXORA can be extended with the following improvements:

- **Graphical User Interface:** Develop a user-friendly graphical interface.
- **Persistent Storage:** Save and restore directory structures using files or a database.
- **Real File-System Integration:** Add controlled operations on actual files and directories.
- **Advanced Search:** Introduce filtering and improved search capabilities.
- **File Metadata:** Manage additional file information.
- **Database Integration:** Store directory information persistently.
- **Improved User Experience:** Add navigation tools and better error messages.

# 13. CONCLUSION

NEXORA demonstrates the practical implementation of a General Tree using the First Child-Next Sibling representation in the C programming language.

The project successfully applies important Data Structures concepts such as structures, pointers, recursion, dynamic memory allocation and tree traversal.

It implements essential directory management operations, including insertion, searching, display, renaming, movement and deletion.

Testing also provided practical experience in validating program behaviour, preventing invalid tree operations and checking memory management.

Overall, NEXORA helped bridge the gap between theoretical Data Structures concepts and their practical implementation through a modular, console-based application.

---

**NEXORA — Structure. Organize. Navigate.**

# 14. REFERENCES

The following resources were used as references for understanding the concepts and technologies involved in the project.

1. Reema Thareja, *Data Structures Using C*, Oxford University Press.
2. Brian W. Kernighan and Dennis M. Ritchie, *The C Programming Language*, Prentice Hall.
3. GCC Documentation — https://gcc.gnu.org/onlinedocs/
4. Microsoft Learn — Windows Subsystem for Linux (WSL), https://learn.microsoft.com/windows/wsl/
5. AddressSanitizer Documentation — https://clang.llvm.org/docs/AddressSanitizer.html
6. C Programming Language Documentation — https://en.cppreference.com/w/c

---