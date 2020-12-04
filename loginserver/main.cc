#include <cstring>
#include <stdint.h>
#include <signal.h>
#include <cstdio>
#include <map>
#include <thread>
#include <memory>


#include "loginserver.h"

using namespace demo;

int main(int argc, char** argv){
    char const *p;
    char const *q;
    if(argc<3){
	p = "127.0.0.1";
	q = "8888";
    }else{
    	p = argv[1];
	    q = argv[2];
    }
    std::shared_ptr<LoginServer> serverptr = std::make_shared<LoginServer>();
    std::unique_ptr<std::thread> pthread;
    serverptr->init(p,q,p,"8889");
    pthread.reset(new std::thread(std::bind(&LoginServer::inner_work, serverptr)));
    serverptr->work();
    pthread->join();
    return 0;
}