/*
 * minlog.h
 *
 *  Created on: 3 de out de 2019
 *      Author: Elias Henrique Ferreira
 */

#ifndef INC_MINLOG_H_
#define INC_MINLOG_H_

#ifdef _WIN32
#include <Windows.h>
typedef uint64_t __uint64_t;
typedef int pid_t;
#define usleep(us)  Sleep(us/1000+1)
#define SPRINTF sprintf_s               // int sprintf_s(char *buffer,size_t sizeOfBuffer,const char *format,...);      // Windows
#define STRCAT(d,s,n)   strcat_s(d,n,s) // errno_t strcat_s(char *strDestination, size_t numberOfElements, const char *strSource);
#define STRCPY(d,s,n)   strcpy_s(d,n,s) // errno_t strcpy_s(   char *dest,   rsize_t dest_size,   const char *src);
#define vsnprintf   vsprintf_s          // int vsprintf_s(char *buffer, size_t numberOfElements, const char *format, va_list argptr);       // Windows  // int vsnprintf(char* str, size_t size, const char* format, va_list ap);           // Linux
#define GMTIME(s,d) gmtime_s(d, s)      //errno_t    gmtime_s(struct tm* tmDest, const __time_t * sourceTime);      // Windows
#define getpid	GetCurrentProcessId		// DWORD GetCurrentProcessId();
#else
#include <unistd.h>  // for usleep
#define SPRINTF snprintf                //int snprintf(char *str, size_t size, const char *format, ...);
#define STRCAT(d,s,n)   strncat(d,s,n)  //char *strncat(char *dest, const char *src, size_t n);
#define STRCPY(d,s,n)   strncpy(d,s,n)  //char *strncpy(char *dest, const char *src, size_t n);
#define GMTIME(s,d)     gmtime_r(s,d)   //struct tm* gmtime_r(const time_t * timep, struct tm* result);             // Linux
#endif




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

#ifdef _WIN32
#define MINLOG(msg, ...) MINLOG_INFO(msg, __VA_ARGS__);
#else
#define MINLOG(msg,...) MINLOG_INFO(msg, ##__VA_ARGS__,6596,6596,6596,6596,6596,6596,6596);
#endif

void *minlog_open(const char *argv_zero, int level, const char *pname_aux);
void minlog_close(void *);
int minlog(const char *psourcefile, int sourceline, int level, const char *pfmtmsg, int ctparam, ... );
int mindump(__uint64_t address, int size);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* INC_MINLOG_H_ */
