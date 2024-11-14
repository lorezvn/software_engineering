#ifndef bibliotecario_h
#define bibliotecario_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../services/redis/src/con2redis.h"

class Bibliotecario {
    private:


    public:

        std::string nome;
        std::string cognome;
        std::string email;
        std::string data_ass;
        
        Bibliotecario(std::string bibl_nome, std::string bibl_cognome, std::string bibl_email, std::string bibl_data_ass);

        static Bibliotecario* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif