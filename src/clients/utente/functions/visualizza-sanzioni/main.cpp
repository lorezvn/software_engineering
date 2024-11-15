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

        // Creazione della query per visualizzare le sanzioni
        sprintf(query, "SELECT  s.costo, s.stato, s.motivazione, s.dataSanzione, u.username, b.nome "
                       "FROM Sanzione s "
                       "JOIN Utente u ON s.utente = u.id "
                       "JOIN Bibliotecario b ON s.bibliotecario = b.id "
                       "WHERE u.username = '%s'", client_id); 

        query_res = db.execQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        // Creazione della risposta
        int rows = PQntuples(query_res);
        if (rows == 0) {
            send_response_status(c2r, WRITE_STREAM, client_id, "NO_PENALTY_FOUND", msg_id, 0);
            continue;
        }

        // Elaborazione dei risultati e invio risposta
        for (int i = 0; i < rows; i++) {
            snprintf(response, RESPONSE_SIZE, "ID: %s, Costo: %s, Stato: %s, Motivazione: %s, Data: %s, Utente: %s, Bibliotecario: %s",
                     PQgetvalue(query_res, i, 0), PQgetvalue(query_res, i, 1),
                     PQgetvalue(query_res, i, 2), PQgetvalue(query_res, i, 3),
                     PQgetvalue(query_res, i, 4), PQgetvalue(query_res, i, 5),
                     PQgetvalue(query_res, i, 6));

            send_response_status(c2r, WRITE_STREAM, client_id, response, msg_id, 0);
        }

        PQclear(query_res);
    }
}
