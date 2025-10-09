#!/bin/bash
function imprimir_msg() {

    echo " A data de hoje é $(date +%d/%m/%Y) "
    echo "o nome do computador é $(hostname)  "
    echo "O nome de utilizador é $(whoami)"

    return 0
}
imprimir_msg