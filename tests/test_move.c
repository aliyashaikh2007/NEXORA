#include <stdio.h>
#include "directory.h"

int main()
{
    Node *root = createNode("NEXORA", 0, 0);

    Node *documents = createNode("Documents", 0, 0);
    Node *projects = createNode("Projects", 0, 0);
    Node *threatLens = createNode("ThreatLens", 0, 0);
    Node *src = createNode("src", 0, 0);
    Node *mainFile = createNode("main.py", 1, 120);

    insertChild(root, documents);
    insertChild(root, projects);
    insertChild(projects, threatLens);
    insertChild(threatLens, src);
    insertChild(src, mainFile);

    printf("BEFORE MOVE:\n");
    displayTree(root, 0);

    if (moveChild(projects, documents, "ThreatLens"))
        printf("\nMove successful!\n");
    else
        printf("\nMove failed!\n");

    printf("\nAFTER MOVE:\n");
    displayTree(root, 0);

    freeTree(root);

    return 0;
}