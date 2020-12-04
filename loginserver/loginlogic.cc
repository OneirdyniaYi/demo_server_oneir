#include "loginlogic.h"
#include "errorcode.h"


#include <chrono>

namespace demo
{

LoginLogic::LoginLogic():m_id(1){

}

LoginLogic::LoginLogic(std::shared_ptr<LoginServer> ptr):m_server(ptr),m_id(1){
    
}

LoginLogic::~LoginLogic(){

}

int32_t LoginLogic::heartdeal(MsgHead &head,const char* body,const int32_t len){
    GateHeart heart;
    heart.ParseFromArray(body,len);
    m_server->update_load(head.iPlayerID,heart.loadnum());
    //m_server->sort_load();
    //m_server->inner_send();
    return Success;
}

int32_t LoginLogic::addrdeal(MsgHead &head,const char* body,const int32_t len){
    ListenAddr addr;
    addr.ParseFromArray(body,len);
    m_server->update_addr(head.iPlayerID,addr.ip(),addr.port());
    //m_server->sort_load();
    //m_server->inner_send();
    return Success;
}

int32_t LoginLogic::login_handle(MsgHead &head,const char* body,const int32_t len){
    printf("test login in !!!!!\n");
    LoginReq req;
    LoginRsp rsp;
    LoginNot note;
    req.ParseFromArray(body,len);
    m_server->get_mysql()->query(("select * from playerinfo where username = '"+req.username()+"';").c_str());
    std::map<std::string,std::map<std::string,std::string>> resmp = m_server->get_mysql()->parser();
    if(static_cast<int32_t>(resmp.size())!=0){
        //search success
        std::string strname = req.username();
        std::string playid = resmp[req.username()]["playerid"];
        if(resmp[req.username()]["password"] == req.password()){
            //password success
            rsp.set_issuc(1);
            int32_t id = std::stoi(resmp[req.username()]["playerid"]);
            rsp.set_playerid(id);
            int32_t min_load_fd = m_server->get_min_load();
            int32_t ip = m_server->get_addrip(min_load_fd);
            uint16_t port = m_server->get_addrport(min_load_fd);
            rsp.set_ip(ip);
            rsp.set_port(port);
            int64_t time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            rsp.set_sessionkey(time);
            note.set_sessionkey(time);
            note.set_time(time+300);
            note.set_playerid(id);
            send_msg(false,head.iPlayerID,DEMOID::LOGINRSP,rsp);
            send_msg(true,min_load_fd,DEMOID::LOGINNOT,note);
            return Success;
        }else{
            rsp.set_errorcode(0);
            rsp.set_issuc(0);

            send_msg(false,head.iPlayerID,DEMOID::LOGINRSP,rsp);
            printf("wocao wo zheng de zheng de ri le gou\n");
            return Failed;
        }
        
    }else{
        //mei you
        rsp.set_issuc(0);
        rsp.set_errorcode(1);
        send_msg(false,head.iPlayerID,DEMOID::LOGINRSP,rsp);
        return Failed;
    }
}

int32_t LoginLogic::regis_handle(MsgHead &head,const char* body,const int32_t len){
    if(m_id>100000) m_id = 1;
    LoginReq req;
    LoginRsp rsp;
    req.ParseFromArray(body,len);
    m_server->get_mysql()->query(("select * from playerinfo where username = '"+req.username()+"';").c_str());
    std::map<std::string,std::map<std::string,std::string>> resmp = m_server->get_mysql()->parser();
    if(static_cast<int32_t>(resmp.size())==0){
        //success
        std::string str;
        str += "INSERT INTO playerinfo ";
        m_server->get_mysql()->query(( str + "(username,password,playerid) "+"VALUES "+
        "('"+req.username()+"','"+req.password()+"','"+
        std::to_string((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<14) + (m_id++))+"');").c_str());
        rsp.set_issuc(1);
        send_msg(false,head.iPlayerID,DEMOID::REGISTRSP,rsp);
        return Success;
    }else{
        //you le
        rsp.set_issuc(0);
        send_msg(false,head.iPlayerID,DEMOID::REGISTRSP,rsp);
        return Failed;
    }
    
}

void LoginLogic::send_msg(bool isinner ,int32_t sendfd,int32_t cmd_id, google::protobuf::Message &msg){
    if(isinner){
        m_server->inner_send(sendfd,cmd_id,msg);
    }else{
        m_server->out_send(sendfd,cmd_id,msg);
    }
}


} // namespace demo
