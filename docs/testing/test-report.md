# NEXORA — Test Cases and Validation Report

## 1. Introduction

This document records the functional and memory-safety
testing performed on the NEXORA directory management system.

Testing was performed to verify tree operations,
input validation, circular movement prevention and
dynamic memory management.

## 2. Testing Environment

- Programming Language: C
- Standard: C11
- Compiler: GCC
- Development Environment: Windows with WSL
- Memory Safety Tool: AddressSanitizer (ASan)

## 3. Functional Test Results

| Test File | Feature Tested | Result |
|---|---|---|
| test_cycle.c | Circular movement prevention | Passed |
| test_delete.c | Node and subtree deletion | 15/15 passed |
| test_invalid.c | Invalid input validation | 47/47 passed |
| test_memory.c | Dynamic memory allocation and cleanup | 12/12 passed |
| test_move.c | Moving nodes between directories | Passed |
| test_rename.c | Renaming directory nodes | Passed |

## 4. AddressSanitizer Test Results

AddressSanitizer was used to detect memory-related
errors during test execution.

| Test | Result |
|---|---|
| test_memory_asan | 12/12 passed; no ASan diagnostics |
| test_cycle_asan | Passed; no ASan diagnostics |
| test_delete_asan | 15/15 passed; no ASan diagnostics |
| test_invalid_asan | 47/47 passed; no ASan diagnostics |
| test_move_asan | Passed; no ASan diagnostics |
| test_rename_asan | Passed; no ASan diagnostics |

No AddressSanitizer errors were reported during
these test executions.

This does not guarantee the absence of errors in
every possible execution path.

## 5. Main Application Verification

The main application was compiled using GCC with
the C11 standard and warning flags.

The application launched successfully and displayed
the sample NEXORA directory hierarchy.

The displayed structure included:

- NEXORA as the root directory.
- Documents as a child directory.
- Projects as a child directory.
- ThreatLens inside Projects.
- src inside ThreatLens.
- main.py inside src.
- README.md inside the root directory.

## 6. Input Validation

The invalid-input test suite verified rejection of
unsupported or invalid inputs.

Examples include:

- Empty names.
- Names containing invalid path separators.
- Names exceeding the allowed length.
- Invalid file or directory types.
- Negative file sizes.
- Invalid tree operation parameters.

The invalid-input test suite passed all 47 checks.

## 7. Memory Management Validation

Memory-related tests checked:

- Safe handling of NULL during tree cleanup.
- Successful dynamic memory allocation.
- Access to allocated nodes.
- Deletion of directories containing files.
- Recursive release of tree memory.

The normal memory test passed all 12 checks.

## 8. Conclusion

The completed tests demonstrate that the current
NEXORA implementation successfully handles the
tested directory operations, validation cases,
circular movement checks and memory cleanup scenarios.

AddressSanitizer reported no errors during the
recorded test executions.
