#include "functions.h"

std::string get_current_timestamp() {

    // Current time come time_point
    auto now = std::chrono::system_clock::now();

    // Conversione a time_t
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    // Conversione a struct tm
    std::tm tm = *std::localtime(&current_time);

    // Format stringa
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    return oss.str();  
}

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id, const int nrows){
    redisReply *reply;

    printf("Response: %s %s %s %s %d\n", stream, client_id, resp_status, msg_id, nrows);

    reply = RedisCommand(c2r, "XADD %s * client_id %s resp_status %s num_rows %d", stream, client_id, resp_status, nrows);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);
}