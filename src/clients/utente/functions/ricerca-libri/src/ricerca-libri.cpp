#include "ricerca-libri.h"

int main() {
    redisContext* c2r;
    redisReply* reply;
    PGresult* query_res;

    char query[QUERY_SIZE];
    char msg_id[MSG_ID_SIZE];
    char first_key[KEY_SIZE];
    char client_id[VALUE_SIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while (true) {
        reply = RedisCommand(c2r, "XREADGROUP GROUP main utente BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);
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

        std::string titolo;
        char key[KEY_SIZE];
        char value[PARAMETERS_SIZE];

        int max_elements = ReadStreamMsgNumVal(reply, 0, 0);
        for (int elem_num = 2; elem_num < max_elements; elem_num += 2) {
            ReadStreamMsgVal(reply, 0, 0, elem_num, key);
            ReadStreamMsgVal(reply, 0, 0, elem_num + 1, value);

            if (strcmp(key, "titolo") == 0) {
                titolo = value;
            } else {
                send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
                continue;
            }
        } 

        sprintf(query, "SELECT ISBN, titolo, pagine, casaEditrice FROM LibroEdizione WHERE titolo ILIKE '%%%s%%'", titolo.c_str());
        query_res = db.execQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::string result = format_result(query_res);
        send_response_data(c2r, WRITE_STREAM, client_id, result.c_str(), msg_id);
    }
}
