#!/bin/bash 
# This script checks the existence of a file 

# Validação do número de argumentos

if [[ $# -ne 1 ]]; then  # ne = not equal
    echo "Erro: Este script requer exatamente 1 argumento."
    exit 1
fi

echo "Checking..." 
if [[ -f $1 ]] 
then 
 echo "$1 existe." 
else 
 echo "$1 não existe" 
fi 
echo "...done."