#include "file/filetransfer.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <thread>
#include <strings.h>

bool isFileServerRunning = true;

void SigIntHandle(int)
{
    isFileServerRunning = false;
    exit(-1);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, SigIntHandle); 

    int ret;
    int sockfd =socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    std::string ip = "192.168.1.35";
    uint16_t port = 8000;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    CheckErr(ret, "bind err"); 
    ret = listen(sockfd, 1024);
    CheckErr(ret, "listen err");

    struct sockaddr_in clientaddr;
    socklen_t clen = sizeof(clientaddr);
    while (isFileServerRunning)
    {
        int clientfd = accept(sockfd, (sockaddr*)&clientaddr, &clen);
        if(clientfd < 0)
        {
            CheckErr(clientfd, "accept err");
        }
        std::thread([&](){
            while (1)
            {
                char buffer[1024] = {0};
                bzero(buffer,1024);
                filetransfer basetrans;
                basetrans.recvPacket(clientfd, buffer, 1024);

                struct DataPacket *dp = (struct DataPacket*)buffer;
                filetransferReceiver s;
                switch (dp->type)
                {
                case TYPE_UPLOAD:
                    s.processUpload(clientfd, dp);
                    break;                
                default:
                    break;
                }
            }
        }).detach();
    }
    close(sockfd);
    printf("fileserver quit!\n");
    return 0;
}
