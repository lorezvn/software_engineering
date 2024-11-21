#ifndef richiesta_prestito_h
#define richiesta_prestito_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../utilities/src/functions.h"
#include "../../services/redis/src/con2redis.h"

class Sanzione {
    private:


    public:

        float costo;
        std::string stato;
        std::string motivazione;
        std::string data;
        int bibl_id;
        int utente_id;

        Sanzione(float sanz_costo,std::string sanz_motivazione, std::string sanz_data, int sanz_bibl_id, int sanz_utente_id);

        Sanzione(float sanz_costo, std::string sanz_stato, std::string sanz_motivazione, std::string sanz_data, 
                 int sanz_bibl_id, int sanz_utente_id);

        static Sanzione* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif