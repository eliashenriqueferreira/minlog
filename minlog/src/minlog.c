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

int minlog_out(char *buffer);

// Levels of Identification - Trace, Debug, Error, Critical, Warning, Info, Always
char minlog_level_char[] = {'T','D','E','C','W','I','A'};
/*
#define MINLOG_LEVEL_TRACE      0
#define MINLOG_LEVEL_DEBUG      1
#define MINLOG_LEVEL_ERROR      2
#define MINLOG_LEVEL_CRITICAL   3
#define MINLOG_LEVEL_WARNING    4
#define MINLOG_LEVEL_INFO       5
#define MINLOG_LEVEL_ALWAYS     6
*/

char charset[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };


typedef struct minlog_st
{
    int minlevel;
    int timestamp_mode;
    int outconsole;
    int outfile;
    int level_msg;
		time_t start_timestamp_seconds;
    char log_file_path[4096];   // 4096 for linux    
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

void presentation_debug()
{
    char ch = ' ';
    char line_init[] = "...................................................... MINLOG DEBUG - PRINTABLE CHARS ...............................................................";
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

    pminlog->level_msg = MINLOG_LEVEL_ALWAYS;
    minlog_out(line_init);
    minlog_out(line_char);
    minlog_out(line_nibleH);
    minlog_out(line_nibleL);
    minlog_out(line_fina);
    }

void presentation()
    {
    char line_init[MINLOG_MAXBUF];

    switch (pminlog->minlevel) {
    case MINLOG_LEVEL_INFO: SPRINTF(line_init, sizeof(line_init), "...................................................... MINLOG to MINLOG_LEVEL_INFO(%d) ...............................................................", pminlog->minlevel); break;
    case MINLOG_LEVEL_CRITICAL: SPRINTF(line_init, sizeof(line_init), "...................................................... MINLOG to MINLOG_LEVEL_CRITICAL(%d) ...............................................................", pminlog->minlevel); break;
    case MINLOG_LEVEL_ERROR: SPRINTF(line_init, sizeof(line_init), "...................................................... MINLOG to MINLOG_LEVEL_ERROR(%d) ...............................................................", pminlog->minlevel); break;
    case MINLOG_LEVEL_WARNING: SPRINTF(line_init, sizeof(line_init), "...................................................... MINLOG to MINLOG_LEVEL_INFO(%d) ...............................................................", pminlog->minlevel); break;
    }

    pminlog->level_msg = MINLOG_LEVEL_ALWAYS;

    minlog_out(line_init);
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
    pminlog->minlevel = level;
    pminlog->timestamp_mode = timestamp_mode;

    return pminlog;
}

void *minlog_file_open(int argc, char *argv[], int level, int timestamp_mode)
{
	  const char *filename_only;
	  int v0_len;
	  char *ppath;
	  pid_t pid;
	  int log_id;


    if (pminlog != NULL) return 0;


	  pminlog = (minlog_t *)minlog_open(level, timestamp_mode);

    // In Argv[1] will be the level of output console.
    if (argc > 1) {
      pminlog->outconsole = atoi(argv[1]);
    }
    else
    {
      pminlog->outconsole = MINLOG_LEVEL_OFF;
    }
    // In Argv[2] will be the level of output for files.
    if (argc > 2) {
      pminlog->outfile = atoi(argv[2]);
    }
    else
    {
      pminlog->outfile = MINLOG_LEVEL_OFF;
    }

    // Forces others to the minimal level
    if (pminlog->outconsole != MINLOG_LEVEL_OFF) {
      if (pminlog->minlevel > pminlog->outconsole) {
        pminlog->minlevel = pminlog->outconsole;
      }
      else {
        pminlog->outconsole = pminlog->minlevel;
      }
    }
    // Forces others to the minimal level
    if (pminlog->outfile != MINLOG_LEVEL_OFF) {
      if (pminlog->minlevel > pminlog->outfile) {
        pminlog->minlevel = pminlog->outfile;
      }
      else {
        pminlog->outfile = pminlog->minlevel;
      }
    }
    else {
      // No Necessity build log file path
      return 0;
    }


    // TODO - Need to make compatibility with forward slash
    filename_only = strrchr(argv[0], '\\') + 1;

    v0_len = (int)strlen(argv[0]);

    // Initializing File Path
    if (argc > 3) 
    {
        // Path of log directory by command line
        ppath = argv[3];
    }
    else
    {
      // Path of log directory by Environment variable
    ppath = GetEnv("MINLOG_LOGDIR");
    }

    // For log on files
    {
        char buf_value[50];

        memset(pminlog->log_file_path, 0, sizeof(pminlog->log_file_path));

        if (ppath)
        {
            STRCPY(pminlog->log_file_path, ppath, strlen(ppath));
            STRCAT(pminlog->log_file_path, "\\", 1);
            STRCAT(pminlog->log_file_path, filename_only, strlen(filename_only));
        }
        else
        {
            STRCPY(pminlog->log_file_path, argv[0], v0_len);
            //strcpy(pminlog->log_file_path, argv_zero);
        }

        STRCAT(pminlog->log_file_path, "_", 1);
        pid = getpid();
        SPRINTF(buf_value,sizeof(buf_value),"%6.6d", pid);
        STRCAT(pminlog->log_file_path, buf_value, sizeof(buf_value));
        //STRCAT(pminlog->log_file_path, "_",1);
        //log_id = unique_log_id++;
        //SPRINTF(buf_value, sizeof(buf_value), "%4.4d", log_id);
        //STRCAT(pminlog->log_file_path, buf_value, sizeof(buf_value));
        STRCAT(pminlog->log_file_path, ".min",4);
    }

    if (pminlog->minlevel == MINLOG_LEVEL_DEBUG) {
      presentation_debug();
    }
    else {
    presentation();
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
int minlog_out(char *buffer)
{
  // I'm warranty that trace level will be exhibited forever MINLOG_LEVEL_TRACE = 0
  if ((pminlog->level_msg >= pminlog->outconsole)) {
    //printf("%s",buffer);
    puts(buffer);
  }
  if ((pminlog->level_msg >= pminlog->outfile)) {
    FILE *pfl;
    errno_t err;

#ifdef _WIN32
    while ((err = fopen_s(&pfl, pminlog->log_file_path, "a")) != 0) {
      fprintf(stderr, "MINLOG RUNTIME ERROR:[%s]\n", strerror(err));
      Sleep(500);
    }
    fprintf(pfl, "%s\n", buffer);
    //fputs(buffer, pfl);
#else
    pfl = fopen(pminlog->log_file_path, "a");
    fprintf(pfl, "%s\n", buffer);
    //fputs(buffer, pfl);
#endif

    fclose(pfl);
  }

	return 0;
}

int minlog(const char *psourcefile, int sourceline, int level, const char *pfmtmsg, int ctparam, ...)
{
    char file_buffer[MINLOG_MAXBUF];
    char msgbuffer[MINLOG_MAXBUF];
    char tsbuffer[40];
    char *ptimestamp;
	  va_list args;

    if (pminlog == NULL) return 0;

    if (level < pminlog->minlevel) {
        return 0;
    }
    pminlog->level_msg = level;


	  ptimestamp = getTimeStamp(tsbuffer, sizeof(tsbuffer));

    va_start(args, ctparam);
    vsnprintf(msgbuffer, sizeof(msgbuffer), pfmtmsg, args);
    va_end(args);

    SPRINTF(file_buffer,sizeof(file_buffer), "%s[%c]{%s}[%d][%s]", ptimestamp, minlog_level_char[level], msgbuffer, sourceline, psourcefile );
    minlog_out(file_buffer);

	return 0;
}

int minlog_print(const char *pmesg, int size)
{
  char msg_buffer[MINLOG_MAXBUF];
  char msg_nibleH[MINLOG_MAXBUF];
  char msg_nibleL[MINLOG_MAXBUF];
  const unsigned char* pmem = (unsigned char *)pmesg;
  int tam = 0;
  int header_info = 0;

  if (pminlog == NULL) return 0;

  pminlog->level_msg = MINLOG_LEVEL_DEBUG;

  while (tam < size && tam < (MINLOG_MAXBUF-1))
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

  minlog_out(msg_buffer);
    if (header_info > 0) {
    minlog_out(msg_nibleH);
    minlog_out(msg_nibleL);
    }

  //// Output to console
  //printf("%s\n", msg_buffer);
  //if (header_info > 0) {
  //  // Ensuring that at least a certain amount of binary information can be consulted in poor console
  //  msg_nibleH[MINLOG_MAXHDRBIN] = '\0';
  //  msg_nibleL[MINLOG_MAXHDRBIN] = '\0';

  //  printf("%s\n", msg_nibleH);
  //  printf("%s\n", msg_nibleL);
  //}
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

  minlog_out(line_buffer);
	return 0;
}

