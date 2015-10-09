/*
 * tr_file.c
 *
 *  Created on: Oct 8, 2015 8:01:05 PM
 *      Author: jianxin.li
 */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
int judge_absolute_path(const char* path)
{
	return *path=='/'?1:0;
}
static void usage(char *command)
{
	fprintf(stderr,"Usage:\n"
			"\t%s [-d|--directory directoryname]\n",command);
}
int init_arg(int argc,char **argv,char *P_workspace,int size)
{
	int res=0;
	char current_directory[64];
	getcwd(current_directory,sizeof(current_directory));
	if(argc>=3 && (strcasecmp(argv[1],"-d")==0 || strcasecmp(argv[1],"--directory")==0))
	{
		if(judge_absolute_path(argv[2])==1){
			if(argv[2][strlen(argv[2])-1]=='/'){
				snprintf(P_workspace,size,"%s",argv[2]);
			}else{
				snprintf(P_workspace,size,"%s/",argv[2]);
			}
		}else{
			if(argv[2][strlen(argv[2])-1]=='/'){
				snprintf(P_workspace,size,"%s/%s",current_directory,argv[2]);
			}else{
				snprintf(P_workspace,size,"%s/%s/",current_directory,argv[2]);
			}
		}
	}else if(argc!=1){
		res=-1;
		usage(argv[0]);
	}
	return res;
}
