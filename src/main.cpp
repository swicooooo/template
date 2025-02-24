#include <iostream>
#include "qosserver.h"

int main(int argc, char **argv)
{
    // if (argc < 3)
    // {
    //     std::cerr << "command invaild example: ./ExeNAME  IpAddress  port" << std::endl;
    //     exit(-1);
    // }

    // char *ip = argv[1];
    // uint16_t port = atoi(argv[2]);

    EventLoop loop;
    InetAddress listenAddr(/*port*/10086, /*ip*/"192.168.77.39");    

    QosServer server(&loop, listenAddr, "QosServer");
    server.start();
    loop.loop();
    return 0;
}
/**
telnet 192.168.77.39 10086
{"event": "login", "payload": {"name": "zhangsan", "pwd": "0", "platform": "myplatform"}}

// mysql
mysql -u sw -h 192.168.77.39 -p
use db_file;
SELECT * FROM file;
DELETE FROM file;
 */
