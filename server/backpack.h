#ifndef __BACKPACK_H__
#define __BACKPACK_H__

#include <map>
#include <cstdint>
#include <memory>
#include <vector>

#include "item.h"
#include "redisplus.h"

namespace demo{

class Backpack
{
private:
    /* data */
    std::map<uint32_t,std::shared_ptr<ItemInterface>> m_backpack;
    std::map<uint32_t,std::vector<uint32_t>> m_idmap;
public:
    Backpack(/* args */){}
    ~Backpack(){}
    uint32_t add_item(std::map<std::string,std::string> resmp,uint32_t _typeid,int32_t num,int32_t& out);
    int32_t del_item(int32_t uid,int32_t num);
    int32_t merge_item(int32_t src_id,int32_t tar_id);
    
    std::shared_ptr<ItemInterface> get_item(uint32_t id);
    int32_t set_item(std::shared_ptr<ItemInterface> ptr);
    int32_t del_bat_item(uint32_t id);
};



}


#endif