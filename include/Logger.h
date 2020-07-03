#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdarg.h>

inline void LOG(const char* msg, ...){
    va_list vl;
    va_start(vl, msg);
    char buffer[1000];
    char outmsg[1000];
    char format[] = "[%s]: %s\n";
    time_t now = time(NULL);
    // convert now to string form
    char dt[20];
    struct tm * p = localtime(&now);
    strftime(dt, 20, "%F %H:%M:%S ", p);
    snprintf(buffer, 1000, format, dt, msg);
    vsnprintf(outmsg, 1000, buffer, vl);
    std::string filePath ="debug_log.txt";
    std::ofstream ofs;
    ofs.open(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << outmsg;
    ofs.close();
}