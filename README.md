PTLib is a pseudo terminal library running on Windows platform, it can run multiple processes and manage them, PTLlib also redirect standard input and output structure of each process can be operated by PTLib interface. Of course, in theory, you can support Linux.

How to use.

long session = Open("ping www.github.com");
while(State(session)==STILL_ACIVE){
  unsigned char buffer[1024];
  int nRead = Read(session,buffer,1024);
  int nWrite = Write(session,buffer,nRead);
}
Close(session);
