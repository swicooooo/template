#pragma once

#include "MprpcChannel.h"
#include "RpcProvider.h"
#include "MpRpcController.h"
#include <string>

/// @brief mprpc框架的基础类，负责初始化框架
class MprpcApplication
{
public:
    static void init(std::string fileName);    // 初始化
    static MprpcApplication& instance();         // 获取单例对象 
private:
    MprpcApplication() = default;
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(const MprpcApplication&&) = delete;
};
