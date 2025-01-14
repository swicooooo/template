#pragma once

#include <google/protobuf/service.h>

/// @brief protobuf提供了stub类作为客户端，里面的每个方法都是调用的该类的方法
/// 只是methodDesc不同，在服务端具体调用的方法也就不同
class MprpcChannel : public google::protobuf::RpcChannel
{
public:
    void CallMethod(const google::protobuf::MethodDescriptor *method,
                    google::protobuf::RpcController *controller,
                    const google::protobuf::Message *request,
                    google::protobuf::Message *response,
                    google::protobuf::Closure *done);
};
