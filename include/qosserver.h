#pragma once 

#include <mymuduo/TcpServer.h>

class QosServer
{
public:
    QosServer(EventLoop* loop,const InetAddress& listenAddr,const std::string& nameArg);
    ~QosServer() = default;
    void start() { server_.start(); }

private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn,Buffer *buffer,Timestamp timestamp);

    TcpServer server_;
    EventLoop *loop_;
};