#!/bin/bash 
# Testa se ano dado (ou ano actual, se nenhum for dado) 
# é bissexto ou comum. 

[[ $# -eq 1 ]] && year=$1 || year=$(date +%Y)

[[ $(($year % 400)) -eq 0 ]] && echo "Ano bissexto. Fevereiro tem 29 dias." || \
[[ $(($year % 4)) -eq 0 && $(($year % 100)) -ne 0 ]] && echo "Ano bissexto. Fevereiro tem 29 dias." || \
echo "Ano comum. Fevereiro tem 28 dias."