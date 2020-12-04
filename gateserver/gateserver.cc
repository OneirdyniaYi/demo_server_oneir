#include "gateserver.h"

namespace demo
{
GateServer::GateServer(){
    m_client_to_game = std::make_shared<ClientMoudle>();
    m_client_to_login = std::make_shared<ClientMoudle>();
    

}

GateServer::~GateServer(){
    
}

void GateServer::init(char const *ip,char const *port){
    m_gate_logicptr = std::make_shared<GateLogic>(shared_from_this());
    m_bserver = std::make_shared<BaseServer<GateLogic,GateServer>>(shared_from_this());
    
    m_bserver->init(ip,port);

    m_bserver->get_epoll()->addEvent(m_client_to_game->get_fd());
    m_bserver->add_socket(m_client_to_game->get_fd(),m_client_to_game->get_socket());

    m_bserver->get_epoll()->addEvent(m_client_to_login->get_fd());
    m_bserver->add_socket(m_client_to_game->get_fd(),m_client_to_login->get_socket());

    m_bserver->register_msg(std::make_pair(DEMOID::LOGINNOT,&GateLogic::session_handle),
    std::make_pair(DEMOID::LOGINGATEREQ,&GateLogic::login_handle));
}

int32_t GateServer::process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd,std::shared_ptr<MsgHandler<GateLogic>> msghandle){
    if (!pszInCode || iInCodeSize <= 0) {
        return Failed;
    }
	printf("incodesize is %d,socketfd is %d\n",iInCodeSize,socketfd);
    MsgHead stHead;
    char* p = const_cast<char*>(pszInCode);
    int32_t head_outLength = MESSAGE_HEAD_SIZE;
    if (stHead.decode(p, iInCodeSize)) {
        printf("[Server::%s]head decode error\n", __FUNCTION__);
        return Failed;
    }
    if (stHead.iLens != iInCodeSize) {
        printf("[Server::%s]the package size decoded from service was not equal with received size.\n", __FUNCTION__);
        return Failed;
    }

    int32_t iBodySize = stHead.iLens - head_outLength;

    printf("[network][Server::%s][uplayerid:%d][msg_id:%d]\n", __FUNCTION__, stHead.iPlayerID, stHead.iMsgID);
    if(stHead.iMsgID==DEMOID::LOGINNOT||stHead.iMsgID==DEMOID::LOGINGATEREQ){
        auto func = msghandle->get_func(stHead.iMsgID);
		if(NULL != func){
			(m_gate_logicptr.get()->*func)(stHead,pszInCode + head_outLength,iBodySize);
		}
    }else if(stHead.iMsgID==DEMOID::MOVERSP||stHead.iMsgID==DEMOID::ITEMOPERARSP){
        m_bserver->send_byte(m_playermap[stHead.iPlayerID],pszInCode,iInCodeSize);
    }else{
        if(m_playermap.find(stHead.iPlayerID) == m_playermap.end()){
            m_playermap.insert(std::make_pair(stHead.iPlayerID,socketfd));
        }else{
            m_playermap[stHead.iPlayerID] = socketfd;
        }
        m_bserver->send_byte(m_client_to_game->get_fd(),pszInCode,iInCodeSize);
    }

	// }
    return Success;
}

void GateServer::work(){
    int64_t lasttime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while(1){
        int64_t curtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if(curtime > lasttime+10000){
            GateHeart req;
            req.set_loadnum(m_id_auth_mp.size());
            m_client_to_login->send_msg(DEMOID::GATEHEART,req);
            lasttime = std::move(curtime);
        }
        m_bserver->recv_msg();
    }
}

void GateServer::send_msg(int32_t playerID,int32_t cmd_id, google::protobuf::Message &msg){
    m_bserver->send_msg(playerID,cmd_id,msg);
}

void GateServer::send_all_msg(int32_t playerID,int32_t cmd_id, google::protobuf::Message &msg){
    m_bserver->send_all_msg(playerID,cmd_id,msg);
}

void GateServer::update_auth_mp(int32_t playerid,std::pair<int32_t,int64_t> pairr){
    m_id_auth_mp[playerid] = pairr;
}

std::pair<int32_t,int64_t> GateServer::get_mp(int32_t playerid){
    return m_id_auth_mp[playerid];
}

} // namespace demo`
