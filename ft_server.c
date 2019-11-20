#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 1400
void error_handling(char* message);

int main(int argc,char *argv[])
{
  FILE* fp;
  int read_size;
  int serv_sock;
  int clnt_sock;
  int str_len,i;
  char buffer[BUFF_SIZE]={0,};
 struct sockaddr_in serv_addr;
 struct sockaddr_in clnt_addr;
 socklen_t clnt_adr_sz;

 if(argc!=3)
{
 printf("Usage : %s <port><File name>\n",argv[0]);
 exit(1);
}

serv_sock=socket(PF_INET,SOCK_STREAM,0);
if(serv_sock==-1)
  error_handling("socket() error");

memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
serv_addr.sin_port=htons(atoi(argv[1]));

if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
error_handling("bind() error");

if(listen(serv_sock,5)==-1)
error_handling("listen() error");

clnt_adr_sz=sizeof(clnt_addr);

 fp= fopen(argv[2],"r");
if(fp==NULL){
 printf("fail _ exit\n");
 exit(1);
}

clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
if(clnt_sock==-1)
    error_handling("accept() error");
while(feof(fp)==0){
memset(buffer,0,sizeof(buffer));
read_size=fread(buffer ,sizeof(char),5,fp);
if(read_size==0)
  break;
send(clnt_sock,buffer,read_size,0);
}
fclose(fp);
close(clnt_sock);

close(serv_sock);
return 0;
}

void error_handling(char* message)
{
 fputs(message,stderr);
 fputc('\n',stderr);
 exit(1);
}


