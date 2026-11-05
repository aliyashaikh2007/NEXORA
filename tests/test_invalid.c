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
    printf("\n========== INVALID INPUT TEST ==========\n");

    // Create test directory structure
    Node *root = createNode("NEXORA", 0, 0);

    if (root == NULL)
    {
        printf("Test setup failed: Cannot create root.\n");
        return 1;
    }

    Node *dirA = addNode(root, "DirectoryA", 0, 0);
    Node *dirB = addNode(root, "DirectoryB", 0, 0);

    if (!dirA || !dirB)
    {
        printf("Test setup failed.\n");
        freeTree(root);
        return 1;
    }

    Node *fileA = addNode(dirA, "fileA.txt", 1, 100);
    Node *subA = addNode(dirA, "SubDirectory", 0, 0);
    Node *duplicateFile = addNode(dirB, "fileA.txt", 1, 200);

    if (!fileA || !subA || !duplicateFile)
    {
        printf("Test setup failed.\n");
        freeTree(root);
        return 1;
    }

    Node *nestedFile = addNode(subA, "nested.txt", 1, 50);

    if (!nestedFile)
    {
        printf("Test setup failed.\n");
        freeTree(root);
        return 1;
    }

    // ==========================================
    // TEST 1: INVALID NODE CREATION
    // ==========================================

    printf("\n--- TEST 1: INVALID NODE CREATION ---\n");

    check(createNode(NULL, 0, 0) == NULL,
          "NULL node name is rejected");

    check(createNode("", 0, 0) == NULL,
          "Empty node name is rejected");

    check(createNode("Invalid/Name", 0, 0) == NULL,
          "Name containing slash is rejected");

    check(createNode("InvalidFile", 1, -100) == NULL,
          "Negative file size is rejected");

    check(createNode("InvalidType", 2, 100) == NULL,
          "Invalid file/directory type is rejected");

    char longName[60];
    memset(longName, 'A', 55);
    longName[55] = '\0';

    check(createNode(longName, 0, 0) == NULL,
          "Name exceeding 49 characters is rejected");

    // ==========================================
    // TEST 2: INVALID CHILD INSERTION
    // ==========================================

    printf("\n--- TEST 2: INVALID CHILD INSERTION ---\n");

    check(insertChild(NULL, fileA) == 0,
          "NULL parent is rejected during insertion");

    check(insertChild(dirA, NULL) == 0,
          "NULL child is rejected during insertion");

    check(insertChild(dirA, dirA) == 0,
          "Inserting a directory into itself is rejected");

    check(insertChild(subA, dirA) == 0,
          "Circular tree insertion is rejected");

    Node *duplicateCandidate = createNode("fileA.txt", 1, 300);

    if (duplicateCandidate != NULL)
    {
        check(insertChild(dirA, duplicateCandidate) == 0,
              "Duplicate child name is rejected");

        freeTree(duplicateCandidate);
    }
    else
    {
        check(0, "Duplicate child test setup");
    }

    Node *temporary = createNode("temporary.txt", 1, 10);

    if (temporary != NULL)
    {
        check(insertChild(fileA, temporary) == 0,
              "Insertion inside a file is rejected");

        freeTree(temporary);
    }
    else
    {
        check(0, "File-parent insertion test setup");
    }

    // ==========================================
    // TEST 3: INVALID SEARCH OPERATIONS
    // ==========================================

    printf("\n--- TEST 3: INVALID SEARCH OPERATIONS ---\n");

    check(searchNode(NULL, "fileA.txt") == NULL,
          "NULL root is rejected during node search");

    check(searchNode(root, NULL) == NULL,
          "NULL search name is rejected");

    check(searchPath(root, NULL) == NULL,
          "NULL path is rejected");

    check(searchPath(root, "") == NULL,
          "Empty path is rejected");

    check(searchPath(root, "/NEXORA") == NULL,
          "Path starting with slash is rejected");

    check(searchPath(root, "NEXORA/") == NULL,
          "Path ending with slash is rejected");

    check(searchPath(root, "NEXORA//DirectoryA") == NULL,
          "Path containing double slash is rejected");

    check(searchPath(root, "WrongRoot/DirectoryA") == NULL,
          "Incorrect root directory name is rejected");

    check(searchPath(root,
          "NEXORA/DirectoryA/SubDirectory/Missing.txt") == NULL,
          "Non-existent path is rejected");

    check(searchPath(root,
          "NEXORA/DirectoryA/fileA.txt/child.txt") == NULL,
          "Searching inside a file is rejected");

    // ==========================================
    // TEST 4: INVALID RENAME OPERATIONS
    // ==========================================

    printf("\n--- TEST 4: INVALID RENAME OPERATIONS ---\n");

    check(renameChild(NULL, "fileA.txt", "new.txt") == 0,
          "NULL parent is rejected during rename");

    check(renameChild(dirA, NULL, "new.txt") == 0,
          "NULL old name is rejected");

    check(renameChild(dirA, "fileA.txt", NULL) == 0,
          "NULL new name is rejected");

    check(renameChild(dirA, "fileA.txt", "") == 0,
          "Empty new name is rejected");

    check(renameChild(dirA, "fileA.txt", "Invalid/Name") == 0,
          "New name containing slash is rejected");

    check(renameChild(dirA, "Missing.txt", "new.txt") == 0,
          "Renaming a non-existent child is rejected");

    check(renameChild(dirA, "fileA.txt", "SubDirectory") == 0,
          "Duplicate name during rename is rejected");

    check(renameChild(fileA, "anything.txt", "new.txt") == 0,
          "Renaming inside a file is rejected");

    char longNewName[60];
    memset(longNewName, 'B', 55);
    longNewName[55] = '\0';

    check(renameChild(dirA, "fileA.txt", longNewName) == 0,
          "Overlength new name is rejected");

    // ==========================================
    // TEST 5: INVALID MOVE OPERATIONS
    // ==========================================

    printf("\n--- TEST 5: INVALID MOVE OPERATIONS ---\n");

    check(moveChild(NULL, dirB, "fileA.txt") == 0,
          "NULL source directory is rejected");

    check(moveChild(dirA, NULL, "fileA.txt") == 0,
          "NULL destination directory is rejected");

    check(moveChild(dirA, dirB, NULL) == 0,
          "NULL child name is rejected during move");

    check(moveChild(dirA, dirB, "") == 0,
          "Empty child name is rejected during move");

    check(moveChild(dirA, dirB, "Missing.txt") == 0,
          "Moving a non-existent child is rejected");

    check(moveChild(fileA, dirB, "anything.txt") == 0,
          "File cannot be used as source directory");

    check(moveChild(dirA, fileA, "SubDirectory") == 0,
          "File cannot be used as destination directory");

    check(moveChild(dirA, subA, "SubDirectory") == 0,
          "Circular directory movement is rejected");

    check(moveChild(dirA, dirB, "fileA.txt") == 0,
          "Duplicate destination name is rejected");

    // ==========================================
    // TEST 6: INVALID DELETE OPERATIONS
    // ==========================================

    printf("\n--- TEST 6: INVALID DELETE OPERATIONS ---\n");

    check(deleteChild(NULL, "fileA.txt") == 0,
          "NULL parent is rejected during deletion");

    check(deleteChild(dirA, NULL) == 0,
          "NULL child name is rejected during deletion");

    check(deleteChild(fileA, "anything.txt") == 0,
          "Deletion inside a file is rejected");

    check(deleteChild(dirA, "Missing.txt") == 0,
          "Deleting a non-existent child is rejected");

    // ==========================================
    // TEST 7: VERIFY TREE INTEGRITY
    // ==========================================

    printf("\n--- TEST 7: TREE INTEGRITY ---\n");

    check(searchPath(root,
          "NEXORA/DirectoryA/fileA.txt") == fileA,
          "Original file remains intact");

    check(searchPath(root,
          "NEXORA/DirectoryA/SubDirectory/nested.txt") == nestedFile,
          "Nested file remains intact");

    check(searchPath(root,
          "NEXORA/DirectoryB/fileA.txt") == duplicateFile,
          "Other directory remains intact");

    printf("\nFINAL DIRECTORY TREE:\n");
    displayTree(root, 0);

    // Release all allocated memory
    freeTree(root);
    root = NULL;

    // ==========================================
    // FINAL TEST SUMMARY
    // ==========================================

    printf("\n========== TEST SUMMARY ==========\n");

    printf("Total Passed: %d\n", passed);
    printf("Total Failed: %d\n", failed);
    printf("Total Tests : %d\n", passed + failed);

    if (failed == 0)
    {
        printf("\nALL INVALID INPUT TESTS PASSED!\n");
    }
    else
    {
        printf("\nSOME INVALID INPUT TESTS FAILED!\n");
    }

    return failed == 0 ? 0 : 1;
}