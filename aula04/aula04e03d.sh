#!/bin/bash

if [[ $# -ne 1 ]] || [[ ! -f "$1" ]]; then   # verifica se o ficheiro existe (segunda condição)
    echo "Uso: $0 <ficheiro_com_numeros>"
    exit 1
fi

# Lê, valida e ordena em uma linha
if ! sort -n "$1" | grep -E '^-?[0-9]+$' > /dev/null; then
    echo "Erro: ficheiro contém valores inválidos"
    exit 1
fi

echo "Números ordenados:"
sort -n "$1"