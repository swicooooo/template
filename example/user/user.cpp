#include "include/MprpcApplication.h"
#include "user.pb.h"

#include <mymuduo/Logger.h>
#include <nlohmann/json.hpp>
#include <fstream>

const std::string BUCKET_PATH = "buckets.json";
const std::string EXPECTED_TOKEN = "123456";
const std::string DB_TOKEN = "123456"; 

class UserService : public user::UserServiceRpc
{
private:
    struct BaseStructRet
    {
        int errcode;
        std::string errmsg;
        bool success;
        std::string token;
    };
    struct LoginStructRet : public BaseStructRet
    {
        std::string buckets;
    };
    struct CreateBucketStructRet : public BaseStructRet
    {};
    struct FileTransStructRet : public BaseStructRet
    {
        int port;
        std::string ip;
    };
public:
    // 本地方法
    struct LoginStructRet Login(const std::string &name, const std::string &pwd, const std::string &platform)
    {
        LOG_INFO("name: %s, pwd: %s", name.c_str(), pwd.c_str());

        struct LoginStructRet loginstruct;
        loginstruct.errcode = 0;
        loginstruct.errmsg = "";
        loginstruct.token = "123456";// 查找数据库token
        
        if (loginstruct.token != DB_TOKEN) 
        {
            loginstruct.errcode = 1;
            loginstruct.errmsg = "Token 验证失败";
            return loginstruct;
        }

        nlohmann::json j;
        std::ifstream file(BUCKET_PATH);
        if (!file) 
        {
            printf("文件不存在，返回空 buckets");
            j = {{"buckets", nlohmann::json::array()}}; 
        } else 
        {
            file >> j;
            file.close();
        }

        if (j.contains("buckets")) 
        {
            loginstruct.buckets = j.dump(); 
            loginstruct.success = true;
        } else 
        {
            loginstruct.errmsg = "buckets 数据不存在";
            loginstruct.success = false;
        }

        return loginstruct;
    }

    struct CreateBucketStructRet CreateBucketServer(const std::string &bucketname, 
                                                    const std::string &location, 
                                                    const std::string &lastmodified,
                                                    const std::string &token)
    {
        // 创建捅（根目录）
        struct CreateBucketStructRet createBucketStruct;
        createBucketStruct.errcode = 0;
        createBucketStruct.errmsg = "";
        createBucketStruct.success = true;
        
        nlohmann::json j;
        std::ifstream file(BUCKET_PATH);
        if (!file) 
        {
            printf("文件不存在，创建默认 JSON");
            j = {{"token", EXPECTED_TOKEN}, {"buckets", nlohmann::json::array()}};
        } else 
        {
            file >> j;
            file.close();
        }

        nlohmann::json new_bucket = {
            {"location", location},
            {"create_date", lastmodified},
            {"bucketname", bucketname}
        };
        j["buckets"].push_back(new_bucket);

        std::ofstream outFile(BUCKET_PATH);
        if (!outFile) 
        {
            printf("无法打开文件进行写入");
            createBucketStruct.errcode = 1;
            createBucketStruct.errmsg = "保存文件失败";
        }else
        {
            outFile << j.dump(4); 
            outFile.close();
        }

        printf("_++++============ %s %s %s\n", location.c_str(), lastmodified.c_str(), bucketname.c_str()) ;
        printf("_++++============ %s\n", new_bucket.dump().c_str()) ;
        return createBucketStruct;
    }

    struct FileTransStructRet GetFileServer()
    {
        // 文件服务器的ip和port的获取 MDNS 。。。搜寻局域网内的file server
        LOG_INFO("fileTransfer!");

        // 与数据库token对比
        struct FileTransStructRet fileTransStruct;
        fileTransStruct.errcode = 0;
        fileTransStruct.errmsg = "";
        fileTransStruct.success = true;
        fileTransStruct.ip = "192.168.77.39";
        fileTransStruct.port = 8000;
        return fileTransStruct;
    }

public:
    // 重写UserServiceRpc中的Login方法
    void Login(::google::protobuf::RpcController *controller,
               const ::user::LoginRequest *request,
               ::user::LoginResponse *response,
               ::google::protobuf::Closure *done) override
    {
        // 处理业务并返回response
        struct LoginStructRet loginstruct = Login(request->name(), request->pwd(), request->platform());
        auto code = response->mutable_result();
        code->set_errcode(loginstruct.errcode);
        code->set_errmsg(loginstruct.errmsg);
        response->set_success(loginstruct.success);
        response->set_platform(request->platform());
        response->set_token(loginstruct.token);
        response->set_buckets(loginstruct.buckets);
        // 执行回调
        done->Run();
    }

    void CreateBucket(::google::protobuf::RpcController *controller,
        const ::user::CreateBucketRequest *request,
        ::user::CreateBucketResponse *response,
        ::google::protobuf::Closure *done) override
    {
        // 处理业务并返回response
        printf("_++++============ CreateBucket %s %s %s\n", request->bucketname().c_str(), 
        request->location().c_str(), request->lastmodified().c_str()) ;
        struct CreateBucketStructRet createBucketStruct = CreateBucketServer(request->bucketname(), request->location(), request->lastmodified(), request->token());
        auto code = response->mutable_result();
        code->set_errcode(createBucketStruct.errcode);
        code->set_errmsg(createBucketStruct.errmsg);
        response->set_success(createBucketStruct.success);
        response->set_platform(request->platform());
        response->set_token(request->token());
        
        done->Run();
    }

    void GetFileServer(::google::protobuf::RpcController* controller,
                       const ::user::FileTransRequest* request,
                       ::user::FileTransResponse* response,
                       ::google::protobuf::Closure* done) override
    {
        // 这里做处理  连接socket后开始通信，先返回一个ok开始传输，开启线程单独
        struct FileTransStructRet fileTransStruct = GetFileServer();;
        auto code = response->mutable_result();
        code->set_errcode(fileTransStruct.errcode);
        code->set_errmsg(fileTransStruct.errmsg);
        response->set_success(fileTransStruct.success);
        response->set_platform(request->platform());
        response->set_token(request->token());

        auto server = response->mutable_server();
        server->set_ip(fileTransStruct.ip);
        server->set_port(fileTransStruct.port);
        done->Run();
    }
};

/// protobuf rpc会生成两个主要的类 Service(服务端)、Service_stub(客户端)
/// Service：内部有virtual void CallMethod()，通过switch索引到具体的方法，
///          通过重写该方法形成多态，在解析完客户端发送的数据后可直接调用callMethod完成业务方法
/// Service_stub：rpcChannel内部有virtual void CallMethod()，需继承rpcChannel重写该方法
///               stub下的rpc方法都统一走rpcChannel，做rpc方法的数据序列化和网络发送
int main(int argc, char *argv[])
{
    MprpcApplication::init("./test.conf");
    
    RpcProvider provider;
    provider.notifyService(new UserService());
    provider.run();
    return 0;
}