#include "MprpcApplication.h"

void MprpcApplication::init(int argc, char const *argv[]) 
{
    
}

MprpcApplication &MprpcApplication::instance() 
{
    static MprpcApplication app;
    return app;
}

MprpcApplication::MprpcApplication()
{
}
