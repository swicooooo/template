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
    LOG_INFO("new connection to login!"); 

    MprpcApplication::init("./test.conf");

    user::UserServiceRpc_Stub stub(new MprpcChannel());
    user::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("0");

    user::LoginResponse response;
    MprpcController controller;
    stub.Login(&controller, &request, &response, nullptr);

    // 在执行stub Login时出错
    if(controller.Failed())
    {
        LOG_INFO("%s:%d controller error: %s", __FILE__, __LINE__, controller.ErrorText().c_str());
    }

    if(response.result().errcode() == 0) 
    {
        LOG_INFO("%s:%d response success: %d", __FILE__, __LINE__, response.success());
    }
    else
    {
        LOG_ERROR("%s:%d response error: %s", __FILE__, __LINE__, response.result().errmsg().c_str());
    }   
}

void QosService::file_transfer(const TcpConnectionPtr &conn, nlohmann::json &js, Timestamp timestamp)
{
    if (js.contains("payload") && js["payload"].contains("data"))
    {
        std::string decodestr = base64_decode(js["payload"]["data"].get<std::string>());
        int datalen = js["payload"]["datalen"].get<int>();

        struct DataPacket *result = (struct DataPacket*)new char[sizeof(struct DataPacket)+datalen];
        std::memcpy(result, decodestr.data(), sizeof(struct DataPacket) + datalen);
        LOG_ERROR("decode string is: %s", result->data);
        delete []result;
    }
    
}

QosService::QosService()
{
    msgHandlers_.insert({EnMsgType::LOGIN_MSG, std::bind(&QosService::login, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlers_.insert({EnMsgType::FILE_TRANSFER, std::bind(&QosService::file_transfer, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
}
