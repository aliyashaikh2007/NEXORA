# NEXORA — System Architecture

```mermaid
flowchart TD
    A[User] --> B[Console Menu]
    B --> C[Main Program]
    C --> D[Directory Operations]

    D --> E[Create Node]
    D --> F[Search Node / Path]
    D --> G[Rename Node]
    D --> H[Move Node]
    D --> I[Delete Node]
    D --> J[Display Directory Tree]

    E --> K[General Tree]
    F --> K
    G --> K
    H --> K
    I --> K
    J --> K

    K --> L[First Child - Next Sibling]
    L --> M[Dynamic Memory Management]
