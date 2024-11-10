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

#endif