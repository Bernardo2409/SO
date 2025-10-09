#!/bin/bash

remove=false
[[ $1 == "-r" ]] && { remove=true; shift; }
dir=$1

# Verificar argumentos
if [[ -z $dir || ! -d $dir ]]; then
    echo "Uso: $0 [-r] <pasta>"; exit 1
fi

count=0

if $remove; then
    echo "Removendo prefixo 'new_'..."
    for f in "$dir"/new_*; do
        [[ -f $f ]] || continue
        newname="${f##*/}"
        newname="${newname#new_}"
        mv "$f" "$dir/$newname" && echo "  ${f##*/} -> $newname" && ((count++))
    done
else
    echo "Adicionando prefixo 'new_'..."
    for f in "$dir"/*; do
        [[ -f $f ]] || continue
        base="${f##*/}"
        [[ $base == new_* ]] && continue
        mv "$f" "$dir/new_$base" && echo "  $base -> new_$base" && ((count++))
    done
fi

echo "$count ficheiro(s) processado(s)."
