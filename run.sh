#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""

check_args() {
    if [ "$1" = "--kill" ] || [ "$1" = "-k" ]
    then
        pkill main
        exit
    fi
}

check_last_command() {
    if [ $? -ne 0 ]
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

set_current_root() {
    CURRENT_ROOT=$1
    change_dir "$ROOT/$CURRENT_ROOT"
}

try_main() {
    ./main &
    check_last_command
}

run_main() {
    change_dir $1

    printf "Inside $1\n"

    try_main

    printf "Done $1\n\n"

    change_dir "$ROOT/$CURRENT_ROOT"
}

check_args $1

# Start
printf "\n\n### Inizio script 'run.sh' ###\n\n"

pkill main


# Log Database
printf "\n\n### LOG DATABASE ####\n\n"

set_current_root src

run_main non-func-monitors/bin


# Bibliotecario
printf "\n\n### BIBLIOTECARIO ###\n\n"

set_current_root src/clients/bibliotecario

run_main handler/bin

run_main functions/add-bibliotecario/bin
run_main functions/add-prestito/bin
run_main functions/add-richiesta-restock/bin
run_main functions/add-sanzione/bin
run_main functions/estingui-sanzione/bin
run_main functions/revoca-sanzione/bin
run_main functions/visualizza-richieste-prestito/bin


# Fornitore
printf "\n\n### FORNITORE ###\n\n"

set_current_root src/clients/fornitore

run_main handler/bin

run_main functions/add-fornitore/bin
run_main functions/visualizza-richieste-restock/bin


# Utente
printf "\n\n### UTENTE ###\n\n"

set_current_root src/clients/utente

run_main handler/bin

run_main functions/add-utente/bin
run_main functions/add-richiesta-prestito/bin
run_main functions/ricerca-libri/bin
run_main functions/update-username/bin
run_main functions/visualizza-sanzioni/bin
run_main functions/visualizza-prestiti/bin


printf "\n\n### Script 'run.sh' completato ### \n\n"

