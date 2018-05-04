#ifndef PTLIB_H
#define PTLIB_H
/**
*    ����:akchenmail@163.com
*    Ŀ��:windowsƽ̨α�ն�lib���Զ������������
*
*/
//�������Ͷ���
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