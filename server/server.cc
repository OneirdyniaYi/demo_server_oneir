#include <cstdio>
#include <map>
#include <cstdlib>
#include <iostream>

#include "server.h"
#include "errorcode.h"
namespace demo{
Server::Server(){
	m_socket = new Socket();
	m_epoll = new Epoll();
	m_msgHandle = new MsgHandler<Player>();
}

Server::~Server(){
    delete m_socket;
    delete m_msgHandle;
    delete m_epoll;
}

int32_t Server::toIpAddr(char* ipstr,struct in_addr& ip){
	if(!ipstr || ::inet_pton(AF_INET,ipstr,&ip)<=0) return Failed;
	return Success;
}

int32_t Server::init(char const *ip,char const *port){
	
	if(toIpAddr(const_cast<char*>(ip),m_ip)){
		printf("ip fail\n");
		return Failed;
	}
	
	int32_t pport = ::atoi(const_cast<char*>(port));
	
	if(pport>65535||pport<0){
		printf("port error\n");
		return Failed;
	}

	m_port = static_cast<uint16_t>(pport);

	if(m_socket->open()){
		printf("open failed\n");	
		return Failed;
	}
	if(m_socket->bind(m_ip,m_port)){
		printf("bind failed\n");
		return Failed;
	}
	if(m_socket->listen()){	
		printf("listen failed\n");
		return Failed;
	}

	int32_t listen_fd = m_socket->get_fd();

	if(m_epoll->init(EPOLL_MAX_SIZE)){
		printf("epoll init failed\n");
		return Failed;
	}

	
	printf("listen fd is %d\n",listen_fd);
	
	
	if(m_epoll->addEvent(listen_fd)){
		printf("epoll add failed\n");
		return Failed;
	}


	m_mp.insert(std::make_pair(listen_fd,m_socket));

	for(auto it=m_mp.begin();it!=m_mp.end();it++){
		printf("mp sockets are:%d\n",it->first);
	}

	register_all_msg();
	
	printf("success is %d\n",Success);
	
	std::cout<<"iostream"<<" "<<Success<<std::endl;
	return Success;

}

int32_t Server::process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd){
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



    auto func = m_msgHandle->get_func(stHead.iMsgID);
    if(NULL != func){
        (map_players[stHead.iPlayerID].player->*func)(stHead,pszInCode + head_outLength,iBodySize);
    }
    return Success;
}

int32_t Server::recv_msg(){
	int32_t timeout = 1;
	int32_t fd_num = m_epoll->ableEvents(timeout);
	

	for(int32_t i = 0;i < fd_num; ++i){
		int32_t socketfd = m_epoll->get_event_fd(i);

		if(m_mp.find(socketfd) == m_mp.end() || m_mp[socketfd] == nullptr ){
			printf("[Server::%s] get_server_tcpsocket failed fd:%d\n", __FUNCTION__, socketfd);
			
			continue;
		}
		if(socketfd == m_socket->get_fd()){
			int32_t accept_fd = -1;
			in_addr peerIp; 
			uint16_t peerPort;
			if(m_socket->accept(accept_fd,peerIp,peerPort)){
				printf("accept_fd  failed\n");
				continue;
			}
			printf("acceptfd is %d\n",accept_fd);

			Socket* peerSocket = new Socket(accept_fd,peerIp,peerPort);
			peerSocket->set_nonblock(accept_fd);
			peerSocket->set_nodelay(accept_fd);

			if(m_epoll->addEvent(accept_fd)){

				return Failed;
			}
			if(m_mp.find(accept_fd)==m_mp.end())
				m_mp.insert(std::make_pair(accept_fd,peerSocket));
			else m_mp[accept_fd] = 	peerSocket;


    if(map_players.find(stHead.iPlayerID) == map_players.end()){
     	PlayerInfo pinfo;
      	pinfo.fd = socketfd;
      	pinfo.player = new Player(stHead.iPlayerID);
  		map_players.insert(std::make_pair(stHead.iPlayerID,pinfo));
    }else{
    	map_players[stHead.iPlayerID].fd = socketfd;
    }
		}else{
			if(m_mp[socketfd]->freshBuf()){
				printf("read to buffer error\n");
				return Failed;
			}
			if(m_mp[socketfd]->process_packet(std::bind(&Server::process_packet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))){
				printf("socket process_data failed fd:%d\n",socketfd);
				delete m_mp[socketfd];
				m_mp[socketfd] = nullptr;
			}
		
		}

	}
	return Success;
}

void Server::work(){
	while(1) recv_msg();
}

void Server::send_msg(int32_t PlayerID, int32_t cmd_id, google::protobuf::Message &msg){
	static char data[common_buffer_size];

	MsgHead head;
	head.iMsgID = cmd_id;
	head.iPlayerID = PlayerID;
	head.iLens = MESSAGE_HEAD_SIZE + msg.ByteSizeLong();

	int32_t codeLen = 0;
	head.encode(data,codeLen);
	msg.SerializePartialToArray(data+codeLen,msg.ByteSizeLong());
	if(map_players.find(PlayerID) == map_players.end()){
		printf("no uin in server uin:%d\n",PlayerID);
		return;
	}
	int32_t ret = m_mp[map_players[PlayerID].fd]->writen(data,(size_t)head.iLens);
	if(ret){
		printf("send msg (id=%d) error ret=%d,errno:%d ,strerror:%s,fd = %d\n",cmd_id,ret,errno, strerror(errno),map_players[PlayerID].fd);
	}
	printf("send msg  fd:%d    msglen = %d\n",map_players[PlayerID].fd,head.iLens);
}

void Server::send_all_msg(int32_t cmd_id, google::protobuf::Message &msg){
	static char data[common_buffer_size];

	MsgHead head;
	head.iMsgID = cmd_id;
	head.iPlayerID = 0;
	head.iLens = MESSAGE_HEAD_SIZE + msg.ByteSizeLong();

	int32_t codeLen = 0;
	head.encode(data,codeLen);
	msg.SerializePartialToArray(data+codeLen,msg.ByteSizeLong());

		printf("players number is %d\n",(int)map_players.size());
	for(auto it = map_players.begin();it != map_players.end();it++){
		printf("111111111111\n");
		
		if(m_mp.find(it->second.fd)==m_mp.end()){
			printf("cant find player socket\n");
			continue;
		}
		int ret = m_mp[it->second.fd]->writen(data,(size_t)head.iLens);
		if(ret){
			printf("send all msg (id=%d) error ret=%d,errno:%d ,strerror:%s,fd = %d\n",cmd_id,ret,errno, strerror(errno),it->second.fd);
		}
		printf("send all msg  fd:%d    msglen = %d\n",it->second.fd,head.iLens);
	}


}


void Server::register_all_msg(){
    m_msgHandle->RegisterMsg(DEMOID::REQ_TEST, &Player::handle_test);
    m_msgHandle->RegisterMsg(DEMOID::REQ_TEST2, &Player::handle_test2);

}

}

