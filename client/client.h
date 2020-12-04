#ifndef __CLIENT_MOUDLE_H__
#define __CLIENT_MOUDLE_H__

#include <cstdint>
#include <unordered_map>
#include <string>

#include "socket.h"
#include "singleton.h"
#include "msghead.h"
#include "msghandle.h"
#include "epoll.h"
#include "noncopyable.h"
#include "ProtoID.pb.h"
#include "Demo.pb.h"

#define EPOLL_MAX_SIZE 128

namespace demo{

class Client : noncopyable {
public:
    Client();
    ~Client();


    int32_t toIpAddr(char* ipstr,struct in_addr& ip);
    int32_t init(char const *ip,char const *port,char const *playerid);

    void working();

    int32_t recv_msg();
    
    void send_msg(int32_t uin, int32_t cmd_id, google::protobuf::Message &msg);
  
    int32_t handle_test(MsgHead& stHead, const char* body, const int32_t len);
    int32_t handle_test2(MsgHead& stHead, const char* body, const int32_t len);
private:
    Epoll* m_epoll;
    Socket* m_socket;

    struct in_addr m_ip;
	
    uint16_t m_port;    
	
    int32_t m_playerid;

    MsgHandler<Client>* m_msgHandle;
private:
    int32_t del_io_msg();
    void send_msg_req_test1();
    void send_msg_req_test2();
    void send_msg_req_test3(std::string &input);
    void register_all_msg();

    int32_t process_packet(const char *pszInCode, const int32_t iInCodeSize,int32_t socketfd);

};
//#define DEMOCLIENT Singleton<Client>::Instance()

}
#endif
