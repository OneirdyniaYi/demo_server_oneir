#include "buffer.h"
#include "errorcode.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdio>


namespace demo{
Buffer::Buffer(){
	printf("11111111111111111");

}

Buffer::Buffer(int32_t size):m_head(0),
			     m_tail(0),
		    	     m_size(size){
	m_bufptr = new char[size];
	memset(m_bufptr,0,sizeof(char)*size);
}


Buffer::~Buffer(){
	if(m_bufptr != nullptr){
		delete[] m_bufptr;
		m_bufptr = nullptr;
	}
	m_head = 0;
	m_tail = 0;
    m_size = 0;
}

int32_t Buffer::recvbufWrite(int32_t sockfd){
	if(sockfd<0){
        return InvalidParam;
    }

    if(!dataSize()){
        m_tail = m_head = 0;
    }

    int32_t nread = 0;

    do{

        if(m_tail == m_size){
            if(m_head > 0){ //数据前移
                memmove(m_bufptr,m_bufptr+m_head,size_t(m_tail - m_head));
                m_tail -= m_head;
                m_head = 0;
            }else{  //buf满了
               printf("222222\n");
               return QueueFull;
            }
        }
        nread = recv(sockfd,m_bufptr+m_tail,size_t(m_size - m_tail),0);
        if(nread > 0){
            m_tail += nread;
        }else if(nread == 0){
            ::close(sockfd);
            return Success;
        }else if(EAGAIN != errno){//received_byte < 0 && EAGAIN != errno
            ::close(sockfd);
            return Failed;
        }

    }while(nread > 0);

    return Success;
}

int32_t Buffer::getOnePacket(char* data,size_t &size){


	if(data == nullptr) return ReAgain;

	int32_t bufdata_size = m_tail - m_head;
	printf("data size in buffer is %d\n",bufdata_size);
	if(bufdata_size <= 0) return ReAgain;

	if(bufdata_size < (int32_t)sizeof(int32_t)){ //小于表示长度的字节数
		if(m_tail == m_size){
			memcpy(m_bufptr,m_bufptr+m_head,size_t(bufdata_size));
			m_head = 0;
			m_tail = bufdata_size;
		}
		return ReAgain;
	}
	int32_t code_size = (int32_t) ntohl((uint32_t) (*(int32_t*)(m_bufptr+m_head)));//解析长度

	printf("code size is %d\n",code_size);
	if(code_size <= 0) return ReAgain;

	if(code_size >= common_buffer_size) return Failed;

    if (bufdata_size < code_size){
        //缓冲区不足可以读出的长度
        if (m_tail == m_size){
            memmove(m_bufptr, m_bufptr+m_head, size_t(bufdata_size));
            m_head = 0;
            m_tail = bufdata_size;
        }
        return ReAgain;
    }

    size = (size_t)code_size;
    memcpy(data,m_bufptr+m_head,size);
    m_head += code_size;

    if(!dataSize()){
    	m_tail = m_head = 0;
    }

    return Success;
}




}

