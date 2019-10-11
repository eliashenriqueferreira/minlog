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
#include <inttypes.h>
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

char *getMinimalTimeStamp(char *pbuf, size_t buflen)
{
    struct timespec tms;

    /* The C11 way */
    if (! timespec_get(&tms, TIME_UTC))
    {
        SPRINTF(pbuf, buflen, "[timespec_get ERROR]");
        return pbuf;
    }
    //    /* POSIX.1-2008 way */
    //    if (clock_gettime(CLOCK_REALTIME,&tms)) {
    //      sprintf(pbuf, "[clock_gettime ERROR]");
    //      return pbuf;
    //    }

    // Minimal Timestamp
	SPRINTF(pbuf, buflen, "[%llds-%ldns]", tms.tv_sec, tms.tv_nsec);
    return pbuf;
}

char *getGMTTimeStamp(char *pbuf, size_t buflen)
{
    struct timespec tms;
    char buf_aux[40];
    time_t rawtime;

    /* The C11 way */
    if (! timespec_get(&tms, TIME_UTC))
    {
        SPRINTF(pbuf, buflen, "[timespec_get ERROR]");
        return pbuf;
    }
    //    /* POSIX.1-2008 way */
    //    if (clock_gettime(CLOCK_REALTIME,&tms)) {
    //      sprintf(pbuf, "[clock_gettime ERROR]");
    //      return pbuf;
    //    }
    //time (&rawtime);
    rawtime = tms.tv_sec;
	struct tm tmGMT;

	GMTIME(&rawtime, &tmGMT);
	//errno_t    gmtime_s(struct tm* tmDest, const __time_t * sourceTime);		// Windows
	//struct tm* gmtime_r(const time_t * timep, struct tm* result);				// Linux


    // %F  Short YYYY-MM-DD date, equivalent to %Y-%m-%d     2001-08-23
    // %T ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S 14:55:02
    strftime(buf_aux,40,"[GMT %F %T.%%.3ld]",&tmGMT);

	SPRINTF(pbuf, buflen, buf_aux, tms.tv_nsec / 1000000); 

    return pbuf;
}

char *getLOCALTimeStamp(char *pbuf, size_t buflen)
{
    struct timespec tms;
    char buf_aux[40];
    time_t rawtime;

    /* The C11 way */
    if (! timespec_get(&tms, TIME_UTC))
    {
		SPRINTF(pbuf, buflen, "[timespec_get ERROR]");
        return pbuf;
    }
    //    /* POSIX.1-2008 way */
    //    if (clock_gettime(CLOCK_REALTIME,&tms)) {
    //      sprintf(pbuf, "[clock_gettime ERROR]");
    //      return pbuf;
    //    }

    rawtime = tms.tv_sec;
	struct tm tmLoc;
	GMTIME(&rawtime, &tmLoc);
	//errno_t    gmtime_s(struct tm* tmDest, const __time_t * sourceTime);		// Windows
	//struct tm* gmtime_r(const time_t * timep, struct tm* result);				// Linux


    // %F  Short YYYY-MM-DD date, equivalent to %Y-%m-%d     2001-08-23
    // %T ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S 14:55:02
    strftime(buf_aux,40,"[LOC %F %T.%%.3ld]",&tmLoc);
    SPRINTF(pbuf, buflen, buf_aux, tms.tv_nsec/1000000);
    return pbuf;
}


void *minlog_open(const char *argv_zero, int level, const char *pname_aux)
{
    pminlog = (minlog_t *)malloc(sizeof(struct minlog_st));
    assert(pminlog != NULL);
    pminlog->level = level;

    memset(pminlog->log_file_path, 0, sizeof(pminlog->log_file_path));

    // Initializing File Path
    char* ppath = GetEnv("MINLOG_LOGDIR");

    int v0_len = strlen(argv_zero);
    char *filename_only = strrchr(argv_zero, '/') + 1;

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
        pid_t pid = getpid();
        SPRINTF(buf_value,sizeof(buf_value),"%6.6d", pid);
        STRCAT(pminlog->log_file_path, buf_value, sizeof(buf_value));
        STRCAT(pminlog->log_file_path, "_",1);
        int log_id = unique_log_id++;
        SPRINTF(buf_value, sizeof(buf_value), "%4.4d", log_id);
        STRCAT(pminlog->log_file_path, buf_value, sizeof(buf_value));
        STRCAT(pminlog->log_file_path, ".log",4);
    }

    return pminlog;
}

void minlog_close(void *pminlog)
{
    free(pminlog);
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
    assert(pminlog != NULL);

    if (pminlog->level > level)
    {
        return 0;
    }

    char msgbuffer[250];
    char tsbuffer[40];
    char *ptimestamp = getGMTTimeStamp(tsbuffer, sizeof(tsbuffer));

    va_list args;
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

