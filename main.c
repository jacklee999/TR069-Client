/*
 * main.c
 *
 *  Created on: Oct 8, 2015 7:43:16 PM
 *      Author: jianxin.li
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tr_file.h"
#include "tr_log.h"
#include "tr_config.h"
#include "tr_lib.h"


char workspace[FILE_PATH_LEN]="/root/TestWorkspace/TR069_Client/";
static char datamodel_file[16];
int set_datamodel_file(const char *name,const char *file)
{
	int res=-1;
	if(file!=NULL)
	{
		snprintf(datamodel_file,sizeof(datamodel_file),"%s",file);
		res=0;
	}
	return res;
}
int main(int argc,char *argv[])
{
	if(init_arg(argc,argv,workspace,sizeof(workspace))!=0)
		exit(1);
	if(read_config_file()<0)
		exit(1);
	start_log();
	if(lib_init(datamodel_file)<0){
		tr_log(ERROR,"initial datamodel_file:%s failed!",datamodel_file);
		exit(1);
	}

	tr_log(DEBUG,"hello debug");
	tr_log(WARNING,"hello warning");
	tr_log(NOTICE,"hello notice");
	tr_log(ERROR,"hello error");
	return 0;
}
