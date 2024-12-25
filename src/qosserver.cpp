#include "qosserver.h"
#include "qosservice.h"

#include <nlohmann/json.hpp>

QosServer::QosServer(EventLoop *loop, const InetAddress &listenAddr, const std::string &nameArg)
    : server_(loop, listenAddr, nameArg)
    , loop_(loop)
{
    server_.setConnectionCallback(std::bind(&QosServer::onConnection, this,std::placeholders::_1));
    server_.setMessageCallback(std::bind(&QosServer::onMessage, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server_.setThreadNum(4);    
}

int total = 0;
void QosServer::onConnection(const TcpConnectionPtr &conn)
{
    if(!conn->connected()) 
    {
        conn->shutdown();
    }
}

void QosServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp timestamp)
{
    std::string data = buffer->retrieveAllAsString();
    if (nlohmann::json::accept(data))
    {
        nlohmann::json json = nlohmann::json::parse(data);

        auto handler = QosService::getInstance()->getHandler(json["event"].get<std::string>());
        handler(conn, json, timestamp);
    }else
        std::cerr << "Invalid JSON data: " << data << std::endl;
}
