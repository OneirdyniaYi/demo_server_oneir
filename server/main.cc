#include <cstring>
#include <stdint.h>
#include <signal.h>
#include <cstdio>
#include <map>

#include "server.h"
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
    if(!DEMOSERVER.init(p,q)){
        DEMOSERVER.work();
    }
    return 0;
}
