// user/pingpong.c
#include "kernel/types.h"
#include "user/user.h"
int exit_stat=0;
int main(int argc,char **argv)
{
	int pipefd1[2];
	int pipefd2[2];
	pipe(pipefd1);
	pipe(pipefd2);
	int pid=fork();
	if(pid<0){
		printf("fork failed");
		close(pipefd1[0]);
		close(pipefd2[1]);
		close(pipefd1[1]);
		close(pipefd2[0]);
		exit_stat=1;
		exit(exit_stat);
	}
	if(pid==0){
		char buffer='1';
		char signal='1';
		
		close(pipefd2[0]);
		if(read(pipefd1[0],&buffer,sizeof(buffer)!=sizeof(char))){
			printf("child process read error!\n");
			exit_stat=1;
		}
		else{
			printf("%d: received ping\n",getpid());
		}
		close(pipefd1[0]);
		if(write(pipefd2[1],&signal,sizeof(char))!=sizeof(char)){
			printf("child write error\n");
			exit_stat=1;
		}
		close(pipefd1[1]);
		close(pipefd2[1]);
		exit(exit_stat);
	}
	else{
		char signal='1';
		char buffer='1';
		close(pipefd1[0]);
		close(pipefd2[1]);
		if(write(pipefd1[1],&signal,sizeof(char))!=sizeof(char)){
			printf("parent write error\n");
			exit_stat=1;
		}
		close(pipefd1[1]);
		if(read(pipefd2[0],&buffer,sizeof(char))!=sizeof(char)){
			printf("parent read error\n");
			exit_stat=1;
		}else{
			printf("%d: received pong\n",getpid());
		}
		close(pipefd2[0]);
	}
	exit(exit_stat);
}
