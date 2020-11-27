#include <cstdint>
#include <cmath>
#include <random>
#include <chrono>


#include "player.h"
#include "server.h"

namespace demo{

Player::Player(int32_t playerID):m_id(playerID),m_clitime(0){
	m_sertime = get_time();
    m_bag = new Backpack();
}

Player::~Player(){

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
    int32_t pri_y = m_dir[1] + dlt_time * 0.2 * m_run * m_face[1];
    int32_t pri_z = m_dir[2] + dlt_time * 0.2 * m_run * m_face[2];
    int32_t dir = (req.src_dirx() - pri_x) * (req.src_dirx() - pri_x) + (req.src_diry() - pri_y) * (req.src_diry() - pri_y) + (req.src_dirz() - pri_z) * (req.src_dirz() - pri_z);
    if(dir>1000){
        //todo lahui wanjia
        printf("wu cha guo da yi tiao hui\n");
        req.set_src_dirx(m_dir[0]);
        req.set_src_diry(m_dir[1]);
        req.set_src_dirz(m_dir[2]);
        req.set_time(m_clitime);
        req.set_speed(m_run);
        req.set_facex(m_face[0]);
        req.set_facey(m_face[1]);
        req.set_facez(m_face[2]);
        req.set_trg_dirx(m_tar_dir[0]);
        req.set_trg_diry(m_tar_dir[1]);
        req.set_trg_dirz(m_tar_dir[2]);
        send_msg(DEMOID::MOVEREQ,req);
        return Success;
    }
    m_clitime = req.time();
    int32_t num = serdlt - dlt_time*20;
    printf("server client interval is num:%d\n",num);
    if(num>0){
        m_dir[0] = pri_x + req.facex()*req.speed()*num/20;
        m_dir[1] = pri_y + req.facey()*req.speed()*num/20;
        m_dir[2] = pri_z + req.facez()*req.speed()*num/20;
    }else{
    m_dir[0] = req.src_dirx();
    m_dir[1] = req.src_diry();
    m_dir[2] = req.src_dirz();
    m_tar_dir[0] = req.trg_dirx();
    m_tar_dir[1] = req.trg_diry();
    m_tar_dir[2] = req.trg_dirz();
    }
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
    DEMOSERVER.send_fd_msg(get_id(),DEMOID::INITPOS,rsp);

    return Success;
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
                m_bag->add_item(itemp.itemid(),itemp.itemnum());
                
                rsp.set_issuc(true);
                REDIS.redisptr->
                send_msg(DEMOID::ITEMOPERARSP,rsp);
            }
            
            break;
        }
        case 1:{
            for (int32_t i = 0; i < size; i++)
            {
                Itemproto itemp = req.items(i); /* code */
                //auto it = ITEMFAC.create(itemp.itemid,itemp.itemnum);
                m_bag->del_item(itemp.biid(),itemp.itemnum());
                printf("delete item biid:%d,number:%d success!!\n",itemp.biid(),itemp.itemnum());
                rsp.set_issuc(true);
                send_msg(DEMOID::ITEMOPERARSP,rsp);
            }
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
                m_bag->change_item(itemp.biid(),req.tarbiid());
                printf("change item src_id:%d,tar_id:%d success!!\n",itemp.biid(),req.tarbiid());
                rsp.set_issuc(true);
                send_msg(DEMOID::ITEMOPERARSP,rsp);
            }
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
