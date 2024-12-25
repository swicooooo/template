#pragma once

namespace EnMsgType
{
static std::string LOGIN_MSG  = "login";
static std::string FILE_TRANSFER = "file_transfer";
}


static const int BLOCK_SIZE = 2048;
enum OperType
{
    TYPE_DOWNLOAD = 1,
    TYPE_UPLOAD,
    TYPE_ACK,
    TYPE_AUTH,
};

enum FileType
{
    TYPE_NORMAL,    // 普通传输
    TYPE_QUICK,     // 秒传
    TYPE_EXIST,     // 文件已经存在
};

// 结构体和字符数组可以强制转换, struct存储的成员数据是连续的，但是会根据字节对齐而改变总大小
struct DataPacket
{
    int datalen;            // 弹性数组数据长度
    enum OperType type;     // 当前数据包代表的操作类型 
    char data[0];           // 存储可变TransferFile数据
};

struct UploadFile
{
    char filepath[128];     // 文件路径
    uint64_t filesize;      // 文件大小
    char md5[33];           // 32位的md5值
};

struct UploadFileAck
{
    enum FileType type;
    int blockno;
};

struct TransferFile
{
    int blockno;
    char content[BLOCK_SIZE];
};

struct TransferFileAck
{
    char flag;
};

// 全局检查错误
#define CheckErr(ret, str) \
    do \
    {  \
        if (ret < 0)    \
        { \
            printf("%s:%d error message is: %s\n", __FILE__, __LINE__, str);  \
            perror("recv");     \
            exit(-1);    \
        } \
    } while(0);
