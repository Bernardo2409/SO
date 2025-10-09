#!/bin/bash
# aula03e04c.sh
# Valida 2 argumentos:

if [[ $# -ne 2 ]]; then
    echo "Uso: $0 <número 0-99> <string sec...>"
    exit 1
fi

arg1=$1
arg2=$2

# Validação do primeiro argumento (0–99)
case $arg1 in
    [0-9]|[1-9][0-9])   # 0–9 ou 10–99
        echo "Primeiro argumento válido: $arg1"
        ;;
    *)
        echo "Erro: o primeiro argumento deve ser um número entre 0 e 99."
        exit 1
        ;;
esac

# Validação do segundo argumento (começa por "sec")
case $arg2 in
    sec*)  # qualquer coisa que comece por sec
        echo "Segundo argumento válido: $arg2"
        ;;
    *)
        echo "Erro: o segundo argumento deve começar por 'sec'."
        exit 1
        ;;
esac

echo "Ambos os argumentos são válidos!"
