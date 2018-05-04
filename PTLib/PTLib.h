#ifndef PTLIB_H
#define PTLIB_H
/**
*    作者:akchenmail@163.com
*    目的:windows平台伪终端lib，自定义输入输出流
*
*/
//数据类型定义
typedef void *PTHANDLE;
typedef int   PTSTATE;
typedef PTHANDLE PTSESSION;
typedef struct PTProcessNode_{
	PTHANDLE hProcess;
	PTHANDLE hStdInWrite;
	PTHANDLE hStdInRead;
	PTHANDLE hStdOutRead;
	PTHANDLE hStdOutWrite;
	struct PTProcessNode_* pNext;

}PTProcessNode;

//Open a session.
PTSESSION Open(char* command);
//Read from outputstream.
int       Read(PTSESSION session, char* buffer, int buffSize);
//Write into input stream.
int       Write(PTSESSION session, char* buffer,int length);
//Get Process State.
PTSTATE   State(PTSESSION session);
//Close Session.
PTSTATE   Close(PTSESSION session);

#endif