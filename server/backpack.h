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
    std::map<int32_t,std::shared_ptr<AbstractItem>> m_backpack;
    int32_t m_biid;
    std::map<uint32_t,std::vector<int32_t>> m_idmap;
public:
    Backpack(/* args */):m_biid(0){}
    ~Backpack();
    int32_t add_item(int32_t id,int32_t num);
    int32_t del_item(int32_t biid,int32_t num);
    int32_t change_item(int32_t src_id,int32_t tar_id);
    bool is_have(uint32_t biid);
    void set_biid(int32_t _id){m_biid = _id;}
};



}


#endif