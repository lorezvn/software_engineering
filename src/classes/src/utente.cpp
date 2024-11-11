#include "utente.h"

Utente::Utente(std::string utente_nome, std::string utente_cognome, std::string utente_email) {
    nome = utente_nome;
    cognome = utente_cognome;
    email = utente_email;
}

Utente* Utente::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    std::string nome;
    std::string cognome;
    std::string email;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "nome") == 0) {
            nome = value;
        } else if (strcmp(key, "cognome") == 0) {
            cognome = value;
        } else if (strcmp(key, "email") == 0) {
            email = value;
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new Utente(nome, cognome, email);
}