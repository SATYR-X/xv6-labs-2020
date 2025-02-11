#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1
const uint INT_LEN = sizeof(int);

int lpipe_first_data(int l[2],int *dst)
{
	if(read(l[RD],dst,sizeof(int))==sizeof(int)){
		printf("prime %d\n",*dst);
		return 0;
	}
	return -1;
}

void transmit_data(int l[2],int r[2],int first)
{
	int data;
	while(read(l[RD],&data,sizeof(int))==sizeof(int)){
		if(data%first){
			write(r[WR],&data,sizeof(int));
		}
	}
	close(l[RD]);
	close(r[WR]);
}

void primes(int l[2])
{
	close(l[WR]);
	int first;
	if(lpipe_first_data(l,&first)==0){
		int p[2];
		pipe(p);
		transmit_data(l,p,first);
		if(fork()==0){
			primes(p);
		}
		else{
			close(p[RD]);
			wait(0);
		}
	}
	exit(0);
}
int main(int argc,char const *argv[])
{
	int p[2];
	pipe(p);
	for(int i=2;i<=35;i++)
	{
		write(p[WR],&i,INT_LEN);
	}
	if(fork()==0){
		primes(p);
	}
	else{
		close(p[WR]);
		close(p[RD]);
		wait(0);
	}
	exit(0);
}
