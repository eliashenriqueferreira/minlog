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
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>     // for memset
#include "minlog.h"

//#pragma GCC diagnostic [warning|error|ignored] OPTION
#pragma GCC diagnostic warning "-Wformat=0"

// Levels of Identification - Trace, Debug, Info, Warning, Error, Critical, Off
char minlog_level_char[] = {'T','D','I','W','E','C','O'};

typedef struct minlog_st
{
        int level;
        char log_file_path[FILENAME_MAX];   // 4096 for linux
} minlog_t;
minlog_t *pminlog = NULL;

unsigned int unique_log_id = 0;

char *getMinimalTimeStamp(char *pbuf)
{
    struct timespec tms;

    /* The C11 way */
    if (! timespec_get(&tms, TIME_UTC))
    {
        sprintf(pbuf, "[timespec_get ERROR]");
        return pbuf;
    }
    //    /* POSIX.1-2008 way */
    //    if (clock_gettime(CLOCK_REALTIME,&tms)) {
    //      sprintf(pbuf, "[clock_gettime ERROR]");
    //      return pbuf;
    //    }

    // Minimal Timestamp
    sprintf(pbuf, "[%lds-%ldns]", tms.tv_sec, tms.tv_nsec);
    return pbuf;
}

char *getGMTTimeStamp(char *pbuf)
{
    struct timespec tms;
    char buf_aux[40];
    time_t rawtime;

    /* The C11 way */
    if (! timespec_get(&tms, TIME_UTC))
    {
        sprintf(pbuf, "[timespec_get ERROR]");
        return pbuf;
    }
    //    /* POSIX.1-2008 way */
    //    if (clock_gettime(CLOCK_REALTIME,&tms)) {
    //      sprintf(pbuf, "[clock_gettime ERROR]");
    //      return pbuf;
    //    }
    //time (&rawtime);
    rawtime = tms.tv_sec;
    struct tm * tmGMT = gmtime(&rawtime);

    // %F  Short YYYY-MM-DD date, equivalent to %Y-%m-%d     2001-08-23
    // %T ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S 14:55:02
    strftime(buf_aux,40,"[GMT %F %T.%%.3ld]",tmGMT);

    sprintf(pbuf, buf_aux, tms.tv_nsec/1000000);
    return pbuf;
}

char *getLOCALTimeStamp(char *pbuf)
{
    struct timespec tms;
    char buf_aux[40];
    time_t rawtime;

    /* The C11 way */
    if (! timespec_get(&tms, TIME_UTC))
    {
        sprintf(pbuf, "[timespec_get ERROR]");
        return pbuf;
    }
    //    /* POSIX.1-2008 way */
    //    if (clock_gettime(CLOCK_REALTIME,&tms)) {
    //      sprintf(pbuf, "[clock_gettime ERROR]");
    //      return pbuf;
    //    }

    rawtime = tms.tv_sec;
    struct tm * tmLoc = gmtime(&rawtime);

    // %F  Short YYYY-MM-DD date, equivalent to %Y-%m-%d     2001-08-23
    // %T ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S 14:55:02
    strftime(buf_aux,40,"[LOC %F %T.%%.3ld]",tmLoc);
    sprintf(pbuf, buf_aux, tms.tv_nsec/1000000);
    return pbuf;
}


void *minlog_open(const char *argv_zero, int level, const char *pname_aux)
{
    pminlog = (minlog_t *)malloc(sizeof(struct minlog_st));
    assert(pminlog != NULL);
    pminlog->level = level;

    memset(pminlog->log_file_path, 0, sizeof(pminlog->log_file_path));

    // Initializing File Path
    char* ppath = getenv("MINLOG_LOGDIR");

    int v0_len = strlen(argv_zero);
    char *filename_only = strrchr(argv_zero, '/') + 1;

    if (pname_aux)      // For log on files
    {
        char buf_value[50];


        if (ppath)
        {
            strcpy(pminlog->log_file_path, ppath);
            strcat(pminlog->log_file_path, "/");
            strcat(pminlog->log_file_path, filename_only);
        }
        else
        {
            strcpy(pminlog->log_file_path, argv_zero);
        }

        if (pname_aux[0])
        {
            strcat(pminlog->log_file_path, "_");
            strcat(pminlog->log_file_path, pname_aux);
        }

        strcat(pminlog->log_file_path, "_");
        pid_t pid = getpid();
        sprintf(buf_value,"%6.6d", pid);
        strcat(pminlog->log_file_path, buf_value);
        strcat(pminlog->log_file_path, "_");
        int log_id = unique_log_id++;
        sprintf(buf_value,"%4.4d", log_id);
        strcat(pminlog->log_file_path, buf_value);
        strcat(pminlog->log_file_path, ".log");
    }

    return pminlog;
}

void minlog_close(void *pminlog)
{
    free(pminlog);
}


int minlogfile(char *filepath, char *buffer)
{
#ifdef __WINDOWS__
#else
    FILE *pfl;

    pfl = fopen(filepath, "a");

    fputs(buffer, pfl);

    fclose(pfl);

#endif
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
    char *ptimestamp = getGMTTimeStamp(tsbuffer);

    va_list args;
    va_start(args, ctparam);
    vsprintf(msgbuffer, pfmtmsg, args);
    va_end(args);

    if (pminlog->log_file_path[0])
    {
        char file_buffer[512];
        sprintf(file_buffer, "%s[%c]{%s}[%d][%s]\n", ptimestamp, minlog_level_char[level], msgbuffer, sourceline, psourcefile );
        minlogfile(pminlog->log_file_path, file_buffer);
    }
    else
    {
        // Output to console
        printf("%s[%c]{%s}[%d][%s]\n", ptimestamp, minlog_level_char[level], msgbuffer, sourceline, psourcefile );
    }

}

