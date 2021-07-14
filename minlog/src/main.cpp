#include <stdio.h>
#include <string>
#include <stdarg.h>

#include "../inc/minlog.h"
#include <iostream>

using namespace std;

int test_minlog(int argc, char **argv);
void test_dump_stack();
void show_log_init(int argc, char** argv);

int main(int argc, char **argv)
{
    LOGHANDLE log;

	std::string str("Hello World!Lets create a minimal logger for applications writed in CPP!");
	
    cout << str << endl;

    if (argc > 1)
    {
        log = minlog_file_open(argv[0], MINLOG_LEVEL_DEBUG, argv[1], MINLOG_TIMESTAMP_GMT);   // Initializing with logfiles
    }
    else
    {
        log = minlog_open(MINLOG_LEVEL_DEBUG, MINLOG_TIMESTAMP_GMT);   // Initializing on console
    }

    show_log_init(argc,argv);

    test_dump_stack();
    test_minlog(argc,argv);
    minlog_close(log);

    return 0;
}

#ifdef _WIN32
#pragma warning (disable: 4244)
#pragma warning (disable: 4700)
#pragma warning (disable: 4309)
//#pragma warning (disable: 4996)
#endif

void test_dump_stack()
{
    int int1 = 0x11111111;
    int int2 = 0x22222222;
    short short3 = 0x3333;
    short short4 = 0x4444;
    unsigned char char5 = 0x55;
    char buffer[] = { 0xbf,15,15,15,15,15,15,15,15,15,15,15,15,15,0xbf };
    unsigned char char6 = 0x66;
    __uint64_t uint64_7 = 0x7777777777777777;
    __uint64_t uint64_8 = 0x8888888888888888;

    MINLOG_DEBUG(" - - - - - - - - - - - - SHOWING MEMORY STACK STRUCTURE - - - - - - - - - - - - - - - - - - ");

    mindump((__uint64_t)&int1, -64);
    mindump((__uint64_t)&int1, +64);
    mindump((__uint64_t)&int1, sizeof(int1));
    mindump((__uint64_t)&int2, sizeof(int2));
    mindump((__uint64_t)&short3, sizeof(short3));
    mindump((__uint64_t)&short4, sizeof(short4));
    mindump((__uint64_t)&char5, sizeof(char5));
    mindump((__uint64_t)&char6, sizeof(char6));
    mindump((__uint64_t)&uint64_7, sizeof(uint64_7));
    mindump((__uint64_t)&uint64_8, sizeof(uint64_8));
    mindump((__uint64_t)buffer, sizeof(buffer));
    __uint64_t somatudo = int1 + int2 + short3 + short4 + char5 + char6 + uint64_7 + uint64_8;
    for (int i = 0; i < sizeof(buffer); i++)
    {
        somatudo += buffer[i];
    }
    mindump((__uint64_t)&somatudo, sizeof(somatudo));
}

int test_minlog(int argc, char **argv)
{
    unsigned char c = ' ';

    for (int i = 0x20; i < 0x7f; i++, c++)
    {
        usleep(1000);
        MINLOG_ERROR("Message with %d parameters. Valor Decimal %.3d. Valor Hexa 0x%x. Caracter %c. i negativo %d", 5, i, i, c, (-i));
    }


    __uint64_t p1,p2,p3,p4,p5,p6,p7;
    p1 = 1;
#ifdef _MSC_VER
    p2 = 2;
    p3 = 3;
    p4 = 4;
    p5 = 5;
    p6 = 6;
#else
    MINLOG_WARNING("------------------- NOT INITIALIZED VALUES FOR TESTS ---------------------");
#endif
    p7 = 7;
    int i1=p1,i2=p2,i3=p3,i4=p4,i5=p5,i6=p6,i7=p7;

    minlog(__FILE__, __LINE__, MINLOG_LEVEL_DEBUG,     "Debug with 7 parameters p. p1=%d p2=%d p3=%d p4=%d p5=%d p6=%d p7=%d", 7,p1,p2,p3,p4,p5,p6,p7);
    MINLOG("MINLOG with 7 parameters i. i1=%d i2=%d i3=%d i4=%d i5=%d i6=%d i7=%d", i1,i2,i3,i4,i5,i6,i7);

    MINLOG("No parameters.");

    __uint64_t result = 0x65966596;
    __uint64_t diffP1Result = (__uint64_t)(&p1 - &result);

    MINLOG_DEBUG("Stack p1==> [%8.8p]=[%8.8X]", &p1, p1);
    MINLOG_DEBUG("Stack p2==> [%8.8p]=[%8.8X]", &p2, p2);
    MINLOG_DEBUG("Stack p3==> [%8.8p]=[%8.8X]", &p3, p3);
    MINLOG_DEBUG("Stack p4==> [%8.8p]=[%8.8X]", &p4, p4);
    MINLOG_DEBUG("Stack p5==> [%8.8p]=[%8.8X]", &p5, p5);
    MINLOG_DEBUG("Stack p6==> [%8.8p]=[%8.8X]", &p6, p6);
    MINLOG_DEBUG("Stack p7==> [%8.8p]=[%8.8X]", &p7, p7);
    MINLOG_DEBUG("Stack i1==> [%8.8p]=[%8.8X]", &i1, i1);
    MINLOG_DEBUG("Stack i2==> [%8.8p]=[%8.8X]", &i2, i2);
    MINLOG_DEBUG("Stack i3==> [%8.8p]=[%8.8X]", &i3, i3);
    MINLOG_DEBUG("Stack i4==> [%8.8p]=[%8.8X]", &i4, i4);
    MINLOG_DEBUG("Stack i5==> [%8.8p]=[%8.8X]", &i5, i5);
    MINLOG_DEBUG("Stack i6==> [%8.8p]=[%8.8X]", &i6, i6);
    MINLOG_DEBUG("Stack i7==> [%8.8p]=[%8.8X]", &i7, i7);
    MINLOG_DEBUG("  result==> [%8.8p]=[%8.8X]", &result, result);
    MINLOG_DEBUG("diffP1Result[%.16p]=[%.16x]", &diffP1Result, diffP1Result);


    if (0x007BFC00 == (__uint64_t)(&result)) {
        // Run, Copy Adresses and Rebuild;
        MINLOG_WARNING("------- NOT DANGEROUS IF YOU KNOW ADDRESS ALLOCATION  -------");
        (*((__uint64_t *)(0x7fffffffdbF0))) = (*((__uint64_t *)(0x7fffffffdbE0)))+(*((__uint64_t *)(0x7fffffffdbE8)));
    }
    MINLOG_DEBUG("New Result &result=%p Value=%X", &result, result);

    return 0;
}

void show_log_init(int argc, char** argv)
{
#ifdef _MSC_VER
    MINLOG_INFO("INFO: Windows Aplication");
#ifdef _WIN64
    MINLOG_INFO("INFO: compiled in 64bits");
#elif _WIN32
    MINLOG_INFO("INFO: compiled in 32bits");
#endif

#ifdef _DEBUG
    MINLOG_INFO("INFO: This is in DEBUG mode");
#elif NDEBUG
    MINLOG_INFO("INFO: This is NOT in DEBUG mode");
#endif
#endif

}

/*


*/
