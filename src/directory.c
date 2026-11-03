
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "directory.h"

static int containsNode(Node *root, Node *target);

// Create a new file or directory node
Node *createNode(const char *name, int isFile, int fileSize)
{
    // Validate input
    if (name == NULL || name[0] == '\0' || strchr(name, '/') != NULL)
{
    printf("Error: Name cannot be empty or contain '/'.\n");
    return NULL;
}

    if (strlen(name) >= sizeof(((Node *)0)->name))
    {
        printf("Error: Name is too long! Maximum 49 characters allowed.\n");
        return NULL;
    }

    if (isFile != 0 && isFile != 1)
    {
        printf("Error: Invalid file/directory type!\n");
        return NULL;
    }

    if (fileSize < 0)
    {
        printf("Error: File size cannot be negative!\n");
        return NULL;
    }

    // Allocate memory
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }

    // Store node information
    strcpy(newNode->name, name);
    newNode->isFile = isFile;
    newNode->fileSize = fileSize;

    // Initialize pointers
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    return newNode;
}

// Insert a child into a parent directory
int insertChild(Node *parent, Node *child)
{
    if (parent == NULL || child == NULL)
    {
        return 0;
    }

    // Prevent inserting a node into itself or its own subtree
    if (parent == child || containsNode(child, parent))
    {
        return 0;
    }

    if (parent->isFile == 1)
    {
        return 0;
    }

    if (child->nextSibling != NULL)
    {
        return 0;
    }

    // Prevent duplicate names
    Node *current = parent->firstChild;

    while (current != NULL)
    {
        if (strcmp(current->name, child->name) == 0)
        {
            return 0;
        }

        current = current->nextSibling;
    }

    // Insert first child
    if (parent->firstChild == NULL)
    {
        parent->firstChild = child;
        return 1;
    }

    // Find last child
    current = parent->firstChild;

    while (current->nextSibling != NULL)
    {
        current = current->nextSibling;
    }

    current->nextSibling = child;

    return 1;
}

// Display directory tree
void displayTree(Node *root, int level)
{
    if (root == NULL)
    {
        return;
    }

    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }

    if (root->isFile == 1)
    {
        printf("[FILE] %s (%d bytes)\n",
               root->name, root->fileSize);
    }
    else
    {
        printf("[DIR]  %s/\n", root->name);
    }

    displayTree(root->firstChild, level + 1);
    displayTree(root->nextSibling, level);
}

// Search for a node
Node *searchNode(Node *root, const char *name)
{
    if (root == NULL || name == NULL)
    {
        return NULL;
    }

    if (strcmp(root->name, name) == 0)
    {
        return root;
    }

    Node *result = searchNode(root->firstChild, name);

    if (result != NULL)
    {
        return result;
    }

    return searchNode(root->nextSibling, name);
}

// Search for a node using its complete path
Node *searchPath(Node *root, const char *path)
{
    if (root == NULL || path == NULL || path[0] == '\0')
    {
        return NULL;
    }

    char pathCopy[256];

    if (strlen(path) >= sizeof(pathCopy))
    {
        return NULL;
    }

    // Reject paths with invalid separators
size_t pathLength = strlen(path);

if (path[0] == '/' ||
    path[pathLength - 1] == '/' ||
    strstr(path, "//") != NULL)
{
    return NULL;
}

    strcpy(pathCopy, path);

    char *token = strtok(pathCopy, "/");

    // First part of the path must match the root
    if (token == NULL || strcmp(root->name, token) != 0)
    {
        return NULL;
    }

    Node *current = root;

    // Process each remaining directory or file name
    token = strtok(NULL, "/");

    while (token != NULL)
    {
        if (current->isFile == 1)
        {
            return NULL;
        }

        Node *child = current->firstChild;

        while (child != NULL && strcmp(child->name, token) != 0)
        {
            child = child->nextSibling;
        }

        if (child == NULL)
        {
            return NULL;
        }

        current = child;
        token = strtok(NULL, "/");
    }

    return current;
}

// Delete a child
int deleteChild(Node *parent, const char *name)
{
    if (parent == NULL || name == NULL || parent->isFile == 1)
    {
        return 0;
    }

    Node *current = parent->firstChild;
    Node *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            if (previous == NULL)
            {
                parent->firstChild = current->nextSibling;
            }
            else
            {
                previous->nextSibling = current->nextSibling;
            }

            current->nextSibling = NULL;

            freeTree(current);

            return 1;
        }

        previous = current;
        current = current->nextSibling;
    }

    return 0;
}

// Check whether a node exists in another node's subtree
static int containsNode(Node *root, Node *target)
{
    if (root == NULL)
    {
        return 0;
    }

    if (root == target)
    {
        return 1;
    }

    Node *child = root->firstChild;

    while (child != NULL)
    {
        if (containsNode(child, target))
        {
            return 1;
        }

        child = child->nextSibling;
    }

    return 0;
}

// Move a child between directories
int moveChild(Node *sourceParent, Node *destinationParent,
              const char *childName)
{
    if (sourceParent == NULL ||
        destinationParent == NULL ||
        childName == NULL ||
        childName[0] == '\0')
    {
        return 0;
    }

    if (sourceParent->isFile == 1 ||
        destinationParent->isFile == 1)
    {
        return 0;
    }

    Node *current = sourceParent->firstChild;
    Node *previous = NULL;

    while (current != NULL &&
           strcmp(current->name, childName) != 0)
    {
        previous = current;
        current = current->nextSibling;
    }

    if (current == NULL)
    {
        return 0;
    }

    if (sourceParent == destinationParent)
    {
        return 1;
    }

    if (containsNode(current, destinationParent))
    {
        return 0;
    }

    // Prevent duplicate names in destination
    Node *temp = destinationParent->firstChild;

    while (temp != NULL)
    {
        if (strcmp(temp->name, current->name) == 0)
        {
            return 0;
        }

        temp = temp->nextSibling;
    }

    // Detach from source
    if (previous == NULL)
    {
        sourceParent->firstChild = current->nextSibling;
    }
    else
    {
        previous->nextSibling = current->nextSibling;
    }

    current->nextSibling = NULL;

    // Attach to destination
    if (destinationParent->firstChild == NULL)
    {
        destinationParent->firstChild = current;
    }
    else
    {
        temp = destinationParent->firstChild;

        while (temp->nextSibling != NULL)
        {
            temp = temp->nextSibling;
        }

        temp->nextSibling = current;
    }

    return 1;
}

// Rename a child
int renameChild(Node *parent, const char *oldName, const char *newName)
{
    if (parent == NULL || oldName == NULL || newName == NULL)
    {
        return 0;
    }

    if (parent->isFile == 1)
    {
        return 0;
    }

   if (oldName[0] == '\0' || newName[0] == '\0' ||
    strchr(newName, '/') != NULL)
{
    printf("Error: New name cannot be empty or contain '/'.\n");
    return 0;
}

    if (strlen(newName) >= sizeof(parent->name))
    {
        return 0;
    }

    Node *current = parent->firstChild;

    while (current != NULL &&
           strcmp(current->name, oldName) != 0)
    {
        current = current->nextSibling;
    }

    if (current == NULL)
    {
        return 0;
    }

    if (strcmp(oldName, newName) == 0)
    {
        return 1;
    }

    // Prevent duplicate names
    Node *temp = parent->firstChild;

    while (temp != NULL)
    {
        if (temp != current &&
            strcmp(temp->name, newName) == 0)
        {
            return 0;
        }

        temp = temp->nextSibling;
    }

    char newNameCopy[sizeof(current->name)];

    snprintf(newNameCopy, sizeof(newNameCopy), "%s", newName);

    strcpy(current->name, newNameCopy);

    return 1;
}

// Free the entire tree
void freeTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    freeTree(root->firstChild);
    freeTree(root->nextSibling);

    free(root);
}
