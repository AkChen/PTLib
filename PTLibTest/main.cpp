#include "../PTLib/PTLib.h"
#include <stdio.h>
#include <Windows.h>
#include <string>
#ifdef _DEBUG
#pragma comment (lib,"../debug/PTLib.lib")
#else
#pragma comment (lib,"../RELEASE/PTLib.lib")
#endif
void main(){
	PTSESSION session = Open("ping www.cctv.com");	
	char buf[1024];
	int code;
	int read = 0;
	while (State(session)){
		int read = Read(session, buf, 1024);

		//Write(session, "1\r\n", strlen("1\r\n
		if (read > 0){
			buf[read] = '\0';
			printf(buf);
		}
	}
	//Sleep(5000);
	read = Read(session, buf, 1024);
	read = Read(session, buf, 1024);
	Close(session);
	printf("Exit\n");
	system("pause");   
}