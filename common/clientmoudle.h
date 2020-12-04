#ifndef __CLIENT_MOUDLE_H__
#define __CLIENT_MOUDLE_H__

#include <cstdint>
#include <unordered_map>
#include <string>

#include "socket.h"
#include "msghead.h"
#include "epoll.h"
#include "ProtoID.pb.h"
#include "Demo.pb.h"
#include "errorcode.h"

namespace demo
{

class ClientMoudle{
public:
    ClientMoudle();
    ~ClientMoudle();


    int32_t toIpAddr(char* ipstr,struct in_addr& ip);
    int32_t init(char const *ip,char const *port);
    
    void send_msg(int32_t playerid, int32_t cmd_id, google::protobuf::Message &msg);
    
    void send_msg(int32_t cmd_id, google::protobuf::Message &msg);
    // int32_t handle_test(MsgHead& stHead, const char* body, const int32_t len);
    // int32_t handle_test2(MsgHead& stHead, const char* body, const int32_t len);

    int32_t get_fd() const {return m_socket->get_fd();}

    std::shared_ptr<Socket> get_socket() const {return m_socket;}
private:

    struct in_addr m_ip;
	
    uint16_t m_port;  

    std::shared_ptr<Socket> m_socket;
};




} // namespace demo




#endif