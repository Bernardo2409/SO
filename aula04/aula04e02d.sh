#!/bin/bash

comparar() {
    if [[ $1 -eq $2 ]]; then
        return 0        
    elif [[ $1 -gt $2 ]]; then
        return 1        
    else
        return 2        
    fi
}

if [[ $# -ne 2 ]]; then
    echo "Uso: $0 <num1> <num2>"
    exit 1
fi

comparar "$1" "$2"
resultado=$?

if [[ $resultado -eq 0 ]]; then
    echo "Os números são iguais."
elif [[ $resultado -eq 1 ]]; then
    echo "O maior número é $1."
else
    echo "O maior número é $2."
fi
