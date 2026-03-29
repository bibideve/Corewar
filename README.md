# Corewar

Corewar is a two-part C project:

- `asm/asm`: an assembler that turns `.s` champion sources into `.cor` bytecode
- `corewar/corewar`: a virtual machine that loads and executes champion bytecode in the arena

The objective is to run multiple champions in the same memory space and determine which one survives the battle.

## Project layout

```text
.
├── asm/                 # Assembler binary output
├── corewar/             # Virtual machine binary output
├── debug/               # Sample champions used for manual testing
├── include/             # Shared headers
├── libmy/               # Utility library
└── src/
    ├── asm/             # Assembler sources
    └── vm/              # Virtual machine sources
```

## Build

```bash
make
```

This builds:

- `./asm/asm`
- `./corewar/corewar`

To clean generated files:

```bash
make clean
make fclean
```

## Assembler usage

```bash
./asm/asm [options] input.s [output.cor]
```

Examples:

```bash
./asm/asm debug/simple.s
./asm/asm debug/zork.s zork.cor
```

If no output path is provided, the assembler generates `<input_basename>.cor`.

Useful flags:

- `-h`, `--help`: display usage
- `-debug`: print additional compilation details
- `-Wall`: enable all warning flags
- `-Werror`: treat warnings as errors

## Virtual machine usage

```bash
./corewar/corewar [-dump cycle] [-n id] [-a address] champion1.cor champion2.cor [champion3.cor champion4.cor]
```

Examples:

```bash
./corewar/corewar debug/simple.cor debug/simple.cor
./corewar/corewar -dump 100 debug/zork.cor debug/labels.cor
```

Rules:

- the VM requires between 2 and 4 champions
- `-dump` prints the arena state after the requested cycle
- `-n` sets a champion id
- `-a` sets a load address

## Quick verification

A small smoke test script is provided:

```bash
./scripts/smoke_test.sh
```

It builds the project, assembles a couple of sample champions, and runs a basic VM check.

For a full round-robin tournament over the bundled champions:

```bash
./scripts/championship.sh
```

You can also pass your own `.s` or `.cor` entrants:

```bash
./scripts/championship.sh debug/zork.s debug/simple.s debug/github.s
```

The championship script builds the project, assembles source entrants when needed, plays every pairing twice with reversed load order, and prints a standings table based on actual VM winners.

By default, each duel is allowed to run for up to 2 minutes before being marked unresolved. You can override that with `CHAMPIONSHIP_TIMEOUT`, for example:

```bash
CHAMPIONSHIP_TIMEOUT=30s ./scripts/championship.sh debug/zork.s debug/github.s
```

## Notes

- This repository contains sample sources in `debug/` that are useful for manual validation.
- The project was originally developed as an educational Corewar implementation and has been cleaned up to improve stability, documentation, and repository hygiene.
