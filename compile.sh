#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""


check_last_command() {
    if [$? -ne 0]
    then
        printf "\n\nErrore durante l'esecuzione dello script \n\n"
        exit
    fi
}

change_dir() {
    cd $1
    check_last_command
    printf "\nEntering $1 \n"
}

run_sh() {
    sh $1 $2
    check_last_command
}

change_dir src
change_dir services
change_dir database
change_dir db-scripts
run_sh create.sh
change_dir ..
change_dir ..
change_dir ..
change_dir ..
run_sh make.sh --c
run_sh make.sh

printf "\n\n### Script 'compile.sh' completato ### \n\n"