#include <cstdint>
#include "player.h"
#include "server.h"

namespace demo{

Player::Player(int32_t playerID):m_id(playerID){
	
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

void Player::send_msg(int32_t cmd_id, google::protobuf::Message &msg) {
    DEMOSERVER.send_msg(get_id(),cmd_id,msg);
}

void Player::broadcast(int32_t cmd_id, google::protobuf::Message &msg) {
    DEMOSERVER.send_all_msg(cmd_id,msg);
}

}
