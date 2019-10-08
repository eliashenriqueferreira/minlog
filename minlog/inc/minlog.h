/*
 * minlog.h
 *
 *  Created on: 3 de out de 2019
 *      Author: Elias Henrique Ferreira
 */

#ifndef INC_MINLOG_H_
#define INC_MINLOG_H_
#ifdef __cplusplus
extern "C" {
#endif


#define MINLOG_LEVEL_TRACE      0
#define MINLOG_LEVEL_DEBUG      1
#define MINLOG_LEVEL_INFO       2
#define MINLOG_LEVEL_WARNING    3
#define MINLOG_LEVEL_ERROR      4
#define MINLOG_LEVEL_CRITICAL   5
#define MINLOG_LEVEL_OFF        6

#define MINLOG_TRACE(msg,...)    minlog(__FILE__, __LINE__, MINLOG_LEVEL_TRACE,    msg, 7, ##__VA_ARGS__, 6596, 6596, 6596, 6596, 6596, 6596, 6596);
#define MINLOG_DEBUG(msg,...)    minlog(__FILE__, __LINE__, MINLOG_LEVEL_DEBUG,    msg, 7, ##__VA_ARGS__, 6596, 6596, 6596, 6596, 6596, 6596, 6596);
#define MINLOG_INFO(msg,...)     minlog(__FILE__, __LINE__, MINLOG_LEVEL_INFO,     msg, 7, ##__VA_ARGS__, 6596, 6596, 6596, 6596, 6596, 6596, 6596);
#define MINLOG_WARNING(msg,...)  minlog(__FILE__, __LINE__, MINLOG_LEVEL_WARNING,  msg, 7, ##__VA_ARGS__, 6596, 6596, 6596, 6596, 6596, 6596, 6596);
#define MINLOG_ERROR(msg,...)    minlog(__FILE__, __LINE__, MINLOG_LEVEL_ERROR,    msg, 7, ##__VA_ARGS__, 6596, 6596, 6596, 6596, 6596, 6596, 6596);
#define MINLOG_CRITICAL(msg,...) minlog(__FILE__, __LINE__, MINLOG_LEVEL_CRITICAL, msg, 7, ##__VA_ARGS__, 6596, 6596, 6596, 6596, 6596, 6596, 6596);

#define MINLOG(msg,...) MINLOG_INFO(msg, ##__VA_ARGS__, 6596, 6596, 6596, 6596, 6596, 6596, 6596);

void *minlog_open(const char *argv_zero, int level, const char *pname_aux);
void minlog_close(void *);
int minlog(const char *psourcefile, int sourceline, int level, const char *pfmtmsg, int ctparam, ... );

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* INC_MINLOG_H_ */
