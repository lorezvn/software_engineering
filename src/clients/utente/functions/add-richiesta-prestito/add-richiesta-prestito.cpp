#include "add-richiesta-prestito.h"

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

    while(true) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main prestito BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);
        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        } 

        ReadStreamNumMsgID(reply, 0, 0, msg_id);
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    

        if (strcmp(first_key, "client_id") != 0) {
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        int libro_id, utente_id;
        std::string dataInizio, dataFine;
        char key[KEY_SIZE];
        char value[VALUE_SIZE];

        int max_elements = ReadStreamMsgNumVal(reply, 0, 0);
        for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
            ReadStreamMsgVal(reply, 0, 0, elem_num, key);
            ReadStreamMsgVal(reply, 0, 0, elem_num + 1, value);

            try {
                if (strcmp(key, "libro_id") == 0) {
                    libro_id = atoi(value);
                } else if (strcmp(key, "utente_id") == 0) {
                    utente_id = atoi(value);
                } else if (strcmp(key, "dataInizio") == 0) {
                    dataInizio = value;
                } else if (strcmp(key, "dataFine") == 0) {
                    dataFine = value;
                } else {
                    throw std::invalid_argument("Errore stream: chiave non riconosciuta");
                }
            } catch(std::invalid_argument exp) {
                send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
                continue;
            }
        }

        sprintf(query, "INSERT INTO RichiestaPrestito (dataInizio, dataFine, istante, stato, libro, utente) VALUES ('%s', '%s', CURRENT_TIMESTAMP, 'IN ATTESA', %d, %d)", 
                        dataInizio.c_str(), dataFine.c_str(), libro_id, utente_id);

        query_res = db.execQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
    }
}
