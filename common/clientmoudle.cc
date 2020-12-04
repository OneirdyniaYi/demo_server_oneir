#include "clientmoudle.h"

namespace demo
{
ClientMoudle::ClientMoudle(){

}
ClientMoudle::~ClientMoudle(){

}


int32_t ClientMoudle::toIpAddr(char* ipstr,struct in_addr& ip){
    if(!ipstr || ::inet_pton(AF_INET,ipstr,&ip)<=0) return Failed;
    return Success;
}


int32_t ClientMoudle::init(char const *ip,char const *port){
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
    int32_t ret = m_socket->open();
    if(ret){
        printf("socked creat failed \n");
        return Failed;
    }
    ret = m_socket->connect(m_ip,m_port);
    if(ret){
        printf("socked connect failed \n");
        return Failed;
    }
    int32_t flags = 1;
    struct linger ling = {0, 0};
    setsockopt(m_socket->get_fd(), SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
    setsockopt(m_socket->get_fd(), SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    m_socket->set_nodelay(m_socket->get_fd());

    return Success;
}





} // namespace demo
