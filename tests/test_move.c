

#include <stdio.h>
#include <stdlib.h>
#include "directory.h"

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

    // Build sample directory structure
    Node *documents = addTestNode(root, "Documents", 0, 0);
    Node *projects = addTestNode(root, "Projects", 0, 0);

    if (documents == NULL || projects == NULL)
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

    if (addTestNode(src, "main.py", 1, 120) == NULL)
    {
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Display original structure
    printf("\nBEFORE MOVE:\n");
    displayTree(root, 0);

    // Move ThreatLens from Projects to Documents
    if (moveChild(projects, documents, "ThreatLens"))
    {
        printf("\nSUCCESS: ThreatLens moved successfully!\n");
    }
    else
    {
        printf("\nERROR: Move operation failed.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Display updated structure
    printf("\nAFTER MOVE:\n");
    displayTree(root, 0);

    // Verify the complete subtree
    if (searchPath(root, "NEXORA/Documents/ThreatLens/src/main.py") != NULL &&
        searchPath(root, "NEXORA/Projects/ThreatLens") == NULL)
    {
        printf("\nTEST PASSED: Complete subtree moved correctly.\n");
    }
    else
    {
        printf("\nTEST FAILED: Subtree verification failed.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Release allocated memory
    freeTree(root);

    printf("\nMemory released successfully.\n");

    return EXIT_SUCCESS;
}
