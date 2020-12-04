#ifndef __LOGIN_LOGIC_H__
#define __LOGIN_LOGIC_H__

#include <cstdint>

#include "loginserver.h"

namespace demo
{

class LoginServer;

class LoginLogic
{
private:
    /* data */
    std::shared_ptr<LoginServer> m_server;
    int64_t m_id;
public:
    LoginLogic();
    LoginLogic(/* args */std::shared_ptr<LoginServer> ptr);
    ~LoginLogic();

    int32_t addrdeal(MsgHead &head,const char* body,const int32_t len);

    int32_t heartdeal(MsgHead &head,const char* body,const int32_t len);

    int32_t login_handle(MsgHead &head,const char* body,const int32_t len);

    int32_t regis_handle(MsgHead &head,const char* body,const int32_t len);

    void send_msg(bool isinner ,int32_t sendfd,int32_t cmd_id, google::protobuf::Message &msg);
};
   


} // namespace demo



#endif