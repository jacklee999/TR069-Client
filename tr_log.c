/*
 * tr_log.c
 *
 *  Created on: Oct 9, 2015 10:57:20 AM
 *      Author: jianxin.li
 */
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "tr_log.h"
#include "tr_file.h"

#define DEFAULT_LIMIT 1*1024
typedef enum{
	TO_BOTH,
	TO_SCREEN,
	TO_FILE,
	TO_NONE
}Mode;

typedef struct{
	FILE *fp;
	char file_name[FILE_PATH_LEN];

	int rotate;//if rotate==1,auto rotate when the log file reach the limited size,or not rotate
	int backup;//the number of backup log files
	unsigned long limit;//the limit size of log file
	unsigned long current;//current size of log file

	int level;//only those whose level is not smaller than this level can be record
	Mode mode;//the log recorded mode
}LogConf;

static LogConf log_conf={
		NULL,
		"tr.log",
		1,
		5,
		DEFAULT_LIMIT,
		0,
		_DEBUG,
		TO_BOTH
};

struct {
	char color[32];
	char nocolor[16];
}log_descriptions[]={
		{"\033[1;32mDEBUG","DEBUG"},
		{"\033[1;34mNOTICE","NOTICE"},
		{"\033[1;33mWARNING","WARNING"},
		{"\033[1;31mERROR","ERROR"}
};
int start_log()
{
	int res=0;
	if(log_conf.mode==TO_BOTH || log_conf.mode==TO_FILE)
	{
		log_conf.fp=tr_fopen(log_conf.file_name,"a");
		if(log_conf.fp==NULL){
			log_conf.mode=TO_SCREEN;
			tr_log(WARNING,"open log file:%s failed!",log_conf.file_name);
			res=-1;
		}else{
			log_conf.current=ftell(log_conf.fp);
		}
	}
	return res;
}
int set_log_conf(const char *name, const char *value)
{
	if(strcmp(name,"LOG_FILE_NAME")==0)
	{
		strcpy(log_conf.file_name,value);
	}
	else if(strcmp(name,"LOG_ROTATE")==0)
	{
		if(strcmp(value,"1")==0||strcasecmp(value,"true")==0){
			log_conf.rotate=1;
		}else if(strcmp(value,"0")==0||strcasecmp(value,"false")==0){
			log_conf.rotate=0;
		}
	}
	else if(strcmp(name,"LOG_BACKUP")==0)
	{
		log_conf.backup=atoi(value);
		if(log_conf.backup<1){
			fprintf(stderr,"Invalid log_backup:%s!\n",value);
			log_conf.backup=1;
		}
	}
	else if(strcmp(name,"LOG_LIMIT")==0)
	{
		int num=atoi(value);
		if(num>0)
		{
			char *unit=value;
			while(*unit!='\0'&&isdigit(*unit))
				unit++;
			if(strcasecmp(unit,"kb")==0){
				log_conf.limit=num*1024;
			}else if(strcasecmp(unit,"mb")==0){
				log_conf.limit=num*1024*1024;
			}else if(strcasecmp(unit,"gb")==0){
				log_conf.limit=num*1024*1024;
			}else{
				fprintf(stderr,"Invalid unit:%s\n",unit);
			}
		}
	}
	else if(strcmp(name,"LOG_LEVEL")==0)
	{
		if(strcasecmp(value,"DEBUG")==0){
			log_conf.level=_DEBUG;
		}else if(strcasecmp(value,"NOTICE")==0){
			log_conf.level=_NOTICE;
		}else if(strcasecmp(value,"WARNING")==0){
			log_conf.level=_WARNING;
		}else if(strcasecmp(value,"ERROR")==0){
			log_conf.level=_ERROR;
		}
	}
	else if(strcmp(name,"LOG_MODE")==0)
	{
		if(strcasecmp(value,"TO_BOTH")==0){
			log_conf.mode=TO_BOTH;
		}else if(strcasecmp(value,"TO_SCREEN")==0){
			log_conf.mode=TO_SCREEN;
		}else if(strcasecmp(value,"TO_FILE")==0){
			log_conf.mode=TO_FILE;
		}else if(strcasecmp(value,"TO_NONE")==0){
			log_conf.mode=TO_NONE;
		}
	}
	return 0;
}
void tr_log(int level, const char *file, int line, const char *function, const char *fmt, ...)
{
	if(level>=log_conf.level && log_conf.mode !=TO_NONE)
	{
		char buf[1024];
		char date[64];
		struct tm *tm;
		va_list vp;
		int num;
		time_t current_time=time(NULL);
		tm=localtime(&current_time);
		//snprintf(date,strlen(date)-1,"%d-%d %d:%d:%d",tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		strftime(date,sizeof(date),"%b %e %T",tm);
		va_start(vp,fmt);
		vsnprintf(buf,sizeof(buf),fmt,vp);
		va_end(vp);
		if(log_conf.mode==TO_SCREEN || log_conf.mode==TO_BOTH)
		{
			fprintf(stderr,"\033[37;40m[%s] %s \033[37m%s()@%s:%d => \033[0;37;40m%s\n\033[0m",date,log_descriptions[level].color,function,file,line,buf);
			fflush(stderr);
		}
		if(log_conf.fp!=NULL)
		{
			num=fprintf(log_conf.fp,"[%s] %s %s()@%s:line %d  %s\n",date,log_descriptions[level].nocolor,function,file,line,buf);
			if(num>0)
			{
				fflush(log_conf.fp);
				log_conf.current+=num;
				if(log_conf.current>=log_conf.limit)
				{
					fclose(log_conf.fp);
					log_conf.fp=NULL;
					if(log_conf.rotate==1){
						int i;
						char old_name[FILE_PATH_LEN];
						char new_name[FILE_PATH_LEN];
						for(i=log_conf.backup;i>=1;i--)
						{
							snprintf(old_name,sizeof(old_name),"%s.bak.%d",log_conf.file_name,i);
							snprintf(new_name,sizeof(new_name),"%s.bak.%d",log_conf.file_name,i+1);
							if(tr_exist(old_name)==0){
								continue;
							}else{
								if(i==log_conf.backup){
									tr_remove(old_name);
									continue;
								}
								tr_rename(old_name,new_name);
							}
						}
						snprintf(new_name,sizeof(new_name),"%s.bak.1",log_conf.file_name);
						tr_rename(log_conf.file_name,new_name);
					}else{
						tr_remove(log_conf.file_name);
					}
					start_log();
				}
			}
		}
	}
}
