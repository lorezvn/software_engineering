#include "main.h"

int main() {
    
    redisContext* c2r;
    redisReply* reply;

    PGresult *query_res;
    char query[QUERY_SIZE];
    char msg_id[MSG_ID_SIZE];
    char first_key[KEY_SIZE];
    char client_id[VALUE_SIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    RichiestaRestock* richiesta;

    while(true) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main bibliotecario BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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
        
        // Conversione richiesta
        try {
            richiesta = RichiestaRestock::fromRedisStream(reply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "INSERT INTO RichiestaRestock (bibliotecario, fornitore, quantita, istante, edizione) VALUES (%d, \'%s\', %d, CURRENT_TIMESTAMP, \'%s\')", 
                        richiesta->bibl_id, richiesta->forn_nome.c_str(), 
                        richiesta->quantita, richiesta->edizione.c_str());

        query_res = db.execQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
    }

    db.endDBConnection();
    return 0;
}
