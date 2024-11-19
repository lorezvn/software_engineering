#ifndef richiesta_prestito_h
#define richiesta_prestito_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../services/redis/src/con2redis.h"

class Prestito {
    private:


    public:

        int richiesta_id;
        int utente_id;
        int bibl_id;
        int libro_id;
        std::string data_inizio;
        std::string data_fine;
        std::string data_rest;
        bool is_terminato;

        
        Prestito(int p_richiesta_id, int p_bibl_id);
        Prestito(int p_richiesta_id, int p_utente_id, int p_bibl_id, int p_libro_id, 
                 std::string p_data_inizio, std::string p_data_fine, 
                 std::string p_data_rest, bool p_is_terminato); // Costruttore completo

        static Prestito* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
        std::string toRedisFormat() const;


};

#endif