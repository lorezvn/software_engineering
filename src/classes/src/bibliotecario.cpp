#include "bibliotecario.h"

Bibliotecario::Bibliotecario(std::string bibl_nome, std::string bibl_cognome, std::string bibl_email, std::string bibl_data_ass) {
    nome = bibl_nome;
    cognome = bibl_cognome;
    email = bibl_email;
    data_ass = bibl_data_ass;
}

Bibliotecario* Bibliotecario::fromRedisStream(redisReply* reply, int stream_num, int msg_num) {

    std::string nome;
    std::string cognome;
    std::string email;
    std::string data_ass;

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
        } else if (strcmp(key, "data_ass") == 0) {
            data_ass = value;
        } else {
            throw std::invalid_argument("Errore stream: chiave non riconosciuta");
        }
    }

    return new Bibliotecario(nome, cognome, email, data_ass);
}

