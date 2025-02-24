#pragma 

#include <functional>
#include <mymuduo/TcpServer.h>
#include <nlohmann/json.hpp>

using MsgHandler = std::function<void(const TcpConnectionPtr&, nlohmann::json&,Timestamp)>;

class QosService
{
public:
    static QosService* getInstance();
    MsgHandler getHandler(std::string event);  

    void login(const TcpConnectionPtr& conn, nlohmann::json &js, Timestamp timestamp);
    void create_bucket(const TcpConnectionPtr& conn, nlohmann::json &js, Timestamp timestamp);
    void file_server(const TcpConnectionPtr& conn, nlohmann::json &js, Timestamp timestamp);
private:
    QosService();
    std::unordered_map<std::string, MsgHandler> msgHandlers_;
};

