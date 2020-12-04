#include "gatelogic.h"
#include "errorcode.h"


#include <chrono>

namespace demo
{
    
GateLogic::GateLogic(){

}

GateLogic::GateLogic(std::shared_ptr<GateServer> serverptr):m_server(serverptr){
    
}

GateLogic::~GateLogic(){

}



int32_t GateLogic::session_handle(MsgHead &head,const char* body,const int32_t len){
    LoginNot rsp;
    rsp.ParseFromArray(body,len);
    m_server->update_auth_mp(rsp.playerid(),std::make_pair(rsp.sessionkey(),rsp.time()));
    return Success;
}

int32_t GateLogic::login_handle(MsgHead &head,const char* body,const int32_t len){
    LoginGateReq req;
    req.ParseFromArray(body,len);
    int64_t curtime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if(curtime>m_server->get_mp(req.playerid()).second){
        InitDny dny;
        dny.set_errorcode(1);
        GATESERVERI.send_msg(head.iPlayerID,DEMOID::INITDNY,dny);
        return Failed;
    }else{
        if(req.sessionkey()==m_server->get_mp(req.playerid()).first){
            MoveRsp rr;
            m_server->get_game_client()->send_msg(head.iPlayerID,DEMOID::INITPOS,rr);
            return Success;
        }
        InitDny dny;
        dny.set_errorcode(2);
        GATESERVERI.send_msg(head.iPlayerID,DEMOID::INITDNY,dny);
        return Failed;
    }
}


}