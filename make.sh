#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""
CLEAN=false

check_args() {
    if [ "$1" = "--clean" ] || [ "$1" = "-c" ]
    then
        CLEAN=true
    fi
}

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

set_current_root() {
    CURRENT_ROOT=$1
    change_dir "$ROOT/$CURRENT_ROOT"
}

try_make() {
    if $CLEAN = true
    then
        make clean
    else
        make
    fi

    check_last_command
}

make_folder() {
    change_dir $1

    printf "Inside $1\n"

    try_make

    printf "Done $1\n\n"

    change_dir "$ROOT/$CURRENT_ROOT"
}

check_args $1

# Start
printf "\n\n### Inizio script 'make.sh' ###\n\n"

# Services
printf "\n\n### SERVICES ###\n\n"

set_current_root src/services

make_folder database/src
make_folder redis/src

# Utilities
printf "\n\n### UTILITIES ###\n\n"

set_current_root src

make_folder utilities/src


# Classes
printf "\n\n### CLASSES ###\n\n"

set_current_root src

make_folder classes/src


# Server
printf "\n\n### SERVER ###\n\n"

set_current_root src

make_folder server/src


# Non func monitors
printf "\n\n### NON FUNC MONITORS ###\n\n"

set_current_root src

make_folder non-func-monitors/src


# Clients
printf "\n\n### BIBLIOTECARIO ###\n\n"

set_current_root src/clients/bibliotecario

make_folder handler/src


printf "\n\n### FORNITORE ###\n\n"

set_current_root src/clients/fornitore

make_folder handler/src


printf "\n\n### UTENTE ###\n\n"

set_current_root src/clients/utente

make_folder handler/src

make_folder functions/add-utente/src

printf "\n\n### 
Script 'make.sh' completato ### \n\n"
