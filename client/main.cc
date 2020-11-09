#include "client.h"
using namespace demo;
int main(int  argc,  char*  argv[]){
	if (argc < 4){
	    printf("Usage: %s ip port userid \n", argv[0]);
	    return 0;
    }
  


    if(!demo::DEMOCLIENT.init(argv[1],argv[2],argv[3])) {
        demo::DEMOCLIENT.working();
    }
    return 0;
}
