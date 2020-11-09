#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>

#include "errorcode.h"
#include "epoll.h"

namespace demo{
	Epoll::Epoll(): m_epfd(-1),
					m_events(nullptr),
					m_size(0){

	}

	Epoll::~Epoll(){
		if(m_epfd != -1){
			if(::close(m_epfd) < 0){
				printf("close epoll error = %d, info = %s", errno, strerror(errno));
			}
			m_epfd = -1;
			if(m_events!=nullptr){
				delete[] m_events;
				m_events = nullptr;
			}
			m_size = 0;
		}
	}

	int32_t Epoll::init(int32_t size){
		if(size < 1){
			return InvalidParam;
		}
		

    	m_events = new struct epoll_event[size];
	memset(m_events,0,sizeof(struct epoll_event)*size);
    	if (NULL == m_events) {
        	return AllocErr;
    	}

		int32_t epfd = ::epoll_create(size);
		if(epfd < 0){
			printf("create epoll error = %d, info = %s", errno, strerror(errno));
			return FdCreatFail;
		}
		m_epfd = epfd;
		m_size = size;
		return Success;
	}

		

	int32_t Epoll::ableEvents(const int32_t timeout){
		int32_t fdnum = ::epoll_wait(m_epfd,m_events,m_size,timeout);
		if(fdnum < 0){
			printf("epoll wait error = %d, info = %s", errno, strerror(errno));
		}
		return fdnum;
	}

	int32_t Epoll::addEvent(int32_t fd){
		if(fd < 0){
			return InvalidParam;
		}
		struct epoll_event ev;
		memset(&ev, 0, sizeof(ev));
   		ev.events = EPOLLIN | EPOLLERR |EPOLLHUP;
    	ev.data.fd = fd;
		if(::epoll_ctl(m_epfd,EPOLL_CTL_ADD,fd,&ev) < 0){
			printf("add event to epoll error = %d, info = %s", errno, strerror(errno));
			return Failed;
		}
		return Success;
	}



}

