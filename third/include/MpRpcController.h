#pragma once

#include <string>
#include <google/protobuf/service.h>

/// @brief 用于rpc获取调试信息在执行callMethod时的对象
class MprpcController : public google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset();
    bool Failed() const;
    void SetFailed(const std::string& reason);
    std::string ErrorText() const;
    /// 待实现
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);
private:
    bool failed_;
    std::string errStr_;
};