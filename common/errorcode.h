#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__
enum EComm
{
	Failed = -1,
	Success = 0,                     
	
	InvalidParam = 1,                 
	FdCreatFail = 2,                    
	


	AllocErr = 3, 
	QueueFull = 4,                
	SystemCallErr = 5,                
	ReAgain = 6,

};
#endif
