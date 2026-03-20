# Turing Machine Simulator

A console-based 1-tape and 2-tape Turing Machine simulator written in C++. Reads machine configuration and tape input from a file, then visualizes the execution step-by-step in the terminal.

---

## Features

- Supports **1-tape and 2-tape** Turing Machines
- Real-time **visual display** of tape contents and head position in the terminal
- **Pause/Resume** execution with `P`
- **Stop** execution early with `E`
- Automatically **expands the tape** left or right as the head moves beyond current bounds
- Detects and reports whether the machine **halted normally** or hit a **missing transition**
- Tape growth **limit of 100 cells** to catch infinite loops

---

## Build

Requires Windows (uses `<windows.h>` and `<conio.h>`).

Compile with any C++ compiler, e.g. MSVC or MinGW:

```bash
g++ -o tm_simulator main.cpp
```

Run by passing the input file as a command-line argument:

```bash
tm_simulator.exe input.txt
```

---

## Input File Format

### 1-Tape Machine

```
1
<tape contents>
<initial head position (1-indexed)>
<transition rules...>
```

Each transition rule line:
```
<currentState> <currentSymbol> <newSymbol> <direction> <newState>
```

### 2-Tape Machine

```
2
<tape1 contents>
<tape2 contents>
<head1 position (1-indexed)>
<head2 position (1-indexed)>
<transition rules...>
```

Each transition rule line:
```
<currentState> <currentSymbol1> <currentSymbol2> <newSymbol1> <newSymbol2> <direction1> <direction2> <newState>
```

### Notes

- Use `b` in the input file to represent a **blank** (space) symbol — it is converted internally.
- Directions: `R` (right), `L` (left), `S` or any other character = stay (no movement).
- States are strings; the machine starts in state `0` and halts when no matching rule is found for the current state.

### Example (1-tape)

```
1
101
1
0 1 1 R 0
0 0 0 R 1
1 1 1 R 1
1 b b S halt
```
