/* Copyright(C)
 * For free
 * All right reserved
 * 
 */
/**
 * @file demo.cpp
 * @brief 
 * @author wuzhibin(@baidu.com)
 * @version 1
 * @date 2017-01-13
 */
#include <pthread.h>
#include "log.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>

#ifndef __NR_gettid
#define __NR_gettid 224
#endif

#define gettid() syscall(__NR_gettid)


void* thread_fun_notice(void* param = NULL){
    do{
        pthread_t tid = gettid();
        char name[1024];
        snprintf(name, sizeof(name), "thread_%d", tid);
        dragon::thread_init_log(name);
        dragon::log_notice("====1 begin===="); 
    }while (0);
    return NULL;
}

void* thread_fun_trace(void* param = NULL){
    do{
        pthread_t tid = gettid();
        char name[1024];
        snprintf(name, sizeof(name), "thread_%d", tid);
        dragon::thread_init_log(name);
        dragon::log_warning("++++2 begin++++"); 
    }while (0);
    return NULL;
}

/**
 * @brief main 
 *
 * @param argc
 * @param argv[]
 *
 * @return 
 */
int main(int argc, char* argv[]){
    dragon::init_log(dragon::DEBUG);
    
    dragon::log_notice("start");   
    pthread_t t1, t2;
    void *tmp1, *tmp2;
    pthread_create(&t1, NULL, thread_fun_notice, NULL);
    pthread_create(&t2, NULL, thread_fun_trace, NULL);

    pthread_join(t1, &tmp1);
    pthread_join(t2, &tmp2);
    dragon::log_notice("end");
    
    return 0;
}
