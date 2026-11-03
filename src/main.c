
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "directory.h"

// Track whether input has ended unexpectedly
int inputEnded = 0;

// Read a complete line of input
void readText(const char *message, char *buffer, int size)
{
    printf("%s", message);

    if (fgets(buffer, size, stdin) != NULL)
    {
        size_t length = strlen(buffer);

        if (length > 0 && buffer[length - 1] == '\n')
        {
            buffer[length - 1] = '\0';
        }
        else
        {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF)
            {
                // Clear remaining input
            }
        }
    }
    else
    {
        buffer[0] = '\0';
        inputEnded = 1;
    }
}

// Read an integer choice
int readNumber(const char *message)
{
    char buffer[30];
    char *end;
    long number;

    readText(message, buffer, sizeof(buffer));

    if (inputEnded)
{
    return -1;
}

if (buffer[0] == '\0')
{
    printf("Error: Input cannot be empty!\n");
    return -1;
}

    errno = 0;
    number = strtol(buffer, &end, 10);

    if (end == buffer || *end != '\0')
    {
        printf("Error: Please enter a valid number!\n");
        return -1;
    }

    if (errno == ERANGE || number < INT_MIN || number > INT_MAX)
    {
        printf("Error: Number is out of range!\n");
        return -1;
    }

    return (int)number;
}

void displayMenu(void)
{
    printf("\n====================================\n");
    printf("             NEXORA\n");
    printf("   SMART FILE DIRECTORY SYSTEM\n");
    printf("====================================\n");
    printf("1. Create File / Directory\n");
    printf("2. Display Directory Structure\n");
    printf("3. Search File / Directory\n");
    printf("4. Rename File / Directory\n");
    printf("5. Move File / Directory\n");
    printf("6. Delete File / Directory\n");
    printf("7. Exit\n");
    printf("====================================\n");
}

int main(void)
{
    // Create root directory
    Node *root = createNode("NEXORA", 0, 0);

    if (root == NULL)
    {
        printf("Unable to create NEXORA.\n");
        return 1;
    }

    // Create initial sample structure
    Node *documents = createNode("Documents", 0, 0);
    Node *projects = createNode("Projects", 0, 0);
    Node *readme = createNode("README.md", 1, 15);

    Node *threatLens = createNode("ThreatLens", 0, 0);
    Node *src = createNode("src", 0, 0);
    Node *mainFile = createNode("main.py", 1, 120);

    if (documents == NULL || projects == NULL ||
        readme == NULL || threatLens == NULL ||
        src == NULL || mainFile == NULL)
    {
        printf("Memory allocation failed.\n");

        freeTree(root);
        freeTree(documents);
        freeTree(projects);
        freeTree(readme);
        freeTree(threatLens);
        freeTree(src);
        freeTree(mainFile);

        return 1;
    }

    insertChild(root, documents);
    insertChild(root, projects);
    insertChild(root, readme);
    insertChild(projects, threatLens);
    insertChild(threatLens, src);
    insertChild(src, mainFile);

    int choice;

    do
    {
        displayMenu();
        choice = readNumber("Enter your choice: ");

        if (inputEnded)
        {
            break;
        }

        switch (choice)
        {
            case 1:
            {
                char parentPath[256];
                char itemName[256];
                int type;
                int size = 0;

                readText("Enter parent directory path: ",
                        parentPath, sizeof(parentPath));

                Node *parent = searchPath(root, parentPath);

                if (parent == NULL || parent->isFile)
                {
                    printf("Parent directory not found.\n");
                    break;
                }

                readText("Enter new file/directory name: ",
                        itemName, sizeof(itemName));

                if (itemName[0] == '\0')
                {
                    printf("Error: Name cannot be empty!\n");
                    break;
                }

                type = readNumber("Enter 1 for File, 0 for Directory: ");

                if (type != 0 && type != 1)
                {
                    printf("Invalid type.\n");
                    break;
                }

                if (type == 1)
                {
                    size = readNumber("Enter file size in bytes: ");

                    if (size < 0)
                    {
                        printf("Invalid file size.\n");
                        break;
                    }
                }

                Node *newNode = createNode(itemName, type, size);

                if (newNode != NULL)
                {
                    if (insertChild(parent, newNode))
                    {
                        printf("Created successfully!\n");
                    }
                    else
                    {
                        printf("Creation failed.\n");
                        freeTree(newNode);
                    }
                }

                break;
            }

            case 2:
                printf("\nNEXORA DIRECTORY STRUCTURE:\n\n");
                displayTree(root, 0);
                break;

            case 3:
            {
                char path[256];

                readText("Enter complete path to search: ",
                        path, sizeof(path));

                Node *result = searchPath(root, path);

                if (result != NULL)
                {
                    printf("\nItem found: %s\n", result->name);

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
                    printf("Item not found. Please check the path.\n");
                }

                break;
            }

            case 4:
            {
                char parentPath[256];
                char oldName[256];
                char newName[256];

                readText("Enter parent directory path: ",
                        parentPath, sizeof(parentPath));

                Node *parent = searchPath(root, parentPath);

                if (parent == NULL || parent->isFile)
                {
                    printf("Parent directory not found.\n");
                    break;
                }

                readText("Enter current item name: ",
                        oldName, sizeof(oldName));

                readText("Enter new item name: ",
                        newName, sizeof(newName));

                if (renameChild(parent, oldName, newName))
                {
                    printf("Renamed successfully!\n");
                }
                else
                {
                    printf("Rename failed.\n");
                }

                break;
            }

            case 5:
            {
                char sourcePath[256];
                char destinationPath[256];
                char itemName[256];

                readText("Enter source directory path: ",
                        sourcePath, sizeof(sourcePath));

                Node *source = searchPath(root, sourcePath);

                if (source == NULL || source->isFile)
                {
                    printf("Source directory not found.\n");
                    break;
                }

                readText("Enter destination directory path: ",
                        destinationPath, sizeof(destinationPath));

                Node *destination = searchPath(root, destinationPath);

                if (destination == NULL || destination->isFile)
                {
                    printf("Destination directory not found.\n");
                    break;
                }

                readText("Enter item name to move: ",
                        itemName, sizeof(itemName));

                if (moveChild(source, destination, itemName))
                {
                    printf("Moved successfully!\n");
                }
                else
                {
                    printf("Move failed.\n");
                }

                break;
            }

            case 6:
            {
                char parentPath[256];
                char itemName[256];

                readText("Enter parent directory path: ",
                        parentPath, sizeof(parentPath));

                Node *parent = searchPath(root, parentPath);

                if (parent == NULL || parent->isFile)
                {
                    printf("Parent directory not found.\n");
                    break;
                }

                readText("Enter item name to delete: ",
                        itemName, sizeof(itemName));

                if (deleteChild(parent, itemName))
                {
                    printf("Deleted successfully!\n");
                }
                else
                {
                    printf("Delete failed. Item not found.\n");
                }

                break;
            }

            case 7:
                printf("\nThank you for using NEXORA!\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 7 && !inputEnded);

    // Release all dynamically allocated memory
    freeTree(root);

    printf("Memory released successfully.\n");

    return 0;
}
