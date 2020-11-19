#include "msghead.h"
#include "errorcode.h"

#include <string.h>
#include <arpa/inet.h>
#include <cstdio>


namespace demo{

MsgHead::MsgHead():iLens(0),
       		   iMsgID(-1),
		   iPlayerID(0)	{

}

MsgHead::~MsgHead(){

}

MsgHead::MsgHead(MsgHead& head){
	iLens = head.iLens;
	iMsgID = head.iMsgID;
	iPlayerID = head.iPlayerID;
}

int32_t MsgHead::encode(char* target,int32_t& outLens){
	if(target == NULL) return Failed;
	outLens = 0;
	int32_t len = static_cast<int32_t>(sizeof(uint32_t));
	uint32_t res = htonl(static_cast<uint32_t>(iLens));
	memcpy(target,&res,sizeof(res));
	outLens += len;

	res = htonl(static_cast<uint32_t>(iMsgID));
	memcpy(target+outLens,&res,sizeof(res));
	outLens += len;

	res = htonl(static_cast<uint32_t>(iPlayerID));
	memcpy(target+outLens,&res,sizeof(res));
	outLens += len;
	
	return Success;
}

int32_t MsgHead::decode(char* target,const int32_t inLens){	
	if(target == NULL || inLens <= 0) return Failed;

	int32_t len = static_cast<int32_t>(sizeof(uint32_t));
	int32_t codelen = 0;

	iLens = ntohl(*(reinterpret_cast<uint32_t*>(target)));
	codelen += len;

	iMsgID = ntohl(*(reinterpret_cast<uint32_t*>(target+codelen)));
	codelen += len;

	iPlayerID = ntohl(*(reinterpret_cast<uint32_t*>(target+codelen)));
	codelen += len;

	if(codelen > inLens) return Failed;

	return Success;
}


}
/*
int main(){
	demo::MsgHead head;
	head.iLens = 100;
	head.iMsgID = 23;
	head.iPlayerID = 354;

	char res[100000];
	int32_t size = 0;

	head.encode(res,size);
	for(int32_t i=0;i<size;++i){
		printf("%d is %x ", i+1 , res[i]);
	}
	printf("\n");
	head.decode(res,size);
	printf("len is %d,msg is %d,if is %d",head.iLens,head.iMsgID,head.iPlayerID);





	return 0;
}

*/
