#ifndef __MSG_HEAD_H__
#define __MSG_HEAD_H__

#include <cstdint>

#define MESSAGE_HEAD_SIZE	( 3 * sizeof(int32_t) )
namespace demo{

class MsgHead{
public:
	MsgHead();
	~MsgHead();
	MsgHead(MsgHead& head);

	int32_t encode(char* target,int32_t& outLens);
	int32_t decode(char* target,const int32_t inLens);

	int32_t iLens;
	int32_t iMsgID;
	int32_t iPlayerID;
};






}




#endif
