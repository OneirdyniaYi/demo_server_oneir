#include <unistd.h>
#include <functional>

#include "client.h"
#include "msghead.h"
#include "errorcode.h"

namespace demo{

Client::Client() {
    m_socket = new Socket();
    m_epoll = new Epoll();
    m_msgHandle = new MsgHandler<Client>();
}

Client::~Client() {
    delete m_socket;
    delete m_msgHandle;
    delete m_epoll;
}


int32_t Client::toIpAddr(char* ipstr,struct in_addr& ip){
    if(!ipstr || ::inet_pton(AF_INET,ipstr,&ip)<=0) return Failed;
    return Success;
}

int32_t Client::init(char const *ip,char const *port,char const* playerid) {
    if(toIpAddr(const_cast<char*>(ip),m_ip)){
        printf("ip fail\n");
        return Failed;
    }
    
    int32_t pport = ::atoi(const_cast<char*>(port));
    int32_t ppid = ::atoi(const_cast<char*>(playerid));

    m_playerid = ppid;
    if(pport>65535||pport<0){
        printf("port error\n");
        return Failed;
    }

    m_port = static_cast<uint16_t>(pport);

    int32_t ret = m_socket->open();
    if(ret){
        printf("socked creat failed \n");
        return Failed;
    }
    printf("sdfdsfskdfjlkj fd is %d\n",m_socket->get_fd()); 
    
    ret = m_socket->connect(m_ip,m_port);
    if(ret){
        printf("socked connect failed \n");
        return Failed;
    
    }
    
    int32_t flags = 1;
    struct linger ling = {0, 0};
    setsockopt(m_socket->get_fd(), SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
    setsockopt(m_socket->get_fd(), SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

    m_socket->set_nodelay(m_socket->get_fd());



    if(m_epoll->init(EPOLL_MAX_SIZE)){
        printf("epoll init failed\n");
        return Failed;
    }

    if(m_epoll->addEvent(m_socket->get_fd())){
        printf("epoll epoll add failed\n");
        return Failed;
    }


    if(m_epoll->addEvent(STDIN_FILENO)){
        printf("epoll epoll add failed\n");
        return Failed;
    }

    register_all_msg();

    return Success;
}

void Client::working() {
    while(1) {
        recv_msg();
    }
}

int32_t Client::recv_msg() {
    int32_t timeout = 1;
    int32_t fd_num = m_epoll->ableEvents(timeout);
    
    for (int32_t i = 0; i < fd_num; ++i) {
        int32_t socketfd =  m_epoll->get_event_fd(i);
        if(socketfd == m_socket->get_fd()){
            printf("now socketfd is %d\n",socketfd);			
            if(m_socket->freshBuf()){
                printf("read to buffer error\n");
                return Failed;
            }
            m_socket->process_packet(std::bind(&Client::process_packet, this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }else{
            del_io_msg();
        }

    }

    return 0;
}

int32_t Client::del_io_msg() {
    static char buffer[common_buffer_size];
    int ret = ::read(STDIN_FILENO,buffer,sizeof(buffer));
    std::string inputstr(buffer,ret);
    
    if(inputstr[0] == '1' && inputstr.size() == 2){
	send_msg_req_test1();
    }else if(inputstr[0] == '2' && inputstr.size() == 2){
        send_msg_req_test2();
    }else{
        send_msg_req_test3(inputstr);
    }
    return 0;
}

void Client::send_msg_req_test1() {
    ReqTest req;
    req.set_a(123);
    send_msg(m_playerid, DEMOID::REQ_TEST, req);
}

void Client::send_msg_req_test2() {
    ReqTest req;
    req.set_a(321);
    send_msg(m_playerid, DEMOID::REQ_TEST, req);
}

void Client::send_msg_req_test3(std::string &input) {
    ReqTest2 req;
    //req.set_aa(4567890);
    //req.set_msg(input);
    send_msg(m_playerid, DEMOID::REQ_TEST2, req);
}

void Client::send_msg(int32_t player_id, int32_t cmd_id, google::protobuf::Message &msg) {
    printf("now now socket fd is %d\n",m_socket->get_fd());
    static char data[common_buffer_size];
    MsgHead head;
    head.iMsgID = cmd_id;
    head.iLens = MESSAGE_HEAD_SIZE + msg.ByteSizeLong();
    head.iPlayerID = player_id;
	printf("head.iLens is %d\n",head.iLens);
    int32_t coded_length = 0;
    head.encode(data,coded_length);
    msg.SerializePartialToArray(data+coded_length,msg.ByteSizeLong());
    int32_t ret = m_socket->writen(data,(size_t)head.iLens);
    
    if(ret){
        printf("send msg (id=%d) error ret=%d,errno:%d ,strerror:%s,fd = %d\n",cmd_id,ret,errno, strerror(errno),m_socket->get_fd());
    }
    printf("send msg  fd:%d    msglen = %d\n",m_socket->get_fd(),head.iLens);
}

int32_t Client::process_packet(const char *pszInCode, const int32_t iInCodeSize,int32_t socketfd) {
    if (!pszInCode || iInCodeSize <= 0) {
        return Failed;
    }
    MsgHead stHead;
    char* p = const_cast<char*>(pszInCode);
    int32_t head_outLength = MESSAGE_HEAD_SIZE;
    if (stHead.decode(p, iInCodeSize)) {
        printf("[Client::%s]head decode error\n", __FUNCTION__);
        return Failed;
    }
    if (stHead.iLens != iInCodeSize) {
        printf("[Client::%s]the package size decoded from service was not equal with received size.\n", __FUNCTION__);
        return Failed;
    }

    int32_t iBodySize = stHead.iLens - head_outLength;

    printf("[network][Client::%s][player_id:%d][msg_id:%d]\n", __FUNCTION__, stHead.iPlayerID, stHead.iMsgID);

    auto func = m_msgHandle->get_func(stHead.iMsgID);
    if(NULL != func){
        (this->*func)(stHead,pszInCode + head_outLength,iBodySize);
    }
    return Success;
}

int32_t Client::handle_test(MsgHead &stHead, const char *body, const int32_t len) {
    RspTest rsp;
    rsp.ParseFromArray(body, len);
    printf("client recv msg  rsp.b:%d\n",rsp.b());
    return 0;
}


int32_t Client::handle_test2(MsgHead &stHead, const char *body, const int32_t len) {
    RspTest2 rsp;
    rsp.ParseFromArray(body, len);
    //printf("client recv msg  rsp.bb:%d , rsq.msg:%s\n",rsp.bb(),rsp.msg2().c_str());
    return 0;
}


void Client::register_all_msg() {
    //m_msgHandle->RegisterMsg(DEMOID::RSP_TEST, &Client::handle_test);
    //m_msgHandle->RegisterMsg(DEMOID::RSP_TEST2, &Client::handle_test2);
}


}
