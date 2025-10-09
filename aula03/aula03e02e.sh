#!/bin/bash 
# Conditional block if 
if [[ $1 -gt 5 && $1 -lt 10 ]]
then 
    echo "número maior que 5 e menor do que 10" 
else 
    echo "O número não está entre 5 e 10" 
fi