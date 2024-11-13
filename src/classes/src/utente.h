#ifndef utente_h
#define utente_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../services/redis/src/con2redis.h"

class Utente {
    private:


    public:

        std::string nome;
        std::string cognome;
        std::string email;
        std::string username;
        
        Utente(std::string utente_nome, std::string utente_cognome, std::string utente_email, std::string utente_username);

        static Utente* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif