#include<stdio.h>
#include<time.h>
#ifndef LOG_H
#define LOG_H
#define S_DEBUG 0
#define S_INFO  1
#define S_WARN  2
#define S_ERROR 3
#define S_CRIT  4
static const char * LOG_STR[]={"DEBUG","INFO","WARN","ERROR","CRIT"};
#define LOG(level,format,...) do{\
    time_t now=time(NULL);      \
    char msg[1024];     \
    char buf[64];   \
    sprintf(msg,format,##__VA_ARGS__);\
    strftime(buf,sizeof(buf),"%Y/%m/%d %H:%M",localtime(&now));\
    fprintf(stdout,"[%s][%s][file:%s][line:%d]%s\n",buf,LOG_STR[level],__FILE__,__LINE__,msg);\
    if(level==S_ERROR)\
        exit(-1);\
    }while(0)
#endif
