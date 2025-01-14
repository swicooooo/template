#pragma once

#include <unordered_map>
#include <string>

/// @brief mprpc框架的配置读取类
class MprpcConfig
{
public:
    static MprpcConfig& instance(); 
    void loadConfigFile(const char *file);      // 读取配置文件
    std::string load(const std::string &key);   // 查询配置项信息
private:
    std::unordered_map<std::string,std::string> configs_;
};
