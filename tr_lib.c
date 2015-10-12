/*
 * tr_lib.c
 *
 *  Created on: Oct 12, 2015 11:00:01 AM
 *      Author: jianxin.li
 */
#include <stdio.h>

typedef struct xml_node Node;
typedef struct xml_node* p_Node;
struct xml_node{
	char name[64];
	unsigned int rw;
	unsigned int getc;
	unsigned int noc;
	unsigned int nocc;
	unsigned int acl;
	unsigned int nin;
	unsigned int il;
	char type[32];
	char value[256];
	Node *parent;
	Node *child;
	Node *brother;
};

static char xml_file[16];
static Node *root=NULL;

static p_Node xml2tree(const char *file)
{
	p_Node ptr=NULL;

	return ptr;
}
int lib_init(const char *arg)
{
	snprintf(xml_file,sizeof(xml_file),"%s",arg);
	if((root=xml2tree(xml_file)!=NULL))
		return 0;
	else
		return -1;
}
