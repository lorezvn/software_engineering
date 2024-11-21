#include "restock.h"

Restock::Restock(int r_richiesta_id) {
    richiesta_id = r_richiesta_id;
}

Restock::Restock(int r_richiesta_id, int r_quantita, std::string r_istante, std::string r_forn_nome, std::string r_edizione) {
    richiesta_id = r_richiesta_id;
    quantita = r_quantita;
    istante = r_istante;
    forn_nome = r_forn_nome;
    edizione = r_edizione;
}

Restock* Restock::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

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
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new Restock(richiesta_id);
}
