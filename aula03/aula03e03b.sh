#!/bin/bash 
# This script checks the existence of a file and provides detailed information

# Validação do número de argumentos
if [[ $# -ne 1 ]]; then  # ne = not equal
    echo "Erro: Este script requer exatamente 1 argumento."
    echo "Uso: $0 <caminho>"
    exit 1
fi

echo "Checking '$1'..." 

# Verificar se existe
if [[ -e $1 ]]; then
    echo "$1 existe."

    # Verificar o tipo de ficheiro
    if [[ -f $1 ]]; then
        echo "  - É um ficheiro regular"
    elif [[ -d $1 ]]; then
        echo "  - É uma diretoria"
    elif [[ -L $1 ]]; then
        echo "  - É um link simbólico"
    elif [[ -c $1 ]]; then
        echo "  - É um dispositivo de caracteres"
    elif [[ -b $1 ]]; then
        echo "  - É um dispositivo de blocos"
    elif [[ -p $1 ]]; then
        echo "  - É um pipe nomeado (FIFO)"
    elif [[ -S $1 ]]; then
        echo "  - É um socket"
    fi
    
    # Verificar permissões
    echo "  - Permissões:"
    if [[ -r $1 ]]; then
        echo "    * Legível (readable)"
    fi
    if [[ -w $1 ]]; then
        echo "    * É possível escrever (writable)"
    fi
    if [[ -x $1 ]]; then
        echo "    * Executável (executable)"
    fi
    
    # Verificar características especiais
    if [[ -s $1 ]]; then
        echo "  - Tem conteúdo (size > 0)"
    else
        echo "  - Está vazio (size = 0)"
    fi
    
    if [[ -O $1 ]]; then
        echo "  - És o proprietário"
    fi
    
    if [[ -G $1 ]]; then
        echo "  - Pertence ao teu grupo"
    fi
    
    # Para ficheiros, verificar se tem permissões especiais
    if [[ -f $1 ]]; then
        if [[ -u $1 ]]; then
            echo "  - Tem bit SUID definido"
        fi
        if [[ -g $1 ]]; then
            echo "  - Tem bit SGID definido"
        fi
        if [[ -k $1 ]]; then
            echo "  - Tem sticky bit definido"
        fi
    fi
    
else 
    echo "$1 não existe"
fi 

echo "...done."