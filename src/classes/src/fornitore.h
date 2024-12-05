#ifndef fornitore_h
#define fornitore_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../utilities/src/functions.h"
#include "../../services/redis/src/con2redis.h"

class Fornitore {
    private:


    public:

        std::string nome;
        std::string email;
        
        Fornitore(std::string fornitore_nome, std::string fornitore_email);

        static Fornitore* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif