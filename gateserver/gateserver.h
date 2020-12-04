#ifndef __GATE_SERVER_H__
#define __GATE_SERVER_H__

#include "baseserver.h"
#include "baseserver.cc"
#include "noncopyable.h"
#include "clientmoudle.h"
#include "gatelogic.h"

namespace demo
{

class GateLogic;

class GateServer:public noncopyable, public std::enable_shared_from_this<GateServer> {
public:
    GateServer();

    ~GateServer();

    void init(char const *ip,char const *port);

    void work();

	void send_msg(int32_t playerID,int32_t cmd_id, google::protobuf::Message &msg);

	void send_all_msg(int32_t playerID,int32_t cmd_id, google::protobuf::Message &msg);

    void update_auth_mp(int32_t playerid,std::pair<int32_t,int64_t> pairr);

    std::pair<int32_t,int64_t> get_mp(int32_t playerid);

    int32_t process_packet(const char *pszInCode, const int32_t iInCodeSize, int32_t socketfd,std::shared_ptr<MsgHandler<GateLogic>> msghandle);

    std::shared_ptr<ClientMoudle> get_game_client() const {return m_client_to_game;}

    std::shared_ptr<ClientMoudle> get_login_client() const {return m_client_to_login;}
private:
    std::shared_ptr<BaseServer<GateLogic,GateServer>> m_bserver;

    std::map<int32_t,std::pair<int32_t,int64_t>> m_id_auth_mp;

    std::shared_ptr<GateLogic> m_gate_logicptr;
    std::shared_ptr<ClientMoudle> m_client_to_login; 
    std::shared_ptr<ClientMoudle> m_client_to_game; 
    // std::shared_ptr<ClientMoudle> m_game_client;

    // std::shared_ptr<ClientMoudle> m_login_client;
    std::map<int32_t, int32_t> m_playermap;

};

#define GATESERVERI Singleton<GateServer>::Instance()


} // namespace demo


#endif