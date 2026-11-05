# NEXORA — Function Reference

This document describes the functions implemented in
`src/directory.c`.

## 1. createNode()

**Return type:** `Node *`

**Parameters:**
- `name`: Name of the file or directory.
- `isFile`: Indicates whether the node is a file or directory.
- `fileSize`: Size of the file.

**Responsibility:**
Dynamically allocates memory for a new node, initializes
its properties and sets its child and sibling pointers.

It validates the supplied name, node type and file size.
Returns the created node or NULL if creation fails.

## 2. insertChild()

**Return type:** `int`

**Parameters:**
- `parent`: Parent directory node.
- `child`: Node to be inserted.

**Responsibility:**
Inserts a child node into the parent's child list using
the First Child-Next Sibling representation.

It performs validation to prevent invalid insertion
and circular tree relationships.

## 3. displayTree()

**Return type:** `void`

**Parameters:**
- `root`: Node from which traversal begins.
- `level`: Current depth in the hierarchy.

**Responsibility:**
Displays the directory structure in an indented format.

It uses recursion to traverse child and sibling nodes.
Files and directories are displayed differently.

## 4. searchNode()

**Return type:** `Node *`

**Parameters:**
- `root`: Starting node for the search.
- `name`: Name of the node to find.

**Responsibility:**
Searches the tree recursively for a node matching
the specified name.

Returns the matching node if found, otherwise NULL.

## 5. searchPath()

**Return type:** `Node *`

**Parameters:**
- `root`: Root node of the directory tree.
- `path`: Path identifying the required node.

**Responsibility:**
Searches for a node using its hierarchical path.

It processes the path and traverses the directory
structure to locate the requested node.

Returns the matching node if found, otherwise NULL.

## 6. deleteChild()

**Return type:** `int`

**Parameters:**
- `parent`: Parent directory of the node.
- `name`: Name of the child to delete.

**Responsibility:**
Finds and removes the specified child from its
parent's child list.

It also releases the memory occupied by the deleted
node and its descendants.

## 7. containsNode()

**Return type:** `static int`

**Parameters:**
- `root`: Root of the subtree to examine.
- `target`: Node whose presence is being checked.

**Responsibility:**
Checks whether a target node exists within a subtree.

It is used internally by tree operations, including
movement validation, to help prevent circular structures.

This is a private helper function and is not exposed
through the public header interface.

## 8. moveChild()

**Return type:** `int`

**Parameters:**
- `sourceParent`: Current parent of the node.
- `destinationParent`: New parent for the node.
- `childName`: Name of the node to move.

**Responsibility:**
Moves a node from its source parent to a destination
parent without recreating the node.

It updates the relevant sibling links and validates
the operation to prevent circular movement.

## 9. renameChild()

**Return type:** `int`

**Parameters:**
- `parent`: Parent directory containing the node.
- `oldName`: Existing name of the node.
- `newName`: New name to assign.

**Responsibility:**
Finds a child by its existing name and changes its name.

It validates the new name and checks for duplicate
names among the parent's children.

## 10. freeTree()

**Return type:** `void`

**Parameters:**
- `root`: Root node of the tree or subtree to release.

**Responsibility:**
Recursively releases dynamically allocated memory.

It frees the child subtree, sibling nodes and current
node to avoid memory leaks.

## Conclusion

These functions collectively provide the core operations
required to create, traverse, search, modify and delete
nodes in NEXORA's General Tree.

The implementation uses dynamic memory allocation,
recursion and First Child-Next Sibling representation.
