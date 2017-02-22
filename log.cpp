/* Copyright(C)
 * For free
 * All right reserved
 * 
 */
/**
 * @file log.cpp
 * @brief 
 * @author wuzhibin(@baidu.com)
 * @version 1
 * @date 2017-01-16
 */

#include "log.h"
#include <time.h>
#include <cstdarg>
#include <cstring>

namespace dragon{
    namespace{

        std::string time_string(){
            char buf[128];
            time_t raw;
            time(&raw);
            struct tm lt;
            localtime_r(&raw, &lt);
            snprintf(buf, sizeof(buf), "%02d-%02d %02d:%02d:%02d",
                    lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);
            return buf;
        }

        const char* log_level_string(int level){
            switch (level)
            {
                case DEBUG: return "DEBUG";
                case TRACE: return "TRACE";
                case NOTICE: return "NOTICE";
                case WARNING: return "WARNING";
                case FATAL: return "FATAL";
                default: return "UNKNOWN";
            }
        }

        void print_log(file_t* file, std::string thread_name, int log_level, \
                        const char* fmt, va_list& arg_ptr){
            pthread_mutex_lock(&file->mutex);
            char msg[2048];
            vsnprintf(msg, sizeof(msg), fmt, arg_ptr);
            char format_msg[4096];
            std::string ts = time_string();
            snprintf(format_msg, sizeof(format_msg),
                    "%s %s %s %s\n", thread_name.c_str(), ts.c_str(), \
                    log_level_string(log_level), msg);  
            file->fd->write(format_msg, strlen(format_msg));
            file->fd->flush();
            pthread_mutex_unlock(&file->mutex);
        }
    }
    
    void close_file(file_t* file){
        // because ostream did not have close interface
        pthread_mutex_lock(&g_mutex);
        if (--file->ref == 0){
            if (file->fd != NULL){
                file->fd->flush();
            }
        }
        pthread_mutex_unlock(&g_mutex);
    }

    void thread_close(void* param){
        thread_log_t *value = (thread_log_t*)param;
        file_t *pfile = value->pfile;
        file_t *pwfile = value->pwfile;
        close_file(pfile);
        close_file(pwfile);
        return ;
    }

    void once_init_key(){
        std::cout << "g_log_key only init once" << std::endl;
        pthread_key_create(&g_log_key, thread_close);
    } 

    int init_log(int log_level, std::string filename){
        pthread_once(&g_once, once_init_key);
        g_log_level = log_level;
        thread_log_t *thread_value = new thread_log_t();
        pthread_setspecific(g_log_key, thread_value);
        thread_value->thread_name = "main";
        thread_value->pfile = &g_log_file;
        thread_value->pwfile = &g_wflog_file;
        g_log_file.ref++;
        g_wflog_file.ref++;
        if (filename == ""){
            g_log_file.fd = &std::cout;
            g_wflog_file.fd = &std::cerr;
            return 0;
        }
        g_log_file.fd = new std::ofstream(filename.c_str());
        std::string wffilename = filename + ".wf";
        g_wflog_file.fd = new std::ofstream(wffilename.c_str());  
        if (g_log_file.fd->fail()){
            std::cerr << time_string() << " open log file: " << filename << " fail" << std::endl;
            g_log_file.fd = NULL;
            return -1;
        }

        if (g_wflog_file.fd->fail()){
            std::cerr << time_string() << " open wflog file: " 
            << wffilename << " fail" << std::endl;
            g_wflog_file.fd = NULL;
            return -1;
        }
        return 0;
    }
    
    int thread_init_log(const std::string thread_name){
        pthread_once(&g_once, once_init_key);
        thread_log_t* thread_value = new thread_log_t();
        pthread_setspecific(g_log_key, thread_value);
        thread_value->thread_name = thread_name;
        thread_value->pfile = &g_log_file;
        thread_value->pwfile = &g_wflog_file;
        pthread_mutex_lock(&g_mutex);
        g_log_file.ref++;
        g_wflog_file.ref++;
        pthread_mutex_unlock(&g_mutex);
        return 0;
    }

    void log_fatal(const char* fmt, ...){
        if (g_log_level < FATAL){
            return;
        }
        thread_log_t *thread_value = (thread_log_t *)pthread_getspecific(g_log_key);
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        print_log(thread_value->pwfile, thread_value->thread_name, FATAL, fmt, arg_ptr);
        va_end(arg_ptr); 
    }
    void log_warning(const char* fmt, ...){
        if (g_log_level < WARNING){
            return;
        }
        thread_log_t *thread_value = (thread_log_t*)pthread_getspecific(g_log_key);
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        print_log(thread_value->pwfile, thread_value->thread_name, WARNING, fmt, arg_ptr); 
        return;
    }   
    void log_notice(const char* fmt, ...){
        if (g_log_level < NOTICE){
            return;
        }
        thread_log_t *thread_value = (thread_log_t*)pthread_getspecific(g_log_key);
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        print_log(thread_value->pfile, thread_value->thread_name, NOTICE, fmt, arg_ptr); 
        return;
    }

    void log_trace(const char* fmt, ...){
        if (g_log_level < TRACE){
            return;
        }
        thread_log_t *thread_value = (thread_log_t*)pthread_getspecific(g_log_key);
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        print_log(thread_value->pfile, thread_value->thread_name, TRACE, fmt, arg_ptr); 
        return;
    }

    void log_debug(const char* fmt, ...){
        if (g_log_level < DEBUG){
            return;
        }
        thread_log_t *thread_value = (thread_log_t*)pthread_getspecific(g_log_key);
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        print_log(thread_value->pfile, thread_value->thread_name, DEBUG, fmt, arg_ptr); 
        return;
    }
}
