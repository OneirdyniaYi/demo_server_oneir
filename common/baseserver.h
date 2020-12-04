#ifndef __BASE_SERVER_H__
#define __BASE_SERVER_H__

#include <cstdint>
#include <memory>
#include <map>
#include <unordered_map>

#include "epoll.h"
#include "socket.h"
#include "noncopyable.h"
#include "mysqlplus.h"
#include "msghandle.h"
#include "Demo.pb.h"
#include "ProtoID.pb.h"

#define EPOLL_MAX_SIZE 128

namespace demo
{
    
template<typename T,typename S>
class BaseServer {
public:
	//BaseServer();
    BaseServer(std::shared_ptr<S> serverptr);
	~BaseServer();

	int32_t toIpAddr(char* ipstr,struct in_addr& ip);

	int32_t init(char const *ip,char const *port);

	int32_t recv_msg();

	void work();

    //int32_t process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd);

    template<typename H,typename... Args>
    void register_msg(H head,Args... args);
    
    template<typename H>
    void register_msg(H head);

    void send_fd_msg(int32_t fd,int32_t cmd_id, google::protobuf::Message &msg);

    void send_msg(int32_t playerID,int32_t cmd_id, google::protobuf::Message &msg);

	void send_all_msg(int32_t playerID,int32_t cmd_id, google::protobuf::Message &msg);

    void send_byte(int32_t fd,const char *pszInCode, const int32_t iInCodeSize);

    int32_t process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd);

    std::shared_ptr<Socket> get_socket(int32_t fd);

    std::shared_ptr<Epoll> get_epoll() const {return m_epollptr;}

    std::shared_ptr<MsgHandler<T>> get_msghandle() const {return m_msghandleptr;}

    void add_socket(int32_t fd,std::shared_ptr<Socket> socket) {m_mp.insert(std::make_pair(fd,socket));}

    //int32_t get_player_fd(int32_t playerid) const {return m_playermap[playerid];}

protected:


std::shared_ptr<Socket> m_socketptr;
std::shared_ptr<Epoll> m_epollptr;
//std::shared_ptr<T> m_Tptr;
std::shared_ptr<S> m_Sptr;

std::unordered_map<int32_t, std::shared_ptr<Socket>> m_mp;


// //std::map<int32_t,std::pair<std::shared_ptr<Socket>,int32_t>> m_gatemp; 
// std::shared_ptr<Mysqlplus> m_mysqlptr;
// std::shared_ptr<Client> m_clientptr;

std::shared_ptr<MsgHandler<T>> m_msghandleptr;

struct in_addr m_ip;

uint16_t m_port;


};





} // namespace demo



#endif