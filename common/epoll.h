#ifndef __EPOLL_H__
#define __EPOLL_H__

#include <cstdint>
#include <sys/epoll.h>
#include "noncopyable.h"

namespace demo{
class Epoll : noncopyable {
public:
	Epoll();
	~Epoll();

	int32_t init(int32_t listenCount);
	int32_t ableEvents(const int32_t timeout);
	int32_t addEvent(int32_t fd);
	int32_t get_event_fd(int32_t i) const {return m_events[i].data.fd;};
private:
	int32_t m_epfd;
	struct epoll_event* m_events;
	int32_t m_size;
};


}

#endif
