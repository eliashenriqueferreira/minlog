/*
 * minlog.c
 *
 *  Created on: 3 de out de 2019
 *      Author: Elias Henrique Ferreira
 *      Minimal Log
 */



#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>     // for memset

#include "minlog.h"

#ifdef __GNUC__
//#pragma GCC diagnostic [warning|error|ignored] OPTION
#pragma GCC diagnostic warning "-Wformat=0"
#endif

int minlogfile(char *filepath, char *buffer);

// Levels of Identification - Trace, Debug, Info, Warning, Error, Critical, Off
char minlog_level_char[] = {'T','D','I','W','E','C','O'};

char charset[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };


typedef struct minlog_st
{
    int level;
    int timestamp_mode;
		time_t start_timestamp_seconds;
    char log_file_path[FILENAME_MAX];   // 4096 for linux
		char aux_buffer[1024];				// Aux Buffer to be used by thread.
} minlog_t;
minlog_t *pminlog = NULL;

unsigned int unique_log_id = 0;


char* GetEnv(char *name)
{
#ifdef _WIN32
//	errno_t getenv_s(size_t * pReturnValue, char* buffer, size_t numberOfElements, const char* varname);
//Parameters:
//	pReturnValue - The buffer size that's required, or 0 if the variable is not found.
//	buffer - Buffer to store the value of the environment variable.
//	numberOfElements - Size of buffer.
//	varname - Environment variable name.
	size_t pReturnValue;
	getenv_s(&pReturnValue, pminlog->aux_buffer, sizeof(pminlog->aux_buffer), name);
	if (pReturnValue > 0 && pReturnValue < sizeof(pminlog->aux_buffer))
	{
		return pminlog->aux_buffer;	// TODO - There is something wrong here - // BUG BUG BUG
	}
	return NULL;
#else
	return getenv(name);
#endif
}

#if ( _MSC_VER < 1900 )
#ifndef _CRT_NO_TIME_T
    struct timespec
    {
        time_t tv_sec;  // Seconds - >= 0
        long   tv_nsec; // Nanoseconds - [0, 999999999]
    };
#endif
char* getTimeStamp(char* pbuf, size_t buflen)
{
    struct timespec tms;
    char buf_aux[40];
    time_t rawtime;
    struct tm tmGMT;
	  SYSTEMTIME st;

    time (&rawtime);
	  tms.tv_sec = rawtime;
	  tms.tv_nsec = 0;

    if (pminlog->timestamp_mode == MINLOG_TIMESTAMP_GMT) 
    {
        GMTIME(&rawtime, &tmGMT);
        //errno_t    gmtime_s(struct tm* tmDest, const __time_t * sourceTime);		// Windows
        //struct tm* gmtime_r(const time_t * timep, struct tm* result);				// Linux
        strftime(buf_aux, 40, "[GMT %Y-%m-%d %H:%M:%S.%%.3ld]", &tmGMT);	// Build %.3ld from the format part %%.3ld
		    GetSystemTime(&st);		// struct tm does not have miliseconds
		    SPRINTF(pbuf, buflen, buf_aux, st.wMilliseconds);
    }
    else if (pminlog->timestamp_mode == MINLOG_TIMESTAMP_LOCAL)
    {
        LOCALTIME(&rawtime, &tmGMT);
        strftime(buf_aux, 40, "[LOC %Y-%m-%d %H:%M:%S.%%.3ld]", &tmGMT);	// Build %.3ld from the format part %%.3ld
        GetSystemTime(&st);		// struct tm does not have miliseconds
        SPRINTF(pbuf, buflen, buf_aux, st.wMilliseconds);
    }
    else if (pminlog->timestamp_mode == MINLOG_TIMESTAMP_LITTLE)
    {
        LOCALTIME(&rawtime, &tmGMT);
        strftime(buf_aux, 40, "[%m%d %H:%M:%S.%%.2ld]", &tmGMT);	
        GetSystemTime(&st);		// struct tm does not have miliseconds
        SPRINTF(pbuf, buflen, buf_aux, st.wMilliseconds / 10);
    }
    else
    {
		    SPRINTF(buf_aux, sizeof(buf_aux), "[UPTIME:%.12ld.%%.3ld]", tms.tv_sec - pminlog->start_timestamp_seconds);
		    GetSystemTime(&st);
		    SPRINTF(pbuf, buflen, buf_aux, st.wMilliseconds);
    }

    return pbuf;
}
#else
char* getTimeStamp(char* pbuf, size_t buflen)
{
    struct timespec tms;
    char buf_aux[40];
    //time_t rawtime;
    struct tm tmGMT;

    //time(&rawtime);


    /* The C11 way */
    if (!timespec_get(&tms, TIME_UTC))
    {
        SPRINTF(pbuf, buflen, "[timespec_get ERROR]");
        return pbuf;
    }

    ///* POSIX.1-2008 way */
    //if (clock_gettime(CLOCK_REALTIME,&tms)) {
    //    sprintf(pbuf, "[clock_gettime ERROR]");
    //    return pbuf;
    //}
    //time (&rawtime);


    if (pminlog->timestamp_mode == MINLOG_TIMESTAMP_GMT) 
    {
        GMTIME(&tms.tv_sec, &tmGMT);
        //errno_t    gmtime_s(struct tm* tmDest, const __time_t * sourceTime);		// Windows
        //struct tm* gmtime_r(const time_t * timep, struct tm* result);				// Linux
        // %F  Short YYYY-MM-DD date, equivalent to %Y-%m-%d     2001-08-23
        // %T ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S 14:55:02
        strftime(buf_aux, 40, "[GMT %F %T.%%.3ld]", &tmGMT);
        SPRINTF(pbuf, buflen, buf_aux, tms.tv_nsec / 1000000);
    }
    else if (pminlog->timestamp_mode == MINLOG_TIMESTAMP_LOCAL)
    {
        LOCALTIME(&tms.tv_sec, &tmGMT);
        strftime(buf_aux, 40, "[LOC %F %T.%%.3ld]", &tmGMT);
        SPRINTF(pbuf, buflen, buf_aux, tms.tv_nsec / 1000000);
    }
    else
    {
        SYSTEMTIME st;
        SPRINTF(buf_aux, sizeof(buf_aux), "[UPTIME:%.12ld.%%.3ld]", (long)(tms.tv_sec - pminlog->start_timestamp_seconds));
        GetSystemTime(&st);
        SPRINTF(pbuf, buflen, buf_aux, st.wMilliseconds);
    }

    return pbuf;
}

#endif

void presentation()
{
    char ch = ' ';
    char line_init[] = "...................................................... MINLOG PRINTABLE CHARS .......................................................................";
    char line_fina[] = ".....................................................................................................................................................";
    char line_char[128];
    char line_nibleH[128];
    char line_nibleL[128];
    int i = 0;

    // 126      176    07E   01111110        ~    (tilde)
    while (ch <= '~') {
        line_char[i] = ch;
        line_nibleH[i] = charset[ch / 16];
        line_nibleL[i] = charset[ch % 16];
        i++;
        ch++;
    }
    line_char[i] = '\0';
    line_nibleH[i] = '\0';
    line_nibleL[i] = '\0';


    if (pminlog->log_file_path[0])
    {
        minlogfile(pminlog->log_file_path, line_init);
        minlogfile(pminlog->log_file_path, line_char);
        minlogfile(pminlog->log_file_path, line_nibleH);
        minlogfile(pminlog->log_file_path, line_nibleL);
        minlogfile(pminlog->log_file_path, line_fina);
    }
    else
    {
        printf("%s\n", line_init);
        printf("%s\n", line_char);
        printf("%s\n", line_nibleH);
        printf("%s\n", line_nibleL);
        printf("%s\n", line_fina);
    }
}

void minlog_force_open(int level, int timestamp_mode)
{
    if (pminlog == NULL) {
        minlog_open(level, timestamp_mode);
    }
}

void* minlog_open(int level, int timestamp_mode)
{
    time_t rawtime;

    if (pminlog != NULL) {
      return pminlog;
    }

    time (&rawtime);

    pminlog = (minlog_t*)malloc(sizeof(struct minlog_st));
    assert(pminlog != NULL);

    memset(pminlog->log_file_path, 0, sizeof(pminlog->log_file_path));

	  pminlog->start_timestamp_seconds = rawtime;
    pminlog->level = level;
    pminlog->timestamp_mode = timestamp_mode;

    presentation();

    return pminlog;
}

void *minlog_file_open(const char *argv_zero, int level, const char *pname_aux, int timestamp_mode)
{
	  const char *filename_only;
	  int v0_len;
	  char *ppath;
	  pid_t pid;
	  int log_id;

    if (pminlog == NULL) return 0;

	  pminlog = (minlog_t *)minlog_open(level, timestamp_mode);

    filename_only = strrchr(argv_zero, '/') + 1;

    v0_len = (int)strlen(argv_zero);

    // Initializing File Path
    ppath = GetEnv("MINLOG_LOGDIR");

    if (pname_aux)      // For log on files
    {
        char buf_value[50];


        if (ppath)
        {
            STRCPY(pminlog->log_file_path, ppath, strlen(ppath));
            STRCAT(pminlog->log_file_path, "/", 1);
            STRCAT(pminlog->log_file_path, filename_only, strlen(filename_only));
        }
        else
        {
            STRCPY(pminlog->log_file_path, argv_zero, v0_len);
        }

        if (pname_aux[0])
        {
            STRCAT(pminlog->log_file_path, "_", 1);
            STRCAT(pminlog->log_file_path, pname_aux, strlen(pname_aux));
        }


        STRCAT(pminlog->log_file_path, "_", 1);
        pid = getpid();
        SPRINTF(buf_value,sizeof(buf_value),"%6.6d", pid);
        STRCAT(pminlog->log_file_path, buf_value, sizeof(buf_value));
        STRCAT(pminlog->log_file_path, "_",1);
        log_id = unique_log_id++;
        SPRINTF(buf_value, sizeof(buf_value), "%4.4d", log_id);
        STRCAT(pminlog->log_file_path, buf_value, sizeof(buf_value));
        STRCAT(pminlog->log_file_path, ".log",4);
    }

    presentation();

    return pminlog;
}

void minlog_close(void *pref)
{
    assert(((void*)pref == (void*)pminlog));
    assert(pminlog != NULL);
    free(pminlog);
    pminlog = NULL;
}

// Output to a file previosly defined
int minlogfile(char *filepath, char *buffer)
{
    FILE *pfl;

#ifdef _WIN32
	  fopen_s(&pfl, filepath, "a");
#else
	  pfl = fopen(filepath, "a");
#endif

    fputs(buffer, pfl);

    fclose(pfl);

	return 0;
}

int minlog(const char *psourcefile, int sourceline, int level, const char *pfmtmsg, int ctparam, ...)
{
    char msgbuffer[4096];
    char tsbuffer[40];
    char *ptimestamp;
	  va_list args;

    if (pminlog == NULL) return 0;

    if (pminlog->level > level)
    {
        return 0;
    }

	  ptimestamp = getTimeStamp(tsbuffer, sizeof(tsbuffer));

    va_start(args, ctparam);
    vsnprintf(msgbuffer, sizeof(msgbuffer), pfmtmsg, args);
    va_end(args);

    if (pminlog->log_file_path[0])
    {
        char file_buffer[512];
        SPRINTF(file_buffer,sizeof(file_buffer), "%s[%c]{%s}[%d][%s]\n", ptimestamp, minlog_level_char[level], msgbuffer, sourceline, psourcefile );
        minlogfile(pminlog->log_file_path, file_buffer);
    }
    else
    {
        // Output to console
        printf("%s[%c]{%s}[%d][%s]\n", ptimestamp, minlog_level_char[level], msgbuffer, sourceline, psourcefile );
    }

	return 0;
}

int minlog_print(const char *pmesg, int size)
{
  char msg_buffer[MINLOG_MAXBUF + 1];
  char msg_nibleH[MINLOG_MAXBUF + 1];
  char msg_nibleL[MINLOG_MAXBUF + 1];
  const unsigned char* pmem = (unsigned char *)pmesg;
  int tam = 0;
  int header_info = 0;

  if (pminlog == NULL) return 0;

  while (tam < size && tam < MINLOG_MAXBUF)
  {
    if (*pmem >= ' ' && *pmem <= '~')
    {
      // Put printable char in the first line
      msg_buffer[tam] = *pmem;
      msg_nibleH[tam] = ' ';
      msg_nibleL[tam] = ' ';
    }
    else
    {
      // Especial Char in first line
      msg_buffer[tam] = '~';
      msg_nibleH[tam] = charset[(*pmem) / 16];
      msg_nibleL[tam] = charset[(*pmem) % 16];
      if (tam < MINLOG_MAXHDRBIN) header_info++;
    }
    tam++;
    pmem++;
  }
  msg_buffer[tam] = '\0';
  msg_nibleH[tam] = '\0';
  msg_nibleL[tam] = '\0';

  if (pminlog->log_file_path[0])
  {
    minlogfile(pminlog->log_file_path, msg_buffer);
    if (header_info > 0) {
      minlogfile(pminlog->log_file_path, msg_nibleH);
      minlogfile(pminlog->log_file_path, msg_nibleL);
    }
  }
  else
  {
    // Output to console
    printf("%s\n", msg_buffer);
    if (header_info > 0) {
      // Ensuring that at least a certain amount of binary information can be consulted in poor console
      msg_nibleH[MINLOG_MAXHDRBIN] = '\0';
      msg_nibleL[MINLOG_MAXHDRBIN] = '\0';

      printf("%s\n", msg_nibleH);
      printf("%s\n", msg_nibleL);
    }
  }
  return tam;
}

int mindump(__uint64_t address, int size)
{
	int c = 0;
	int indx = 0;
	char line_buffer[1024];
	unsigned char* pmem = (unsigned char *)address;

  if (pminlog == NULL) return 0;

	SPRINTF(line_buffer, sizeof(line_buffer), "[%p] %+3.3d ", pmem, size);
	// 01234567  . 
	while (line_buffer[c] != '\0')
	{
		c++;
	}

	while (size > 0 && indx < size)
	{
		line_buffer[c++] = charset[(*pmem) / 16];
		line_buffer[c++] = charset[(*pmem) % 16];
		line_buffer[c++] = ' ';
		indx++;
		pmem++;
	}
	while (size < 0 && indx > size)
	{
		line_buffer[c++] = charset[(*pmem) / 16];
		line_buffer[c++] = charset[(*pmem) % 16];
		line_buffer[c++] = ' ';
		indx--;
		pmem--;
	}
	line_buffer[c] = '\0';


	if (pminlog->log_file_path[0])
	{
		minlogfile(pminlog->log_file_path, line_buffer);
	}
	else
	{
		// Output to console
		printf("%s\n", line_buffer);
	}
	return 0;
}

