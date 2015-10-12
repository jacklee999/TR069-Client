/*
 * tr_log.h
 *
 *  Created on: Oct 9, 2015 10:49:00 AM
 *      Author: jianxin.li
 */

#ifndef TR_LOG_H_
#define TR_LOG_H_

#define _A_ __FILE__, __LINE__, __FUNCTION__

#ifdef DEBUG
#undef DEBUG
#endif
#define _DEBUG 0
#define DEBUG _DEBUG, _A_

#ifdef NOTICE
#undef NOTICE
#endif
#define _NOTICE 1
#define NOTICE _NOTICE, _A_

#ifdef WARNING
#undef WARNING
#endif
#define _WARNING 2
#define WARNING _WARNING, _A_

#ifdef ERROR
#undef ERROR
#endif
#define _ERROR 3
#define ERROR _ERROR, _A_

int start_log();
int set_log_conf(const char *name, const char *value);
void tr_log(int level, const char *file, int line, const char *function, const char *fmt, ...);
#endif /* TR_LOG_H_ */
