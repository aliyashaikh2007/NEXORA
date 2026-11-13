python3 -c "from pathlib import Path; p=Path('docs/architecture/application-architecture.md'); p.write_text('''# NEXORA – Application Architecture

## 1. Overview

NEXORA is a Smart Virtual File Directory Management System implemented in C. It provides a console-based interface, a C-based directory management engine, an HTTP backend, and SQLite database persistence.

The system uses a General Tree represented through the First Child–Next Sibling (FCNS) technique.

NEXORA manages a virtual directory structure. It does not directly modify the computer’s physical file system.

## 2. Architecture Objectives

- Provide a menu-driven console interface.
- Implement directory operations using a General Tree.
- Support creating, displaying, searching, renaming, moving, and deleting nodes.
- Provide HTTP API endpoints through CivetWeb.
- Exchange API data using JSON and cJSON.
- Persist directory information using SQLite.
- Maintain modular, testable C source code.

## 3. Technology Stack

| Component | Technology |
|---|---|
| Programming Language | C |
| User Interface | Console |
| Core Data Structure | General Tree |
| Tree Representation | First Child–Next Sibling |
| HTTP Backend | CivetWeb |
| JSON Processing | cJSON |
| Database | SQLite |
| Compiler | GCC |
| Build System | Makefile |
| Development Environment | WSL Ubuntu |

## 4. System Components

### 4.1 Console Interface

The console interface is the primary way users interact with NEXORA.

It provides menu options for:
- Creating files and directories.
- Displaying the directory structure.
- Searching for files and directories.
- Renaming items.
- Moving items.
- Deleting items.
- Exiting the application.

Implementation: `src/main.c`

### 4.2 Core Directory Management Engine

The core engine implements directory operations using a General Tree and the First Child–Next Sibling representation.

Implementation:
- `src/core/directory.c`
- `src/core/directory.h`

### 4.3 HTTP Backend

The HTTP backend is implemented using CivetWeb. It exposes API endpoints that allow external clients to interact with the directory management system.

JSON request and response processing is handled using cJSON.

Implementation:
- `src/backend/server.c`
- `src/backend/server.h`

The backend does not require a browser-based frontend. API endpoints can be tested using command-line HTTP clients such as curl.

### 4.4 Database Layer

SQLite provides persistent storage for the virtual directory structure.

The database layer supports saving and loading directory information.

Implementation:
- `src/database/database.c`
- `src/database/database.h`

Database file: `database/nexora.db`

## 5. Core Data Structure

Each directory or file is represented by a node.

The node structure contains:

| Field | Description |
|---|---|
| `name` | Name of the file or directory |
| `isFile` | Identifies whether the node is a file |
| `fileSize` | Stores the file size |
| `firstChild` | Points to the first child node |
| `nextSibling` | Points to the next sibling node |

The First Child–Next Sibling representation allows a general tree to be implemented using two pointers per node.

## 6. Application Workflow

### Startup

1. The application initializes the SQLite database.
2. Previously stored directory information is loaded.
3. The HTTP backend is started.
4. The console menu is displayed.
5. The user selects an operation.
6. The core engine performs the requested operation.
7. Changes are persisted in the database.

### Shutdown

1. The HTTP server is stopped.
2. The directory structure is saved.
3. Database resources are closed.
4. Allocated memory is released as appropriate.

## 7. HTTP API Communication

The backend exposes HTTP endpoints for directory operations.

Clients send HTTP requests containing JSON data. The backend processes these requests and returns JSON responses.

The API can be tested independently using tools such as curl.

There is no HTML, CSS, JavaScript, or browser-based dashboard in the current project scope.

## 8. Project Directory Structure

```text
NEXORA/
|
|-- src/
|   |-- core/
|   |   |-- directory.c
|   |   |-- directory.h
|   |
|   |-- backend/
|   |   |-- server.c
|   |   |-- server.h
|   |
|   |-- database/
|   |   |-- database.c
|   |   |-- database.h
|   |
|   |-- main.c
|
|-- database/
|   |-- nexora.db
|
|-- tests/
|   |-- test_cycle.c
|   |-- test_delete.c
|   |-- test_invalid.c
|   |-- test_memory.c
|   |-- test_move.c
|   |-- test_rename.c
|
|-- docs/
|-- build/
|-- Makefile
|-- README.md
|-- .gitignore

## 9. Testing

The project includes separate test programs for:

- Circular movement prevention.
- File and directory deletion.
- Invalid input handling.
- Memory allocation and cleanup.
- Directory movement.
- Renaming operations.

The Makefile provides commands to build the application and execute the test suite.

## 10. Project Scope

The current implementation focuses on a C-based virtual file directory system.

The project includes:

- A console-based user interface.
- General Tree and FCNS implementation.
- Directory management operations.
- An HTTP backend.
- JSON-based API communication.
- SQLite persistence.
- Automated C test programs.

A browser-based frontend is not part of the current project scope.

## 11. Conclusion

NEXORA combines Data Structures, C programming, HTTP communication, and database management in a modular virtual file directory system.

Its console interface provides direct interaction, while the backend and database layers extend its functionality and provide persistent storage.