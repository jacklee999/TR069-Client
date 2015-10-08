/*
 * tr_file.c
 *
 *  Created on: Oct 8, 2015 8:01:05 PM
 *      Author: jianxin.li
 */
#include <string.h>
#include <stdio.h>
int judge_absolute_path(const char* path)
{
	return *path=="/"?1:0;
}
int int_arg(int argc,char **argv,char *P_workspace,int size)
{
	int res=0;
	if(argc>=3 && (strcasecmp(argv[1],"-d")==0 || strcasecmp(argv[1],"--directory")==0))
	{
		if(judge_absolute_path(argv[2])==1){
			if(argv[2][strlen(argv[2]-1)]=="/"){
				snprintf(P_workspace,"%s",argv[2]);
			}
		}else{

		}
	}else if(argc!=1){
		res=-1;
		usage();
	}
	return res;
}
