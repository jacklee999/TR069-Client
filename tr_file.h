/*
 * tr_file.h
 *
 *  Created on: Oct 8, 2015 8:03:12 PM
 *      Author: jianxin.li
 */

#ifndef TR_FILE_H_
#define TR_FILE_H_

#define FILE_PATH_LEN 128
/*
 * judge whether the path is absolute path
 * return 1 when yes,or return 0
 * */
int judge_absolute_path(const char* path);
/*
 * initial workspace
 * */
int init_arg(int argc,char **argv,char *P_workspace,int size);

/*
 * calculate the full path
 * if file is absolute path,save it in full_path;or it will be a file under workspace
 * param file:the file name
 * param full_path:the buffer to hold full path
 * param size:the full path buff size
 * */
void tr_full_path(const char *file, char *full_path,int size);

/*
 * delete the file
 * */
int tr_remove(const char *file);
/*
 * if file is exist,return 1;or return 0
 * */
int tr_exist(const char* file);

int tr_backup(const char *file);
FILE* tr_fopen(const char* file,const char *mode);
int tr_rename(const char *old_name, const char *new_name);
#endif /* TR_FILE_H_ */
