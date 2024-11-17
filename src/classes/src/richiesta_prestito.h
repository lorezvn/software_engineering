#ifndef richiesta_prestito_h
#define richiesta_prestito_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../services/redis/src/con2redis.h"

class RichiestaPrestito {
    private:


    public:

        int utente_id;
        int libro_id;
        std::string data_inizio;
        std::string data_fine;

        
        RichiestaPrestito(int r_utente_id, int r_libro_id, std::string r_data_inizio, std::string r_data_fine);

        static RichiestaPrestito* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif