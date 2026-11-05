# NEXORA — Tree Architecture

## 1. Overview

NEXORA uses a General Tree data structure to represent
a hierarchical file and directory system.

The tree is implemented using the First Child-Next Sibling
(FCNS) representation.

## 2. Node Structure

Each node represents either a file or a directory.

The C structure contains:

- name: Stores the file or directory name.
- isFile: Identifies whether the node represents a file.
- fileSize: Stores the file size.
- firstChild: Points to the first child node.
- nextSibling: Points to the next sibling node.

## 3. First Child-Next Sibling Representation

The FCNS representation uses two pointers to represent
relationships in a General Tree.

### First Child

Points to the first child of the current node.

### Next Sibling

Points to the next node that shares the same parent.

This representation supports multiple children
without requiring a fixed-size child array.

## 4. Example Directory Hierarchy

NEXORA/
|
|-- Documents/
|
|-- Projects/
    |
    |-- ThreatLens/
        |
        |-- src/
            |
            |-- main.py

In this example:
- NEXORA is the root directory.
- Documents and Projects are children of NEXORA.
- ThreatLens is a child of Projects.
- src is a child of ThreatLens.
- main.py is a file inside src.

## 5. Node Relationships

Each node maintains:

1. A firstChild pointer to its first child.
2. A nextSibling pointer to its next sibling.

The final sibling at a level has a nextSibling pointer
of NULL.

A node without children has a firstChild pointer of NULL.

## 6. Advantages

- Supports a variable number of children.
- Represents hierarchical relationships naturally.
- Uses dynamic memory allocation.
- Avoids fixed-size child arrays.
- Supports recursive tree traversal and management.

## 7. Applications in NEXORA

The tree structure supports:

- Creating files and directories.
- Displaying directory hierarchies.
- Searching by name and path.
- Renaming existing nodes.
- Moving subtrees.
- Deleting nodes and their descendants.
- Recursive memory cleanup.

## 8. Memory Management

NEXORA dynamically allocates memory for tree nodes.

When a directory subtree is deleted, its descendants
must also be released appropriately.

The freeTree operation is used to release tree memory.

## 9. Conclusion

The First Child-Next Sibling representation provides
a flexible way to implement NEXORA's hierarchical
directory management system using a General Tree.
