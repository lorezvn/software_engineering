#ifndef richiesta_restock_h
#define richiesta_restock_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../utilities/src/functions.h"
#include "../../services/redis/src/con2redis.h"

class RichiestaRestock {
    private:


    public:

        int bibl_id;
        std::string forn_nome;
        int quantita;
        std::string istante;
        std::string edizione;
        std::string stato;

        
        RichiestaRestock(int r_bibl_id, std::string r_forn_nome, int r_quantita, std::string r_edizione);

        RichiestaRestock(int r_bibl_id, std::string r_forn_nome, int r_quantita, std::string r_edizione, 
                         std::string r_istante, std::string r_stato);

        static RichiestaRestock* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif