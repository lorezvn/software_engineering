#include "richiesta_prestito.h"

RichiestaPrestito::RichiestaPrestito(int r_utente_id, int r_libro_id, std::string r_data_inizio, std::string r_data_fine) {
    utente_id = r_utente_id;
    libro_id = r_libro_id;
    data_inizio = r_data_inizio;
    data_fine = r_data_fine;
}

RichiestaPrestito* RichiestaPrestito::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    int utente_id;
    int libro_id;
    std::string data_inizio;
    std::string data_fine;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "utente_id") == 0) {
            utente_id = atoi(value);
        } else if (strcmp(key, "libro_id") == 0) {
            libro_id = atoi(value);
        } else if (strcmp(key, "data_inizio") == 0) {
            data_inizio = value;
        } else if (strcmp(key, "data_fine") == 0) {
            data_fine = value;
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new RichiestaPrestito(utente_id, libro_id, data_inizio, data_fine);
}