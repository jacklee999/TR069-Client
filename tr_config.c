/*
 * tr_config.c
 *
 *  Created on: Oct 10, 2015 3:54:10 PM
 *      Author: jianxin.li
 */
#include <stdio.h>
#include <string.h>

#include "tr_log.h"
#include "tr_string.h"
#include "tr_file.h"
#include "main.h"
typedef struct{
	const char *config_name;
	int (*p_func)(const char *name,const char *value);
}ConfProc;
static ConfProc cps[]={
		{"LOG_FILE_NAME",set_log_conf},
		{"LOG_ROTATE",set_log_conf},
		{"LOG_BACKUP",set_log_conf},
		{"LOG_LIMIT",set_log_conf},
		{"LOG_LEVEL",set_log_conf},
		{"LOG_MODE",set_log_conf},
		{"DATAMODEL_FILE",set_datamodel_file}
};
static int parse_conf(const char *conf,char *name,char *value)
{
	int res;
	char buf[1024];
	char *ptr;
	conf=skip_blanks(conf);
	if(*conf=='#'||*conf=='\0'||*conf=='=')
	{
		res=-1;
	}
	else
	{
		strcpy(buf,conf);
		if((ptr=strchr(buf,'='))==NULL)
			return -1;
		*ptr='\0';
		strcpy(name,trim_blanks(buf));

		ptr++;
		ptr=skip_blanks(ptr);
		if(*ptr=='\0'|| *ptr=='#')
			return -1;
		char *tmp=NULL;
		if((tmp=strchr(ptr,'#'))!=NULL)
			*tmp='\0';
		strcpy(value,trim_blanks(ptr));
		res=0;
	}
	return res;
}
int read_config_file()
{
	char buf[1024];
	char name[64];
	char value[512];
	int i;
	FILE *fp=tr_fopen("tr.conf","r");
	if(fp==NULL)
	{
		tr_log(ERROR,"open file:tr.conf fail!");
		return -1;
	}
	while(fgets(buf,sizeof(buf),fp))
	{
		if(parse_conf(buf,name,value)<0)
			continue;
		for(i=0;i<sizeof(cps)/sizeof(cps[0]);i++)
		{
			if(strcasecmp(name,cps[i].config_name)==0)
			{
				cps[i].p_func(name,value);
				break;
			}
		}
	}
	return 0;
}
