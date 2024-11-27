#include "richiesta_prestito.h"

RichiestaPrestito::RichiestaPrestito(int r_utente_id, std::string r_edizione) {
    utente_id = r_utente_id;
    edizione = r_edizione;
}

RichiestaPrestito::RichiestaPrestito(int r_utente_id, std::string r_edizione, std::string r_istante, std::string r_stato) {
    utente_id = r_utente_id;
    edizione = r_edizione;
    istante = r_istante;
    stato = r_stato;
}

RichiestaPrestito* RichiestaPrestito::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    int utente_id;
    std::string edizione;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "utente_id") == 0) {
            utente_id = atoi(value);
        } else if (strcmp(key, "edizione") == 0) {
            edizione = value;
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new RichiestaPrestito(utente_id, edizione);
}