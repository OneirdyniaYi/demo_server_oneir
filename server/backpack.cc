#include "backpack.h"
#include "errorcode.h"

#include <cstring>
#include <algorithm>



namespace demo {


uint32_t Backpack::add_item(std::map<std::string,std::string> resmp,uint32_t _typeid,int32_t num,int32_t& out){
    auto it = m_idmap.find(_typeid);
    if(it!=m_idmap.end()){
        printf("shi fou ke yi die jia %s\n",resmp["overlay"].c_str());
        if(resmp["overlay"]=="1"){
            int32_t hasnum = m_backpack[*(it->second.begin())]->get_itemCount();
            m_backpack[*(it->second.begin())]->set_itemCount(hasnum+num);
            printf("add item id:%u,breif:%s success!!\n",m_backpack[*(it->second.begin())]->get_itemId(),m_backpack[*(it->second.begin())]->get_itemBrief().c_str());
            out = hasnum;
            return m_backpack[*(it->second.begin())]->get_itemId();
        }else{
            auto ptr = ITEMFAC.create(resmp,_typeid,num);
            
            //ptr->set_itemAttr(hhh,HEALTH,stoi(resmp["hp"]));
            m_backpack.insert(std::make_pair(ptr->get_itemId(),ptr));
            it->second.push_back(ptr->get_itemId());
            printf("add item id:%u,breif:%s success!!\n",ptr->get_itemId(),ptr->get_itemBrief().c_str());
            out = ptr->get_itemCount();
            //m_idmap.insert(std::make_pair(ptr->get_itemId(),));
            return ptr->get_itemId();
        }
    }else{
        printf("meiyou zhao dao m_idmap zhong de shu ju\n");
        auto ptr = ITEMFAC.create(resmp,_typeid,num);
        m_backpack.insert(std::make_pair(ptr->get_itemId(),ptr));
        std::vector<uint32_t> vec;
        vec.push_back(ptr->get_itemId());
        m_idmap.insert(std::make_pair(ptr->get_itemType(),vec));
        printf("add item id:%u,breif:%s success!!\n",ptr->get_itemId(),ptr->get_itemBrief().c_str());
        out = ptr->get_itemCount();
        return ptr->get_itemId();
    }
    //}else return Failed;
    
    
}

int32_t Backpack::del_item(int32_t uid,int32_t num){
    if(num==0) return Failed;
    if(m_backpack.find(uid)!=m_backpack.end()){
        auto it = m_idmap.find(m_backpack[uid]->get_itemType());
        if(it!=m_idmap.end()){

            if(m_backpack[uid]->get_itemCount()<num){
                printf("dont have %d items\n",num);
                return Failed;
            }else if(m_backpack[uid]->get_itemCount()==num){
                
                auto itt = find(it->second.begin(),it->second.end(),uid);
                if(itt!=it->second.end()){ 
                    it->second.erase(itt);
                    ITEMFAC.delitem(m_backpack[uid]);
                    
                }
                else{
                    printf("del item failed!!\n");
                    return Failed;
                }
                m_backpack.erase(uid);
            }else{
                m_backpack[uid]->set_itemCount(m_backpack[uid]->get_itemCount()-num);
                printf("jian shao le %d ge dao ju\n",num);
                return m_backpack[uid]->get_itemCount();
            }

        }else{
            printf("dont have this item:%d\n",uid);
            return Failed;
        }
    }else{
        printf("biid failed :%d\n",uid);
        return Failed;
    }



    return Success;
}

int32_t Backpack::merge_item(int32_t src_id,int32_t tar_id){
    if(m_backpack[src_id]->get_itemId()==m_backpack[tar_id]->get_itemId()&&(m_backpack[src_id]->flagbit&0x20)!=0){
        m_backpack[tar_id]->set_itemCount(m_backpack[tar_id]->get_itemCount()+m_backpack[src_id]->get_itemCount());
        del_item(src_id,m_backpack[src_id]->get_itemCount());
    }else{
        printf("cant merge item\n");
        return Failed;
    }
    return Success;
}



std::shared_ptr<AbstractItem> Backpack::get_item(uint32_t id){
    return m_backpack[id];
}

int32_t Backpack::set_item(std::shared_ptr<AbstractItem> ptr){
    m_backpack.insert(std::make_pair(ptr->get_itemId(),ptr));
    return Success;
}

int32_t Backpack::del_bat_item(uint32_t id){
    if(m_backpack.find(id)!=m_backpack.end()){
        m_backpack.erase(id);
        return Success;
    }else return Failed;
}

}