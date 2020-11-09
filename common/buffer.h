#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cstdint>
#include <cstddef>


enum{
	common_buffer_size = 0x20000,
};

namespace demo{

	
class Buffer{
public:
	Buffer();
	Buffer(int32_t size);
	~Buffer();

	int32_t recvbufWrite(int32_t sockfd);
	int32_t getOnePacket(char* data,size_t &size);

	int32_t dataSize() const { return m_tail - m_head;}

private:

	char* m_bufptr;

	int32_t m_head;
	int32_t m_tail;

	int32_t m_size;

};





}


#endif
