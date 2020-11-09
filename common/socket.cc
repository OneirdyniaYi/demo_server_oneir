#include "socket.h"
#include "errorcode.h"

#include <cstdio>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/tcp.h>
#include <cstddef>

namespace demo{

Socket::Socket(int32_t sockfd,struct in_addr ip,uint16_t port):m_sockfd(sockfd),m_ip(ip),m_port(port){
    m_buf = new Buffer(common_buffer_size);
}

Socket::~Socket(){
    if(m_buf!=nullptr){
        delete m_buf;
        m_buf = nullptr;
    }
    close();
    m_ip = {0};
    m_port = 0;
}
/*
const char* Socket::toIpStr(const struct in_addr& ip){
    char res[4];
    return ::inet_ntop(AF_INET,&ip,res,sizeof(res));
}

int32_t Socket::toIpAddr(const char* ipstr,struct in_addr& ip){
    if(!ipstr || ::inet_pton(AF_INET,ipstr,&ip)<=0) return Failed;
    return Success;
}
*/

void Socket::close(){
    if(m_sockfd < 0) return;
    ::close(m_sockfd);
    m_sockfd = -1;
}

void Socket::shutdownWrite(int32_t fd){
    //todo
    return;
}

int32_t Socket::open(){
    int32_t fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
          printf("create socket error = %d, info = %s", errno, strerror(errno));
          return Failed;
    }
    m_sockfd = fd;

    return Success;
}

int32_t Socket::bind(struct in_addr& ip,uint16_t port){
    struct sockaddr_in servAddr;
    memset(&servAddr,0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr = ip;
    if(::bind(m_sockfd,(struct sockaddr*)&servAddr,sizeof(servAddr))==-1){
        //printf("bind ip = %d, port = %d, error = %d, info = %s", ip, port, errno, strerror(errno));
        return Failed;
    }

	m_ip = ip;
	m_port = port;
    return Success;

}

int32_t Socket::listen(){
    if(::listen(m_sockfd,MAX_LISTEN) != 0){
        printf("listen failed with code[%d]\n", errno);
        return Failed;
    }
    return Success;

}

int32_t Socket::connect(struct in_addr& ip,uint16_t port){
    struct sockaddr_in cliAddr;
    memset(&cliAddr,0,sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_port = htons(port);
    cliAddr.sin_addr = ip;

    if(set_nonblock(m_sockfd)){
        return Failed;
    }
    int ret = ::connect(m_sockfd,(struct sockaddr*)&cliAddr, sizeof(cliAddr));
    
    printf("connect is %d\n",ret);

    if(ret==-1){
        if(errno != EINPROGRESS){
          	printf("connect error\n");
		return Failed;
        }
    }
    
	m_ip = ip;
	m_port = port;
    return Success;
}

int32_t Socket::accept(int32_t& fd, in_addr& peerIp, uint16_t& peerPort){
    struct sockaddr_in peerAddr;
    memset(&peerAddr,0,sizeof(peerAddr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof(peerAddr));
    fd = ::accept(m_sockfd,(struct sockaddr*)&peerAddr,&addrlen);
    printf("real accept fd is %d\n",fd);
    if(fd < 0){
        if(errno != EAGAIN && errno != EINTR){
            //printf("accept connect server error ip = %s, port = %d, error = %d, info = %s", m_ip, m_port, errno, strerror(errno));
        }
        return Failed;
    }

    int32_t ret = set_nonblock(fd);
    if(ret){
        return Failed;
    }
    set_nodelay(fd);


    peerIp = peerAddr.sin_addr;
    peerPort = ntohs(peerAddr.sin_port);

    return Success;
}

int32_t Socket::writen(const char* data,size_t size){
    if(m_sockfd < 0){
          return Failed;
    }
    if(!data || !size){
          return Failed;
    }
    char* ptr = const_cast<char*>(data);
    int32_t nleft = size;
    int32_t nsend = 0;
	printf("send size is %d\n",nleft);
    while(nleft > 0){
        if((nsend = send(m_sockfd, ptr, (size_t)nleft,0)) <= 0){
            if(nsend == 0 || (nsend<0 && errno != EINTR && EAGAIN != errno && EWOULDBLOCK != errno)){
                  close();
                  break;
            }
        }else {
	
            ptr += nsend;
            nleft -= nsend;
	}
    }

    return Success;
}

int32_t Socket::freshBuf(){
	return m_buf->recvbufWrite(m_sockfd);
}

int32_t Socket::process_packet(std::function<int32_t(const char *, const int32_t, int32_t)> callback){
    size_t packet_size = 0;
    static char recv[common_buffer_size];
    while(1){
        int32_t ret = m_buf->getOnePacket(recv,packet_size);
	if(ret == Failed){
            printf("[Socket::%s] get_one_code failed. socket:%d errorCode:%d\n",
                       __FUNCTION__, m_sockfd, ret);
            return Failed;
        }else if(ret == Success){
            callback(recv,packet_size,m_sockfd);
            continue;
        }
	break;
    }
    return Success;
}




int32_t Socket::set_nonblock(int32_t fd){
  	// non-block
  	int32_t flags = ::fcntl(fd, F_GETFL, 0);
  	flags |= O_NONBLOCK;
  	int32_t ret = ::fcntl(fd, F_SETFL, flags);
  	if(ret<0){
  		    printf("set accepted socket O_NONBLOCK failed, just close it!\n");
		      return Failed;
 	 }	

  	// close-on-exec
  	flags = ::fcntl(fd, F_GETFD, 0);
  	flags |= FD_CLOEXEC;
  	ret = ::fcntl(fd, F_SETFD, flags);
  	if(ret<0){
		    printf("set accepted socket F_SETFD failed, just close it!\n");
    }

	   return Success;
}

int32_t Socket::set_nodelay(int32_t fd){
    int32_t flags = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags)); //set TCP_CORK
    return Success;
}


}
/*
int main(){
    demo::Socket tcpsock("127.0.0.1",8888);
    return 0;

}
*/




