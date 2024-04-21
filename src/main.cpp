#include "MprpcApplication.h"
#include "RpcProvider.h"

int main(int argc, char const *argv[])
{
    MprpcApplication::init(argc, argv);

    RpcProvider provider;
    // provider.notifyService();
    provider.run();
    return 0;
}
