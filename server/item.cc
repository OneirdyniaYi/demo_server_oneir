

#include "item.h"


namespace demo{

void Money::init() {
    set_itemType(1);
    //set_iconfid(1);
    set_itemCount(0);
    
}

void Money::init(uint32_t _id){
    set_itemId(_id);
    std::string str;
    //to do ... read from config   
    set_itemBrief(str); 
    init();
}

void Item::init() {

    set_itemType(2);
    set_itemCount(0);

}

void Item::init(uint32_t _id){
    std::string str;
    //set_itemId();
    set_itemId(_id);
    set_itemBrief(str);
    init();
}



void Item::set_itemAttr(itemModuleType _type,itemAttrKey _key,int32_t _value){
    if(m_attr.find(_type)!=m_attr.end())
        m_attr.find(_type)->second->set_itemAttr(_key,_value);
    else{
        m_attr.insert(std::make_pair(_type,std::make_shared<ItemAttr>(shared_from_this())));
    }
}

int32_t Item::get_itemAttr(itemAttrKey _key){
    int32_t _attr = 0;
    for(auto &i:m_attr){
        _attr += i.second->get_itemAttr(_key);
    }
    return _attr;
}

// void Equipment::init() {
//     std::string str;
//     //set_itemId();
//     set_itemType(1);
//     set_itemCount(0);
//     set_itemBrief(str);
// }

// void Equipment::init(uint32_t _id) {
//     set_itemId(_id);
//     init();
// }

void ItemAttr::set_itemAttr(itemAttrKey key,int32_t _value){
    auto it = m_attrvalue.find(key);
    if(it!=m_attrvalue.end())
        it->second += _value;
    else{
        m_attrvalue.insert(std::make_pair(key,_value));
    }
}

int32_t ItemAttr::get_itemAttr(itemAttrKey key){
    auto it = m_attrvalue.find(key);
    if(it!=m_attrvalue.end()){
        return it->second;
    }else{
        printf("this item:%p dont have this:%d attr\n",static_cast<void*>(m_papaptr.get()),key);
        return 0;
    }
}

std::shared_ptr<AbstractItem> ItemFactory::create(uint32_t id,int32_t num){
    switch (id)
    {
        case 1 ... 3:{
            auto ptr = std::make_shared<Money>(true);
            ptr->init(id);
            //ptr->set_itemId(id);
            ptr->set_itemCount(num);
            return ptr;
            break;
        }
        case 4 ... 20:{
            auto ptr = std::make_shared<Item>(true);
            ptr->init(id);
            ptr->set_itemCount(num);
            return ptr;
            break;
        }
        case 21 ... 100:{
            auto ptr = std::make_shared<Item>(false);
            ptr->init(id);
            ptr->set_itemCount(num);
            return ptr;
            break;

        }
        default:
            printf("do nothing\n");
            return nullptr;
            break;
    }
}

void ItemFactory::delitem(std::shared_ptr<AbstractItem> ptr){
    ptr.reset();
}

}