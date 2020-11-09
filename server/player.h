#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <cstdint>
#include "ProtoID.pb.h"
#include "Demo.pb.h"
#include "singleton.h"
#include "msghead.h"
#include "epoll.h"
#include "msghandle.h"
#include "errorcode.h"


namespace demo{
class Player{
public:
	Player(int32_t playerID = -1);
	~Player();

	int32_t get_id() const {return m_id;}

	int32_t handle_test(MsgHead &stHead, const char *body, const int32_t len);
	int32_t handle_test2(MsgHead &stHead, const char *body, const int32_t len);
private:
	int32_t m_id;
	void send_msg(int32_t cmd_id, google::protobuf::Message &msg);
	void broadcast(int32_t cmd_id, google::protobuf::Message &msg);
};

}



#endif
