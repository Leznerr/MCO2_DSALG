# DSALG Project

This repository contains source code for various graph algorithms and data structures used in the course project. The actual project files are stored under:

```
CCDSALG_S22_MartinJavierEleydo-20250727T110158Z-1-001/CCDSALG_S22_MartinJavierEleydo
```

## Directory Layout

- `doc/` – project documentation.
- `src/` – C source files implementing data structures and algorithms.
  - `data_structures/` – stack, queue, heap and linked list implementations.
  - `algorithms/` – traversal, connectivity, shortest path and MST routines.
  - `graph/` – graph ADT and supporting code.
  - `main.c` – command line program demonstrating graph operations.
- `test/` – unit tests for major components written in C.
- `test2/` – additional scenario and integration tests.

## Building

A top level `Makefile` is provided. From the repository root simply run:

```
make
```

This compiles the library and builds the main example program.

## Running Tests

To build and run the unit tests execute:

```
make test
```

Individual tests produce binaries under `test/` which can be run directly after compilation.
