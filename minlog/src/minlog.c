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

#include "../inc/minlog.h"

#ifdef __GNUC__
//#pragma GCC diagnostic [warning|error|ignored] OPTION
#pragma GCC diagnostic warning "-Wformat=0"
#endif

// Levels of Identification - Trace, Debug, Info, Warning, Error, Critical, Off
char minlog_level_char[] = {'T','D','I','W','E','C','O'};

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

#if ( _MSC_VER < 1800 )
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


void* minlog_open(int level, int timestamp_mode)
{
    time_t rawtime;

    time (&rawtime);

    assert(pminlog == NULL);
    pminlog = (minlog_t*)malloc(sizeof(struct minlog_st));
    assert(pminlog != NULL);

    memset(pminlog->log_file_path, 0, sizeof(pminlog->log_file_path));

	pminlog->start_timestamp_seconds = rawtime;
    pminlog->level = level;
    pminlog->timestamp_mode = timestamp_mode;
    return pminlog;
}

void *minlog_file_open(const char *argv_zero, int level, const char *pname_aux, int timestamp_mode)
{
	const char *filename_only;
	int v0_len;
	char *ppath;
	pid_t pid;
	int log_id;

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
    char msgbuffer[250];
    char tsbuffer[40];
    char *ptimestamp;
	va_list args;

    assert(pminlog != NULL);

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

int mindump(__uint64_t address, int size)
{
	int c = 0;
	int indx = 0;
	char line_buffer[1024];
	char charset[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	unsigned char* pmem = (unsigned char *)address;

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

