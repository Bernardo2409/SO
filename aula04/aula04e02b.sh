#!/bin/bash
function numeric_to_string()
{
    case "$1" in
        1)
            echo "Um"
            ;;
        2)
            echo "Dois"
            ;;
        3)
            echo "Três"
            ;;
        *)
            echo "Outro numero"
    esac
    # Retorna o valor passado como argumento
    return $1
}

# Chama a função
numeric_to_string $1

# Acessa o valor de retorno através de $?
echo "Valor de retorno da função: $?"