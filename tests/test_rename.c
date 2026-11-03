#include <stdio.h>
#include "directory.h"

int main()
{
    Node *root = createNode("NEXORA", 0, 0);

    Node *documents = createNode("Documents", 0, 0);
    Node *projects = createNode("Projects", 0, 0);

    insertChild(root, documents);
    insertChild(root, projects);

    printf("Before Rename:\n");
    displayTree(root, 0);

    if (renameChild(root, "College", "Projects"))
    printf("\nRename successful!\n");
else
    printf("\nRename failed!\n");
    
    printf("\nAfter Rename:\n");
    displayTree(root, 0);

    freeTree(root);

    return 0;
}