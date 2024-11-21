#include "sanzione.h"

Sanzione::Sanzione(float sanz_costo, std::string sanz_motivazione, std::string sanz_data, 
                   int sanz_bibl_id, int sanz_utente_id) {
    costo = sanz_costo;
    motivazione = sanz_motivazione;
    data = sanz_data;
    bibl_id = sanz_bibl_id;
    utente_id = sanz_utente_id;
}

Sanzione::Sanzione(float sanz_costo, std::string sanz_stato, std::string sanz_motivazione, std::string sanz_data, 
                   int sanz_bibl_id, int sanz_utente_id) {
    costo = sanz_costo;
    stato = sanz_stato;
    motivazione = sanz_motivazione;
    data = sanz_data;
    bibl_id = sanz_bibl_id;
    utente_id = sanz_utente_id;
}

Sanzione* Sanzione::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    float costo;
    std::string motivazione;
    std::string data;
    int bibl_id;
    int utente_id;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "costo") == 0) {
            costo = atof(value);
        } else if (strcmp(key, "motivazione") == 0) {
            motivazione = replace_all(value, SPACE_REDIS_STRING, SPACE);
        } else if (strcmp(key, "data") == 0) {
            data = value;
        } else if (strcmp(key, "bibl_id") == 0) {
            bibl_id = atoi(value);
        } else if (strcmp(key, "utente_id") == 0) {
            utente_id = atoi(value);
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new Sanzione(costo, motivazione, data, bibl_id, utente_id);
}