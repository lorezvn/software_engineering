#include "libro_edizione.h"

LibroEdizione::LibroEdizione(std::string libro_ISBN, std::string libro_titolo, int libro_pagine, std::string libro_casa_editrice, 
                             int libro_autore_id, std::string libro_genere) {
    ISBN = libro_ISBN;
    titolo = libro_titolo;
    pagine = libro_pagine;
    casa_editrice = libro_casa_editrice; 
    autore_id = libro_autore_id;
    genere = libro_genere;
}

LibroEdizione* LibroEdizione::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    std::string ISBN;
    std::string titolo;
    int pagine;
    std::string casa_editrice;
    int autore_id;
    std::string genere;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "ISBN") == 0) {
            ISBN = value;
        } else if (strcmp(key, "titolo") == 0) {
            titolo = value;
        } else if (strcmp(key, "pagine") == 0) {
            pagine = atoi(value);
        } else if (strcmp(key, "casa_editrice") == 0) {
            casa_editrice = value;
        } else if (strcmp(key, "autore_id") == 0) {
            autore_id = atoi(value);
        } else if (strcmp(key, "genere") == 0) {
            genere = value;
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new LibroEdizione(ISBN, titolo, pagine, casa_editrice, autore_id, genere);
}