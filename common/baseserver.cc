#include "baseserver.h"
#include "errorcode.h"

namespace demo
{
// template<typename T,typename S>
// BaseServer<T,S>::BaseServer(){
//     m_epollptr = std::make_shared<Epoll>();
//     m_socketptr = std::make_shared<Socket>();
//     //m_Tptr = std::make_shared<T>(serverptr);
//     m_msghandleptr = std::make_shared<MsgHandler<T>>();
// }

template<typename T,typename S>
BaseServer<T,S>::BaseServer(std::shared_ptr<S> serverptr):m_Sptr(serverptr){
    m_epollptr = std::make_shared<Epoll>();
    m_socketptr = std::make_shared<Socket>();
    //m_Tptr = std::make_shared<T>(serverptr);
    m_msghandleptr = std::make_shared<MsgHandler<T>>();
}

template<typename T,typename S>
BaseServer<T,S>::~BaseServer(){
    m_socketptr.reset();
    m_epollptr.reset();
    //m_Tptr.reset();
    m_msghandleptr.reset();
}

template<typename T,typename S>
int32_t BaseServer<T,S>::toIpAddr(char* ipstr,struct in_addr& ip){
	if(!ipstr || ::inet_pton(AF_INET,ipstr,&ip)<=0) return Failed;
	return Success;
}

template<typename T,typename S>
int32_t BaseServer<T,S>::init(char const *ip,char const *port){
	
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

	if(m_socketptr->open()){
		printf("open failed\n");	
		return Failed;
	}
	if(m_socketptr->bind(m_ip,m_port)){
		printf("bind failed\n");
		return Failed;
	}
	if(m_socketptr->listen()){	
		printf("listen failed\n");
		return Failed;
	}

	int32_t listen_fd = m_socketptr->get_fd();

	if(m_epollptr->init(EPOLL_MAX_SIZE)){
		printf("epoll init failed\n");
		return Failed;
	}

	
	printf("listen fd is %d\n",listen_fd);
	
	
	if(m_epollptr->addEvent(listen_fd)){
		printf("epoll add failed\n");
		return Failed;
	}

    m_mp.insert(std::make_pair(listen_fd,m_socketptr));

	printf("add listenfd success is %d\n",Success);

	return Success;

}

// template<typename T,typename S>
// int32_t BaseServer<T,S>::p(std::function<int32_t(const char *, const int32_t, int32_t,std::shared_ptr<T>)> callback){
//     callback(pszInCode,iInCodeSize,socketfd,m_msghandleptr);
// }

template<typename T,typename S>
int32_t BaseServer<T,S>::process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd){
    return m_Sptr->process_packet(pszInCode,iInCodeSize,socketfd,m_msghandleptr);
}

template<typename T,typename S>
int32_t BaseServer<T,S>::recv_msg(){
	int32_t timeout = 1;
	int32_t fd_num = m_epollptr->ableEvents(timeout);
	

	for(int32_t i = 0;i < fd_num; ++i){
		int32_t socketfd = m_epollptr->get_event_fd(i);
		printf("***********************%d\n",socketfd);
		if(m_mp.find(socketfd) == m_mp.end() || m_mp[socketfd] == nullptr ){
			printf("[Server::%s] get_server_tcpsocket failed fd:%d\n", __FUNCTION__, socketfd);
			
			continue;
		}
		if(socketfd == m_socketptr->get_fd()){
			int32_t accept_fd = -1;
			in_addr peerIp; 
			uint16_t peerPort;
			if(m_socketptr->accept(accept_fd,peerIp,peerPort)){
				printf("accept_fd  failed\n");
				continue;
			}
			printf("acceptfd is %d\n",accept_fd);

			std::shared_ptr<Socket> peerSocket = std::make_shared<Socket>(accept_fd,peerIp,peerPort);
			peerSocket->set_nonblock(accept_fd);
			peerSocket->set_nodelay(accept_fd);

			if(m_epollptr->addEvent(accept_fd)){

				return Failed;
			}
			if(m_mp.find(accept_fd)==m_mp.end())
				m_mp.insert(std::make_pair(accept_fd,peerSocket));
			else m_mp[accept_fd] = peerSocket;


            // if(m_playermap.find(stHead.iPlayerID) == m_playermap.end()){
            //     m_playermap.insert(std::make_pair(stHead.iPlayerID,accept_fd));
            // }else{
            //     m_playermap[stHead.iPlayerID] = accept_fd;
            // }
		}else{
			int32_t ret = m_mp[socketfd]->freshBuf();
			if(ret==FdClose){
                m_mp[socketfd].reset();
                m_mp.erase(socketfd);
			}else if(ret){
				printf("read to buffer error\n");
				return Failed;
			}
			if(m_mp[socketfd]->process_packet(std::bind(&BaseServer::process_packet, this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))){
				printf("socket process_data failed fd:%d\n",socketfd);
			}
		
		}

	}
	return Success;
}

template<typename T,typename S>
void BaseServer<T,S>::work(){
    int a=0;
    while(1){
        if(a==10000){
            printf("this thread is %u\n",(uint32_t)pthread_self());
            a = 0;
        }
        recv_msg();
        a++;
    }
}

template<typename T,typename S>
void BaseServer<T,S>::send_fd_msg(int32_t fd, int32_t cmd_id, google::protobuf::Message &msg){
	static char data[common_buffer_size];

	MsgHead head;
	head.iMsgID = cmd_id;
	//head.iPlayerID = PlayerID;
	head.iLens = MESSAGE_HEAD_SIZE + msg.ByteSizeLong();

	int32_t codeLen = 0;
	head.encode(data,codeLen);
	msg.SerializePartialToArray(data+codeLen,msg.ByteSizeLong());

	int32_t ret = m_mp[fd]->writen(data,(size_t)head.iLens);
	if(ret){
		printf("send msg (id=%d) error ret=%d,errno:%d ,strerror:%s,fd = %d\n",cmd_id,ret,errno, strerror(errno),fd);
	}
	printf("send msg  fd:%d    msglen = %d\n",fd,head.iLens);
}

template<typename T,typename S>
void BaseServer<T,S>::send_byte(int32_t fd,const char *pszInCode, const int32_t iInCodeSize){
    int32_t ret = m_mp[fd]->writen(pszInCode,(size_t)iInCodeSize);
	if(ret){
		printf("send error ret=%d,errno:%d ,strerror:%s,fd = %d\n",ret,errno, strerror(errno),fd);
	}
	printf("send msg  fd:%d    msglen = %d\n",fd,iInCodeSize);
}

template<typename T,typename S>
std::shared_ptr<Socket> BaseServer<T,S>::get_socket(int32_t fd){
    return m_mp[fd];
}

template<typename T,typename S>
    template<typename H,typename... Args>
    void BaseServer<T,S>::register_msg(H head,Args... args){
        //int32_t id = head.first;
        //std::function<int32_t(MsgHead& stHead, const char* body, const int32_t len)> func = head.second;
        //m_msghandleptr->RegisterMsg();
        m_msghandleptr->RegisterMsg(head.first,head.second);
        register_msg(args...);
    }

template<typename T,typename S>
    template<typename H>
    void BaseServer<T,S>::register_msg(H head){
        m_msghandleptr->RegisterMsg(head.first,head.second);
        //return ;
    }
} // namespace demo
