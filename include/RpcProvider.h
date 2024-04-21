#pragma once

#include <google/protobuf/service.h>

/// @brief 框架提供专门发布rpc服务的网络对象类
class RpcProvider
{
public:
    void notifyService(google::protobuf::Service *service);   // 发布rpc节点服务对象,多态
    void run();  // 启动rpc节点服务，开始提供rpc远程网络调用服务
};