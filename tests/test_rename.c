
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

    // Create sample directories
    if (addTestNode(root, "Documents", 0, 0) == NULL ||
        addTestNode(root, "Projects", 0, 0) == NULL)
    {
        freeTree(root);
        return EXIT_FAILURE;
    }

    printf("\nBEFORE RENAME:\n");
    displayTree(root, 0);

    // Test 1: Successful rename
    if (renameChild(root, "Documents", "College"))
    {
        printf("\nTEST 1 PASSED: Documents renamed to College.\n");
    }
    else
    {
        printf("\nTEST 1 FAILED: Rename operation failed.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Test 2: Duplicate-name rejection
    if (!renameChild(root, "College", "Projects"))
    {
        printf("\nTEST 2 PASSED: Duplicate name rejected.\n");
    }
    else
    {
        printf("\nTEST 2 FAILED: Duplicate name was accepted.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Test 3: Nonexistent item rejection
    if (!renameChild(root, "MissingFolder", "NewFolder"))
    {
        printf("\nTEST 3 PASSED: Nonexistent item rejected.\n");
    }
    else
    {
        printf("\nTEST 3 FAILED: Nonexistent item was renamed.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Verify final structure
    printf("\nAFTER RENAME TESTS:\n");
    displayTree(root, 0);

    if (searchPath(root, "NEXORA/College") != NULL &&
        searchPath(root, "NEXORA/Documents") == NULL &&
        searchPath(root, "NEXORA/Projects") != NULL)
    {
        printf("\nFINAL VERIFICATION PASSED: Directory structure is correct.\n");
    }
    else
    {
        printf("\nFINAL VERIFICATION FAILED: Directory structure is incorrect.\n");
        freeTree(root);
        return EXIT_FAILURE;
    }

    // Release allocated memory
    freeTree(root);

    printf("\nMemory released successfully.\n");
    printf("\nALL RENAME TESTS PASSED!\n");

    return EXIT_SUCCESS;
}
