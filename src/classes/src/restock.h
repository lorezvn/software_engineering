#ifndef restock_h
#define restock_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../services/redis/src/con2redis.h"

class Restock {
    private:


    public:

        int richiesta_id;
        int quantita;
        std::string istante;
        std::string forn_nome;
        std::string edizione;
        
        Restock(int r_richiesta_id);

        Restock(int r_richiesta_id, int r_quantita, std::string r_istante, std::string r_forn_nome, std::string r_edizione);

        static Restock* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif