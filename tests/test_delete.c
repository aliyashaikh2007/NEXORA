
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/directory.h"

int passed = 0;
int failed = 0;

// Check test result
void check(int condition, const char *testName)
{
    if (condition)
    {
        printf("[PASS] %s\n", testName);
        passed++;
    }
    else
    {
        printf("[FAIL] %s\n", testName);
        failed++;
    }
}

// Create and insert a node
Node *addNode(Node *parent, const char *name,
              int isFile, int fileSize)
{
    if (parent == NULL)
        return NULL;

    Node *node = createNode(name, isFile, fileSize);

    if (node == NULL)
        return NULL;

    if (!insertChild(parent, node))
    {
        freeTree(node);
        return NULL;
    }

    return node;
}

int main()
{
    printf("\n========== DELETE OPERATION TEST ==========\n");

    // Create root directory
    Node *root = createNode("NEXORA", 0, 0);

    if (root == NULL)
    {
        printf("Test setup failed: Cannot create root.\n");
        return 1;
    }

    // Build directory structure
    Node *projects = addNode(root, "Projects", 0, 0);
    Node *threatLens = addNode(projects, "ThreatLens", 0, 0);
    Node *otherProject = addNode(projects, "OtherProject", 0, 0);

    Node *readme = addNode(threatLens, "README.md", 1, 1024);
    Node *src = addNode(threatLens, "src", 0, 0);
    Node *tests = addNode(threatLens, "tests", 0, 0);

    Node *app = addNode(src, "app.c", 1, 2048);
    Node *header = addNode(src, "app.h", 1, 512);

    Node *testFile = addNode(tests, "test_delete.c", 1, 1500);

    // Verify test setup
    if (!projects || !threatLens || !otherProject ||
        !readme || !src || !tests ||
        !app || !header || !testFile)
    {
        printf("Test setup failed: Node creation or insertion failed.\n");
        freeTree(root);
        return 1;
    }

    printf("\nINITIAL DIRECTORY TREE:\n");
    displayTree(root, 0);

    // TEST 1: File deletion
    printf("\n--- TEST 1: FILE DELETION ---\n");

    check(deleteChild(threatLens, "README.md") == 1,
          "File deletion successful");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/README.md") == NULL,
          "Deleted file is no longer searchable");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/src/app.c") != NULL,
          "Other files remain intact after file deletion");

    // TEST 2: Directory subtree deletion
    printf("\n--- TEST 2: DIRECTORY SUBTREE DELETION ---\n");

    check(deleteChild(threatLens, "src") == 1,
          "Directory deletion successful");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/src") == NULL,
          "Deleted directory is no longer searchable");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/src/app.c") == NULL,
          "Child file app.c is no longer searchable");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/src/app.h") == NULL,
          "Child file app.h is no longer searchable");

    // TEST 3: Preserve unrelated directories and files
    printf("\n--- TEST 3: TREE INTEGRITY ---\n");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/tests") != NULL,
          "Unrelated tests directory remains intact");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/tests/test_delete.c") != NULL,
          "Unrelated test file remains intact");

    check(searchPath(root,
          "NEXORA/Projects/OtherProject") != NULL,
          "OtherProject remains intact");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens") == threatLens,
          "Parent directory remains intact");

    // TEST 4: Invalid deletion attempts
    printf("\n--- TEST 4: INVALID DELETION ---\n");

    check(deleteChild(threatLens, "MissingFile.txt") == 0,
          "Deleting a non-existent child is rejected");

    check(deleteChild(NULL, "README.md") == 0,
          "NULL parent is rejected");

    check(deleteChild(threatLens, NULL) == 0,
          "NULL child name is rejected");

    check(deleteChild(testFile, "anything.txt") == 0,
          "Deletion inside a file is rejected");

    // Display final tree
    printf("\nFINAL DIRECTORY TREE:\n");
    displayTree(root, 0);

    // Release remaining memory
    freeTree(root);
    root = NULL;

    // TEST SUMMARY
    printf("\n========== TEST SUMMARY ==========\n");

    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);

    if (failed == 0)
        printf("\nALL DELETE TESTS PASSED!\n");
    else
        printf("\nSOME DELETE TESTS FAILED!\n");

    return failed == 0 ? 0 : 1;
}
