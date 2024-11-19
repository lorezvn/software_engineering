#include "prestito.h"


Prestito::Prestito(int p_richiesta_id, int p_utente_id, int p_bibl_id, int p_libro_id, 
                   std::string p_data_inizio, std::string p_data_fine, std::string p_data_rest, bool p_is_terminato) {
    richiesta_id = p_richiesta_id;
    utente_id = p_utente_id;
    bibl_id = p_bibl_id;
    libro_id = p_libro_id;
    data_inizio = p_data_inizio;
    data_fine = p_data_fine;
    data_rest = p_data_rest;
    is_terminato = p_is_terminato;
}


Prestito::Prestito(int p_richiesta_id, int p_bibl_id) {
    richiesta_id = p_richiesta_id;
    bibl_id = p_bibl_id;
}


Prestito* Prestito::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    int richiesta_id;
    int bibl_id;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "richiesta_id") == 0) {
            richiesta_id = atoi(value);
        } else if (strcmp(key, "bibl_id") == 0) {
            bibl_id = atoi(value);
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new Prestito(richiesta_id, bibl_id);
}
std::string Prestito::toRedisFormat() const {
    return is_terminato ? "true" : "false";
}