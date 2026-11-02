
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directory.h"

int main(void)
{
    // ==========================================
    // STEP 1: Create the root directory
    // ==========================================

    Node *root = createNode("NEXORA", 0, 0);

    if (root == NULL)
    {
        printf("Error: Unable to create root directory.\n");
        return 1;
    }

    // ==========================================
    // STEP 2: Create directories and files
    // ==========================================

    Node *documents = createNode("Documents", 0, 0);
    Node *projects = createNode("Projects", 0, 0);
    Node *readme = createNode("README.md", 1, 15);

    Node *threatLens = createNode("ThreatLens", 0, 0);
    Node *src = createNode("src", 0, 0);
    Node *mainFile = createNode("main.py", 1, 120);

    // Check whether all nodes were created successfully
    if (documents == NULL ||
        projects == NULL ||
        readme == NULL ||
        threatLens == NULL ||
        src == NULL ||
        mainFile == NULL)
    {
        printf("Error: Memory allocation failed.\n");

        freeTree(root);
        freeTree(documents);
        freeTree(projects);
        freeTree(readme);
        freeTree(threatLens);
        freeTree(src);
        freeTree(mainFile);

        return 1;
    }

    // ==========================================
    // STEP 3: Insert nodes into the tree
    // ==========================================

    if (!insertChild(root, documents) ||
        !insertChild(root, projects) ||
        !insertChild(root, readme) ||
        !insertChild(projects, threatLens) ||
        !insertChild(threatLens, src) ||
        !insertChild(src, mainFile))
    {
        printf("Error: Unable to build directory structure.\n");

        // Free the complete tree and any nodes
        // that were not successfully attached.
        freeTree(root);

        if (documents->firstChild == NULL &&
            documents->nextSibling == NULL &&
            root->firstChild != documents)
        {
            freeTree(documents);
        }

        return 1;
    }

    // ==========================================
    // STEP 4: Display the complete file system
    // ==========================================

    printf("\n====================================\n");
    printf("       WELCOME TO NEXORA\n");
    printf("  Structure. Organize. Navigate.\n");
    printf("====================================\n");

    printf("\nNEXORA File System:\n\n");

    displayTree(root, 0);

    // ==========================================
    // STEP 5: Search for a file or directory
    // ==========================================

    char searchName[50];

    printf("\n====================================\n");
    printf("          SEARCH IN NEXORA\n");
    printf("====================================\n");

    printf("Enter file or directory name to search: ");

    if (fgets(searchName, sizeof(searchName), stdin) != NULL)
    {
        searchName[strcspn(searchName, "\n")] = '\0';

        Node *result = searchNode(root, searchName);

        if (result != NULL)
        {
            printf("\nSearch successful!\n");
            printf("Name: %s\n", result->name);

            if (result->isFile)
            {
                printf("Type: File\n");
                printf("Size: %d bytes\n", result->fileSize);
            }
            else
            {
                printf("Type: Directory\n");
            }
        }
        else
        {
            printf("\n'%s' was not found in NEXORA.\n", searchName);
        }
    }
    else
    {
        printf("\nUnable to read search input.\n");
    }

    // ==========================================
    // STEP 6: Delete a directory
    // ==========================================

    printf("\n====================================\n");
    printf("        DELETE FROM NEXORA\n");
    printf("====================================\n");

    printf("\nDeleting Projects directory...\n");

    if (deleteChild(root, "Projects"))
    {
        printf("Projects deleted successfully!\n");
    }
    else
    {
        printf("Directory not found or deletion failed!\n");
    }

    // ==========================================
    // STEP 7: Display updated file system
    // ==========================================

    printf("\nUpdated NEXORA File System:\n\n");

    displayTree(root, 0);

    // ==========================================
    // STEP 8: Free allocated memory
    // ==========================================

    printf("\nCleaning up allocated memory...\n");

    freeTree(root);

    printf("Memory released successfully.\n");
    printf("\nThank you for using NEXORA!\n");

    return 0;
}
