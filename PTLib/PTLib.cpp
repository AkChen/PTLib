#include "PTLib.h"
#include <Windows.h>
#include <stdio.h>
PTProcessNode* pProcessList;


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		pProcessList = (PTProcessNode*)malloc(sizeof(PTProcessNode));//初始化链表头
		ZeroMemory(pProcessList, sizeof(PTProcessNode));
		break;
	}
	return TRUE;
}

void AddProcessNode(PTProcessNode* node){
	PTProcessNode* p;
	for (p = pProcessList; p->pNext != NULL; p = p->pNext){}
	p->pNext = node;
	node->pNext = NULL;
}
PTSTATE RemoveProcessNode(PTProcessNode* node){
	PTProcessNode* pre,*cur;
	for (pre = pProcessList, cur = pProcessList->pNext; cur != NULL && cur != node; pre = pre->pNext, cur = cur->pNext){}
	if (cur == NULL)
		return 0;
	pre->pNext = cur->pNext;
	free(node);
	return 1;
}


PTSESSION Open(char* command){
	
	PTProcessNode* process = (PTProcessNode*)malloc(sizeof(PTProcessNode));
	if (!process)
		return NULL;
	SECURITY_ATTRIBUTES saAttr = {0};
	PROCESS_INFORMATION piProcInfo = {0};
	STARTUPINFOA siStartInfo = {0};
	HANDLE hStdInWriteTmp, hStdOutReadTmp,hStdErrWrite;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	//create pipes
	if (!CreatePipe(&process->hStdInRead, &hStdInWriteTmp, &saAttr, 1024))
		return NULL;
	if (!CreatePipe(&hStdOutReadTmp, &process->hStdOutWrite, &saAttr,1024))
		return NULL;
	if (!DuplicateHandle(GetCurrentProcess(), hStdInWriteTmp, GetCurrentProcess(), &process->hStdInWrite, 0, TRUE, DUPLICATE_SAME_ACCESS))
		return NULL;
	if (!DuplicateHandle(GetCurrentProcess(), hStdOutReadTmp, GetCurrentProcess(), &process->hStdOutRead, 0, TRUE, DUPLICATE_SAME_ACCESS))
		return NULL;
	

	CloseHandle(hStdOutReadTmp); hStdOutReadTmp = NULL;
	CloseHandle(hStdInWriteTmp); hStdInWriteTmp = NULL;

	//StartUpInfo
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.dwFlags = STARTF_USESTDHANDLES |STARTF_USESHOWWINDOW;;
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.hStdInput = process->hStdInRead;
	siStartInfo.hStdOutput = process->hStdOutWrite;
	siStartInfo.hStdError = process->hStdOutWrite;
	
	//CreateProcess
	BOOL  bSuccess = CreateProcessA(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo);
	if (!bSuccess)
		return NULL;
	process->hProcess = piProcInfo.hProcess;
	//Add node
	AddProcessNode(process);

	WaitForInputIdle(process->hProcess, 10 * 1000);

	return process;
}

int Write(PTSESSION session, char* buffer, int length){
	PTProcessNode* node = (PTProcessNode*)session;
	
	DWORD dwWritten = 0;
	WriteFile(node->hStdInWrite, buffer, length, &dwWritten, 0);
	return dwWritten;
}
int Read(PTSESSION session, char* bufer, int bufferSize){
	PTProcessNode* node = (PTProcessNode*)session;
	DWORD dwRead = 0;
	DWORD dwAvail = 0;
	BOOL bSuccess = FALSE;
	
	//printf("ReadFunction\n");
	bSuccess = PeekNamedPipe(node->hStdOutRead, NULL, NULL, NULL, &dwAvail, NULL);
	//printf("ReadFunction:%d\n",dwAvail);
	if (!bSuccess)
		return 0;
    if (dwAvail == 0)
		return 0;
	bSuccess = ReadFile(node->hStdOutRead, bufer, bufferSize, &dwRead, NULL);
	if (!bSuccess)
		return 0;

	return dwRead;
}

PTSTATE State(PTSESSION session){
	PTProcessNode* node = (PTProcessNode*)session;
	DWORD code = 0;
	BOOL bSuccess = GetExitCodeProcess(node->hProcess, &code);
	return code;
}

PTSTATE   Close(PTSESSION session){
	PTProcessNode* node = ((PTProcessNode*)session);
	PTSTATE a = TerminateProcess(((PTProcessNode*)session)->hProcess, 0);
	CloseHandle(node->hStdInWrite);
	CloseHandle(node->hStdInRead);
	CloseHandle(node->hStdOutWrite);
	CloseHandle(node->hStdOutRead);
	PTSTATE b = RemoveProcessNode((PTProcessNode*)session);
	return a && b;
}