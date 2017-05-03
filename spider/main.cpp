#include"LOG.h"
#include<iostream>
#include<stdlib.h>
#include"socket.h"
using namespace std;
int main(int argc,char **argv){
    int sockfd;
    queue <int> ii;
    sockfd=tcp_connect(argv[1],argv[2]);
    cout<<sockfd<<endl;
}
