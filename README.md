# mc-utils

A Minecraft utility tool for calculating item stacks, netherite requirements, and extracting crafting recipes from Minecraft JAR files.

## Features

- Calculate item stacks and totals
- Netherite beacon requirements calculator
- Extract and query crafting recipes from Minecraft client JARs
- Enchanting table and anvil requirements

## Build Process

The project uses a Makefile for building. It combines C code with a statically linked Rust library for recipe extraction.

### Prerequisites

- A C compiler
- Cargo (Rust toolchain)
- Make
- System libraries: libbz2 (for bzip2 compression in JAR handling)

### Build Targets

- **`make extractor`**: Builds the Rust library (`libmc_recipe_extractor.a`) in release mode. This creates the recipe extraction functionality.
- **`make mc-util`**: Builds the main C binary (`bin/mc-util`) by compiling `src/mc-utils.c` and statically linking it with the Rust library. This produces a single executable with all functionality.
- **`make noembed`**: Alias for `make mc-util` (legacy name).
- **`make clean`**: Removes built binaries and cleans the Rust target directory.

### Quick Build

To build the complete tool:

```bash
make mc-util
```

This will:
1. Build the Rust extractor library
2. Compile the C code
3. Statically link everything into `bin/mc-util`

### How It Works

- The Rust code (`extractor/`) provides a `#[no_mangle] extern "C"` function `extract_recipes()` for extracting recipes from Minecraft JARs.
- The C code (`src/`) handles CLI parsing, calculations, and calls the Rust function directly via static linking.
- No runtime dependencies on the Rust library because it is bundled into one binary.

## Usage

After building:

```bash
./bin/mc-util --help
./bin/mc-util --recipe diamond_pickaxe
./bin/mc-util --generate-recipes
```

## Project Structure

- `src/mc-utils.c`: Main C program
- `src/recipes.c`: Recipe parsing and display
- `src/netherite-left.c`: Netherite calculator
- `src/aux.c`: Helper functions
- `extractor/`: Rust recipe extractor
- `Makefile`: Build configuration
- `build.sh`: Alternative build script