
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUFF_SIZE 1400
void error_handling(char* message);

int main(int argc,char* argv[])
{
   FILE* rp;
   int sock;
   struct sockaddr_in serv_addr;
   char buffer[BUFF_SIZE]={0,};
   int str_len;
   int recv_size;
 if(argc!=4)
{
 printf("Usage : %s <IP> <port><File name>\n",argv[0]);
 exit(1);
}

sock=socket(PF_INET, SOCK_STREAM,0);
if(sock==-1)
 error_handling("socket() error");

memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
serv_addr.sin_port=htons(atoi(argv[2]));

if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
  error_handling("connect() error!");
else
 puts("connected......");
while(1){
memset(buffer,0,sizeof(buffer));
recv_size=recv(sock,buffer,sizeof(buffer),0);
if(recv_size==0)
   break;
rp = fopen(argv[3],"w");
fwrite(buffer,recv_size,1,rp);
}
fclose(rp);
close(sock);

return 0;
}
void error_handling(char* message)
{
 fputs(message,stderr);
 fputc('\n',stderr);
 exit(1);
}


