# NEXORA — First Child-Next Sibling Tree Diagram

```mermaid
flowchart TD
    A["NEXORA (Root)"]
    B["Documents (Directory)"]
    C["Projects (Directory)"]
    D["ThreatLens (Directory)"]
    E["src (Directory)"]
    F["main.py (File)"]

    A -->|firstChild| B
    B -->|nextSibling| C
    C -->|firstChild| D
    D -->|firstChild| E
    E -->|firstChild| F
