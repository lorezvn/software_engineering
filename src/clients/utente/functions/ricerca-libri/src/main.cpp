#include "main.h"

int main() {
    redisContext* c2r;
    redisReply* reply;
    PGresult* query_res;

    char query[QUERY_SIZE];
    char msg_id[MSG_ID_SIZE];
    char first_key[KEY_SIZE];
    char client_id[VALUE_SIZE];
    char second_key[KEY_SIZE];
    char titolo[PARAMETERS_SIZE];

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

        ReadStreamMsgVal(reply, 0, 0, 2, second_key);
        ReadStreamMsgVal(reply, 0, 0, 3, titolo);

        int max_elements = ReadStreamMsgNumVal(reply, 0, 0);
        if (strcmp(second_key, "titolo") != 0 || max_elements > 4) {
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        std::string ricerca = "%"+std::string(titolo)+"%";
        sprintf(query, "SELECT * FROM LibroEdizione WHERE titolo LIKE \'%s\'", ricerca.c_str());
        query_res = db.execQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<LibroEdizione*> libri;

        for (int row = 0; row < PQntuples(query_res); row++) {
            LibroEdizione* libro = new LibroEdizione(
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "ISBN"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "titolo"))),
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "pagine"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "casaEditrice")))
            );

            libri.push_back(libro);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row<PQntuples(query_res); row++){

            LibroEdizione *libro = libri.front();

            libri.pop_front();

            reply = RedisCommand(c2r, "XADD %s * row %d ISBN %s titolo %s pagine %d casa_editrice %s", WRITE_STREAM, row, 
                                 libro->ISBN.c_str(), libro->titolo.c_str(), libro->pagine, libro->casa_editrice.c_str());
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
        }
    }

    db.endDBConnection();
    return 0;
}
