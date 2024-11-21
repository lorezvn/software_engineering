#ifndef functions_h
#define functions_h

#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>

#include "../../services/redis/src/con2redis.h"

std::string get_current_timestamp();

std::string replace_all(std::string input_str, const std::string& from, const std::string& to);

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id, const int nrows);

#endif