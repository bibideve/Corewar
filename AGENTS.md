# AGENTS.md

## Cursor Cloud specific instructions

This is a pure C project (Corewar — an Epitech programming game) with no external services, databases, or containers. It builds two CLI binaries and a static library using `gcc` and `make`.

### Components

| Component | Output | Description |
|-----------|--------|-------------|
| `libmy` | `libmy/libmy.a` | Custom C utility library (static archive) |
| `asm` | `asm/asm` | Assembler: compiles `.s` champion files to `.cor` bytecode |
| `corewar` | `corewar/corewar` | Virtual machine: runs `.cor` champions in a battle arena |

### Build

```
make        # builds libmy, asm, corewar
make re     # clean rebuild
make fclean # remove all build artifacts
```

### Running

```
./asm/asm debug/<champion>.s                         # assemble a champion
./corewar/corewar debug/file1.cor debug/file2.cor    # run a battle
```

### Notes

- There are no automated tests, linters, or CI pipelines in this project. Validation is done by building and running the binaries.
- Sample champion `.s` files are in the `debug/` directory (e.g. `simple.s`, `zork.s`).
- The `corewar` VM prints "alive" messages during execution and declares a winner at the end, followed by a memory dump.
- The build requires only `gcc`, `make`, and `ar` (all standard on Linux).
