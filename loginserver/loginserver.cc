#include "loginserver.h"
#include "errorcode.h"

#include <mutex>
#include <vector>

namespace demo
{
LoginServer::LoginServer(){
    // m_epollptr = std::make_shared<Epoll>();
    // m_socketptr = std::make_shared<Socket>();
    m_mysqlptr = std::make_shared<Mysqlplus>();
    //m_logicptr = std::make_shared<LoginLogic>();

    //m_logicptr = std::make_shared<LoginLogic>(shared_from_this());
}

LoginServer::~LoginServer(){
    m_mysqlptr.reset();
    m_inner_server.reset();
    m_out_server.reset();
    //m_logicptr.reset();
    // m_socketptr.reset();
    // m_epollptr.reset();
}

int32_t LoginServer::init(char const *ip,char const *port,const char* inner_ip,const char* inner_port){
    m_logicptr = std::make_shared<LoginLogic>(shared_from_this());
    m_inner_server = std::make_shared<BaseServer<LoginLogic,LoginServer>>(shared_from_this());
    m_out_server = std::make_shared<BaseServer<LoginLogic,LoginServer>>(shared_from_this());
    // BaseServer::init(ip,port);
    // struct in_addr inip;
    m_inner_server->init(inner_ip,inner_port);
    m_out_server->init(ip,port);
    m_mysqlptr->init("127.0.0.1","root","123456","test");

    m_inner_server->register_msg(std::make_pair(DEMOID::GATEHEART,&LoginLogic::heartdeal),
    std::make_pair(DEMOID::GATELISTENADDR,&LoginLogic::addrdeal));
    // if(toIpAddr(const_cast<char*>(inner_ip),m_inner_ip)){
	// 	printf("ip fail\n");
	// 	return Failed;
	// }
	m_out_server->register_msg(std::make_pair(DEMOID::REGISTREQ,&LoginLogic::regis_handle),
    std::make_pair(DEMOID::LOGINREQ,&LoginLogic::login_handle));
	// m_inner_port = static_cast<uint16_t>(::atoi(const_cast<char*>(inner_port)));
	
    // m_thread.reset(new std::thread(std::bind(&LoginServer::inner_work, this)));

	return Success;

}

void LoginServer::update_load(int32_t fd,int32_t value){
    m_load[fd] = value;
}

void LoginServer::update_addr(int32_t fd,uint32_t ip,uint16_t port){
    m_addr[fd] = std::make_pair(ip,port);
}

int32_t LoginServer::get_min_load(){
    int32_t min = 130;
    int32_t fd = -1;
    for(auto& i:m_load){
        if(i.second<min){
            fd = i.first;
            min = i.second;
        }
    }
    return fd;
}

int32_t LoginServer::process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd,std::shared_ptr<MsgHandler<LoginLogic>> msghandle){
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
    stHead.iPlayerID = socketfd;
    printf("[network][Server::%s][uplayerid:%d][msg_id:%d]\n", __FUNCTION__, stHead.iPlayerID, stHead.iMsgID);
    auto func = msghandle->get_func(stHead.iMsgID);
    
    if(NULL != func){
        (m_logicptr.get()->*func)(stHead,pszInCode + head_outLength,iBodySize);
    }
    return Success;
}

void LoginServer::inner_send(int32_t fd, int32_t cmd_id, google::protobuf::Message &msg){
    m_inner_server->send_fd_msg(fd,cmd_id,msg);
}

void LoginServer::out_send(int32_t fd, int32_t cmd_id, google::protobuf::Message &msg){
    m_out_server->send_fd_msg(fd,cmd_id,msg);
}

uint32_t LoginServer::get_addrip(int32_t fd){
    return m_addr[fd].first;
}

uint16_t LoginServer::get_addrport(int32_t fd){
    return m_addr[fd].second;
}
// int32_t LoginServer::inner_init(){
// 	if(m_inner_socketptr->open()){
// 		printf("open failed\n");	
// 		return Failed;
// 	}
// 	if(m_inner_socketptr->bind(m_inner_ip,m_inner_port)){
// 		printf("bind failed\n");
// 		return Failed;
// 	}
// 	if(m_inner_socketptr->listen()){	
// 		printf("listen failed\n");
// 		return Failed;
// 	}
    
//     int32_t listen_fd = m_inner_socketptr->get_fd();

// 	if(m_inner_epollptr->init(EPOLL_MAX_SIZE)){
// 		printf("epoll init failed\n");
// 		return Failed;
// 	}
    	
// 	printf("listen fd is %d\n",listen_fd);
	
	
// 	if(m_inner_epollptr->addEvent(listen_fd)){
// 		printf("epoll add failed\n");
// 		return Failed;
// 	}


// 	//m_gatemp.insert(std::make_pair(listen_fd,std::make_pair(m_inner_socketptr,std::atomic<int32_t>(-1))));
//     m_gatemp[listen_fd] = std::make_pair(m_inner_socketptr,std::atomic<int32_t>(-1));


// 	register_all_msg();
// }



/* int32_t LoginServer::inner_recv(){
    int32_t timeout = 1;
	int32_t fd_num = m_inner_epollptr->ableEvents(timeout);
	

	for(int32_t i = 0;i < fd_num; ++i){
		int32_t socketfd = m_inner_epollptr->get_event_fd(i);
		printf("***********************%d\n",socketfd);
		if(m_gatemp.find(socketfd) == m_gatemp.end() || m_gatemp[socketfd].first == nullptr ){
			printf("[Server::%s] get_server_tcpsocket failed fd:%d\n", __FUNCTION__, socketfd);
			
			continue;
		}
		if(socketfd == m_inner_socketptr->get_fd()){
			int32_t accept_fd = -1;
			in_addr peerIp; 
			uint16_t peerPort;
			if(m_inner_socketptr->accept(accept_fd,peerIp,peerPort)){
				printf("accept_fd  failed\n");
				continue;
			}
			printf("acceptfd is %d\n",accept_fd);

			std::shared_ptr<Socket> peerSocketptr = std::make_shared<Socket>(accept_fd,peerIp,peerPort);
			peerSocketptr->set_nonblock(accept_fd);
			peerSocketptr->set_nodelay(accept_fd);

			if(m_inner_epollptr->addEvent(accept_fd)){

				return Failed;
			}
			if(m_gatemp.find(accept_fd)==m_gatemp.end())
				//m_gatemp.insert(std::make_pair(accept_fd,std::make_pair(peerSocketptr,0)));
                m_gatemp[accept_fd] = std::make_pair(peerSocketptr,std::atomic<int32_t>(0));
            else m_gatemp[accept_fd] = 	std::make_pair(peerSocketptr,0);


    // if(map_players.find(stHead.iPlayerID) == map_players.end()){
    //  	PlayerInfo pinfo;
    //   	pinfo.fd = socketfd;
    //   	pinfo.player = new Player(stHead.iPlayerID);
  	// 	map_players.insert(std::make_pair(stHead.iPlayerID,pinfo));
    // }else{
    // 	map_players[stHead.iPlayerID].fd = socketfd;
    // }
		}else{
			int32_t ret = m_gatemp[socketfd].first->freshBuf();
			if(ret==FdClose){
                m_gatemp[socketfd].first.reset();
				m_gatemp.erase(socketfd);
			}else if(ret){
				printf("read to buffer error\n");
				return Failed;
			}
			if(m_gatemp[socketfd].first->process_packet(std::bind(&LoginServer::inner_process_packet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))){
				printf("socket process_data failed fd:%d\n",socketfd);
			}
		
		}

	}
	return Success;
}
*/

void LoginServer::inner_work(){
    m_inner_server->work();
}

void LoginServer::work(){
    m_out_server->work();
}

/*
int32_t LoginServer::inner_process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd){
	if (!pszInCode || iInCodeSize <= 0) {
        return Failed;
    }
	printf("incodesize is %d,socketfd is %d\n",iInCodeSize,socketfd);
    MsgHead stHead;
    char* p = const_cast<char*>(pszInCode);
    int32_t head_outLength = MESSAGE_HEAD_SIZE;
    if (stHead.decode(p, iInCodeSize)) {
        printf("[LoginServer::%s]head decode error\n", __FUNCTION__);
        return Failed;
    }
    if (stHead.iLens != iInCodeSize) {
        printf("[LoginServer::%s]the package size decoded from service was not equal with received size.\n", __FUNCTION__);
        return Failed;
    }

    int32_t iBodySize = stHead.iLens - head_outLength;

    printf("[network][LoginServer::%s][uplayerid:%d][msg_id:%d]\n", __FUNCTION__, stHead.iPlayerID, stHead.iMsgID);
    //std::function<int32_t(MsgHead&, const char *, int32_t)> func = 
    //std::bind(&MsgHandler<LoginLogic>::get_func, m_msghandleptr, std::placeholders::_1);
    auto func = m_msghandleptr->get_func(stHead.iMsgID);
    if(NULL != func){
        (m_logicptr.get()->*func)(stHead,pszInCode + head_outLength,iBodySize);
    }

    if(stHead.iMsgID==DEMOID::GATEHEART){
        GateHeart heart;
        heart.ParseFromArray(pszInCode + head_outLength,iBodySize);
        heart.gateid();
        heart.curtime();
        heart.loadnum();
        m_gatemp[socketfd].second = heart.loadnum();
    }else if(stHead.iMsgID==DEMOID::CHECKAGAIN){
        CheckAgain req;
        LoginRsp rsp;
        req.ParseFromArray(pszInCode + head_outLength,iBodySize);
        if(m_playercache.find(req.playerid())!=m_playercache.end()){
            if(std::get<0>(m_playercache[req.playerid()])==req.username()&&std::get<1>(m_playercache[req.playerid()])==req.password()&&std::get<2>(m_playercache[req.playerid()])==req.ip()&&std::get<3>(m_playercache[req.playerid()])==req.port()){
                rsp.set_issuc(true);
            }
            
        }
        rsp.set_issuc(false);
    }


    return Success;
} */

} // namespace demo
