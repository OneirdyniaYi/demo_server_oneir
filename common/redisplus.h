#ifndef __REDIS_PLUS_H__
#define __REDIS_PLUS_H__

#include "singleton.h"
#include <string>
#include <memory>
#include <sw/redis++/redis++.h>

namespace demo
{
class Redisplus{
public:
    Redisplus();
    ~Redisplus();
    
    std::shared_ptr<sw::redis::Redis> redisptr;
    void init(std::string ip,uint16_t port,int32_t timeout,int32_t db_th);
private:
    
};


#define REDIS Singleton<Redisplus>::Instance()

} // namespace demo



#endif