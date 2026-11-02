#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "directory.h"

Node *createNode(const char *name, int isFile, int fileSize)
{
    // Step 1: Allocate memory for a new node
    Node *newNode = (Node *)malloc(sizeof(Node));

    // Step 2: Check whether memory allocation succeeded
    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }

    // Step 3: Store the name
    snprintf(newNode->name, sizeof(newNode->name), "%s", name);

    // Step 4: Store file or directory information
    newNode->isFile = isFile;
    newNode->fileSize = fileSize;

    // Step 5: Initialize both pointers
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    // Step 6: Return the address of the new node
    return newNode;
}

int insertChild(Node *parent, Node *child)
{
    // Check for valid nodes
    if (parent == NULL || child == NULL)
    {
        return 0;
    }

    // A file cannot contain other entries
    if (parent->isFile == 1)
    {
        return 0;
    }

    // The child must be a detached node
    if (child->nextSibling != NULL)
    {
        return 0;
    }

    // If parent has no children
    if (parent->firstChild == NULL)
    {
        parent->firstChild = child;
        return 1;
    }

    // Find the last child
    Node *current = parent->firstChild;

    while (current->nextSibling != NULL)
    {
        current = current->nextSibling;
    }

    // Attach the new child
    current->nextSibling = child;

    return 1;
}


void displayTree(Node *root, int level)
{
    // Base case: stop if node does not exist
    if (root == NULL)
    {
        return;
    }

    // Print indentation according to tree depth
    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }

    // Display directory or file
    if (root->isFile == 1)
    {
        printf("[FILE] %s (%d bytes)\n",
               root->name, root->fileSize);
    }
    else
    {
        printf("[DIR]  %s/\n", root->name);
    }

    // Visit the first child at the next level
    displayTree(root->firstChild, level + 1);

    // Visit the next sibling at the same level
    displayTree(root->nextSibling, level);
}


Node *searchNode(Node *root, const char *name)
{
    // Base case: node does not exist
    if (root == NULL)
    {
        return NULL;
    }

    // Compare current node's name
    if (strcmp(root->name, name) == 0)
    {
        return root;
    }

    // Search in the first child and its subtree
    Node *result = searchNode(root->firstChild, name);

    if (result != NULL)
    {
        return result;
    }

    // Search among the next siblings
    return searchNode(root->nextSibling, name);
}

int deleteChild(Node *parent, const char *name)
{
    // Validate the parent
    if (parent == NULL || name == NULL || parent->isFile == 1)
    {
        return 0;
    }

    Node *current = parent->firstChild;
    Node *previous = NULL;

    // Search for the child to delete
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            // If target is the first child
            if (previous == NULL)
            {
                parent->firstChild = current->nextSibling;
            }
            else
            {
                previous->nextSibling = current->nextSibling;
            }

            // Disconnect target from remaining siblings
            current->nextSibling = NULL;

            // Free target and its descendants
            freeTree(current);

            return 1;
        }

        previous = current;
        current = current->nextSibling;
    }

    // Child was not found
    return 0;
}

// Release the entire tree
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

