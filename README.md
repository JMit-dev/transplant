# Transplant

A C++ utility for serializing and deserializing directory trees with file metadata preservation.

## Features

- Serialize directory structures to binary format
- Deserialize binary data back to directory trees
- Preserve file permissions and metadata
- Modern C++ implementation with class-based architecture
- Cross-platform support (Linux/Unix)

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
# Serialize a directory
./transplant -s -p /path/to/directory > output.bin

# Deserialize to a directory
./transplant -d -p /path/to/output < output.bin

# Deserialize with clobber (overwrite existing files)
./transplant -d -c -p /path/to/output < output.bin

# Show help
./transplant -h
```

## Architecture

The project follows modern C++ design principles with clear separation of concerns:

- **PathManager**: Handles path manipulation and tracking
- **BinaryIO**: Big-endian binary I/O operations
- **Serializer**: Directory tree serialization logic
- **Deserializer**: Directory tree deserialization logic
- **CommandLineParser**: Argument validation and parsing

## Testing

Tests use the Criterion framework:

```bash
cd build
make transplant_tests
./transplant_tests
```

## Requirements

- C++17 compatible compiler
- CMake 3.15+
- Criterion (for testing)
