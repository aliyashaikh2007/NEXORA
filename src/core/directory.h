
#ifndef DIRECTORY_H
#define DIRECTORY_H

typedef struct Node {
    char name[50];
    int isFile;
    int fileSize;

    struct Node *firstChild;
    struct Node *nextSibling;

} Node;

Node *createNode(const char *name, int isFile, int fileSize);

int insertChild(Node *parent, Node *child);

void freeTree(Node *root);

void displayTree(Node *root, int level);

Node *searchNode(Node *root, const char *name);

Node *searchPath(Node *root, const char *path);

int deleteChild(Node *parent, const char *name);

int renameChild(Node *parent, const char *oldName, const char *newName);

int moveChild(Node *sourceParent, Node *destinationParent,
              const char *childName);

#endif
