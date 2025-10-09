#!/bin/bash
for file in "$@"; do
    chmod +x "$file"
    echo "Permissão de execução dada a $file"
done
