# NEXORA — Application Architecture

## 1. Project Overview

NEXORA is a Smart Virtual File Directory Management System designed to provide a web-based interface for managing virtual files and directories.

The application uses a General Tree represented through the First Child–Next Sibling (FCNS) technique as its core data structure.

The existing console-based C implementation will be extended into a complete web application while preserving its original Data Structures implementation.

## 2. Architectural Objectives

- Preserve the existing C-based directory management engine.
- Provide a modern web-based user interface.
- Implement a backend entirely in C.
- Support persistent virtual directory storage.
- Reconstruct the in-memory FCNS tree from SQLite during startup.
- Provide HTTP APIs for frontend communication.
- Maintain consistency between the in-memory tree and database.

## 3. Technology Stack

| Component | Technology |
|---|---|
| Frontend | HTML, CSS, JavaScript |
| Backend | C Programming |
| HTTP Server | CivetWeb |
| JSON Processing | cJSON |
| Core Data Structure | General Tree |
| Tree Representation | First Child–Next Sibling |
| Database | SQLite |
| Database Interface | SQLite C API |
| Compiler | GCC |
| Development Environment | WSL Ubuntu and VS Code |

Library versions, compatibility and licensing will be verified before integration.

## 4. System Architecture

NEXORA consists of five major layers.

### 4.1 Presentation Layer

Technologies: HTML, CSS and JavaScript.

Responsibilities:
- Display the NEXORA dashboard.
- Display folders and files.
- Visualize the directory hierarchy.
- Provide search and navigation.
- Send requests to backend APIs.

### 4.2 HTTP Server Layer

Technology: CivetWeb.

Responsibilities:
- Start the embedded HTTP server.
- Serve frontend resources.
- Receive HTTP requests.
- Forward API requests to application logic.
- Return HTTP responses.

### 4.3 Application Layer

Technology: C and cJSON.

Responsibilities:
- Process API requests.
- Parse JSON input.
- Validate user operations.
- Coordinate database and tree operations.
- Generate JSON responses.
- Handle errors safely.

### 4.4 Core Data Structure Layer

Technology: C with General Tree and FCNS representation.

Responsibilities:
- Create directory and file nodes.
- Insert child nodes.
- Search files and directories.
- Rename items.
- Move directory subtrees.
- Delete items.
- Traverse and display the hierarchy.
- Manage dynamic memory.

The existing core engine will be preserved and integrated into the new application.

### 4.5 Persistence Layer

Technology: SQLite using the SQLite C API.

Responsibilities:
- Store virtual directories and files.
- Maintain parent-child relationships.
- Preserve metadata.
- Support database transactions.
- Load records during application startup.

## 5. Data Structure Design

Each virtual file or directory will be represented by a node.

The initial FCNS structure contains:

- name
- isFile
- fileSize
- firstChild
- nextSibling

The database will maintain persistent identifiers and parent relationships. These identifiers will be mapped to in-memory nodes during reconstruction.

## 6. Database Persistence Strategy

SQLite will act as the persistent source of truth.

### Startup Procedure

1. Open the SQLite database.
2. Initialize the database schema if required.
3. Retrieve stored directory and file records.
4. Validate parent-child relationships.
5. Reconstruct the General Tree.
6. Establish FCNS pointers.
7. Start the HTTP server.

### Operation Procedure

1. Receive an API request.
2. Validate the requested operation.
3. Prepare the required tree and database changes.
4. Execute database modifications inside a transaction.
5. Apply the corresponding in-memory tree operation.
6. Commit or roll back safely.
7. Return the operation result.

The implementation must prevent inconsistent states between SQLite and the in-memory tree.

## 7. Communication Architecture

The frontend and backend will communicate through HTTP requests and JSON responses.

Proposed endpoints:

| Method | Endpoint | Responsibility |
|---|---|---|
| GET | /api/tree | Retrieve directory hierarchy |
| POST | /api/folders | Create a folder |
| POST | /api/files | Create a virtual file |
| GET | /api/search | Search an item |
| PUT | /api/rename | Rename an item |
| PUT | /api/move | Move an item |
| DELETE | /api/items | Delete an item |

Detailed request formats, response formats and error codes will be defined during API development.

## 8. Target Project Structure

```text
NEXORA/
├── src/
│   ├── core/
│   │   ├── directory.c
│   │   └── directory.h
│   ├── backend/
│   │   ├── server.c
│   │   ├── routes.c
│   │   └── routes.h
│   ├── database/
│   │   ├── database.c
│   │   └── database.h
│   └── main.c
├── frontend/
│   ├── index.html
│   ├── css/
│   ├── js/
│   └── assets/
├── database/
├── tests/
├── docs/
├── build/
├── README.md
└── .gitignore
```

This is the proposed target structure. Existing source files will be reorganized only after verifying dependencies and build configuration.

## 9. Security and Reliability Considerations

- Validate all incoming API requests.
- Use parameterized SQLite statements.
- Validate names and paths.
- Prevent circular directory movement.
- Prevent duplicate sibling names.
- Use database transactions.
- Handle memory allocation failures.
- Release allocated memory safely.
- Validate database records before tree reconstruction.
- Restrict the initial server to local development access.

## 10. Development Roadmap

1. Finalize application architecture.
2. Design SQLite database schema.
3. Verify and install required dependencies.
4. Integrate CivetWeb and cJSON.
5. Build the C backend.
6. Connect the existing FCNS core engine.
7. Develop frontend dashboard.
8. Integrate frontend and backend.
9. Implement persistence and error handling.
10. Perform end-to-end testing.
11. Prepare deployment and final documentation.

## 11. Current Scope

NEXORA will initially operate as a Virtual File System.

It will manage virtual files and directories stored in SQLite. It will not manipulate actual operating-system files.

## 12. Conclusion

The proposed architecture transforms NEXORA from a console-based Data Structures project into a modular C-based web application.

The General Tree and FCNS representation remain the foundation of the system, while CivetWeb, cJSON and SQLite provide the infrastructure required for web-based interaction and persistent storage.