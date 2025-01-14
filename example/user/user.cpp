#include "include/MprpcApplication.h"
#include "user.pb.h"

#include <mymuduo/Logger.h>

class UserService : public user::UserServiceRpc
{
private:
    struct LoginStruct
    {
        int errcode;
        std::string errmsg;
        bool success;
        std::string token;
    };
    struct FileTransStruct
    {
        int errcode;
        std::string errmsg;
        bool success;
        int port;
        std::string ip;
    };
public:
    // 本地方法
    struct LoginStruct Login(const std::string &name, const std::string &pwd, const std::string &platform)
    {
        LOG_INFO("name: %s, pwd: %s", name.c_str(), pwd.c_str());

        // 与数据库token对比
        struct LoginStruct loginstruct;
        loginstruct.errcode = 0;
        loginstruct.errmsg = "";
        loginstruct.success = true;
        loginstruct.token = "token.....";

        return loginstruct;
    }

    struct FileTransStruct GetFileServer()
    {
        // 文件服务器的ip和port的获取 MDNS 。。。搜寻局域网内的file server
        LOG_INFO("fileTransfer!");

        // 与数据库token对比
        struct FileTransStruct fileTransStruct;
        fileTransStruct.errcode = 0;
        fileTransStruct.errmsg = "";
        fileTransStruct.success = true;
        fileTransStruct.ip = "192.168.1.35";
        fileTransStruct.port = 10086;
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
        struct LoginStruct loginstruct = Login(request->name(), request->pwd(), request->platform());
        auto code = response->mutable_result();
        code->set_errcode(loginstruct.errcode);
        code->set_errmsg(loginstruct.errmsg);
        response->set_success(loginstruct.success);
        response->set_platform(request->platform());
        response->set_token(loginstruct.token);
        // 执行回调
        done->Run();
    }

    void GetFileServer(::google::protobuf::RpcController* controller,
                       const ::user::FileTransRequest* request,
                       ::user::FileTransResponse* response,
                       ::google::protobuf::Closure* done) override
    {
        // 这里做处理  连接socket后开始通信，先返回一个ok开始传输，开启线程单独
        struct FileTransStruct fileTransStruct = GetFileServer();;
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