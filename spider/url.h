#include"head.h"
#include"LOG.h"
#ifndef URL_H
#define URL_H

typedef struct Surl{
    char *url;
    int level;
    int type;
}Surl;

typedef struct Url{
    char *domain;
    char *path;
    int port;
    char *ip;
    int level;
}Url;

static queue<Surl  *> surl_queue;
static queue<Url *> ourl_queue;
static map<string,string> host_ip_map;

static surl2url(Surl *surl){
    Url *ourl=(Url *)calloc(1,sizeof(Url));//使用calloc动态分配内存
    char *p=strchr(surl->url,'/');//查询url中出现首次/的位置
    if(p==NULL){
        ourl->domain=surl->url;
        ourl->path=surl->url+strlen(surl->url);
    }
    else{
        *p='\0';                //覆盖'/'
        ourl->domain = surl->url;
        ourl->path = p+1;
    }
    //port端口，在冒号后面
    p=strrchr(ourl->domain,':');//反向查找冒号
    if(p!=NULL){
        *p='\0';
        ourl->port=atol(p+1);
        if(ourl->port==0)
            ourl->port=80;
    }
    else{
        ourl->port=80;
    }
    
    ourl->level=surl->level;
    return ourl;
}

//dns解析回调函数
static void callback(int result,char type,int count,int ttl,void *addresses,void *arg){
    Url *ourl=(Url *)arg;
    struct in_addr *addrs=(in_addr *) addresses;
    
    if(result!=DNS_ERR_NONE||count==0){//如果结果出错或者ip地址数量为0
        LOG(S_ERROR,"dns 解析出错:%s",ourl->domain);
    }
    else{
        char *ip=inet_ntoa(addrs[0]);
        LOG(S_DEBUG,"dns 解析成功:%s",ourl->domain);
        host_ip_map[ourl->domain]=strdup(ip);
        ourl->ip=strdup(ip);
        LOG(S_INFO,"ip 地址为:%s",ourl->ip);
        push_ourlqueue(ourl);
    }       

    
    //event_loopexit(NULL);

}
void *urlparse(){               //采用libevent中的api进行dns解析
    Surl *url=NULL;
    Url *ourl=NULL;
    map<string,string>::const_iterator itr;
    event_base *base=event_base_new();
    evdns_base *dnsbase=evdns_base_new(base,1);
    
    url=surl_queue.front();
    surl_queue.pop();
    
    ourl=surl2url(url);         //surl->url

    itr=host_ip_map.find(ourl->domain);

    if(itr==host_ip_map.end()){ //于主机ip的map中未找到，进行dns解析
        event_base *base=event_init();
        evdns_init();
        evdns_resolve_ipv4(ourl->domain,0,callback,ourl);//调用了evdns_ bas\
e_resolve_ipv4函数，可以尝试采用evdns_getaddrinfo函数来异步解析dns
        event_dispatch();
        event_base_free(base);
    }
    else{
        ourl->ip=strdup(itr->second.c_str());//之前解析过，直接拷贝
        push_ourlqueue(ourl);//送入队列
    }

    evdns_base_free(dnsbase,0);
    event_base_free(base);
    
    return NULL;
}
