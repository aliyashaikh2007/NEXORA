
#include <stdio.h>
#include <stdlib.h>
#include "core/directory.h"

// Create a node and safely attach it to its parent
Node *addTestNode(Node *parent, const char *name, int isFile, int fileSize)
{
    Node *newNode = createNode(name, isFile, fileSize);

    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed for %s.\n", name);
        return NULL;
    }

    if (!insertChild(parent, newNode))
    {
        printf("Error: Could not insert %s.\n", name);
        freeTree(newNode);
        return NULL;
    }

    return newNode;
}

int main(void)
{
    // Create root directory
    Node *root = createNode("NEXORA", 0, 0);

    if (root == NULL)
    {
        printf("Error: Could not create root directory.\n");
        return EXIT_FAILURE;
    }

    // Build directory structure
    Node *projects = addTestNode(root, "Projects", 0, 0);

    if (projects == NULL)
    {
        freeTree(root);
        return EXIT_FAILURE;
    }

    Node *threatLens = addTestNode(projects, "ThreatLens", 0, 0);

    if (threatLens == NULL)
    {
        freeTree(root);
        return EXIT_FAILURE;
    }

    Node *src = addTestNode(threatLens, "src", 0, 0);

    if (src == NULL)
    {
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Display original structure
    printf("\nBEFORE CYCLE TEST:\n");
    displayTree(root, 0);

    // Attempt to move Projects inside its own subdirectory src
    if (!moveChild(root, src, "Projects"))
    {
        printf("\nTEST PASSED: Circular movement was rejected.\n");
    }
    else
    {
        printf("\nTEST FAILED: Circular movement was allowed.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Verify original structure remains unchanged
    if (searchPath(root, "NEXORA/Projects/ThreatLens/src") != NULL &&
        searchPath(root, "NEXORA/Projects") != NULL)
    {
        printf("\nVERIFICATION PASSED: Original tree remains intact.\n");
    }
    else
    {
        printf("\nVERIFICATION FAILED: Tree structure was changed.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Display final structure
    printf("\nAFTER CYCLE TEST:\n");
    displayTree(root, 0);

    // Release memory
    freeTree(root);

    printf("\nMemory released successfully.\n");
    printf("\nALL CYCLE PREVENTION TESTS PASSED!\n");

    return EXIT_SUCCESS;
}
