#!/bin/bash

# Ficheiro com funções para serem importadas
function imprime_msg() {
    echo "A minha primeira funcao"
    return 0
}

function imprimir_msg() {
    echo "A data de hoje é $(date +%d/%m/%Y) "
    echo "o nome do computador é $(hostname)  "
    echo "O nome de utilizador é $(whoami)"
    return 0
}