#include "fornitore.h"

Fornitore::Fornitore(std::string fornitore_nome, std::string fornitore_email) {
    nome = fornitore_nome;
    email = fornitore_email;
}

Fornitore* Fornitore::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    std::string nome;
    std::string email;

    char key[KEY_SIZE];
    char value[PARAMETERS_SIZE];

    int max_elements = ReadStreamMsgNumVal(reply, stream_num, msg_num);

    for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, elem_num + 1, value);

        if (strcmp(key, "nome") == 0) {
            nome = replace_all(value, SPACE_REDIS_STRING, SPACE);
        } else if (strcmp(key, "email") == 0) {
            email = value;
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new Fornitore(nome, email);
}

