#ifndef SOCKET_H
#define SOCKET_H
#include"head.h"
#include"LOG.h"
int tcp_connect(const char *host,const char *serv){
    int sockfd,n;
    struct addrinfo *res,hints,*ressave;
    
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;

    if(n=getaddrinfo(host,serv,&hints,&res)!=0)
        LOG(S_ERROR,"tcp connect error for %s,%s:%s",host,serv,gai_strerror(n));
    ressave=res;
    
    do{
        sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(sockfd<0)
            continue;
        printf("now connect %s",res->ai_canonname);
        if(connect(sockfd,res->ai_addr,res->ai_addrlen)==0)
            break;
        close(sockfd);
    }while((res=res->ai_next)!=NULL);

    if(res == NULL)
        LOG(S_ERROR,"tcp connect error for %s,%s",host,serv);

    freeaddrinfo(ressave);

    return(sockfd);
}



#endif


