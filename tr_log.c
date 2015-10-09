/*
 * tr_log.c
 *
 *  Created on: Oct 9, 2015 10:57:20 AM
 *      Author: jianxin.li
 */
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "tr_log.h"

#define DEFAULT_LIMIT 1*1024*1024
typedef enum{
	TO_BOTH,
	TO_SCREEN,
	TO_FILE,
	TO_NONE
}Mode;

typedef struct{
	FILE *fp;
	char file_name[128];

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
		0,
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
		log_conf.fp=fopen(log_conf.file_name,"a");
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
		strftime(date,sizeof(date)-1,"%b %e %T",tm);
		va_start(vp,fmt);
		vsnprintf(buf,sizeof(buf)-1,fmt,vp);
		va_end(vp);
		if(log_conf.mode==TO_SCREEN || log_conf.mode==TO_BOTH)
		{
			fprintf(stderr,"\033[37;40m[%s] %s \033[37m%s()@%s:%d => \033[0;37;40m%s\n\033[0m",date,log_descriptions[level].color,function,file,line,buf);
			fflush(stderr);
		}
		if(log_conf.fp!=NULL)
		{
			num=fprintf(log_conf.fp,"[%s] %s()@%s:line %d  %s\n",date,function,file,line,buf);
			if(num>0)
			{
				fflush(log_conf.fp);
				log_conf.current+=num;
				if(log_conf.current>=log_conf.limit)
				{
					fclose(log_conf.fp);
					if(log_conf.rotate==1){

					}else{
						tr_remove(log_conf.file_name);
					}
					start_log();
				}
			}
		}
	}
}
