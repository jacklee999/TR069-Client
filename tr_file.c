/*
 * tr_file.c
 *
 *  Created on: Oct 8, 2015 8:01:05 PM
 *      Author: jianxin.li
 */
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "tr_file.h"

extern char workspace[FILE_PATH_LEN];
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
	char current_directory[FILE_PATH_LEN];
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

void tr_full_path(const char *file, char *full_path,int size)
{
	if(judge_absolute_path(file)==1){
		snprintf(full_path,size,"%s",file);
	}else{
		snprintf(full_path,size,"%s%s",workspace,file);
	}
}

int tr_remove(const char *file)
{
	char full_path[FILE_PATH_LEN];
	tr_full_path(file,full_path,sizeof(full_path));
	return unlink(full_path);
}

FILE* tr_fopen(const char* file,const char *mode)
{
	char full_file_name[FILE_PATH_LEN];
	tr_full_path(file,full_file_name,sizeof(full_file_name));
	return fopen(full_file_name,mode);
}

int tr_exist(const char* file)
{
	FILE *fp;
	int res;
	fp=tr_fopen(file,"rb");
	if(fp!=NULL)
	{
		fclose(fp);
		res=1;
	}else{
		res=0;
	}
	return res;
}

int tr_backup(const char *file)
{
	char bak_file[FILE_PATH_LEN];
	snprintf(bak_file,sizeof(bak_file),"%s.bak",file);
	return rename(file,bak_file);
}


int tr_rename(const char *old_name, const char *new_name)
{
	if(old_name !=NULL)
	{
		char old_full_name[FILE_PATH_LEN];
		char new_full_name[FILE_PATH_LEN];
		tr_full_path(old_name,old_full_name,sizeof(old_full_name));
		tr_full_path(new_name,new_full_name,sizeof(new_full_name));
		tr_remove(new_full_name);
		return rename(old_full_name,new_full_name);
	}else{
		return -1;
	}
}
