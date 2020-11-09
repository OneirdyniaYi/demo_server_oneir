#ifndef __SOCKET_H__
#define __SOCKET_H__


#include "noncopyable.h"
#include "buffer.h"

#include <arpa/inet.h>

#include <functional>

#define MAX_LISTEN 128

namespace demo{

class Socket : noncopyable{
public:
	Socket():m_sockfd(-1),m_ip({0}),m_port(0){
		m_buf = new Buffer(common_buffer_size);
	}
	Socket(int32_t sockfd,struct in_addr ip,uint16_t port);
	~Socket();
	
	int32_t get_fd() const {return m_sockfd;}

	int32_t set_nonblock(int32_t fd);

	int32_t set_nodelay(int32_t fd);


	void close();
	void shutdownWrite(int32_t sockfd);

	int32_t open();
	int32_t bind(struct in_addr& ip,uint16_t port);
	int32_t listen();
	int32_t connect(struct in_addr& ip,uint16_t port);
	int32_t accept(int32_t& fd, in_addr& peerIp, uint16_t& peerPort);

	
	int32_t writen(const char* data,size_t size);
  	int32_t freshBuf();

	int32_t process_packet(std::function<int32_t(const char *, const int32_t, int32_t)> callback);
	

	//const char* toIpStr(const struct in_addr& ip);
	//int32_t toIpAddr(const char* ipstr,struct in_addr& ip);

protected:
	int32_t m_sockfd;
	struct in_addr m_ip;
	uint16_t m_port;	
	Buffer* m_buf;
	
};













}






#endif
