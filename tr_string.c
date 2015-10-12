/*
 * tr_string.c
 *
 *  Created on: Oct 10, 2015 5:11:56 PM
 *      Author: jianxin.li
 */
#include <stdio.h>
#include <string.h>
char* skip_blanks(char *str)
{
	char *res=NULL;
	if(str==NULL){
		return res;
	}
	for(res=str;*res<33 && *res!='\0';res++)
		continue;
	return res;
}

char* trim_blanks(char *str)
{
	if(str!=NULL)
	{
		while(*str!='\0'&&*(str+strlen(str)-1)<33)
			*(str+strlen(str)-1)='\0';
	}
	return str;
}
