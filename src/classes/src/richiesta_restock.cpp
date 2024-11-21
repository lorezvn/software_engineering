#include "richiesta_restock.h"

RichiestaRestock::RichiestaRestock(int r_bibl_id, std::string r_forn_nome, int r_quantita, std::string r_edizione) {
    bibl_id = r_bibl_id;
    forn_nome = r_forn_nome;
    quantita = r_quantita;
    edizione = r_edizione;
}

RichiestaRestock::RichiestaRestock(int r_bibl_id, std::string r_forn_nome, int r_quantita, std::string r_edizione, 
                                   std::string r_istante, std::string r_stato) {
    bibl_id = r_bibl_id;
    forn_nome = r_forn_nome;
    quantita = r_quantita;
    istante = r_istante;
    edizione = r_edizione;
    stato = r_stato;
}

RichiestaRestock* RichiestaRestock::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    int bibl_id;
    std::string forn_nome;
    int quantita;
    std::string edizione;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "bibl_id") == 0) {
            bibl_id = atoi(value);
        } else if (strcmp(key, "forn_nome") == 0) {
            forn_nome = replace_all(value, SPACE_REDIS_STRING, SPACE);
        } else if (strcmp(key, "quantita") == 0) {
            quantita = atoi(value);
        } else if (strcmp(key, "edizione") == 0) {
            edizione = value;
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new RichiestaRestock(bibl_id, forn_nome, quantita, edizione);
}