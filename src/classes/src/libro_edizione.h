#ifndef libro_edizione_h
#define libro_edizione_h

#include <string.h>
#include <iostream>

#include "../../utilities/src/constants.h"
#include "../../services/redis/src/con2redis.h"

class LibroEdizione {
    private:


    public:

        std::string ISBN;
        std::string titolo;
        int pagine;
        std::string casa_editrice;

        
        LibroEdizione(std::string libro_ISBN, std::string libro_titolo, int libro_pagine, std::string libro_casa_editrice);

        static LibroEdizione* fromRedisStream(redisReply* reply, int stream_num, int msg_num);
};

#endif