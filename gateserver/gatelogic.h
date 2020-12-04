#ifndef __GATE_LOGIC_H__
#define __GATE_LOGIC_H__

#include "gateserver.h"

namespace demo
{

class GateServer;

class GateLogic{
public:
    GateLogic();
    GateLogic(std::shared_ptr<GateServer> serverptr);
    ~GateLogic();

    int32_t session_handle(MsgHead &head,const char* body,const int32_t len);

    int32_t login_handle(MsgHead &head,const char* body,const int32_t len);


    void send_msg(bool isinner ,int32_t sendfd,int32_t cmd_id, google::protobuf::Message &msg);

    void send_all_msg(bool isinner ,int32_t sendfd,int32_t cmd_id, google::protobuf::Message &msg);
private:

    std::shared_ptr<GateServer> m_server;




};


} // namespace demo



#endif