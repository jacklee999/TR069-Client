/*
 * main.c
 *
 *  Created on: Oct 8, 2015 7:43:16 PM
 *      Author: jianxin.li
 */
#include <stdlib.h>
#include <stdio.h>

#include "tr_file.h"
char workspace[128];
int main(int argc,char *argv[])
{
	if(init_arg(argc,argv,workspace,sizeof(workspace))!=0)
		exit(1);

	return 0;
}
