#ifndef __ITEM_INTERFACE_H__
#define __ITEM_INTERFACE_H__

#include <cstdint>
#include <string>

#include "noncopyable.h"

enum itemAttrKey{
    HEALTH=0,
    HEALTHPERCENT=1,
    ATTACK=2,
    ATTACKPERCENT=3,
    ARMOR=4,
    MAGICRESISTANCE=5,
    MOVESPEED=6,
    MOVESPEEDPERCENT=7,
    ATTACKSPPED=8,
    MANA=9,
    MANAPERCENT=10,
};

enum itemModuleType{
    hhh,
};

namespace demo {

class ItemInterface
{
private:
    /* data */
public:
    virtual ~ItemInterface(){}
    virtual void set_itemId(uint32_t _id)=0;
    virtual uint32_t get_itemId()=0;
    virtual void set_itemType(uint32_t _type)=0;
    virtual uint32_t get_itemType()=0;
    virtual void set_itemAttr(itemModuleType _type,itemAttrKey _key,int32_t _value)=0;
    virtual int32_t get_itemAttr(itemAttrKey _key)=0;
    virtual void set_itemBrief(std::string _brief)=0;
    virtual std::string get_itemBrief()=0;
    virtual void init()=0;
    virtual void set_itemCount(int32_t _count)=0;
    virtual int32_t get_itemCount()=0;
};












} // namespace demo

#endif