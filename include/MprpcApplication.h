#pragma once

/// @brief mprpc框架的基础类，负责初始化框架
class MprpcApplication
{
public:
    static void init(int argc, char const *argv[]);    // 初始化
    static MprpcApplication& instance();               // 获取单例对象 
private:
    MprpcApplication();
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(const MprpcApplication&&) = delete;
};
