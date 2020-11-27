#ifndef __ITEM_H__
#define __ITEM_H__

#include <cstdint>
#include <string>
#include <map>
#include <memory>

#include "iteminterface.h"
#include "singleton.h"



namespace demo{
class ItemAttr;
class AbstractItem : public ItemInterface
{
private:
    /* data */
    uint32_t m_iid;
    //uint32_t m_biid;
    int32_t m_itype;
    int32_t m_iconfid;
    int32_t m_icount;
    bool m_isavenow;
    std::string m_ibrief;
public:
    AbstractItem(bool _issave):m_iid(0),
                               m_itype(-1),
                               m_iconfid(-1),
                               m_icount(0),
                               m_isavenow(_issave)
    {

    }
    virtual ~AbstractItem(){}

    char flagbit;// (1bit is tradable,2bit is bindable,3bit is overlyable,4bit is job bindable,last 4 bits job)
    void set_itemId(uint32_t _id) override {m_iid = _id;}
    uint32_t get_itemId() override{return m_iid;}
    void set_itemType(int32_t _type) override {m_itype = _type;}
    int32_t get_itemType() override {return m_itype;}
    // void set_itemAttr(itemModuleType _type,itemAttrKey key,int32_t _value) override{}
    // int32_t get_itemAttr(itemAttrKey _key) override{return -1;}
    void set_itemBrief(std::string _brief) override {m_ibrief = _brief;}
    std::string get_itemBrief() override {return m_ibrief;}

    void set_itemCount(int32_t _count) override {m_icount = _count;}
    int32_t get_itemCount() override {return m_icount;}
    virtual void init()=0;

};


class Money : public AbstractItem
{
private:
    /* data */
public:
    Money(bool _issave):AbstractItem(_issave){}
    ~Money(){}
    void init() override;
    void init(uint32_t _id);
    void set_itemAttr(itemModuleType _type,itemAttrKey key,int32_t _value) override{}
    int32_t get_itemAttr(itemAttrKey _key) override{return -1;}
    //void set_itemBrief(std::string _brief) override;

    //void init()override{}
    
};

class Item : public AbstractItem , public std::enable_shared_from_this<Item>
{
private:
    std::map<itemModuleType,std::shared_ptr<ItemAttr>> m_attr; 
    /* data */
public:
    Item(bool _issave):AbstractItem(_issave){}
    virtual ~Item(){}
    void init() override;
    void init(uint32_t _id);

    void set_itemAttr(itemModuleType _type,itemAttrKey _key,int32_t _value) override;
    int32_t get_itemAttr(itemAttrKey _key) override;
};

// class Equipment : public Item
// {
// private:
//     /* data */
// public:
//     Equipment(bool _issave):Item(_issave){}
//     ~Equipment(){}
//     void init() override;
//     void init(uint32_t _id);
// };

// class Consume : public Item{
// private:

// public:
//     Consume(bool _issave):Item(_issave){}
//     ~Consume(){}
//     void init() override;
//     void init(uint32_t _id);

// };

class ItemAttr{
public:
    ItemAttr(std::shared_ptr<Item> _ptr):m_papaptr(_ptr){

    }
    ~ItemAttr(){}
    void set_itemAttr(itemAttrKey key,int32_t _value);
    int32_t get_itemAttr(itemAttrKey key);
private:
    std::map<itemAttrKey,int> m_attrvalue;
    std::shared_ptr<Item> m_papaptr;
};


class ItemFactory : noncopyable
{
private:
    /* data */
public:
    std::shared_ptr<AbstractItem> create(uint32_t id,int32_t num);
    void delitem(std::shared_ptr<AbstractItem> ptr);
};


#define ITEMFAC Singleton<ItemFactory>::Instance()



}
#endif