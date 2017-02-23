/* Copyright(C)
 * For free
 * All right reserved
 * 
 */
/**
 * @file log.h
 * @brief 
 * @author wuzhibin(@baidu.com)
 * @version 1
 * @date 2017-01-16
 */

#ifndef DRAGON_LOG_H
#define DRAGON_LOG_H

#include <string>
#include <fstream>
#include <iostream>
#include <pthread.h>

namespace dragon{
    
    struct file_t{
        std::ostream *fd;
        pthread_mutex_t mutex;
        int ref;
    };
    
    struct thread_log_t{
        std::string thread_name;
        file_t *pfile;
        file_t *pwfile;
    };
    static pthread_key_t g_log_key;
    static pthread_once_t g_once = PTHREAD_ONCE_INIT;
    static int g_log_level;
    static file_t g_log_file = {NULL, PTHREAD_MUTEX_INITIALIZER, 0};
    static file_t g_wflog_file = {NULL, PTHREAD_MUTEX_INITIALIZER, 0};
    //static std::ostream* g_log_fd;
    //static std::ostream* g_wflog_fd;
    static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    enum {
        FATAL = 1,
        WARNING = 2,
        NOTICE = 4,
        TRACE = 8,
        DEBUG = 16
    };

    int init_log(int log_level = NOTICE, std::string filename = "");
    int thread_init_log(const std::string thread_name);
    //void close_log();

    
    void log_warning(const char* fmt, ...);
    void log_fatal(const char* fmt, ...);
    void log_notice(const char* fmt, ...);
    void log_trace(const char* fmt, ...);
    void log_debug(const char* fmt, ...);

}
#endif
