#include "db/mysql.h"

#include <mymuduo/Logger.h>

MySQL::MySQL(std::string dbname, std::string server, std::string user, std::string password)
    :   dbname_(dbname),
    server_(server),
    user_(user),
    password_(password)
{
    conn_ = mysql_init(nullptr);    // nor not mysql_init(conn_)
}

MySQL::~MySQL()
{
    if(conn_ != nullptr)
    {
        mysql_close(conn_);
    }
}

bool MySQL::connect()
{
    MYSQL *p = mysql_real_connect(conn_,server_.c_str(),user_.c_str(),password_.c_str(),dbname_.c_str(),3306,nullptr,0);
    if(p == nullptr)
    {
        LOG_ERROR("%s:%d: mysql connect error: %s",__FILE__,__LINE__, mysql_error(conn_));
    }
    return p;
}

bool MySQL::update(std::string sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_ERROR("%s:%d:%s update error: %s",__FILE__,__LINE__,sql.c_str(), mysql_error(conn_));
        return false;
    }
    return true;
}

MYSQL_RES *MySQL::query(std::string sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_ERROR("%s:%d:%s query error: %s",__FILE__,__LINE__,sql.c_str(), mysql_error(conn_));
        return nullptr;
    }
    return mysql_use_result(conn_);
}

MYSQL* MySQL::getConn()
{
    return conn_;
}
