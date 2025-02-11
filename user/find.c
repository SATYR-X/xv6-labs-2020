#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *path,const char *filename)
{
	char buf[512],*p;
	int fd;
	struct dirent de;
	struct stat st;
	if((fd=open(path,0))<0){
		printf("find: cannot open %s\n",path);
		return;
	}
	if(fstat(fd,&st)<0){
		printf("find: cannot fstat %s\n",path);
		close(fd);
		return;
	}
	if(st.type!=T_DIR){
		printf("usage: find <DIRECTORY> <filename>\n");
		return;
	}
	if(strlen(path)+1+DIRSIZ+1 > sizeof(buf)){
		printf("find: path too long\n");
		return;
	}
	strcpy(buf,path);
	p=buf + strlen(buf);
	*p++ = '/';
	while(read(fd,&de,sizeof(de))==sizeof(de)){
		if(de.inum==0)
			continue;
		memmove(p,de.name,DIRSIZ);
		p[DIRSIZ]=0;
		if(stat(buf,&st)<0){
			printf("find: cannot stat %s\n",buf);
			continue;
		}
		if(st.type==T_DIR&&strcmp(p,".")!=0&&strcmp(p,"..")!=0){
			find(buf,filename);
		}else if(strcmp(filename,p)==0)
			printf("%s\n",buf);
	}
	close(fd);
}
int main(int argc,char *argv[])
{
	if(argc!=3){
		printf("usage: find <directory> <filename>\n");
		exit(1);
	}
	find(argv[1],argv[2]);
	exit(0);
}
