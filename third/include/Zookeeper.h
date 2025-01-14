#pragma once

#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    void start();   // 启动连接zkserver
    void create(const char *path,const char *data,int datalen,int state=0); // 根据path创建znode节点
    std::string get(const char *path);  // 根据path获取znode的值
private:
    zhandle_t *zhandle_;    // zk的客户端句柄
};