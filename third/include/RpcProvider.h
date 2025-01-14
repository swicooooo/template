#pragma once

#include <google/protobuf/service.h>
#include <mymuduo/TcpServer.h>
#include <unordered_map>
#include <string>

/// @brief 框架提供专门发布rpc服务的网络对象类
class RpcProvider
{
public:
    void notifyService(google::protobuf::Service *service);     // 发布rpc节点服务对象,多态
    void run();             // 启动rpc节点服务，开始提供rpc远程网络调用服务
private:
    void onConnection(const TcpConnectionPtr&);
    void onMessage(const TcpConnectionPtr&,Buffer*,Timestamp);  // 处理rpcConsumer发送来的message
    void onRpcCallMethod(const TcpConnectionPtr&, google::protobuf::Message*); // rpc执行方法后的回调
    EventLoop loop_;        // 在此种情况下，不能用指针，TcpServer需要EventLoop的实例

    // service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *service_;    // 服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> methods_;  // 映射服务名称和方法
    };
    std::unordered_map<std::string,ServiceInfo> services_;
};