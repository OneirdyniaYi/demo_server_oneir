#ifndef __MYSQL_PLUS_H__
#define __MYSQL_PLUS_H__

#include <cstdint>
#include <string>

#include <mysql/mysql.h>
#include <map>
#include <string>

#include "singleton.h"


namespace demo
{
    class Mysqlplus
    {
    private:
        /* data */
        
        
        MYSQL *con;
    public:
        Mysqlplus(/* args */);
        ~Mysqlplus();

        int32_t init(const char* ip,const char* username,const char* password,const char* db);
        int32_t query(const char* queryer);

        std::map<std::string,std::map<std::string,std::string>> parser();

    };
    
    #define MYSQLPP Singleton<Mysqlplus>::Instance()






} // namespace demo



#endif