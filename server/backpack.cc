#include "backpack.h"
#include "errorcode.h"

#include <cstring>
#include <algorithm>



namespace demo {

bool Backpack::is_have(uint32_t _id){
    if(m_idmap.find(_id)==m_idmap.end()){
        return false;
    }
    return true;
}

int32_t Backpack::add_item(int32_t id,int32_t num){
    auto ptr = ITEMFAC.create(id,num);
    auto it = m_idmap.find(ptr->get_itemId());
    if(it!=m_idmap.end()){
        if((ptr->flagbit&0x20)!=0){
            m_backpack[*(it->second.begin())]->set_itemCount(ptr->get_itemCount());
            ptr.reset();
        }else{
            m_backpack.insert(std::make_pair(m_biid,ptr));
            it->second.push_back(m_biid++);
            //m_idmap.insert(std::make_pair(ptr->get_itemId(),));
        }
    }else{
        m_backpack.insert(std::make_pair(m_biid,ptr));
        m_idmap.insert(std::make_pair(ptr->get_itemId(),std::vector<int32_t>(m_biid++)));
    }
    printf("add item id:%u,breif:%s success!!\n",ptr->get_itemCount(),ptr->get_itemBrief().c_str());
    return Success;
}

int32_t Backpack::del_item(int32_t biid,int32_t num){
    auto it = m_idmap.find(m_backpack[biid]->get_itemId());
    if(it!=m_idmap.end()){

        if(m_backpack[biid]->get_itemCount()<num){
            printf("dont have %d items\n",num);
            return Failed;
        }else if(m_backpack[biid]->get_itemCount()==num){
            m_backpack.erase(biid);
            auto itt = find(it->second.begin(),it->second.end(),biid);
            if(itt!=it->second.end()){ 
                it->second.erase(itt);
                ITEMFAC.delitem(m_backpack[biid]);
            }
            else{
                printf("del item failed!!\n");
                return Failed;
            }
        }else{
            m_backpack[biid]->set_itemCount(-num);
        }

    }else{
        printf("dont have this item:%d\n",biid);
        return Failed;
    }
    



    return Success;
}

int32_t Backpack::change_item(int32_t src_id,int32_t tar_id){
    if(m_backpack[src_id]->get_itemId()==m_backpack[tar_id]->get_itemId()&&(m_backpack[src_id]->flagbit&0x20)!=0){
        m_backpack[tar_id]->set_itemCount(m_backpack[src_id]->get_itemCount());
        del_item(src_id,m_backpack[src_id]->get_itemCount());
    }else{
        printf("cant change item\n");
        return Failed;
    }
    return Success;
}

}