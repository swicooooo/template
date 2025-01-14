#include "qosservice.h"
#include <mymuduo/Logger.h>
#include "public.h"
#include "file/filetransfer.h"
#include "include/MprpcApplication.h"
#include "user.pb.h"

QosService *QosService::getInstance()
{
    static QosService qosService;
    return &qosService;
}

MsgHandler QosService::getHandler(std::string event)
{
    if(msgHandlers_.find(event) == msgHandlers_.end()) 
    {
        return [&](const TcpConnectionPtr&, nlohmann::json&, Timestamp)
        {
            LOG_ERROR("event: %s can not find handler!", event.c_str());
        };
    }
    return msgHandlers_[event];
}

void QosService::login(const TcpConnectionPtr &conn, nlohmann::json &js, Timestamp timestamp)
{
    user::LoginRequest request;
    try {
        request.set_name(js["payload"]["name"]);
        request.set_pwd(js["payload"]["pwd"]);
        request.set_platform(js["payload"]["platform"]);
    } catch (const std::exception& e) {
        ("Error extracting data to JSON: %s", e.what());
    }

    LOG_INFO("%s", __FUNCTION__); 
    MprpcApplication::init("./test.conf");
    user::UserServiceRpc_Stub stub(new MprpcChannel());

    user::LoginResponse response;
    MprpcController controller;
    stub.Login(&controller, &request, &response, nullptr);

    if(controller.Failed())
        LOG_INFO("%s:%d controller error: %s", __FILE__, __LINE__, controller.ErrorText().c_str());

    if(!response.success()) 
        LOG_ERROR("%s:%d response error: %s %d", __FILE__, __LINE__, response.result().errmsg().c_str(), response.result().errcode());

    nlohmann::json resp = {
        {"msgid", EnMsgType::LOGIN_ACK_MSG},
        {"errcode", response.result().errcode()},
        {"errmsg", response.result().errmsg()},
        {"success", response.success()},
        {"platform", response.platform()},
        {"token", response.token()}
    };
    conn->send(resp.dump());
}

void QosService::file_server(const TcpConnectionPtr &conn, nlohmann::json &js, Timestamp timestamp)
{
    user::FileTransRequest request;
    try {
        request.set_filemd5(js["payload"]["filemd5"]);
        request.set_platform(js["payload"]["platform"]);
        request.set_token(js["payload"]["token"]);
    } catch (const std::exception& e) {
        ("Error extracting data to JSON: %s", e.what());
    }

    LOG_INFO("%s", __FUNCTION__); 

    MprpcApplication::init("./test.conf");
    user::UserServiceRpc_Stub stub(new MprpcChannel());

    user::FileTransResponse response;
    MprpcController controller;
    stub.GetFileServer(&controller, &request, &response, nullptr);

    if(controller.Failed())
        LOG_INFO("%s:%d controller error: %s", __FILE__, __LINE__, controller.ErrorText().c_str());

    if(!response.success()) 
        LOG_ERROR("%s:%d response error: %s %d", __FILE__, __LINE__, response.result().errmsg().c_str(), response.result().errcode());

    nlohmann::json resp = {
        {"msgid", EnMsgType::LOGIN_ACK_MSG},
        {"errcode", response.result().errcode()},
        {"errmsg", response.result().errmsg()},
        {"success", response.success()},
        {"platform", response.platform()},
        {"token", response.token()},
        {"ip", response.server().ip()},
        {"port", response.server().port()}
    };
}

QosService::QosService()
{
    msgHandlers_.insert({EnMsgType::LOGIN_MSG, std::bind(&QosService::login, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlers_.insert({EnMsgType::FILE_SERVER, std::bind(&QosService::file_server, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
}
