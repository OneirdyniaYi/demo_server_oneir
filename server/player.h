#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <cstdint>
#include <chrono> 
#include "ProtoID.pb.h"
#include "Demo.pb.h"
#include "singleton.h"
#include "msghead.h"
#include "epoll.h"
#include "msghandle.h"
#include "errorcode.h"
#include "backpack.h"

#define serverinternal 0.02

namespace demo{
class Player{
public:
	Player(int32_t playerID = -1);
	~Player();

	int32_t get_id() const {return m_id;}

	int32_t handle_test(MsgHead &stHead, const char *body, const int32_t len);
	int32_t handle_test2(MsgHead &stHead, const char *body, const int32_t len);

	int32_t move_test(MsgHead &head,const char* body,const int32_t len);
	int32_t regis_test(MsgHead &head,const char* body,const int32_t len);

	int32_t item_test(MsgHead &head,const char* body,const int32_t len);

	uint64_t get_time(){
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
private:
	int32_t m_id;
	int32_t m_dir[3] = {};
	int32_t m_face[3] = {};
	int32_t m_tar_dir[3] = {};
	int32_t m_run;
	int64_t m_clitime;
	uint64_t m_sertime;

	Backpack *m_bag;

	void send_msg(int32_t cmd_id, google::protobuf::Message &msg);
	void broadcast(int32_t cmd_id, google::protobuf::Message &msg);
};

}



#endif
