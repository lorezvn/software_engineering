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

    while (true) {

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

        if (strcmp(first_key, "client_id") != 0) {
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

        // Creazione della query per visualizzare i prestiti
        sprintf(query, "SELECT * FROM Prestito WHERE utente = %d", atoi(utente_id)); 

        query_res = db.execQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Prestito*> prestiti;

        for (int row = 0; row < PQntuples(query_res); row++) {
            Prestito *prestito = new Prestito(
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "richiesta"))),
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "utente"))),
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "bibliotecario"))),
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "libro"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "dataInizio"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "dataFine"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "dataRest"))),
                (atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "isTerminato"))) != 0)
            );

            prestiti.push_back(prestito);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for (int row = 0; row < PQntuples(query_res); row++) {

            Prestito *prestito = prestiti.front();

            prestiti.pop_front();

            reply = RedisCommand(c2r, "XADD %s * row %d richiesta_id %d utente_id %d bibl_id %d libro_id %d data_inizio %s data_fine %s data_rest %s is_terminato %s",
                                 WRITE_STREAM, row, 
                                 prestito->richiesta_id, prestito->utente_id, prestito->bibl_id, prestito->libro_id, 
                                 prestito->data_inizio.c_str(), prestito->data_fine.c_str(), 
                                 prestito->data_rest.c_str(), prestito->toRedisFormat().c_str());
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
        }
    }

    db.endDBConnection();
    return 0;
}

