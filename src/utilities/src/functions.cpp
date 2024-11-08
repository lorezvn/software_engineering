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