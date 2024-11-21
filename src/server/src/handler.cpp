#include "handler.h"

Handler::Handler(const char* redis_ip, int redis_port, std::string requests[], int num_requests) {
    
    c2r = redisConnect(redis_ip, redis_port);
    this->requests = requests;
    this->num_requests = num_requests;

    initHandlerStreams();
}

bool Handler::sendToFunctions(int client_id, std::string msg) {

    redisReply* reply;
    bool is_valid; 
    int i;

    // Ottieni il tipo di richiesta
    for (i = 0; i < msg.length(); i++) {
        if (msg[i] == 32)  break;   // 32 è il carattere Spazio in ASCII
    }

    if (i > msg.length()) {
        std::cout << "\nMessaggio non valido" << std::endl;
        return false;
    }

    std::string req_type = msg.substr(0, i); // Estrae il tipo di richiesta dalla stringa del messaggio
    std::string req_cmd  = msg.substr(i);    // Estrae il comando della richiesta dalla stringa del messaggio

    is_valid = false;
    for (i = 0; i < num_requests; i++) {
        if (req_type.compare(requests[i]) == 0) {
            is_valid = true;
            break;
        }
    }

    // Tipo di richiesta non valido
    if (!is_valid) {
        std::cout << "\nTipo di richiesta non valido" << std::endl;
        return false;
    }

    // Invia il comando sullo stream -in del gestore corrispondente
    std::string redis_cmd = "XADD " + req_type + "-in * client_id " + std::to_string(client_id) + " " + req_cmd;

    std::cout << "\n" << redis_cmd << std::endl;

    reply = RedisCommand(c2r, redis_cmd.c_str());
    assertReply(c2r, reply);
    return true;
}

bool Handler::readFromFunctions(std::string* out_str_ptr, int* client_id_ptr) {

    redisReply* reply;
    char msg_id[MSG_ID_SIZE];
    char tmp_buffer[PARAMETERS_SIZE];
    char client_id[VALUE_SIZE];
    char resp_status[30];
    char num_rows[30];
    char row[30];
    int i, j, num_rows_int, curr_row, row_columns;
    std::string tmp_str;
    std::string out_str;

    for (i = 0; i < num_requests; i++) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main handler COUNT 1 STREAMS %s-out >", requests[i].c_str());
        assertReply(c2r, reply);
        if (ReadNumStreams(reply) == 0)     // Se vuoto, controlla il prossimo gestore
            continue;

        // Ottieni l'ID del client
        ReadStreamMsgVal(reply, 0, 0, 0, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);

        if (strcmp(tmp_buffer, "client_id") != 0)
            continue;   

        *client_id_ptr = strtol(client_id, NULL, 10);

        // Ottieni il numero di righe
        ReadStreamMsgVal(reply, 0, 0, 4, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 5, num_rows);

        if (strcmp(tmp_buffer, "num_rows") != 0)
            continue;   

        num_rows_int = strtol(num_rows, NULL, 10);

        // Ottieni lo stato della risposta
        ReadStreamMsgVal(reply, 0, 0, 2, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 3, resp_status);

        if (strcmp(tmp_buffer, "resp_status") != 0)
            continue;   

        if (strcmp(resp_status, "REQUEST_SUCCESS") != 0)
            num_rows_int = 0;   

        freeReplyObject(reply);

        curr_row = 0;

        // Aggiungi lo stato della risposta
        tmp_str = resp_status;
        out_str = tmp_str + "\n";

        while (curr_row < num_rows_int - 1) {

            reply = RedisCommand(c2r, "XREADGROUP GROUP main handler BLOCK 0 COUNT 1 STREAMS %s-out >", requests[i].c_str());

            assertReply(c2r, reply);
            if (ReadNumStreams(reply) == 0) {
                out_str = "BAD_RESPONSE";
                break;
            }

            // Controlla se la prima coppia chiave/valore è il num_rows
            ReadStreamMsgVal(reply, 0, 0, 0, tmp_buffer);    
            ReadStreamMsgVal(reply, 0, 0, 1, row);

            if (strcmp(tmp_buffer, "row")) {
                out_str = "BAD_TUPLES";
                break;
            }

            curr_row = strtol(row, NULL, 10);
            row_columns = ReadStreamMsgNumVal(reply, 0, 0);

            out_str = out_str + "{";

            for (j = 2; j < row_columns; j += 2) {
                ReadStreamMsgVal(reply, 0, 0, j, tmp_buffer);
                tmp_str = tmp_buffer;
                out_str = out_str + tmp_str + ": ";

                ReadStreamMsgVal(reply, 0, 0, j + 1, tmp_buffer);
                tmp_str = tmp_buffer;
                out_str = out_str + tmp_str + ", ";
            }

            out_str = out_str + "}\n";

        }

        *out_str_ptr = out_str;

        return true;
    }

    return false;
}

void Handler::initHandlerStreams() {

    redisReply* reply; 
    std::string read_stream; 
    std::string write_stream; 

    for (int i = 0; i < num_requests; i++) { 

        read_stream  = requests[i] + "-in"; 
        write_stream = requests[i] + "-out"; 
        
        reply = RedisCommand(c2r, "DEL %s", read_stream.c_str()); 
        assertReply(c2r, reply); 

        reply = RedisCommand(c2r, "DEL %s", write_stream.c_str()); 
        assertReply(c2r, reply); 

        initStreams(c2r, read_stream.c_str()); 
        initStreams(c2r, write_stream.c_str()); 
    }
}
