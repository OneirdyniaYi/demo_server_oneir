#ifndef __SERVER_H__
#define __SERVER_H__

#define EPOLL_MAX_SIZE 128

#include <unordered_map>
#include <string>

#include "player.h"
#include "noncopyable.h"
#include "ProtoID.pb.h"
#include "Demo.pb.h"
#include "socket.h"
#include "msghandle.h"
#include "epoll.h"
#include "singleton.h"

namespace demo{

struct PlayerInfo{
    int32_t fd;
    Player* player;
    //PlayerInfo(int32_t sockfd,Player* p):fd(sockfd),player(p){}
};

class Server : noncopyable{
public:
	Server();
	~Server();

	int32_t toIpAddr(char* ipstr,struct in_addr& ip);

	int32_t init(char const *ip,char const *port);

	int32_t recv_msg();

	void work();

	void send_msg(int32_t playerID,int32_t cmd_id, google::protobuf::Message &msg);

	void send_all_msg(int32_t cmd_id, const char* body,const int32_t len);


private:
	Socket* m_socket;
	
	struct in_addr m_ip;

	uint16_t m_port;	

	std::unordered_map<int32_t, Socket*> m_mp;

	std::unordered_map<int32_t, PlayerInfo> map_players;
	
	Epoll* m_epoll;

	MsgHandler<Player>* m_msgHandle = nullptr;

	void register_all_msg();

	int32_t process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd);
};
#define DEMOSERVER Singleton<Server>::Instance()

}

#endif
