#include "../database/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <civetweb.h>
#include <cjson/cJSON.h>
#include <stdint.h>
#include "server.h"

static struct mg_context *server = NULL;

/* Convert a directory tree node into JSON. */
static cJSON *nodeToJSON(Node *node)
{
    if (node == NULL)
        return NULL;

    cJSON *jsonNode = cJSON_CreateObject();

    if (jsonNode == NULL)
        return NULL;

    cJSON_AddStringToObject(jsonNode, "name", node->name);
    cJSON_AddStringToObject(
        jsonNode, "type", node->isFile ? "file" : "directory"
    );
    cJSON_AddNumberToObject(jsonNode, "size", node->fileSize);

    cJSON *children = cJSON_CreateArray();

    if (children == NULL) {
        cJSON_Delete(jsonNode);
        return NULL;
    }

    Node *child = node->firstChild;

    while (child != NULL) {
        cJSON *childJSON = nodeToJSON(child);

        if (childJSON == NULL) {
            cJSON_Delete(children);
            cJSON_Delete(jsonNode);
            return NULL;
        }

        cJSON_AddItemToArray(children, childJSON);
        child = child->nextSibling;
    }

    cJSON_AddItemToObject(jsonNode, "children", children);

    return jsonNode;
}

/* Send a JSON HTTP response. */
static void sendJSON(
    struct mg_connection *conn,
    int status,
    const char *statusText,
    const char *json
)
{
    mg_printf(conn,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        status,
        statusText,
        strlen(json),
        json
    );
}

/* GET /api/status */
static int handleStatus(
    struct mg_connection *conn,
    void *userData
)
{
    (void)userData;

    const char *response =
        "{\"status\":\"online\",\"application\":\"NEXORA\"}";

    sendJSON(conn, 200, "OK", response);

    return 1;
}

/* GET /api/tree */
static int handleTree(
    struct mg_connection *conn,
    void *userData
)
{
    Node *root = (Node *)userData;

    cJSON *tree = nodeToJSON(root);

    if (tree == NULL) {
        sendJSON(
            conn,
            500,
            "Internal Server Error",
            "{\"error\":\"Unable to generate directory tree\"}"
        );
        return 1;
    }

    char *response = cJSON_PrintUnformatted(tree);

    if (response == NULL) {
        cJSON_Delete(tree);

        sendJSON(
            conn,
            500,
            "Internal Server Error",
            "{\"error\":\"JSON generation failed\"}"
        );
        return 1;
    }

    sendJSON(conn, 200, "OK", response);

    free(response);
    cJSON_Delete(tree);

    return 1;
}

/* POST /api/create */
static int handleCreate(
    struct mg_connection *conn,
    void *userData
)
{
    Node *root = (Node *)userData;

    const struct mg_request_info *info =
        mg_get_request_info(conn);

    if (strcmp(info->request_method, "OPTIONS") == 0) {
        mg_printf(conn,
            "HTTP/1.1 204 No Content\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Methods: POST, OPTIONS\r\n"
            "Access-Control-Allow-Headers: Content-Type\r\n"
            "Content-Length: 0\r\n\r\n");
        return 1;
    }

    if (strcmp(info->request_method, "POST") != 0) {
        sendJSON(conn, 405, "Method Not Allowed",
                 "{\"error\":\"Use POST\"}");
        return 1;
    }

    char body[4096];
    int64_t total = 0;
    int64_t expected = info->content_length;

    if (expected <= 0 || expected >= (int64_t)sizeof(body)) {
        sendJSON(conn, 400, "Bad Request",
                 "{\"error\":\"Invalid request body size\"}");
        return 1;
    }

    while (total < expected) {
        int received = (int)mg_read(
            conn,
            body + total,
            (size_t)(expected - total)
        );

        if (received <= 0)
            break;

        total += received;
    }

    body[total] = '\0';

    if (total != expected) {
        sendJSON(conn, 400, "Bad Request",
                 "{\"error\":\"Incomplete request body\"}");
        return 1;
    }

    cJSON *request = cJSON_Parse(body);

    if (request == NULL) {
        sendJSON(conn, 400, "Bad Request",
                 "{\"error\":\"Invalid JSON\"}");
        return 1;
    }

    cJSON *parentPath = cJSON_GetObjectItemCaseSensitive(
        request, "parentPath"
    );

    cJSON *name = cJSON_GetObjectItemCaseSensitive(
        request, "name"
    );

    cJSON *type = cJSON_GetObjectItemCaseSensitive(
        request, "type"
    );

    cJSON *size = cJSON_GetObjectItemCaseSensitive(
        request, "size"
    );

    if (!cJSON_IsString(parentPath) ||
        !cJSON_IsString(name) ||
        !cJSON_IsString(type) ||
        parentPath->valuestring[0] == '\0' ||
        name->valuestring[0] == '\0' ||
        strlen(name->valuestring) >= sizeof(root->name) ||
        (strcmp(type->valuestring, "file") != 0 &&
         strcmp(type->valuestring, "directory") != 0) ||
        (size != NULL &&
         (!cJSON_IsNumber(size) ||
          size->valuedouble < 0 ||
          size->valuedouble > 2147483647.0 ||
          size->valuedouble != (double)(int)size->valuedouble)))
    {
        cJSON_Delete(request);

        sendJSON(conn, 400, "Bad Request",
                 "{\"error\":\"Invalid item details\"}");
        return 1;
    }

    Node *parent = searchPath(root, parentPath->valuestring);

    if (parent == NULL || parent->isFile) {
        cJSON_Delete(request);

        sendJSON(conn, 404, "Not Found",
                 "{\"error\":\"Parent directory not found\"}");
        return 1;
    }

    int isFile = strcmp(type->valuestring, "file") == 0;
    int fileSize = isFile && size != NULL
        ? size->valueint : 0;

    Node *newNode = createNode(
        name->valuestring,
        isFile,
        fileSize
    );

    if (newNode == NULL) {
        cJSON_Delete(request);

        sendJSON(conn, 500, "Internal Server Error",
                 "{\"error\":\"Memory allocation failed\"}");
        return 1;
    }

    if (!insertChild(parent, newNode)) {
        freeTree(newNode);
        cJSON_Delete(request);

        sendJSON(conn, 400, "Bad Request",
                 "{\"error\":\"Unable to create item\"}");
        return 1;
    }

    cJSON_Delete(request);

    if (!database_save(root)) {
        sendJSON(conn, 500, "Internal Server Error",
                 "{\"error\":\"Item created but database save failed\"}");
        return 1;
    }

    sendJSON(conn, 201, "Created",
             "{\"success\":true,\"message\":\"Item created successfully\"}");

    return 1;
}

/* Start the CivetWeb server. */
int server_start(Node *root)
{
    if (root == NULL) {
        printf("Cannot start server: directory tree is empty.\n");
        return 0;
    }

    const char *options[] = {
        "listening_ports", "8080",
        NULL
    };

    server = mg_start(NULL, NULL, options);

    if (server == NULL) {
        printf("Failed to start CivetWeb server.\n");
        return 0;
    }

    mg_set_request_handler(
        server, "/api/status", handleStatus, NULL
    );

    mg_set_request_handler(
        server, "/api/tree", handleTree, root
    );

    mg_set_request_handler(
    server, "/api/create", handleCreate, root
    );

    printf("\nNEXORA HTTP Server started successfully!\n");
    printf("Status API: http://localhost:8080/api/status\n");
    printf("Tree API:   http://localhost:8080/api/tree\n");

    return 1;
}

/* Stop the server. */
void server_stop(void)
{
    if (server != NULL) {
        mg_stop(server);
        server = NULL;

        printf("NEXORA HTTP Server stopped.\n");
    }
}
