#include "main.h"

int main() {
    
    redisContext* c2r;
    redisReply* reply;

    PGresult *query_res;

    char query[QUERY_SIZE];
    char response[RESPONSE_SIZE]; 
    char msg_id[MSG_ID_SIZE];
    char first_key[KEY_SIZE];
    char client_id[VALUE_SIZE];
    char second_key[KEY_SIZE];
    char utente_id[PARAMETERS_SIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(true) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main utente BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        } 

        // stream_num = 0
        // msg_num = 0
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Check per la prima coppia chiave-valore
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    

        if(strcmp(first_key, "client_id") != 0){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        ReadStreamMsgVal(reply, 0, 0, 2, second_key);    
        ReadStreamMsgVal(reply, 0, 0, 3, utente_id);  

        int max_elements = ReadStreamMsgNumVal(reply, 0, 0);
        if (strcmp(second_key, "utente_id") != 0 || max_elements > 4) {
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Creazione della query per visualizzare le sanzioni
        sprintf(query, "SELECT * FROM Sanzione WHERE utente = %d", atoi(utente_id)); 

        query_res = db.execQuery(query, true);

         if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Sanzione*> sanzioni;

        for (int row = 0; row < PQntuples(query_res); row++) {
            Sanzione *sanzione = new Sanzione(
                atof(PQgetvalue(query_res, row, PQfnumber(query_res, "costo"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "stato"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "motivazione"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "data"))),
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "bibliotecario"))),
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "utente")))
            );

            sanzioni.push_back(sanzione);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row<PQntuples(query_res); row++){

            Sanzione *sanzione = sanzioni.front();

            sanzioni.pop_front();

            reply = RedisCommand(c2r, "XADD %s * row %d costo %f stato %s motivazione %s data %s bibl_id %d utente_id %d", WRITE_STREAM, row, 
                                 sanzione->costo, sanzione->stato.c_str(), sanzione->motivazione.c_str(), sanzione->data.c_str(),
                                 sanzione->bibl_id, sanzione->utente_id);
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
        }
    }
}
