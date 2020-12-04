#include <cstdint>
#include <cmath>
#include <random>
#include <chrono>
#include <string>
#include <cstring>

#include "player.h"
#include "server.h"



namespace demo{

Player::Player(int32_t playerID):m_id(playerID),m_clitime(0){
	m_sertime = get_time();
    m_bag = new Backpack();
    m_battle_bag = new Backpack();
}

Player::~Player(){
    delete m_bag;
    delete m_battle_bag;
}

int32_t Player::handle_test(MsgHead &stHead, const char *body, const int32_t len){
    ReqTest req;
    req.ParseFromArray(body, len);

    printf("server receive msg :req.a:%d\n",req.a());

    RspTest rsp;
    rsp.set_b(req.a());
    send_msg(DEMOID::RSP_TEST, rsp);
    return Success;
}

int32_t Player::handle_test2(MsgHead &stHead, const char *body, const int32_t len){
    ReqTest2 req;
    req.ParseFromArray(body, len);
    printf("server receive msg :req.name:%s,req.dir:%d,req.x:%d,req.y%d,req.z%d,req.otherinfo%s\n",req.name().c_str(),req.dir(),req.x(),req.y(),req.z(),req.otherinfo().c_str());

    // RspTest2 rsp;
    // rsp.set_bb(1000);
    // rsp.set_msg2(req.msg());
    broadcast(DEMOID::RSP_TEST2,req);
    
    return Success;
}
int32_t Player::move_test(MsgHead &head,const char* body,const int32_t len){
    MoveReq req;
    MoveRsp rsp;
    req.ParseFromArray(body,len);
    //if();
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    uint64_t serdlt = now - m_sertime;
    printf("m_sertime = %lu,now = %lu\n",m_sertime,now);
    m_sertime = now;
    if(serdlt>119){
        m_dir[0] = m_tar_dir[0];
        m_dir[1] = m_tar_dir[1];
        m_dir[2] = m_tar_dir[2];
        m_run = 0;
        //m_sertime += 100;
        printf("yi jing guo le 100 hao miao\n");
    }
    printf("zhe shi m_dirx:%d,zhe shi req.src_dirx%d\n",m_dir[0],req.src_dirx());
    printf("zhe shi m_diry:%d,zhe shi req.src_diry%d\n",m_dir[1],req.src_diry());
    printf("zhe shi m_dirz:%d,zhe shi req.src_dirz%d\n",m_dir[2],req.src_dirz());
    ////todo    time 0-999999
    int64_t dlt_time = req.time() - m_clitime;
    printf("zhe shi last m_clitime:%ld,zhe shi req.time%ld\n",m_clitime,req.time());
    int32_t pri_x = m_dir[0] + dlt_time * 0.2 * m_run * m_face[0];
    printf("zhe shi pri_x:%d,zhe shi m_face[0]%d,zhe shi m_run:%d\n",pri_x,m_face[0],m_run);
    // int32_t pri_y = m_dir[1] + dlt_time * 0.2 * m_run * m_face[1];
    // int32_t pri_z = m_dir[2] + dlt_time * 0.2 * m_run * m_face[2];
    // int32_t dir = (req.src_dirx() - pri_x) * (req.src_dirx() - pri_x) + (req.src_diry() - pri_y) * (req.src_diry() - pri_y) + (req.src_dirz() - pri_z) * (req.src_dirz() - pri_z);
    // if(dir>1000){
    //     //todo lahui wanjia
    //     printf("wu cha guo da yi tiao hui\n");
    //     req.set_src_dirx(m_dir[0]);
    //     req.set_src_diry(m_dir[1]);
    //     req.set_src_dirz(m_dir[2]);
    //     req.set_time(m_clitime);
    //     req.set_speed(m_run);
    //     req.set_facex(m_face[0]);
    //     req.set_facey(m_face[1]);
    //     req.set_facez(m_face[2]);
    //     req.set_trg_dirx(m_tar_dir[0]);
    //     req.set_trg_diry(m_tar_dir[1]);
    //     req.set_trg_dirz(m_tar_dir[2]);
    //     send_msg(DEMOID::MOVEREQ,req);
    //     return Success;
    // }
    m_clitime = req.time();
    int32_t num = serdlt - dlt_time*20;
    printf("server client interval is num:%d\n",num);
    // if(num>0){
    //     m_dir[0] = pri_x + req.facex()*req.speed()*num/20;
    //     m_dir[1] = pri_y + req.facey()*req.speed()*num/20;
    //     m_dir[2] = pri_z + req.facez()*req.speed()*num/20;
    // }else{
    // m_dir[0] = req.src_dirx();
    // m_dir[1] = req.src_diry();
    // m_dir[2] = req.src_dirz();
    // m_tar_dir[0] = req.trg_dirx();
    // m_tar_dir[1] = req.trg_diry();
    // m_tar_dir[2] = req.trg_dirz();
    // }
    m_face[0] = req.facex();
    m_face[1] = req.facey();
    m_face[2] = req.facez();
    m_run = req.speed();
    rsp.set_name(req.name());
    rsp.set_facex(m_face[0]);
    rsp.set_facey(m_face[1]);
    rsp.set_facez(m_face[2]);
    rsp.set_dirx(m_dir[0]);
    rsp.set_diry(m_dir[1]);
    rsp.set_dirz(m_dir[2]);
    rsp.set_time(req.time());
    rsp.set_step(m_run);
    DEMOSERVER.send_except_msg(get_id(),DEMOID::MOVERSP,rsp);

    return Success;
}

int32_t Player::regis_test(MsgHead &head,const char* body,const int32_t len){
    printf("this is register_test\n");
    std::random_device rd;
    std::mt19937 e(rd());
    std::uniform_int_distribution<unsigned> u(0, 99);
    m_face[0] = (int32_t)(u(e)/10);
    m_face[1] = 0;
    m_face[2] = (int32_t)(u(e)/10);
    m_dir[0] = u(e)+(int32_t)(u(e)/10);
    m_dir[1] = 0;
    m_dir[2] = u(e)+(int32_t)(u(e)/10);
    printf("chu shi x,z:%d,%d\n",m_dir[0],m_dir[2]);
    m_tar_dir[0] = m_dir[0];
    m_tar_dir[1] = m_dir[1];
    m_tar_dir[2] = m_dir[2]; 
    m_sertime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    m_run = 0;
    MoveRsp rsp;
    rsp.set_facex(m_face[0]);
    rsp.set_facey(m_face[1]);
    rsp.set_facez(m_face[2]);
    rsp.set_dirx(m_dir[0]);
    rsp.set_diry(m_dir[1]);
    rsp.set_dirz(m_dir[2]);
    return Success;

}

int32_t Player::regis_test1(MsgHead &head,const char* body,const int32_t len){
    LoginReq req;
    LoginRsp rsp;
    req.ParseFromArray(body,len);
    MYSQLPP.query(("select * from playerinfo where username = '"+req.username()+"';").c_str());
    std::map<std::string,std::map<std::string,std::string>> resmp = MYSQLPP.parser();
    if(static_cast<int32_t>(resmp.size())==0){
        //success
        std::string str;
        str += "INSERT INTO playerinfo ";
        MYSQLPP.query(( str + "(username,password, playerid) "+"VALUES "+
        "('"+req.username()+"','"+req.password()+"','"+std::to_string(get_id())+"');").c_str());
        rsp.set_issuc(1);
        rsp.set_playerid(get_id());
        send_msg(DEMOID::REGISTRSP,rsp);
        return Success;
    }else{
        //you le
        rsp.set_issuc(0);
        send_msg(DEMOID::REGISTRSP,rsp);
        return Failed;
    }
    
}
int32_t Player::login_test(MsgHead &head,const char* body,const int32_t len){
    printf("test login in !!!!!\n");
    LoginReq req;
    LoginRsp rsp;
    
    req.ParseFromArray(body,len);
    MYSQLPP.query(("select * from playerinfo where username = '"+req.username()+"';").c_str());
    std::map<std::string,std::map<std::string,std::string>> resmp = MYSQLPP.parser();
    if(static_cast<int32_t>(resmp.size())!=0){
        //success
        Playerit initrsp;
        MoveRsp *mov;
        std::string strname = req.username();
        std::string playid = resmp[req.username()]["playerid"];
        std::string str = resmp[req.username()]["playerid"] +"basedata";
        if(resmp[req.username()]["password"] == req.password()){
            rsp.set_issuc(1);
            int32_t id = std::stoi(resmp[req.username()]["playerid"]);
            rsp.set_playerid(id);
            send_msg(DEMOID::LOGINRSP,rsp);
            
            if(REDIS.redisptr->llen(str)==0){
                printf("xxxxxxxxx\n");
                int ret = MYSQLPP.query(("select * from playerbasedata where playerid = '"+std::to_string(id)+"';").c_str());
                if(ret==Success){
                    resmp.clear();
                    resmp = MYSQLPP.parser();
                    printf("aaaaaaaaaaaaaaaaaaaaa size is %d\n",(int32_t)(resmp.size()));
                    if((int32_t)(resmp.size())!=0){
                        for(auto it = resmp.begin();it!=resmp.end();it++){
                            printf("bbbbbbbbbbbbbbb size is %d\n",(int32_t)(it->second.size()));
                            // for(auto itt = it->second.begin();itt!=it->second.end();itt++){
                            //     REDIS.redisptr->rpush(str,itt->second);
                            //     printf("typeid: %s,attr is: %s,value is: %s\n",it->first.c_str(),itt->first.c_str(),itt->second.c_str());

                            // }
                            REDIS.redisptr->rpush(str,it->second["posx"]);
                            REDIS.redisptr->rpush(str,it->second["posy"]);
                            REDIS.redisptr->rpush(str,it->second["posz"]);
                            REDIS.redisptr->rpush(str,it->second["hp"]);
                            REDIS.redisptr->rpush(str,it->second["atk"]);

                        }
                    
                    }else{
                        std::random_device rd;
                        std::mt19937 e(rd());
                        std::uniform_int_distribution<unsigned> u(0, 99);
                        int32_t dirx = u(e)+(int32_t)(u(e)/10);
                        int32_t diry = 0;
                        int32_t dirz = u(e)+(int32_t)(u(e)/10);
                        int32_t hp = 100;
                        int32_t atk = 10;
                        std::string strs;
                        strs += "INSERT INTO playerbasedata ";
                        MYSQLPP.query(( strs + "(playerid,posx, posy,posz,hp,atk) "+"VALUES "+
                        "('"+std::to_string(id)+"','"+std::to_string(dirx)+"','"+std::to_string(diry)+"','"+std::to_string(dirz)+"','"+std::to_string(hp)+"','"+std::to_string(atk)+"');").c_str());
                        printf("wan le wan le  mysql zhong yu lian bu shang le!!!\n");
                        mov = initrsp.mutable_playerpos();
                        
                        mov->set_dirx(dirx);
                        mov->set_diry(diry);
                        mov->set_dirz(dirz);
                        initrsp.set_playerid(id);
                        initrsp.set_hp(hp);
                        initrsp.set_atk(atk);
                        broadcast(DEMOID::INITPOS,initrsp);
                        printf("wo zheng shi ri le gou \n");
                        return Success;
                    }

                }

            }else{
                resmp[playid].insert(std::make_pair("posx",*(REDIS.redisptr->lindex(str,0))));
                resmp[playid].insert(std::make_pair("posy",*(REDIS.redisptr->lindex(str,1))));
                resmp[playid].insert(std::make_pair("posz",*(REDIS.redisptr->lindex(str,2))));
                resmp[playid].insert(std::make_pair("hp",*(REDIS.redisptr->lindex(str,3))));
                resmp[playid].insert(std::make_pair("atk",*(REDIS.redisptr->lindex(str,4))));
            }
            mov = initrsp.mutable_playerpos();
            mov->set_dirx(std::stoi(resmp[playid]["posx"]));
            mov->set_diry(std::stoi(resmp[playid]["posy"]));
            mov->set_dirz(std::stoi(resmp[playid]["posz"]));
            initrsp.set_playerid(id);
            
            initrsp.set_hp(std::stoi(resmp[playid]["hp"]));
            initrsp.set_atk(std::stoi(resmp[playid]["atk"]));
            broadcast(DEMOID::INITPOS,initrsp);
            return Success;
        }else{
            rsp.set_errorcode(0);
            rsp.set_issuc(0);

            send_msg(DEMOID::LOGINRSP,rsp);
            printf("wocao wo zheng de zheng de ri le gou\n");
            return Failed;
        }
        
    }else{
        //mei you
        rsp.set_issuc(0);
        rsp.set_errorcode(1);
        send_msg(DEMOID::LOGINRSP,rsp);
        return Failed;
    }

}

int32_t Player::item_test(MsgHead &head,const char* body,const int32_t len){
    printf("test item operation\n");
    ItemOperaReq req;
    ItemOperaRsp rsp;
    req.ParseFromArray(body,len);
    int32_t size = req.items_size();
    switch(req.type()){
        case 0:{
            //do something
            for (int32_t i = 0; i < size; i++)
            {
                Itemproto itemp = req.items(i); /* code */
                printf("wai mian de items id is %d,num is %d\n",itemp.itemid(),itemp.itemnum());
                std::string str = std::to_string(itemp.itemid());
                std::map<std::string,std::map<std::string,std::string>> resmp;
                printf("printf str is :%s\n",str.c_str());
                if(REDIS.redisptr->llen(str)==0){
                    printf("xxxxxxxxx\n");
                    int ret = MYSQLPP.query("select * from itembasedata;");
                    if(ret==Success){
                        resmp = MYSQLPP.parser();
                        printf("aaaaaaaaaaaaaaaaaaaaa size is %d\n",(int32_t)(resmp.size()));
                        if(resmp.size()!=0){
                            for(auto it = resmp.begin();it!=resmp.end();it++){
                                printf("bbbbbbbbbbbbbbb size is %d\n",(int32_t)(it->second.size()));
                                for(auto itt = it->second.begin();itt!=it->second.end();itt++){
                                    REDIS.redisptr->rpush(it->first,itt->second);
                                    printf("typeid: %s,attr is: %s,value is: %s\n",it->first.c_str(),itt->first.c_str(),itt->second.c_str());

                                }

                            }
                        }else
                        {
                            printf("wan le wan le  mysql zhong yu lian bu shang le!!!\n");
                            return Failed;
                        }
                            

                    }

                }else{
                    resmp[str].insert(std::make_pair("savenow",*(REDIS.redisptr->lindex(str,0))));
                    resmp[str].insert(std::make_pair("brief",*(REDIS.redisptr->lindex(str,1))));
                    resmp[str].insert(std::make_pair("atk",*(REDIS.redisptr->lindex(str,2))));
                    resmp[str].insert(std::make_pair("def",*(REDIS.redisptr->lindex(str,3))));
                    resmp[str].insert(std::make_pair("hp",*(REDIS.redisptr->lindex(str,4))));
                    resmp[str].insert(std::make_pair("overlay",*(REDIS.redisptr->lindex(str,5))));
                }
                int32_t out;
                uint32_t res = m_bag->add_item(resmp[str],itemp.itemid(),itemp.itemnum(),out);
                printf("res is %u\n",res);
                if(res!=0){
                    printf("have %d tiems add,add item id is %d,number is %d\n",i,itemp.itemid(),itemp.itemnum());
                    std::string strs;
                    strs = "INSERT INTO iteminfo ";
                    MYSQLPP.query((strs + "(itemid,playerid, typeid,num,tradable) "+"VALUES "+
                    "('"+std::to_string(res)+"','"+std::to_string(get_id())+"','"+std::to_string(itemp.itemid())+"','"+
                    std::to_string(itemp.itemnum())+"','"+std::to_string(1)+"') ON DUPLICATE KEY UPDATE num = "+std::to_string(out)+";").c_str());
                    printf(") ON DUPLICATE KEY UPDATE num =%d\n",out);
                    rsp.set_issuc(true);
                    rsp.set_biid(res);
                    rsp.set_type(itemp.itemid());
                    rsp.set_itemnum(out);
                    send_msg(DEMOID::ITEMOPERARSP,rsp);

                }else
                {
                    rsp.set_issuc(false);
                    
                    send_msg(DEMOID::ITEMOPERARSP,rsp);
                    return Failed;
                }
                
            }
            //REDIS.redisptr->set(std::to_string(get_id()),std::string(body,len));
            
            
            break;
        }
        case 1:{
            printf("delete start\n");
            for (int32_t i = 0; i < size; i++)
            {
                Itemproto itemp = req.items(i); /* code */
                //auto it = ITEMFAC.create(itemp.itemid,itemp.itemnum);
                printf("wai mian delete de items id is %d,num is %d\n",itemp.itemid(),itemp.itemnum());
                int ret = m_bag->del_item(itemp.biid(),itemp.itemnum());
                printf("delete item biid:%d,number:%d success!!\n",itemp.biid(),itemp.itemnum());
                if(ret==Success){
                    std::string strs;
                    strs += "DELETE FROM iteminfo WHERE itemid =";
                    MYSQLPP.query(( strs + std::to_string(itemp.biid()) +";").c_str());
                }else if(ret==-1){
                    rsp.set_issuc(false);
                    rsp.set_type(1000);
                    send_msg(DEMOID::ITEMOPERARSP,rsp);
                    return Failed;
                }else{
                    MYSQLPP.query(( "UPDATE iteminfo SET num= "+std::to_string(ret)+" WHERE itemid="+std::to_string(itemp.biid()) +";").c_str());
                    
                }
                rsp.set_itemnum(ret);
                rsp.set_biid(itemp.biid());
                //REDIS.redisptr->set(std::to_string(get_id()),std::string(body,len));
            }
            rsp.set_issuc(true);
            rsp.set_type(1000);
            
            send_msg(DEMOID::ITEMOPERARSP,rsp);
            break;
        }
        case 2:{
            // for (int32_t i = 0; i < size; i++)
            // {
            //     Itemproto itemp = req.ii(i); /* code */
            //     auto it = ITEMFAC.create(itemp.itemid(),itemp.itemnum());
            //     m_bag->add_item(it);
            //     printf("add item id:%u,breif:%s success!!\n",it->get_itemCount(),it->get_itemBrief().c_str());
            // }
            printf("trade to do ...\n");
            break;
        }
        case 3:{
            for (int32_t i = 0; i < size; i++)
            {
                Itemproto itemp = req.items(i); /* code */
                m_bag->merge_item(itemp.biid(),req.tarbiid());
                printf("change item src_id:%d,tar_id:%d success!!\n",itemp.biid(),req.tarbiid());
                rsp.set_issuc(true);
                //REDIS.redisptr->set(std::to_string(get_id()),std::string(body,len));
                send_msg(DEMOID::ITEMOPERARSP,rsp);
            }
            break;
        }case 4:{
            printf("chuan shang  zhuang bei \n");
            for(int32_t i=0;i<size;i++){
                Itemproto itemp = req.items(i);
                if(itemp.itemid()>3&&itemp.itemid()<21){
                    m_battle_bag->set_item(m_bag->get_item(itemp.biid()));

                }else return Failed;
                rsp.set_biid(itemp.biid());
            }
            rsp.set_issuc(true);
            
            rsp.set_type(999);
            send_msg(DEMOID::ITEMOPERARSP,rsp);
            break;
        }case 5:{
            printf("xie xia zhuang bei \n");
            for(int32_t i=0;i<size;i++){
                Itemproto itemp = req.items(i);
                if(m_battle_bag->del_bat_item(itemp.biid())!=Success){
                    return Failed;
                }
                rsp.set_biid(itemp.biid());
            }
            rsp.set_issuc(true);
            
            rsp.set_type(998);
            send_msg(DEMOID::ITEMOPERARSP,rsp);
            break;
        }

        default:{
            printf("this protobuf type parser failed!!!\n");
            rsp.set_issuc(false);
            rsp.set_errorcode(1);
            send_msg(DEMOID::ITEMOPERARSP,rsp);
            return Failed;
            break;
        }
        
    };

    
    return Success;
}

void Player::send_msg(int32_t cmd_id, google::protobuf::Message &msg) {
    DEMOSERVER.send_msg(get_id(),cmd_id,msg);
}

void Player::broadcast(int32_t cmd_id, google::protobuf::Message &msg) {
    DEMOSERVER.send_all_msg(get_id(),cmd_id,msg);
}



}
