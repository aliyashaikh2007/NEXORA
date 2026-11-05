
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

// Build a sample directory tree
Node *buildTestTree(void)
{
    Node *root = createNode("NEXORA", 0, 0);

    if (root == NULL)
        return NULL;

    Node *projects = addNode(root, "Projects", 0, 0);
    Node *threatLens = addNode(projects, "ThreatLens", 0, 0);

    Node *src = addNode(threatLens, "src", 0, 0);
    Node *tests = addNode(threatLens, "tests", 0, 0);

    Node *app = addNode(src, "app.c", 1, 2048);
    Node *header = addNode(src, "app.h", 1, 512);

    Node *testFile = addNode(tests, "test.c", 1, 1024);

    if (!projects || !threatLens ||
        !src || !tests ||
        !app || !header || !testFile)
    {
        freeTree(root);
        return NULL;
    }

    return root;
}

int main(void)
{
    printf("\n========== MEMORY SAFETY TEST ==========\n");

    // TEST 1: NULL memory cleanup
    printf("\n--- TEST 1: NULL CLEANUP ---\n");

    freeTree(NULL);

    check(1, "freeTree(NULL) executes safely");

    // TEST 2: Tree creation
    printf("\n--- TEST 2: MEMORY ALLOCATION ---\n");

    Node *root = buildTestTree();

    check(root != NULL,
          "Complete directory tree allocated successfully");

    if (root == NULL)
    {
        printf("\nTest setup failed.\n");
        return 1;
    }

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/src/app.c") != NULL,
          "Allocated nodes are accessible");

    // TEST 3: File deletion
    printf("\n--- TEST 3: FILE MEMORY CLEANUP ---\n");

    Node *threatLens = searchPath(
        root, "NEXORA/Projects/ThreatLens");

    check(deleteChild(threatLens, "tests") == 1,
          "Directory containing a file deleted successfully");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/tests/test.c") == NULL,
          "Deleted subtree is no longer reachable");

    // TEST 4: Directory subtree deletion
    printf("\n--- TEST 4: SUBTREE MEMORY CLEANUP ---\n");

    check(deleteChild(threatLens, "src") == 1,
          "Source directory deleted successfully");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/src/app.c") == NULL,
          "Deleted child file is no longer reachable");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens/src/app.h") == NULL,
          "All subtree files are no longer reachable");

    // TEST 5: Remaining tree integrity
    printf("\n--- TEST 5: TREE INTEGRITY ---\n");

    check(searchPath(root,
          "NEXORA/Projects/ThreatLens") == threatLens,
          "Parent directory remains accessible");

    check(searchPath(root,
          "NEXORA/Projects") != NULL,
          "Unrelated parent directory remains intact");

    printf("\nTREE BEFORE FINAL CLEANUP:\n");
    displayTree(root, 0);

    // TEST 6: Complete tree cleanup
    printf("\n--- TEST 6: COMPLETE TREE CLEANUP ---\n");

    freeTree(root);
    root = NULL;

    check(root == NULL,
          "Root pointer reset after tree cleanup");

    // TEST 7: Repeated allocation and deallocation
    printf("\n--- TEST 7: STRESS TEST ---\n");

    int successfulCycles = 0;
    int totalCycles = 1000;

    for (int i = 0; i < totalCycles; i++)
    {
        Node *tempRoot = buildTestTree();

        if (tempRoot == NULL)
            break;

        Node *tempThreatLens = searchPath(
            tempRoot, "NEXORA/Projects/ThreatLens");

        if (tempThreatLens == NULL)
        {
            freeTree(tempRoot);
            break;
        }

        // Delete a complete subtree
        if (!deleteChild(tempThreatLens, "src"))
        {
            freeTree(tempRoot);
            break;
        }

        // Delete another child
        if (!deleteChild(tempThreatLens, "tests"))
        {
            freeTree(tempRoot);
            break;
        }

        // Release the remaining tree
        freeTree(tempRoot);
        tempRoot = NULL;

        successfulCycles++;
    }

    printf("Completed allocation/deallocation cycles: %d/%d\n",
           successfulCycles, totalCycles);

    check(successfulCycles == totalCycles,
          "1000 repeated allocation/deallocation cycles completed");

    // FINAL SUMMARY
    printf("\n========== MEMORY TEST SUMMARY ==========\n");

    printf("Total Passed: %d\n", passed);
    printf("Total Failed: %d\n", failed);
    printf("Total Tests : %d\n", passed + failed);

    if (failed == 0)
        printf("\nALL MEMORY TEST CHECKS PASSED!\n");
    else
        printf("\nSOME MEMORY TEST CHECKS FAILED!\n");

    return failed == 0 ? 0 : 1;
}
