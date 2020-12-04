#include <cstring>
#include <stdint.h>
#include <signal.h>
#include <cstdio>
#include <map>
#include <memory>


#include "gateserver.h"

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
    std::shared_ptr<GateServer> serverptr = std::make_shared<GateServer>();
    serverptr->init(p,q);
    serverptr->work();
    return 0;
}