#!/bin/bash 
# For all the files in a folder, show their properties 

# Validação do número de argumentos
if [[ $# -ne 1 ]]; then
    echo "Erro: Este script requer exatamente 1 argumento."
    echo "Uso: $0 <nome_da_directoria>"
    exit 1
fi

# Validação do tipo de argumento (deve ser uma directoria)
if [[ ! -d $1 ]]; then
    echo "Erro: '$1' não é uma directoria ou não existe."
    echo "Por favor, forneça o nome de uma directoria válida."
    exit 1
fi

for f in $1/* 
do 
   file "$f" 
done