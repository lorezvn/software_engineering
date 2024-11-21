#include "main.h"

int main() {
    
    redisContext* c2r;
    redisReply* reply;

    PGresult *t_res;
    PGresult *query_res;  
    char sql_cmd[9];  

    char query[QUERY_SIZE];
    char response[RESPONSE_SIZE]; 
    char msg_id[MSG_ID_SIZE];
    char first_key[KEY_SIZE];
    char client_id[VALUE_SIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    Restock* restock;

    while(true) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main Restock BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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
            restock = Restock::fromRedisStream(reply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Inizia la transazione
        sprintf(sql_cmd, "BEGIN");
        t_res = db.execActionQuery(sql_cmd);
        if (PQresultStatus(t_res) != PGRES_COMMAND_OK) {
            PQclear(t_res);
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;    
        }
        PQclear(t_res);

        // Prima query
        sprintf(query,
                "INSERT INTO Restock (richiesta, quantita, istante, fornitore, edizione) "
                "SELECT r.id, r.quantita, CURRENT_TIMESTAMP, r.fornitore, r.edizione "
                "FROM RichiestaRestock r WHERE r.id = %d ", 
                restock->richiesta_id);

        query_res = db.execActionQuery(query);
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK) {
            PQclear(query_res);
            sprintf(sql_cmd, "ROLLBACK");
            db.execActionQuery(sql_cmd);
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;    
        }
        PQclear(query_res);

        // Seconda query
        sprintf(query, 
                "INSERT INTO LibroFisico (edizione) "
                "SELECT r.edizione " 
                "FROM RichiestaRestock r, generate_series(1, (SELECT r.quantita FROM RichiestaRestock r WHERE r.id = %d)) gs "
                "WHERE r.id = %d", 
                restock->richiesta_id, restock->richiesta_id);
        
        query_res = db.execActionQuery(query);
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK) {
            PQclear(query_res);
            sprintf(sql_cmd, "ROLLBACK");
            db.execActionQuery(sql_cmd);
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;    
        }
        PQclear(query_res);

        // Concludi la transazione
        sprintf(sql_cmd, "COMMIT");
        t_res = db.execActionQuery(sql_cmd);
        if (PQresultStatus(t_res) != PGRES_COMMAND_OK) {
            PQclear(t_res);
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;    
        }
        PQclear(t_res);

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
    }

    db.endDBConnection();
    return 0;
}