#!usr/bin/env bash

HERE=$(dirname $(realpath $0))
CLANG_FORMAT=clang-format

check_clang_format() {
  if ! command -v $CLANG_FORMAT &>/dev/null; then
    echo "clang-format not found. Please install clang-format."
    exit 1
  fi
}

check_clang_format

# Recursively find all macros.h files (excluding the build directory)
find $HERE/.. -name macros.h | grep -v build | while read -r file; do
  echo "Formatting $file"
  $CLANG_FORMAT -i $file -style='{IndentWidth: 4, BasedOnStyle: LLVM, IndentPPDirectives: BeforeHash, UseTab: AlignWithSpaces, TabWidth: 4}'
done
