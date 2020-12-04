#ifndef __LOGIN_SERVER_H__
#define __LOGIN_SERVER_H__

#include <cstdint>
#include <memory>
#include <map>
#include <thread>
#include <atomic>
#include <string>
#include <tuple>

#include "ProtoID.pb.h"
#include "Demo.pb.h"
#include "epoll.h"
#include "socket.h"
#include "noncopyable.h"
#include "mysqlplus.h"
//#include "clientmoudle.h"
#include "msghandle.h"
#include "baseserver.h"
#include "baseserver.cc"
#include "msghead.h"
#include "loginlogic.h"

namespace demo
{

class LoginLogic;

class LoginServer : public noncopyable , public std::enable_shared_from_this<LoginServer>{
public:
	LoginServer();
	~LoginServer();

	// int32_t toIpAddr(char* ipstr,struct in_addr& ip);

	int32_t init(char const *ip,char const *port,const char* inner_ip,const char* inner_port);

	// int32_t recv_msg();

	void work();
    void inner_work();

    // int32_t inner_init();

    // int32_t inner_recv();

    // int32_t inner_process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd);
    int32_t process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd,std::shared_ptr<MsgHandler<LoginLogic>> msghandle);

    void update_load(int32_t fd,int32_t value);

    void update_addr(int32_t fd,uint32_t ip,uint16_t port);

    int32_t get_min_load();

    void inner_send(int32_t fd, int32_t cmd_id, google::protobuf::Message &msg);

    void out_send(int32_t fd, int32_t cmd_id, google::protobuf::Message &msg);
    
    uint32_t get_addrip(int32_t fd);

    uint16_t get_addrport(int32_t fd);

    std::shared_ptr<Mysqlplus> get_mysql() {return m_mysqlptr;}
private:


// std::shared_ptr<MsgHandler<LoginLogic>> m_msghandleptr; 
std::shared_ptr<LoginLogic> m_logicptr;
// // std::shared_ptr<Socket> m_socketptr;
// // std::shared_ptr<Epoll> m_epollptr;
// std::map<int32_t,std::pair<std::shared_ptr<Socket>,std::atomic<int32_t>>> m_gatemp; 
std::shared_ptr<Mysqlplus> m_mysqlptr;
// std::shared_ptr<Client> m_clientptr;

// std::unique_ptr<std::thread> m_thread;
// std::shared_ptr<Socket> m_inner_socketptr;
// std::shared_ptr<Epoll> m_inner_epollptr;
// struct in_addr m_inner_ip;
// uint16_t m_inner_port;

// std::map<int64_t,std::tuple<std::string,std::string,int32_t,uint16_t>> m_playercache; 

std::shared_ptr<BaseServer<LoginLogic,LoginServer>> m_inner_server;
std::shared_ptr<BaseServer<LoginLogic,LoginServer>> m_out_server;

std::map<int32_t,std::atomic<int32_t>> m_load;
std::map<int32_t,std::pair<uint32_t,uint16_t>> m_addr;

//std::shared_ptr<LoginLogic> m_logicptr;

};





} // namespace demo



#endif