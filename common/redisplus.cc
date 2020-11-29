#include "redisplus.h"

namespace demo
{
    Redisplus::Redisplus(){
        
    }




    Redisplus::~Redisplus(){

    }

    void Redisplus::init(std::string ip,uint16_t port,int32_t timeout,int32_t db_th){
        sw::redis::ConnectionOptions connection_options;
        connection_options.host = ip;
        connection_options.port = port;
        connection_options.socket_timeout = std::chrono::milliseconds(timeout);
        connection_options.db = db_th;
        redisptr = std::make_shared<sw::redis::Redis>(connection_options);
    }

} // namespace demo
