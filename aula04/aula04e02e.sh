#!/bin/bash
function comparar() 
{
echo -e -n "\n Diz dois números: " 
read a b
if [[ $a -eq $b ]]; then
    return 0
else 
    if [[ $a -gt $b ]]; then
        return $a
    else 
        return $b
    fi
fi
}

comparar
resultado=$?

if [[ $resultado -eq 0 ]]; then
    echo "Os números são iguais"
else
    echo "O maior número é $resultado"
fi