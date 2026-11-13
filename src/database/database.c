
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "database.h"

static sqlite3 *db = NULL;

typedef struct {
    sqlite3_int64 id;
    Node *node;
} LoadedNode;

/* Find a previously loaded node using its database ID. */
static Node *findLoadedNode(
    LoadedNode *nodes,
    int count,
    sqlite3_int64 id
) {
    for (int i = 0; i < count; i++) {
        if (nodes[i].id == id) {
            return nodes[i].node;
        }
    }

    return NULL;
}

/* Recursively insert a tree into SQLite. */
static int saveNode(
    sqlite3_stmt *stmt,
    Node *node,
    sqlite3_int64 parentId,
    int hasParent,
    int position
) {
    if (node == NULL) {
        return 0;
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    if (hasParent) {
        sqlite3_bind_int64(stmt, 1, parentId);
    } else {
        sqlite3_bind_null(stmt, 1);
    }

    sqlite3_bind_text(stmt, 2, node->name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, node->isFile);
    sqlite3_bind_int(stmt, 4, node->fileSize);
    sqlite3_bind_int(stmt, 5, position);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return 0;
    }

    sqlite3_int64 currentId = sqlite3_last_insert_rowid(db);

    Node *child = node->firstChild;
    int childPosition = 0;

    while (child != NULL) {
        if (!saveNode(stmt, child, currentId, 1, childPosition)) {
            return 0;
        }

        child = child->nextSibling;
        childPosition++;
    }

    return 1;
}

/* Open database and create table if it does not exist. */
int database_init(const char *dbPath)
{
    if (dbPath == NULL) {
        return 0;
    }

    if (sqlite3_open(dbPath, &db) != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        database_close();
        return 0;
    }

    const char *sql =
        "PRAGMA foreign_keys = ON;"

        "CREATE TABLE IF NOT EXISTS nodes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "parent_id INTEGER,"
        "name TEXT NOT NULL,"
        "is_file INTEGER NOT NULL CHECK(is_file IN (0,1)),"
        "file_size INTEGER NOT NULL DEFAULT 0 CHECK(file_size >= 0),"
        "position INTEGER NOT NULL DEFAULT 0,"
        "FOREIGN KEY(parent_id) REFERENCES nodes(id)"
        ");";

    char *errorMessage = NULL;

    if (sqlite3_exec(db, sql, NULL, NULL, &errorMessage) != SQLITE_OK) {
        printf("Database initialization failed: %s\n",
               errorMessage ? errorMessage : sqlite3_errmsg(db));

        sqlite3_free(errorMessage);
        database_close();
        return 0;
    }

    printf("Database initialized successfully.\n");
    return 1;
}

/* Save the complete directory tree in one transaction. */
int database_save(Node *root)
{
    if (db == NULL || root == NULL) {
        return 0;
    }

    char *errorMessage = NULL;

    if (sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL,
                     &errorMessage) != SQLITE_OK) {
        sqlite3_free(errorMessage);
        return 0;
    }

    if (sqlite3_exec(db, "DELETE FROM nodes;", NULL, NULL,
                     &errorMessage) != SQLITE_OK) {
        sqlite3_free(errorMessage);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return 0;
    }

    const char *sql =
        "INSERT INTO nodes "
        "(parent_id, name, is_file, file_size, position) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return 0;
    }

    int success = saveNode(stmt, root, 0, 0, 0);

    sqlite3_finalize(stmt);

    if (!success) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        printf("Failed to save directory tree.\n");
        return 0;
    }

    if (sqlite3_exec(db, "COMMIT;", NULL, NULL,
                     &errorMessage) != SQLITE_OK) {
        printf("Database commit failed: %s\n",
               errorMessage ? errorMessage : sqlite3_errmsg(db));

        sqlite3_free(errorMessage);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return 0;
    }

    return 1;
}

/* Load database records and reconstruct the FCNS tree. */
Node *database_load(void)
{
    if (db == NULL) {
        return NULL;
    }

    const char *sql =
        "SELECT id, parent_id, name, is_file, file_size "
        "FROM nodes ORDER BY id;";

    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    LoadedNode *nodes = NULL;
    int count = 0;
    int capacity = 0;

    Node *root = NULL;
    int failed = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_int64 id = sqlite3_column_int64(stmt, 0);

        int hasParent =
            sqlite3_column_type(stmt, 1) != SQLITE_NULL;

        sqlite3_int64 parentId =
            hasParent ? sqlite3_column_int64(stmt, 1) : 0;

        const unsigned char *name =
            sqlite3_column_text(stmt, 2);

        int isFile = sqlite3_column_int(stmt, 3);
        int fileSize = sqlite3_column_int(stmt, 4);

        if (name == NULL) {
            failed = 1;
            break;
        }

        Node *newNode = createNode(
            (const char *)name,
            isFile,
            fileSize
        );

        if (newNode == NULL) {
            failed = 1;
            break;
        }

        if (!hasParent) {
            if (root != NULL) {
                freeTree(newNode);
                failed = 1;
                break;
            }

            root = newNode;
        } else {
            Node *parent = findLoadedNode(nodes, count, parentId);

            if (parent == NULL || !insertChild(parent, newNode)) {
                freeTree(newNode);
                failed = 1;
                break;
            }
        }

        if (count == capacity) {
            int newCapacity = capacity == 0 ? 16 : capacity * 2;

            LoadedNode *temp = realloc(
                nodes,
                (size_t)newCapacity * sizeof(LoadedNode)
            );

            if (temp == NULL) {
                failed = 1;
                break;
            }

            nodes = temp;
            capacity = newCapacity;
        }

        nodes[count].id = id;
        nodes[count].node = newNode;
        count++;
    }

    sqlite3_finalize(stmt);
    free(nodes);

    if (failed) {
        freeTree(root);
        return NULL;
    }

    if (root != NULL) {
        printf("Directory tree loaded successfully.\n");
    }

    return root;
}

/* Close database connection. */
void database_close(void)
{
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}
