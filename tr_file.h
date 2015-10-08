/*
 * tr_file.h
 *
 *  Created on: Oct 8, 2015 8:03:12 PM
 *      Author: jianxin.li
 */

#ifndef TR_FILE_H_
#define TR_FILE_H_

/*
 * judge whether the path is absolute path
 * return 1 when yes,or return 0
 * */
int judge_absolute_path(const char* path);
int int_arg(int argc,char **argv,char *P_workspace,int size);

#endif /* TR_FILE_H_ */
