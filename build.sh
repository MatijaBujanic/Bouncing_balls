#!/bin/bash

# ========================
# Configuration
# ========================
OUTPUT="game"                       # Name of final executable
BUILD_DIR="build"                   # Where .o files go
CXX=g++                             # Compiler
CXXFLAGS="-std=c++26 -Wall -Wextra -O3 -march=native -mtune=native -flto"   # Compile flags
LIBS="-lsfml-system -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-network"  # SFML libraries

# ========================
# Clean option
# ========================
if [ "$1" == "clean" ]; then
    echo "Cleaning build files..."
    rm -rf "$BUILD_DIR" "$OUTPUT"
    echo "Done."
    exit 0
fi

# ========================
# Prepare build directory
# ========================
mkdir -p "$BUILD_DIR"

# ========================
# Find source files
# ========================
SOURCES=$(find . -name "*.cpp")
OBJECTS=""

# ========================
# Compile each source file
# ========================
for SRC in $SOURCES; do
    OBJ="$BUILD_DIR/$(basename ${SRC%.cpp}.o)"
    echo "Compiling $SRC -> $OBJ"
    $CXX $CXXFLAGS -c "$SRC" -o "$OBJ"
    if [ $? -ne 0 ]; then
        echo "❌ Compilation failed for $SRC"
        exit 1
    fi
    OBJECTS="$OBJECTS $OBJ"
done

# ========================
# Link all objects
# ========================
echo "Linking into $OUTPUT"
$CXX $OBJECTS -o "$OUTPUT" $LIBS
if [ $? -eq 0 ]; then
    echo "✅ Build successful! Run with ./$OUTPUT"
    if [ "$1" == "-r" ]; then
        ./$OUTPUT
    fi
else
    echo "❌ Linking failed."
    exit 1
fi
