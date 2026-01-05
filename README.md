# Transplant

[![CI](https://github.com/JMit-dev/transplant/actions/workflows/ci.yml/badge.svg)](https://github.com/JMit-dev/transplant/actions/workflows/ci.yml)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A C++ utility for serializing and deserializing directory trees with file metadata preservation.

## Features

- Serialize directory structures to binary format
- Deserialize binary data back to directory trees
- Preserve file permissions and metadata
- Modern C++ implementation with class-based architecture
- Cross-platform support (Linux/Unix)

## Building

```bash
cmake -B build
cmake --build build
```

The compiled binary will be in `build/transplant`.

## Usage

```bash
# Serialize a directory
build/transplant -s -p /path/to/directory > output.bin

# Deserialize to a directory
build/transplant -d -p /path/to/output < output.bin

# Deserialize with clobber (overwrite existing files)
build/transplant -d -c -p /path/to/output < output.bin

# Show help
build/transplant -h
```

## Architecture

The project follows modern C++ design principles with clear separation of concerns:

- **PathManager**: Handles path manipulation and tracking
- **BinaryIO**: Big-endian binary I/O operations
- **Serializer**: Directory tree serialization logic
- **Deserializer**: Directory tree deserialization logic
- **CommandLineParser**: Argument validation and parsing

## Testing

Tests use the Criterion framework. All 39 active unit tests pass:

```bash
cmake --build build
build/transplant_tests
```

Test coverage includes:
- Command line argument validation (13 tests)
- Path buffer operations (7 tests)
- File serialization (7 tests)
- File deserialization (8 tests)
- Directory operations (4 tests)

## Requirements

- C++17 compatible compiler
- CMake 3.15+
- Criterion (for testing)
